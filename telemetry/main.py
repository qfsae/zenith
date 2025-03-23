import socket
import sys
import json
from PyQt5.QtWidgets import QApplication
from PyQt5.QtCore import QThread, pyqtSignal, QObject
from gui import TelemetryWindow

class SignalEmitter(QObject):
    newData = pyqtSignal(dict)

def main():
    app = QApplication(sys.argv)
    window = TelemetryWindow()
    window.show()

    emitter = SignalEmitter()
    emitter.newData.connect(window.updateTelemetry)

    host = "0.0.0.0"
    port = 9999
    running = True

    def udp_listner():
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        server_socket.bind((host, port))
        while running:
            data, _ = server_socket.recvfrom(2048)
            telemetry_data = json.loads(data.decode("utf-8"))
            emitter.newData.emit(telemetry_data)

    udp_thread = QThread()
    udp_thread.run = udp_listner
    udp_thread.start()

    window.serverThread = udp_thread

    sys.exit(app.exec_())

if __name__ == '__main__':
    main()