## uGFX-Teensy
This project is an attempt to integrate the uGFX embedded graphics library with the FTDI FT813 display controller found on the screen currently in-house. The controller is not currently supported and requires some basic device drivers to get running.

Testing is being performed with a Teensy 3.2 using PlatformIO's Arduino library, but uGFX is platform agnostic and once the FT813 is shown to be working, the SPI drivers can be modified for the STM32 F4. 

## Why? 
The advantage of uGFX, especially this year, is that it is portable across a wide variety of displays, microcontrollers, and OS'. This would allow the team to develop the UI from the comfort of a standard Win32 or Linux environment, and then simply retarget it to the embedded device. In parallel, should a new hardware configuration be required, the team can also develop any required drivers or HAL to meet the library's standards. 

