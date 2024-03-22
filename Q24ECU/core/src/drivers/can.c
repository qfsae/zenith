/**
 * @file can.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Source File for CAN Driver
 * @version 1.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "drivers/can.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "stream_buffer.h"
#include "task.h"
#include "nvicConfig.h"
#include <stdio.h>

// Static arrays for storing CAN messages
#define CAN_DATA_SIZE 1024
can_msg_t CAN1_DATA[CAN_DATA_SIZE];
// can_msg_t CAN2_DATA[CAN_DATA_SIZE];

#define CAN2_HASH(id) (id % CAN_DATA_SIZE)
#define CAN1_HASH(id) (id % CAN_DATA_SIZE)

// Static Buffer struct for CAN Stream buffers
struct CAN_StreamBuffer{
    can_msg_t buffer[64];
    StaticStreamBuffer_t RXStaticBuffer;
    StreamBufferHandle_t streamHandle;
};

// Static Buffers for CAN recieve channels
static struct CAN_StreamBuffer CAN1_StreamBuffer;

// Definitions for CAN Transmit Mailbox Numberings
enum CAN_TX_SEMAPHORES{
    CAN_TX_SEMAPHORE_COUNT = 0,
    CAN_TX_SEMAPHORE_TX0,
    CAN_TX_SEMAPHORE_TX1,
    CAN_TX_SEMAPHORE_TX2
};
// Create Semaphores for CAN1 TX mailboxes
SemaphoreHandle_t        CAN1_TX_Semaphore[4];         // Semaphores for CAN1 Transmit Mailboxes
static StaticSemaphore_t CAN1_TX_SemaphoreBuffer[4];   // Static Buffer for CAN1 Semaphores

// Static Task for CAN RX Buffer Handler
static TaskHandle_t xTaskHandler_CANRX = NULL;
static StaticTask_t xTaskBuffer_CANRX;
static StackType_t xTaskStack_CANRX[configMINIMAL_STACK_SIZE];
/**
 * @brief CAN RX Buffer Handler Task
 * 
 * @param param NULL
 */
void vTask_CAN_RXBufferHandler(void *param);

void can_init(void){
    // Zero out CAN1 Data
    for(int i = 0; i < CAN_DATA_SIZE; i++){
        CAN1_DATA[i].id = 0;
        CAN1_DATA[i].len = 0;
        for(int j = 0; j < 8; j++){
            CAN1_DATA[i].data[j] = 0;
        }
    }

    // Zero out CAN2 Data
    // for(int i = 0; i < CAN_DATA_SIZE; i++){
    //     CAN2_DATA[i].id = 0;
    //     CAN2_DATA[i].len = 0;
    //     for(int j = 0; j < 8; j++){
    //         CAN2_DATA[i].data[j] = 0;
    //     }
    // }

    // Specific to VCU V0 => Write CAN pin low before initialization
    gpio_set_mode(PIN('A', 10), GPIO_MODE_OUTPUT);
    gpio_write(PIN('A', 10), false);
    // NOTE: NART (no automatic re-transmission) should be set to false in application
    uint8_t can1_status = hal_can_init(CAN1, CAN_1000KBPS, true, PIN('A', 11), PIN('A', 12));
    // On event of CAN bus initialization failure
    if(can1_status != SYS_OK){
        // printf("Initialization Failure: CAN Bus Error: %d", can1_status); // No printf statements in production code
        for(;;) __asm__("nop");
    }

    
    // Initialize Semaphores for Transmit Mailboxes
    CAN1_TX_Semaphore[CAN_TX_SEMAPHORE_COUNT]  = xSemaphoreCreateCountingStatic(
                2, // Number of TX Mailboxes
                0, // Starting Count (Goes up to Max Count)
                &CAN1_TX_SemaphoreBuffer[CAN_TX_SEMAPHORE_COUNT] // Pointer to static Buffer
        );
    xSemaphoreGive(CAN1_TX_Semaphore[CAN_TX_SEMAPHORE_COUNT]);

    CAN1_TX_Semaphore[CAN_TX_SEMAPHORE_TX0] = 
        xSemaphoreCreateBinaryStatic(&CAN1_TX_SemaphoreBuffer[CAN_TX_SEMAPHORE_TX0]);
    xSemaphoreGive(CAN1_TX_Semaphore[CAN_TX_SEMAPHORE_TX0]);

    CAN1_TX_Semaphore[CAN_TX_SEMAPHORE_TX1] =
        xSemaphoreCreateBinaryStatic(&CAN1_TX_SemaphoreBuffer[CAN_TX_SEMAPHORE_TX1]);
    xSemaphoreGive(CAN1_TX_Semaphore[CAN_TX_SEMAPHORE_TX1]);

    CAN1_TX_Semaphore[CAN_TX_SEMAPHORE_TX2] =
        xSemaphoreCreateBinaryStatic(&CAN1_TX_SemaphoreBuffer[CAN_TX_SEMAPHORE_TX2]);
    xSemaphoreGive(CAN1_TX_Semaphore[CAN_TX_SEMAPHORE_TX2]);




    // Create the CAN RX Buffer Handler Task
    xTaskHandler_CANRX = xTaskCreateStatic(
        vTask_CAN_RXBufferHandler,
        "CAN IRQ RX",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY+2,
        xTaskStack_CANRX,
        &xTaskBuffer_CANRX
    );
}

// CAN1 RX IRQ Handler (loads rx stream buffer)
void CAN1_RX0_IRQHandler(void){

    // Initialize a variable to trigger context switch to false
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // Temp message to store the incoming data
    can_msg_t rx_msg;

    // Utilize the hal to read the data and release the FIFO
    hal_can_read(CAN1, &rx_msg);

    // Send the recieved data into the stream buffer
    xStreamBufferSendFromISR(
            CAN1_StreamBuffer.streamHandle,     // Streambuffer to send to
            &rx_msg,                  // Data to copy into the buffer
            sizeof(can_msg_t),        // Size of data
            &xHigherPriorityTaskWoken // Checks if any tasks are waiting on buffer
    );

    // Check and trigger a context switch if needed
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}

void vTask_CAN_RXBufferHandler(void *param){
    // Unused
    (void)(param);
    printf("CAN Interface Task Running\n");
    

    // Create the Recieve Stream buffers
    CAN1_StreamBuffer.streamHandle =  xStreamBufferCreateStatic(
            (64*sizeof(can_msg_t)),     // Size of the buffer (in bytes)
            sizeof(can_msg_t),          // Size of Messages in buffer
            (uint8_t*)CAN1_StreamBuffer.buffer,   // Pointer to the static buffer
            &CAN1_StreamBuffer.RXStaticBuffer     // Pointer to the streambuffer handle
            );


    // Enable the CAN1 RX interrupt on message received
    SET_BIT(CAN1->IER, CAN_IER_FMPIE0);
    NVIC_SetPriority(CAN1_RX0_IRQn, NVIC_Priority_MIN-10U);
    NVIC_EnableIRQ(CAN1_RX0_IRQn);


    // LOOP waiting for CAN messages
    for(;;){
        // LOOP pulling and sorting messages from the stream buffer
        while(xStreamBufferBytesAvailable(CAN1_StreamBuffer.streamHandle) > 0){
            // Temp message to store the data comming out of the buffer
            can_msg_t msg;
            // Recieve a message from the buffer
            xStreamBufferReceive(
                    CAN1_StreamBuffer.streamHandle, // Stream Buffer Handle
                    (void*)&msg,          // Pointer to RX Buffer (void*)
                    sizeof(can_msg_t),    // Size of RX Buffer (Shoudl be size of CAN message)
                    10                    // Ticks to Wait
            );
            // Load the message from the streambuffer into the hash table
            CAN1_DATA[CAN1_HASH(msg.id)] = msg;
            // Timestamp the message
            CAN1_DATA[CAN1_HASH(msg.id)].timestamp = xTaskGetTickCount();
            // printf("%d\n", CAN1_DATA[can1_hash(msg.id)].id);
        }
        vTaskDelay(100);
    }
}

can_msg_t can_read(CAN_TypeDef *CAN, uint32_t id){
    if(CAN == CAN1) return CAN1_DATA[CAN1_HASH(id)];
    // Return a zero message on event of misused CAN bus
    return (can_msg_t){
        .timestamp = 0,
        .id = 0,
        .len = 0
    };
}

enum SYS_ERROR can_write(CAN_TypeDef *CAN, can_msg_t *tx_msg, TickType_t timeout){
    // Check if a transmit mailbox is available
    if(xSemaphoreTake(CAN1_TX_Semaphore[CAN_TX_SEMAPHORE_COUNT], timeout) != pdTRUE){
        // Return failed to aquire TX mailbox
        return HAL_CAN_MAILBOX_NONEMPTY;
    }
    // Attempt to aquire one of the transmit mailboxes
    for(uint8_t i = 1; i < 4; i++){
        if(xSemaphoreTake(CAN1_TX_Semaphore[i], 10) == pdTRUE){
            // Utilize the hal to load the selected mailbox
            hal_can_send(CAN, tx_msg, (uint8_t)(i-1)/*mailbox=0..2*/);
            // Wait for mailbox to be empty
            while(!hal_can_send_ready(CAN, (uint8_t)(i-1)));
            // Give back the semaphores
            xSemaphoreGive(CAN1_TX_Semaphore[i]);
            xSemaphoreGive(CAN1_TX_Semaphore[CAN_TX_SEMAPHORE_COUNT]);
            // Return OK
            return SYS_OK;
        }
    }
    return HAL_CAN_FATAL_ERR;
}
