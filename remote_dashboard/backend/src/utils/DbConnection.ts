/**
 * @file DbConnection.ts
 * @author Ethan Parliament
 * @brief File includes code used to connect to the database
 * @version 1.0
 * @date 2024-01-21
 */

import mysql, { Pool } from 'mysql2/promise';

let pool: Pool;

/**
 * This function is used to hook up to the database. 
 * You need to make sure the DB is setup and running first
 * 
 * @returns Pool
 */
export async function connectToDatabase(dbConfig: any) {
  try {
    pool = mysql.createPool({
      ...dbConfig,
      waitForConnections: true,
      connectionLimit: 10,
      queueLimit: 0
    });
    console.log('Connected to MySQL');
    return pool;
  } catch (err) {
    console.error('Error connecting to MySQL:', err);
  }
}

/**
 * This function is used to close all connections in the pool.
 * 
 * @returns void
 */
export async function closeDatabase() {
  if (pool) {
    await pool.end();
    console.log('All MySQL connections closed');
  }
}