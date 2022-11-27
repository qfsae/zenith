# CAL: CAN Abstraction Layer
## Software Decoder

Auth: Jchisholm204  
Ver: 3.9  

This folder contains CAL, the QFSAE custom library for interacting with CAN bus.
The file `include/cal.h` contains the C++ abstract class declaration, which
specifies the library's interface.  This library was designed to be compatible with any hardware interfact that follows the standard `CAN_msg_t` architecture.

### Data Containers
Included in the program are data containers designed to decode different forms of data coming off the bus. Containers should not matter to the user. Containers consist of many constant variables. Constant variables are fixed within the declaration of the container and include the ID the data belongs to, the byte or bytes where the data can be found, the bitmask of the data and in some cases the multiplier. Constants can be accessed by the user in order to their own decoding, although recommended use is to simply use the update method.

### Update Methods
The update methods are over defined functions which take a data container, a CAN message struct and a user defined variable. The update method uses pass by reference in order to update the user define variable. When using update methods, it can be assumed that only the right data will be passed to the user defined variable as the function checks to ensure that the datatype passed, and the ID both match those set out by the data container.

### Code Organization
Namespaces exist. Please use them. Everything should be under the CAL namespace as a base, after that things are organized by `DATA_` followed by the name of the data. Examples would be `DATA_ECU` or `DATA_PCM`. Furthermore, there is a namespace for MoTec CAN ID’s. This may be updated as more data becomes available over the CAN bus.

### Contributing
When contributing to this code, please follow all naming conventions laid out in this document and the code itself. Please provide adequate documentation of code so that it can be maintained without author presence.
