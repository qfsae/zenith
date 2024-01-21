# QFSAE Remote Dashboard Server

This project is a simple TypeScript-based server designed to communicate with an ESP32 microcontroller to receive live data from a car. The received data is then stored in a MySQL database.

## Getting Started

These instructions will help you set up and run the project on your local machine.

### Prerequisites

Before running the server, make sure you have the following installed:

- [Node.js](https://nodejs.org/)
- [MySQL Server](https://dev.mysql.com/downloads/mysql/)
- [MySQL Workbench](https://www.mysql.com/products/workbench/)

### MySQL Workbench Setup

1. Open MySQL Workbench and create a new connection.

2. Under connection name, put QFSAE. Put `root` as the Username.

3. Under `Password`, click on `Store in Vault` and enter the password you will use for the database. Keep this handy for later

4. Click on `Test Connection`. You should see a success message pop up on screen

5. Click `OK` to proceed


### Database Setup

1. Execute the following SQL script to create the `measurements` table:

   ```sql
    CREATE DATABASE IF NOT EXISTS qfsae_remote;
    USE qfsae_remote;

   CREATE TABLE `measurements` (
     `time` datetime NOT NULL,
     `rpm` float DEFAULT NULL,
     `drive_status` varchar(45) DEFAULT NULL,
     `vbatt_voltage` float DEFAULT NULL,
     `motor_power` float DEFAULT NULL,
     `coolant_temp` float DEFAULT NULL,
     PRIMARY KEY (`time`)
   );

### Server Setup

1. Clone the repository:

   ```bash
   git clone https://github.com/qfsae/telemetry_backend.git
2. Install dependencies:

    ```bash
    yarn install
3. Create a .env file in the root directory and add your MYSQL database connection details:

    ```env
    MYSQL_PASSWORD=your_database_password
    MYSQL_DB_NAME=your_database_name
### Usage

1. Start the server: 
    ```bash
    yarn start
The server will be running at http://localhost:3000/


### Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements, feel free to open an issue or submit a pull request.

### License

This project is licensed under the MIT License - see the LICENSE file for details.
