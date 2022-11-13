# CAL: CAN Abstraction Layer
## Built for Motec ECU, QFSAE 2022
Auth: jchisholm204
Ver: 3.21

### About
This project provides a simple reference-based infrastructure for programmers to access data being outputted by the MoTec ECU and PDM.  Included in the program are data containers designed to store different forms of data coming off the bus.  Current containers are built for 8 and 16 bit integers, floats and booleans.  Containers should not matter to the user. Containers consist of many constant variables as well as a single non constant variable for data storage. Constant variables are fixed within the declaration of the container and include the ID the data belongs to, the byte or bytes where the data can be found, the bitmask of the data and in some cases the multiplier.  Constants can be accessed by the user although recommended use is to simply use the update method.

### Update Methods
The update methods are over defined functions which take a data container, a CAN message struct and a user defined variable.  The update method checks to see if the CAN message has the same ID as the data container and then updates the user defined variable.  It is recommended to call the update methods regularly.

### Code Organization
Namespaces exist.  Please use them.  Everything should be under the `CAL` namespace as a base, after that things are organized by `DATA_` followed by the name of the data.  Examples would be `DATA_ECU` or `DATA_PCM`. Furthermore, there is a namespace for MoTec CAN ID’s.  This will be updated as more data points need to be referenced.

### Contributing
When contributing to this code, please follow all naming conventions laid out in this document and the code itself.  Please provide adequate documentation of code so that it can be maintained without author presence.
