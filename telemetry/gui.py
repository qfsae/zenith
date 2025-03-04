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

    @pyqtSlot(dict)
    def updateTelemetry(self, data):
        # Get the raw timestamp from the incoming data.
        raw_timestamp = data.get("time", len(self.time_data))
        # Compute effective time relative to the current offset.
        effective_timestamp = raw_timestamp - self.time_offset
        self.time_data.append(effective_timestamp)
        # Store the last raw time for reset purposes.
        self.last_raw_time = raw_timestamp
        if len(self.time_data) == 1:
            self.startTime = effective_timestamp

        # Cell temperatures
        cell_temps = data.get("cell_temps", [0 for _ in range(num_BMS_sensors)])
        for i in range(len(cell_temps)):
            self.cell_temp_data[i].append(cell_temps[i])

        # SOC
        soc = data.get("soc", 0)
        self.soc_data.append(soc)

        # Wheel speed
        wheel_speed = data.get("wheel_speed", 0)
        self.wheel_speed_data.append(wheel_speed)

        # Throttle
        throttle = data.get("throttle", 0)
        self.throttle_data.append(throttle)

        # Brake
        brake = data.get("brake", 0)
        self.brake_data.append(brake)

        # Speed (numeric display)
        speed = data.get("speed", 0)
        self.speedValueLabel.setText(f"{speed:.1f}")

        # Fault
        fault = data.get("fault", 0)
        if fault:
            self.faultStatus.setText("FAULT!")
            self.faultStatus.setStyleSheet("background-color: red; color: white;")
        else:
            self.faultStatus.setText("OK")
            self.faultStatus.setStyleSheet("background-color: green; color: white;")

    def updatePlots(self):
        if not self.time_data:
            return

        current_time = self.time_data[-1]
        xmin = max(current_time - 60, 0)

        # Update cell temperature curves and average
        for i, curve in enumerate(self.cellTemp_curves):
            curve.setData(self.time_data, self.cell_temp_data[i])
        self.cellTempPlot.setXRange(xmin, current_time)
        avgTemps = []
        for i in range(num_BMS_sensors):
            temps_in_window = [temp for j, temp in enumerate(self.cell_temp_data[i]) if self.time_data[j] >= xmin]
            if temps_in_window:
                avgTemps.append(sum(temps_in_window) / len(temps_in_window))
        avgTemp = sum(avgTemps) / len(avgTemps) if avgTemps else 0
        self.cellTempStat.setText(f"Avg: {avgTemp:.1f}")

        # Update wheel speed
        self.wheelSpeedCurve.setData(self.time_data, self.wheel_speed_data)
        self.wheelSpeedPlot.setXRange(xmin, current_time)
        speeds_in_window = [speed for j, speed in enumerate(self.wheel_speed_data) if self.time_data[j] >= xmin]
        avgSpeed = sum(speeds_in_window) / len(speeds_in_window) if speeds_in_window else 0
        self.wheelSpeedStat.setText(f"Avg: {avgSpeed:.1f}")

        # SOC plot: full timeline
        self.socCurve.setData(self.time_data, self.soc_data)
        avgSOC = sum(self.soc_data) / len(self.soc_data) if self.soc_data else 0
        self.socStat.setText(f"Avg: {avgSOC:.1f}")

        # Update throttle
        self.throttleCurve.setData(self.time_data, self.throttle_data)
        self.throttlePlot.setXRange(xmin, current_time)
        throttles_in_window = [val for j, val in enumerate(self.throttle_data) if self.time_data[j] >= xmin]
        avgThrottle = sum(throttles_in_window) / len(throttles_in_window) if throttles_in_window else 0
        self.throttleStat.setText(f"Avg: {avgThrottle:.1f}")

        # Update brake
        self.brakeCurve.setData(self.time_data, self.brake_data)
        self.brakePlot.setXRange(xmin, current_time)
        brakes_in_window = [val for j, val in enumerate(self.brake_data) if self.time_data[j] >= xmin]
        avgBrake = sum(brakes_in_window) / len(brakes_in_window) if brakes_in_window else 0
        self.brakeStat.setText(f"Avg: {avgBrake:.1f}")

    def updateElapsedTime(self):
        if self.time_data:
            elapsed = self.time_data[-1] - self.startTime
            self.elapsedTimeLabel.setText(f"Elapsed: {elapsed:.1f} s")

    def resetData(self):
        # Set the time_offset to the last received raw time so that the next incoming
        # data point's effective time will be zero.
        self.time_offset = self.last_raw_time if hasattr(self, "last_raw_time") else 0.0

        self.demoThread.reset()

        # Clear data storage
        self.time_data.clear()
        self.cell_temp_data = [[] for _ in range(num_BMS_sensors)]
        self.soc_data.clear()
        self.wheel_speed_data.clear()
        self.throttle_data.clear()
        self.brake_data.clear()
        self.startTime = 0.0

        # Clear plots
        self.cellTempPlot.clear()
        self.wheelSpeedPlot.clear()
        self.socPlot.clear()
        self.throttlePlot.clear()
        self.brakePlot.clear()

        # Recreate curves after clearing plots
        self.cellTemp_curves = []
        colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k', 'w'][:num_BMS_sensors]
        for i in range(num_BMS_sensors):
            curve = self.cellTempPlot.plot([], [], pen=colors[i], name=f"Cell {i+1}")
            self.cellTemp_curves.append(curve)
        self.wheelSpeedCurve = self.wheelSpeedPlot.plot([], [], pen='w')
        self.socCurve = self.socPlot.plot([], [], pen='c')
        self.throttleCurve = self.throttlePlot.plot([], [], pen='g')
        self.brakeCurve = self.brakePlot.plot([], [], pen='r')

