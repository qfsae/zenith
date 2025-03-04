import csv
from datetime import datetime

from PyQt5.QtCore import QTimer, Qt, pyqtSlot, QUrl
from PyQt5.QtMultimedia import QSoundEffect
from PyQt5.QtWidgets import (
    QMainWindow, QWidget, QLabel, QVBoxLayout, QHBoxLayout, QDial,
    QFrame, QPushButton, QApplication
)
import pyqtgraph as pg