import csv
from datetime import datetime

from PyQt5.QtCore import QTimer, Qt, pyqtSlot, QUrl
from PyQt5.QtMultimedia import QSoundEffect
from PyQt5.QtWidgets import (
    QMainWindow, QWidget, QLabel, QVBoxLayout, QHBoxLayout, QDial,
    QFrame, QPushButton, QApplication
)
import pyqtgraph as pg

num_BMS_sensors = 4

class TelemetryWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("QFSAE 25 Telemetry")
        self.resize(2400, 1800)  # Set size of window

        # Data storage for current plots
        self.time_data = []
        self.cell_temp_data = [[] for _ in range(num_BMS_sensors)]
        self.soc_data = []
        self.wheel_speed_data = []
        self.throttle_data = []
        self.brake_data = []

        # Time offset to make the next incoming timestamp zero after reset.
        self.time_offset = 0.0
        self.last_raw_time = 0.0

        # Start the timer for the data. 
        self.startTime = 0.0

        # Initialize the UI
        self.initUI()

        # Timer to refresh the plots every 50 ms
        self.timer = QTimer()
        self.timer.timeout.connect(self.updatePlots)
        self.timer.start(50)

        # Elapsed time timer (update every 50 ms)
        self.elapsedTimer = QTimer()
        self.elapsedTimer.timeout.connect(self.updateElapsedTime)
        self.elapsedTimer.start(50)
    def initUI(self):
        # The main widget is a box in the middle
        centralWidget = QWidget()
        self.setCentralWidget(centralWidget)
        self.mainLayout = QVBoxLayout()
        centralWidget.setLayout(self.mainLayout)

        # Elapsed time label at top-right
        headerLayout = QHBoxLayout()
        headerLayout.addStretch()
        self.elapsedTimeLabel = QLabel("Elapsed: 0.0 s")
        headerLayout.addWidget(self.elapsedTimeLabel)
        self.mainLayout.addLayout(headerLayout)

        # Row 1: Cell Temperatures and Wheel Speed (each plot with a statistic label)
        row1 = QHBoxLayout()
        self.cellTempPlot, self.cellTempStat = self.createPlotWithStat("Accumulator Cell Temperatures")
        row1.addWidget(self.wrapPlotAndStat(self.cellTempPlot, self.cellTempStat))
        self.wheelSpeedPlot, self.wheelSpeedStat = self.createPlotWithStat("Wheel Speed")
        row1.addWidget(self.wrapPlotAndStat(self.wheelSpeedPlot, self.wheelSpeedStat))
        self.mainLayout.addLayout(row1)

        # Row 2: SOC Plot (y-axis fixed to 0-100, full timeline)
        self.socPlot, self.socStat = self.createPlotWithStat("BMS State of Charge (%)")
        self.socPlot.setYRange(0, 100)
        self.mainLayout.addWidget(self.wrapPlotAndStat(self.socPlot, self.socStat))

        # Row 3: Throttle and Brake Plots side-by-side (both 0-100 on y-axis)
        row3 = QHBoxLayout()
        self.throttlePlot, self.throttleStat = self.createPlotWithStat("Throttle (%) Over Time")
        self.throttlePlot.setYRange(0, 100)
        row3.addWidget(self.wrapPlotAndStat(self.throttlePlot, self.throttleStat))
        self.brakePlot, self.brakeStat = self.createPlotWithStat("Brake (%) Over Time")
        self.brakePlot.setYRange(0, 100)
        row3.addWidget(self.wrapPlotAndStat(self.brakePlot, self.brakeStat))
        self.mainLayout.addLayout(row3)

        # Row 4: Bottom Row: Dials, Speed, Fault, and Battery Warning
        bottomLayout = QHBoxLayout()
        # Throttle Dial
        throttleLayout = QVBoxLayout()
        throttleLabel = QLabel("Throttle (%)")
        throttleLabel.setAlignment(Qt.AlignCenter)
        self.throttleDial = QDial()
        self.throttleDial.setRange(0, 100)
        self.throttleDial.setNotchesVisible(True)
        self.throttleDial.setEnabled(False)
        throttleLayout.addWidget(throttleLabel)
        throttleLayout.addWidget(self.throttleDial)
        bottomLayout.addLayout(throttleLayout)

        # Brake Dial
        brakeLayout = QVBoxLayout()
        brakeLabel = QLabel("Brake (%)")
        brakeLabel.setAlignment(Qt.AlignCenter)
        self.brakeDial = QDial()
        self.brakeDial.setRange(0, 100)
        self.brakeDial.setNotchesVisible(True)
        self.brakeDial.setEnabled(False)
        brakeLayout.addWidget(brakeLabel)
        brakeLayout.addWidget(self.brakeDial)
        bottomLayout.addLayout(brakeLayout)

        # Speed Display
        speedLayout = QVBoxLayout()
        speedLabel = QLabel("Speed")
        speedLabel.setAlignment(Qt.AlignCenter)
        self.speedValueLabel = QLabel("0")
        self.speedValueLabel.setAlignment(Qt.AlignCenter)
        self.speedValueLabel.setFrameStyle(QFrame.Panel | QFrame.Sunken)
        speedLayout.addWidget(speedLabel)
        speedLayout.addWidget(self.speedValueLabel)
        bottomLayout.addLayout(speedLayout)

        # Fault Indicator
        faultLayout = QVBoxLayout()
        faultLabel = QLabel("Fault Indicator")
        faultLabel.setAlignment(Qt.AlignCenter)
        self.faultStatus = QLabel("OK")
        self.faultStatus.setFrameStyle(QFrame.Panel | QFrame.Sunken)
        self.faultStatus.setAlignment(Qt.AlignCenter)
        faultLayout.addWidget(faultLabel)
        faultLayout.addWidget(self.faultStatus)
        bottomLayout.addLayout(faultLayout)


        self.mainLayout.addLayout(bottomLayout)

        # Row 5: Reset and Export Buttons
        buttonLayout = QHBoxLayout()
        self.resetButton = QPushButton("Reset Data")
        self.resetButton.clicked.connect(self.resetData)
        self.exportButton = QPushButton("Export to CSV")
        self.exportButton.clicked.connect(self.exportData)
        buttonLayout.addWidget(self.resetButton)
        buttonLayout.addWidget(self.exportButton)
        self.mainLayout.addLayout(buttonLayout)

        # Initialize plots' curves
        self.cellTemp_curves = []
        colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k', 'w'][:num_BMS_sensors]
        for i in range(num_BMS_sensors):
            curve = self.cellTempPlot.plot([], [], pen=colors[i], name=f"Cell {i+1}")
            self.cellTemp_curves.append(curve)
        self.wheelSpeedCurve = self.wheelSpeedPlot.plot([], [], pen='w')
        self.socCurve = self.socPlot.plot([], [], pen='c')
        self.throttleCurve = self.throttlePlot.plot([], [], pen='g')
        self.brakeCurve = self.brakePlot.plot([], [], pen='r')

    def createPlotWithStat(self, title):
        """Creates a plot widget and a corresponding label for average stats."""
        plot = pg.PlotWidget(title=title)
        statLabel = QLabel("Avg: N/A")
        statLabel.setAlignment(Qt.AlignCenter)
        return plot, statLabel

    def wrapPlotAndStat(self, plot, statLabel):
        """Wraps a plot and a stat label in a vertical layout widget."""
        container = QWidget()
        layout = QVBoxLayout()
        layout.addWidget(plot)
        layout.addWidget(statLabel)
        container.setLayout(layout)
        return container
