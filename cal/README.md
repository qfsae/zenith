# CAL: CAN Abstraction Layer

### Built for Motec ECU, QFSAE 2022

Auth: jchisholm204  
Ver: 3.9

### About

This project provides a simple reference-based infrastructure for programmers to access data being outputted by the MoTec ECU and PDM. The reference-based CAL library itself can be found within `/cal/decoder/` and can be included as a standard arduino library.  This library however only contains the information required to decode the data coming off of the bus, and has no relation to the hardware side of recieving messages.  When using stm32 based chips, this is controlled by a seperate library contained within `/cal/st-f4can/`.  Further information about the decoder library can be found below.

### Data Containers

Included in the program are data containers designed to decode different forms of data coming off the bus. Containers should not matter to the user. Containers consist of many constant variables. Constant variables are fixed within the declaration of the container and include the ID the data belongs to, the byte or bytes where the data can be found, the bitmask of the data and in some cases the multiplier. Constants can be accessed by the user in order to their own decoding, although recommended use is to simply use the update method.

### Update Methods

The update methods are over defined functions which take a data container, a CAN message struct and a user defined variable. The update method uses pass by reference in order to update the user define variable. When using update methods, it can be assumed that only the right data will be passed to the user defined variable as the function checks to ensure that the datatype passed, and the ID both match those set out by the data container.

### Code Organization

Namespaces exist. Please use them. Everything should be under the `CAL` namespace as a base, after that things are organized by `DATA_` followed by the name of the data. Examples would be `DATA_ECU` or `DATA_PCM`. Furthermore, there is a namespace for MoTec CAN ID’s. This may be updated as more data becomes available over the CAN bus.

### Contributing

When contributing to this code, please follow all naming conventions laid out in this document and the code itself. Please provide adequate documentation of code so that it can be maintained without author presence.
