# CAL: CAN Abstraction Layer

### Built for Motec ECU, QFSAE 2022

### About

This project provides a simple reference-based infrastructure for programmers to access data being outputted by the MoTec ECU and PDM. The reference-based CAL library itself can be found within `/cal/decoder/` and can be included as a standard arduino library. This library however only contains the information required to decode the data coming off of the bus, and has no relation to the hardware side of recieving messages. When using stm32 based chips, this is controlled by a seperate library contained within `/cal/st-f4can/`.

### Contributing

When contributing to this code, please follow all naming conventions laid out in this document and the code itself. Please provide adequate documentation of code so that it can be maintained without author presence.
