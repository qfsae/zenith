# Details

Date : 2024-03-10 13:00:42

Directory c:\\Users\\Jacob\\Documents\\GitHub\\QFSAE\\zenith\\Q24ECU\\core

Total : 44 files,  1800 codes, 1398 comments, 573 blanks, all 3771 lines

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)

## Files
| filename | language | code | comment | blank | total |
| :--- | :--- | ---: | ---: | ---: | ---: |
| [core/config/FreeRTOSConfig.h](/core/config/FreeRTOSConfig.h) | C++ | 46 | 79 | 19 | 144 |
| [core/config/nvicConfig.h](/core/config/nvicConfig.h) | C++ | 3 | 20 | 4 | 27 |
| [core/config/pins.h](/core/config/pins.h) | C++ | 39 | 36 | 23 | 98 |
| [core/include/core.h](/core/include/core.h) | C++ | 17 | 17 | 10 | 44 |
| [core/include/errors.h](/core/include/errors.h) | C++ | 19 | 33 | 4 | 56 |
| [core/include/hal/hal_adc.h](/core/include/hal/hal_adc.h) | C++ | 114 | 95 | 22 | 231 |
| [core/include/hal/hal_can.h](/core/include/hal/hal_can.h) | C | 136 | 94 | 42 | 272 |
| [core/include/hal/hal_clock.h](/core/include/hal/hal_clock.h) | C | 24 | 26 | 10 | 60 |
| [core/include/hal/hal_dma.h](/core/include/hal/hal_dma.h) | C++ | 54 | 46 | 18 | 118 |
| [core/include/hal/hal_flash.h](/core/include/hal/hal_flash.h) | C++ | 54 | 57 | 26 | 137 |
| [core/include/hal/hal_gpio.h](/core/include/hal/hal_gpio.h) | C | 52 | 21 | 16 | 89 |
| [core/include/hal/hal_spi.h](/core/include/hal/hal_spi.h) | C++ | 241 | 114 | 34 | 389 |
| [core/include/hal/hal_tim_basic.h](/core/include/hal/hal_tim_basic.h) | C++ | 13 | 21 | 5 | 39 |
| [core/include/hal/hal_uart.h](/core/include/hal/hal_uart.h) | C | 55 | 11 | 15 | 81 |
| [core/include/hal/legacy_hal_adc.h](/core/include/hal/legacy_hal_adc.h) | C++ | 67 | 22 | 12 | 101 |
| [core/include/interfaces/interfaceTaskHandlers.h](/core/include/interfaces/interfaceTaskHandlers.h) | C++ | 17 | 19 | 11 | 47 |
| [core/include/interfaces/interface_adc.h](/core/include/interfaces/interface_adc.h) | C++ | 16 | 37 | 8 | 61 |
| [core/include/interfaces/interface_can.h](/core/include/interfaces/interface_can.h) | C++ | 10 | 44 | 9 | 63 |
| [core/include/interfaces/interface_sysError.h](/core/include/interfaces/interface_sysError.h) | C++ | 14 | 55 | 11 | 80 |
| [core/include/interfaces/interface_uart.h](/core/include/interfaces/interface_uart.h) | C | 48 | 51 | 14 | 113 |
| [core/include/main.h](/core/include/main.h) | C | 8 | 14 | 9 | 31 |
| [core/include/tasks/handler.h](/core/include/tasks/handler.h) | C++ | 16 | 27 | 8 | 51 |
| [core/include/tasks/interfaces.h](/core/include/tasks/interfaces.h) | C++ | 5 | 14 | 8 | 27 |
| [core/include/tasks/taskHandlers.h](/core/include/tasks/taskHandlers.h) | C++ | 26 | 27 | 16 | 69 |
| [core/include/tasks/tasks.h](/core/include/tasks/tasks.h) | C++ | 0 | 10 | 0 | 10 |
| [core/src/core.c](/core/src/core.c) | C | 20 | 21 | 13 | 54 |
| [core/src/interfaces/README.md](/core/src/interfaces/README.md) | Markdown | 8 | 0 | 4 | 12 |
| [core/src/interfaces/interface_adc.c](/core/src/interfaces/interface_adc.c) | C | 49 | 52 | 14 | 115 |
| [core/src/interfaces/interface_can.c](/core/src/interfaces/interface_can.c) | C | 120 | 66 | 42 | 228 |
| [core/src/interfaces/interface_sysError.c](/core/src/interfaces/interface_sysError.c) | C | 86 | 53 | 19 | 158 |
| [core/src/interfaces/interface_uart.c](/core/src/interfaces/interface_uart.c) | C | 35 | 30 | 15 | 80 |
| [core/src/interrupts/README.md](/core/src/interrupts/README.md) | Markdown | 6 | 0 | 3 | 9 |
| [core/src/interrupts/TIM6.c](/core/src/interrupts/TIM6.c) | C | 5 | 17 | 4 | 26 |
| [core/src/main.c](/core/src/main.c) | C | 49 | 22 | 27 | 98 |
| [core/src/system/freertos.c](/core/src/system/freertos.c) | C | 12 | 21 | 5 | 38 |
| [core/src/system/shutdown.c](/core/src/system/shutdown.c) | C | 21 | 35 | 7 | 63 |
| [core/src/system/startup.c](/core/src/system/startup.c) | C | 8 | 22 | 4 | 34 |
| [core/src/system/syscalls.c](/core/src/system/syscalls.c) | C | 101 | 22 | 23 | 146 |
| [core/src/tasks/README.md](/core/src/tasks/README.md) | Markdown | 7 | 0 | 4 | 11 |
| [core/src/tasks/atc_example.c](/core/src/tasks/atc_example.c) | C | 0 | 0 | 1 | 1 |
| [core/src/tasks/canRunner.c](/core/src/tasks/canRunner.c) | C | 36 | 14 | 6 | 56 |
| [core/src/tasks/sdc_locked.c](/core/src/tasks/sdc_locked.c) | C | 0 | 0 | 1 | 1 |
| [core/src/tasks/taskHandlers.c](/core/src/tasks/taskHandlers.c) | C | 110 | 23 | 19 | 152 |
| [core/src/tasks/testTasks.c](/core/src/tasks/testTasks.c) | C | 33 | 10 | 8 | 51 |

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)