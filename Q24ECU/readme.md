# Queen's Formula SAE Q24 VCU
## Code Model V2

Author: Jacob Chisholm

Written for Queen's Formula SAE

## V2 Notes
- Built with RTOS
- CMAKE compiling/linking with GCC
- Makefile uploading (binary blobs produced cross compatible)

## Running
1. Select correct compiler kit in VS code cMake extension
2. Allow cMake automatic configuration
3. Select flash method in the makefile
3. Run `make flash` or upload code with another method

## Required Softwares:
- GCC
- arm-none-eabi-gcc
- arm-none-eabi-objcopy
- arm-none-eabi-gdb
- make
- VS Code
    - cMake extension

## Debugging
Debugging requires additional software: STM32 Cube

If debugging on windows, WSL 2 must be used and the ST-Link must be forwarded through to WSL through [these instructions](https://learn.microsoft.com/en-us/windows/wsl/connect-usb). STM32 Cube must be installed in WSL in addition to all of the aforementioned tools.