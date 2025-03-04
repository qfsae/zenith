# Q25FSAE Telemetry System

## Overview

This telemetry system is designed for the Q25FSAE race car, utilizing PyQt5 for a graphical interface and a simulated data generator. The goal of the system is to provide a live look at the state of the car during testing, for development and optimization purposes. Right now, the system is a demonstration consisting of three main components:

- **`gui.py`**: Provides the graphical user interface (GUI) for displaying telemetry data.
- **`main.py`**: Initializes and runs the telemetry system.
- **`demo_data.py`**: Generates simulated telemetry data for testing.

## Structure and Functionality

### `gui.py`
This module defines `TelemetryWindow`, the main GUI for displaying telemetry data. It includes:

- **PyQt5-based graphical layout** for real-time data visualization.
- **Dial-based and graphical telemetry displays**.
- **CSV logging capability**: Allows data logging for later analysis.
- **Warning sounds** for critical values.

### `demo_data.py`
This module defines `DemoDataThread`, a background thread that generates fake telemetry data for testing purposes.

- **QThread for asynchronous data generation**: Avoids blocking the GUI while generating telemetry data.
- **Simulated telemetry values**:
  - State of Charge (SOC)
  - Voltage, Current, Power
  - Temperature readings
- **Data emission via PyQt5 signals**: Updates the GUI in real time.

### `main.py`
This module serves as the entry point for the telemetry system. It:

- Initializes the PyQt5 application.
- Loads the `TelemetryWindow` GUI.
- Starts `DemoDataThread` when the `--demo` flag is provided.
- Ensures smooth interaction between the GUI and data processing threads.

## Future Improvements

The system currently relies on simulated data. To integrate real-time data from the Q25FSAE race car:

1. We should implement a **server-client architecture** using sockets. This will simulate the data transmission that will be happening on the car, since we will be connecting this GUI to a WIFI hotspot on the car, and communictaing over sockets.

2. Create a **CAN data translation module** to translate CAN data sent from the car over WIFI to the GUI's client socket. This will parse real CAN messages to extract telemetry data, and implement **data filtering** to remove noise and excess data.

### 3. **Performance Enhancements**
Currently, the system relies on **single-threaded GUI updates**. The following optimizations are recommended:

- **Optimize PyQt5 threading** to prevent lag in high-frequency telemetry updates.
- **Implement an efficient data buffer** for handling incoming messages.
- **Reduce GUI overhead** by updating only changed significantly changed values instead of refreshing the entire display with new data every 50ms.
