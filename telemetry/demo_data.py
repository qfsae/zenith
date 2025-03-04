from PyQt5.QtCore import QThread, pyqtSignal
import time
import random
import math

class DemoDataThread(QThread):
    # This signal will emit a dict with telemetry data.
    newData = pyqtSignal(dict)

    def __init__(self, parent=None):
        super().__init__(parent)
        self.running = True
        self.time_counter = 0.0
        self.soc_value = 100.0  # Start with SOC at 100%

    def run(self):
        while self.running:
            data = self.generate_data_dict()
            self.newData.emit(data)
            time.sleep(0.005)  # Update every 0.1 seconds
            self.time_counter += 0.005

    def generate_data_dict(self):
        # Sine wave for throttle (0 to 100)
        throttle = int((math.sin(self.time_counter * 0.5) + 1) * 50)
        
        # Cosine wave for brake (0 to 100)
        brake = int((math.cos(self.time_counter * 0.5) + 1) * 50)
        
        # Cell temperatures: ~30 +/- 5 using sine waves
        cell_temps = [30 + 5 * math.sin(self.time_counter + i) for i in range(4)]
        
        # Decrement SOC slowly but never below 0
        self.soc_value = max(0, self.soc_value - 0.02)
        
        # Wheel speed: cyclical in range ~0 to 100
        wheel_speed = abs(100 * math.sin(self.time_counter * 0.3))
        
        # Speed as a simple function of throttle
        speed = int(throttle * 0.5)
        
        # Fault: 1% random chance
        fault = 1 if random.random() < 0.01 else 0
        
        return {
            "time": self.time_counter,
            "cell_temps": cell_temps,
            "soc": self.soc_value,
            "wheel_speed": wheel_speed,
            "throttle": throttle,
            "brake": brake,
            "speed": speed,
            "fault": fault
        }
    def reset(self):
        self.soc_value = 100.0

    def stop(self):
        self.running = False
