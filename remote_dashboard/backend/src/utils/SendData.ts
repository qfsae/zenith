/**
 * @file SendData.ts
 * @author Ethan Parliament
 * @brief File includes code used to send data to the database through the POST route
 * @version 1.0
 * @date 2024-01-14
 */

import axios from "axios";
import { DataRequest } from "../types";


/**
 * This function is used as the POST route of the server.
 * It is used to send data to the database
 * 
 * @returns void
 */
export async function sendData(data: DataRequest) {

    try {
        const response = await axios.post('http://localhost:3000/sendData', data);
        console.log(response.data);
    } catch (error: any) {
        console.error('Error sending data:', error.message);
    }
};