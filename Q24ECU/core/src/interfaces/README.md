# Interfaces

This subdirectory contains the C files for all interfaces. Interfaces can be a union between a task and an interrupt. They can also be thread safe hardware drivers.
An example of an interface would be: An interrupt that pulls data out of the UART data register and adds it to a queue, combined with a task that blocks on the queue, computing the data when the queue is non-empty.

Interface header files may contain definitions to interact with hardware through thread safe methods. These thread safe methods should always be initialized in their corresponding source files (.c).

## Structure
All files within the interfaces folder should be prefixed with `interface_`. These files should be paired with a matching `interfaces_{name}.h` placed in the `include/interfaces` directory. For large interfaces, such as CAN, a single `interface_can.h` file should be used, regardless of how many `.c` files are present. For smaller interfaces, such as UART, there should only be a single `interfaces_uart.c` file that contains all of the UART interfaces.

## Initialization
All tasks, interrupts, and hardware interfaces (ports) should be initialized within a function prefixed with `os_` and postfixed with `_setup`. This function should be called in main.