#include "interfaces/interface_can.h"
#include "stdio.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stream_buffer.h"
#include "semphr.h"
#include "nvicConfig.h"
#include "taskHandlers.h"



// Static Arrays for storing CAN messages
#define CAN_DATA_ARR_SIZE 1024
can_msg_t CAN1_DATA[CAN_DATA_ARR_SIZE];
can_msg_t CAN2_DATA[CAN_DATA_ARR_SIZE];

uint32_t can1_hash(uint32_t id){
    return id % CAN_DATA_ARR_SIZE;
}

uint32_t can2_hash(uint32_t id){
    return id % CAN_DATA_ARR_SIZE;
}


// Static Buffer struct for CAN Stream buffers
struct can_streambuffer{
    can_msg_t buffer[64];
    StaticStreamBuffer_t RXStaticBuffer;
    StreamBufferHandle_t streamHandle;
};

// Static Buffers for CAN recieve channels
static struct can_streambuffer CAN1_RX;
//static struct can_streambuffer CAN2_RX; ///    Transmission


// Definitions for CAN Transmit Mailbox Numberings
#define CAN_TX_SEMAPHORE_COUNT 0
#define CAN_TX_SEMAPHORE_TX0   1
#define CAN_TX_SEMAPHORE_TX1   2
#define CAN_TX_SEMAPHORE_TX2   3
// Create Semaphores for CAN1 TX mailboxes
SemaphoreHandle_t        CAN1_TX_Semaphore[4];         // Semaphores for CAN1 Transmit Mailboxes
static StaticSemaphore_t CAN1_TX_SemaphoreBuffer[4];   // Static Buffer for CAN1 Semaphores


// Create Binary semaphores for CAN 2 TX mailboxes
//SemaphoreHandle_t        CAN1_TX_Semaphore;         // Counting Semaphore for CAN1 Transmit Mailboxes
//static StaticSemaphore_t CAN1_TX_SemaphoreBuffer;   // Static Buffer for CAN1 Counting Semaphore
//SemaphoreHandle_t        CAN1_TX0_Semaphore;        // CAN1 TX 0 Mailbox
//static StaticSemaphore_t CAN1_TX0_SemaphoreBuffer;  // Static Buffer for TX0 Semaphore
//SemaphoreHandle_t CAN1_TX1_Semaphore;               // CAN1 TX 1 Mailbox
//static StaticSemaphore_t CAN1_TX1_SemaphoreBuffer;  // Static Buffer for TX1 Semaphore
//SemaphoreHandle_t CAN1_TX2_Semaphore;               // CAN1 TX 2 Mailbox
//static SemaphoreHandle_t CAN1_TX2_SemaphoreBuffer;  // Static Buffer for TX2 Semaphore




/**
 * @brief OS CAN Setup Handler
 * 
 * This function should be called to init the CAN bus and handler interfaces
 * This function should not be called more than once
 * ALL CAN bus initialization should happen within this function
 * 
 * Called in main
 * 
 */
void os_can_setup(void){
    // NOTE: NART (no automatic re-transmission) should be set to false in application
    gpio_set_mode(PIN('A', 10), GPIO_MODE_OUTPUT);
    gpio_write(PIN('A', 10), false);
    uint8_t can1_status = hal_can_init(CAN1, CAN_1000KBPS, true, PIN('A', 11), PIN('A', 12));
    // On event of CAN bus initialization failure
    if(can1_status != SYS_OK){
        printf("Initialization Failure: CAN Bus Error: %d", can1_status);
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
            CAN1_RX.streamHandle,     // Streambuffer to send to
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
    printf("\nRunning CAN Interface Task\n");

    // Create the Recieve Stream buffers
    CAN1_RX.streamHandle = xStreamBufferCreateStatic(
            (64*sizeof(can_msg_t)),     // Size of the buffer (in bytes)
            sizeof(can_msg_t),          // Size of Messages in buffer
            (uint8_t*)CAN1_RX.buffer,   // Pointer to the static buffer
            &CAN1_RX.RXStaticBuffer     // Pointer to the streambuffer handle
            );

    // CAN2_RX.streamHandle = xStreamBufferCreateStatic((64*sizeof(can_msg_t)), sizeof(can_msg_t), (uint8_t*)CAN2_RX.buffer, &CAN2_RX.RXStaticBuffer);


    // Enable the CAN1 RX interrupt on message recieved
    SET_BIT(CAN1->IER, CAN_IER_FMPIE0);
    NVIC_SetPriority(CAN1_RX0_IRQn, NVIC_Priority_MIN-10U);
    NVIC_EnableIRQ(CAN1_RX0_IRQn);

    // Enable the CAN2 RX interrupt on message recieved
    //SET_BIT(CAN2->IER, CAN_IER_FMPI0);
    //NVIC_SetPriority(CAN2_RX0_IRQn, NVIC_Priority_MIN-10U);
    //NVIC_EnableIRQ(CAN2_RX0_IRQn); 


    // LOOP waiting for CAN messages
    for(;;){
        // LOOP pulling and sorting messages from the streambuffer
        while(xStreamBufferBytesAvailable(CAN1_RX.streamHandle) > 0){
            // Temp message to store the data comming out of the buffer
            can_msg_t msg;
            // Recieve a message from the buffer
            xStreamBufferReceive(
                    CAN1_RX.streamHandle, // Stream Buffer Handle
                    (void*)&msg,          // Pointer to RX Buffer (void*)
                    sizeof(can_msg_t),    // Size of RX Buffer (Shoudl be size of CAN message)
                    10                    // Ticks to Wait
            );
            // Load the message from the streambuffer into the hash table
            CAN1_DATA[can1_hash(msg.id)] = msg;
            // Timestamp the message
            CAN1_DATA[can1_hash(msg.id)].timestamp = xTaskGetTickCount();
            printf("%d\n", CAN1_DATA[can1_hash(msg.id)].id);
        }
        // while(xStreamBufferBytesAvailable(CAN2_RX.streamHandle) > 0){
        //     // Temp message to store the data comming out of the buffer
        //     can_msg_t msg;
        //     // Recieve a message from the buffer
        //     xStreamBufferReceive(CAN2_RX.streamHandle, (void*)&msg, sizeof(can_msg_t), 10);
        //     CAN2_DATA[can2_hash(msg.id)] = msg;
        //     CAN2_DATA[can2_hash(msg.id)].timestamp = xTaskGetTickCount();
        // }
        vTaskDelay(100);
    }
}

can_msg_t can_fetch(CAN_TypeDef *CAN, uint32_t id){
    if(CAN == CAN1) return CAN1_DATA[can1_hash(id)];
    // Return a zero message on event of misused CAN bus
    return (can_msg_t){
        .timestamp = 0,
        .id = 0,
        .len = 0
    };
}

bool can_check_timestamp(CAN_TypeDef *CAN, uint32_t id, uint32_t maxTicks){
    if(CAN == CAN1) return CAN1_DATA[can1_hash(id)].timestamp + maxTicks < xTaskGetTickCount();
    // Return false on misused can bus
    return false;
}


uint8_t can_send_msg(CAN_TypeDef *CAN, can_msg_t *tx_msg, TickType_t timeout){
    // Check if a transmit mailbox is available
    if(xSemaphoreTake(CAN1_TX_Semaphore[CAN_TX_SEMAPHORE_COUNT], timeout) != pdTRUE){
        // Return failed to aquire TX mailbox
        return HAL_CAN_MAILBOX_NONEMPTY;
    }
    // Attempt to aquire one of the transmit mailboxes
    for(uint8_t i = 1; i < 4; i++){
        if(xSemaphoreTake(CAN1_TX_Semaphore[i], 10) == pdTRUE){
            // Utilize the hal to load the selected mailbox
            hal_can_send(CAN, tx_msg, (i-1)/*mailbox=0..2*/);
            // Wait for mailbox to be empty
            while(!hal_can_send_ready(CAN, (i-1)));
            // Give back the semaphores
            xSemaphoreGive(CAN1_TX_Semaphore[i]);
            xSemaphoreGive(CAN1_TX_Semaphore[CAN_TX_SEMAPHORE_COUNT]);
            // Return OK
            return SYS_OK;
        }
    }
    return HAL_CAN_FATAL_ERR;
}

