/**
 * @file app.ts
 * @author Ethan Parliament
 * @brief File hosts the server for remote dashboard and routes to send data to the database
 * @version 1.0
 * @date 2024-01-14
 */

import express, { Request, Response } from 'express';
import dotenv from 'dotenv';
import { connectToDatabase, closeDatabase } from './utils/DbConnection';
import { DataRequest } from './types';

// Load environment variables from .env
dotenv.config();

const app = express();
const port = 3000;

// Middleware to parse JSON in POST requests
app.use(express.json());

const dbConfig = {
  host: 'localhost',
  user: 'root',
  password: process.env.MYSQL_PASSWORD,
  database: process.env.MYSQL_DB_NAME,
};

// Route for handling POST requests
app.post('/sendData', async (req: Request<{}, {}, DataRequest>, res: Response) => {
  const pool = await connectToDatabase(dbConfig);
  if (pool === undefined) {
    return res.status(500).json({ error: 'Internal Server Error' });
  }

  const { time, rpm, drive_status, vbatt_voltage, motor_power, coolant_temp } = req.body;

  // Insert data into MySQL
  const insertQuery = `
    INSERT INTO measurements (time, rpm, drive_status, vbatt_voltage, motor_power, coolant_temp)
    VALUES (?, ?, ?, ?, ?, ?)
  `;
  try {
    const connection = await pool.getConnection();
    await connection.query(insertQuery, [time, rpm, drive_status, vbatt_voltage, motor_power, coolant_temp]);
    console.log('Data inserted successfully');
    res.status(200).json({ message: 'Data inserted successfully' });
  } catch (err) {
    console.error('Error inserting data into MySQL:', err);
    res.status(500).json({ error: 'Internal Server Error' });
  }
});

app.get('/getData', async (req, res) => {
  const pool = await connectToDatabase(dbConfig);
  if (pool === undefined) {
    return res.status(500).json({ error: 'Internal Server Error' });
  }

  const selectQuery = 'SELECT * FROM measurements';

  try {
    const connection = await pool.getConnection();
    const [rows, fields] = await connection.execute(selectQuery);
    res.json(rows);
  } catch (err) {
    console.error('Error fetching data from MySQL:', err);
    res.status(500).send('Error fetching data from MySQL');
  }
});

// Start the server
app.listen(port, async () => {
  console.log(`Server is listening on port ${port}`);
});

// Close MySQL connection on server exit
process.on('SIGINT', async () => {
  await closeDatabase();
  process.exit();
});


