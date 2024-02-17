# Basic FreeRTOS Information

This project uses the FreeRTOS real time scheduler in conjunction with a custom developed HAL. View the [FreeRTOS Docs Here](https://www.freertos.org/features.html).

Some basic information about the project:
- Heap 4 implementation is used
- Objects are statically allocated where possible.
- Cortex M4 Floating Point Unit is enabled
- 32 Levels of task priority
- 1kHz scheduler tick rate
- Mutexes and Counting Semaphores used
- `xTaskDelayUntil` Used for preemptive scheduling


*The FreeRTOS configuration used is adapted from the STM32 Cube Default Configuration*