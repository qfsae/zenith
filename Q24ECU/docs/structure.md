.
└── VCU_CORE/
    ├── include/
    │   ├── drivers/
    │   │   ├── adc.h
    │   │   ├── can.h
    │   │   ├── spi.h
    │   │   └── uart.h
    │   ├── hal/
    │   │   ├── hal_adc.h
    │   │   ├── hal_can.h
    │   │   ├── hal_clock.h
    │   │   ├── hal_dma.h
    │   │   ├── hal_flash.h
    │   │   └── hal_spi.h
    │   ├── interfaces/
    │   │   ├── cm200dx.h
    │   │   └── orionbms2.h
    │   ├── errors.h
    │   ├── main.h
    │   ├── sys_state.h
    │   └── sys_tasks.h
    ├── src/
    │   ├── drivers/
    │   │   ├── adc.c
    │   │   ├── can.c
    │   │   ├── uart.c
    │   │   └── spi.c
    │   ├── sys_state/
    │   │   ├── controller.c
    │   │   ├── state_error.c
    │   │   ├── state_idle.c
    │   │   ├── state_rtd.c
    │   │   └── state_start.c
    │   ├── sys_tasks/
    │   │   ├── controller.c
    │   │   ├── SBSPD.c
    │   │   ├── logging.c
    │   │   ├── cooling_ctrl.c
    │   │   ├── can1_echo.c
    │   │   ├── can2_echo.c
    │   │   ├── HVD_watchdog.c
    │   │   └── GLV_watchdog.c
    │   ├── system/
    │   │   ├── freertos.c
    │   │   ├── shutdown.c
    │   │   ├── startup.c
    │   │   └── syscalls.c
    │   └── main.c
    └── docs/
        ├── FreeRTOS.md
        ├── interfaces.md
        ├── tasks.md
        ├── canbus.md
        ├── README.md
        └── compiling.md