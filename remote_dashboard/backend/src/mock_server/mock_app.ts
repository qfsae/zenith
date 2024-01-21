/**
 * @file mock_app.ts
 * @author Ethan Parliament
 * @brief File can send mock data to the database for testing purposes
 * @version 1.0
 * @date 2024-01-21
 */

import { connectToDatabase } from '../utils';
import dotenv from 'dotenv';


// ...
dotenv.config();
// Connect to the database
const dbConfig = {
  host: 'localhost',
  user: 'root',
  password: process.env.MYSQL_PASSWORD,
  database: process.env.MYSQL_DB_NAME,
};
connectToDatabase(dbConfig);

const generateRandomData = () => {
    const randomData = {
        time: new Date().toISOString().slice(0, 19).replace("T", " "), // Current timestamp in the format "YYYY-MM-DD HH:mm:ss"
        rpm: Math.random() * 1000,  // Example: Random RPM between 0 and 1000
        drive_status: Math.random() < 0.5 ? 'Running' : 'Stopped',  // Example: Randomly choose between 'Running' and 'Stopped'
        vbatt_voltage: Math.random() * 12,  // Example: Random battery voltage between 0 and 12
        motor_power: Math.random() * 200,  // Example: Random motor power between 0 and 200
        coolant_temp: Math.random() * 100,  // Example: Random coolant temperature between 0 and 100
    };
    return randomData;
};

// Send mock data to the database every 5 seconds
setInterval(async () => {
  const pool = await connectToDatabase(dbConfig);
  if (pool === undefined) {
    console.error('Unable to connect to database');
    return;
  }

  const mockData = generateRandomData();

  const insertQuery = `
    INSERT INTO mock_measurements (time, rpm, drive_status, vbatt_voltage, motor_power, coolant_temp)
    VALUES (?, ?, ?, ?, ?, ?)
  `;

  try {
    const connection = await pool.getConnection();
    await connection.query(insertQuery, [
      mockData.time,
      mockData.rpm,
      mockData.drive_status,
      mockData.vbatt_voltage,
      mockData.motor_power,
      mockData.coolant_temp
    ]);
    connection.release();
    console.log('Mock data inserted successfully');
  } catch (err) {
    console.error('Error inserting mock data into MySQL:', err);
  }
}, 5000);  // 5000 milliseconds = 5 seconds