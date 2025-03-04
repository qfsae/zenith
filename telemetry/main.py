import sys
from PyQt5.QtWidgets import QApplication
from gui import TelemetryWindow
from demo_data import DemoDataThread

def main():
    app = QApplication(sys.argv)
    window = TelemetryWindow()
    window.show()

    # If "--demo" is specified on the command line, start generating demo data
    if "--demo" in sys.argv:
        demo_thread = DemoDataThread()
        demo_thread.newData.connect(window.updateTelemetry)
        demo_thread.start()

        window.demoThread = demo_thread

    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
