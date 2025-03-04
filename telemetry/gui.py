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
