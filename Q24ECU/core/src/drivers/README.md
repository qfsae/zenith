# Drivers
Drivers are hardware software interfaces that act as a middle ground between the HAL (Hardware Abstraction Layer) and system tasks.

Drivers should use FreeRTOS Semaphores for write and read access where needed and should avoid using tasks when possible.

### Structure

All Drivers should contain write and read methods in addition to an init method as described below. UART is used as an example.

Init Method: `void uart_init(void)`
Write Method: `SYS_ERROR uart_write(pHandle, data, timeout)`
Read Method:
    Depending on the interface the read method can be implemented in one of two ways.

1. Buffers

If the interface requires a buffer for receiving data then the read method may be implemented by providing `*_open_buffer` and `*_close_buffer` methods. In this case, the task opening the buffer must be responsible for all data allocation.

2. Return Methods

If the driver has a simple return type it can be implemented with `*_read(pHandle)`