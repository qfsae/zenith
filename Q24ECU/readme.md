# Queen's Formula SAE Q24 VCU
## Code Model V2

Author: Jacob Chisholm

Written for Queen's Formula SAE

## V2 Notes
- Built with FreeRTOS
- CMAKE compiling/linking with GCC
- Makefile uploading (binary blobs produced cross compatible)

## Running

#### VS Code
1. Select correct compiler kit in VS code cMake extension
2. Allow cMake automatic configuration
3. Select flash method in the makefile
3. Run `make flash` or upload code with another method

#### MakeFile
1. Run `make cmake` to generate cmake configuration files
2. Run `make build` to build the project
3. Select the flash method in the makefile
4. Run `make flash`


## Required Softwares:
- GCC
- arm-none-eabi-gcc
- arm-none-eabi-objcopy
- arm-none-eabi-gdb
- GNU make
- VS Code
    - cMake extension
    - Teleplot extension (for plotting serial data)

## Debugging
Debugging requires additional software:
- STM32 Cube
- VS Code
    - Cortex Debug Extension
- GDB (version > 9.0)

### Debugging on Windows with Cube IDE
Open the project in Cube IDE and select debug. Should just work. May need to open the project as an "Existing Cmake Project".

### Debugging on Windows with VS Code
If debugging on windows, WSL 2 must be used and the ST-Link must be forwarded through to WSL through [these instructions](https://learn.microsoft.com/en-us/windows/wsl/connect-usb). STM32 Cube must be installed in WSL in addition to all of the aforementioned tools.

This is because MinGW does not currently have support for GDB version > 9.0 required by the Cortex Debug extension.

#### To Debug
Once the softwares are installed debugging works identically to the standard GDB debugger. Breakpoints can be set, variables can be watched and all of the CPU registers can be read. To start a session, open up the debug pane in VS Code and select the `STlink Launch` or `STlink Attach` option. `STlink Launch` is preferred as it uploads the code to the micro before starting the session.
