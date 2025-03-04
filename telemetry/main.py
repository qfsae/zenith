import sys
from PyQt5.QtWidgets import QApplication
from gui import TelemetryWindow
from demo_data import DemoDataThread

def main():
    app = QApplication(sys.argv)
    window = TelemetryWindow()
    window.show()

    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
