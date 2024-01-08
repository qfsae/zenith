#include "interfaces/interface_can.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stream_buffer.h"
#include "nvicConfig.h"
#include "taskHandlers.h"



// Static Arrays for storing CAN messages
#define CAN_DATA_ARR_SIZE 1024
can_msg_t CAN1_DATA[CAN_DATA_ARR_SIZE];
can_msg_t CAN2_DATA[CAN_DATA_ARR_SIZE];

uint32_t can1_hash(uint32_t id){
    return id;
}

uint32_t can2_hash(uint32_t id){
    return id;
}


// Static Buffer struct for CAN Stream buffers
struct can_streambuffer{
    can_msg_t buffer[64];
    StaticStreamBuffer_t RXStaticBuffer;
    StreamBufferHandle_t streamHandle;
};

// Static Buffers for CAN recieve channels
static struct can_streambuffer CAN1_RX;
static struct can_streambuffer CAN2_RX;



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
    uint8_t can1_status = hal_can_init(CAN1, CAN_1000KBPS, true, PIN('A', 11), PIN('A', 12));
    // On event of CAN bus initialization failure
    if(can1_status != HAL_CAN_OK){
        printf("Initialization Failure: CAN Bus Error: %d", can1_status);
        for(;;) asm("nop");
    }
    xTaskCreate(
        tsk_CAN_RXBufferHandler,
        "CAN1RX",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        &tskh_CANRX_Handler
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
    xStreamBufferSendFromISR(CAN1_RX.streamHandle, &rx_msg, sizeof(can_msg_t), &xHigherPriorityTaskWoken);
    // Check and trigger a context switch if needed
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void tsk_CAN_RXBufferHandler(void *param){
    // Unused
    (void)(param);

    // Create the Recieve Stream buffers
    CAN1_RX.streamHandle = xStreamBufferCreateStatic((64*sizeof(can_msg_t)), sizeof(can_msg_t), (uint8_t*)CAN1_RX.buffer, &CAN1_RX.RXStaticBuffer);
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
            xStreamBufferReceive(CAN1_RX.streamHandle, (void*)&msg, sizeof(can_msg_t), 10);
            CAN1_DATA[can1_hash(msg.id)] = msg;
            CAN1_DATA[can1_hash(msg.id)].timestamp = xTaskGetTickCount();
            // printf("%d\n", CAN1_DATA[can1_hash(msg.id)].id);
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
    return (can_msg_t){
        .timestamp = 0,
        .id = 0,
        .len = 0
    };
}

bool can_check_timestamp(CAN_TypeDef *CAN, uint32_t id, uint32_t maxTicks){
    if(CAN == CAN1) return CAN1_DATA[can1_hash(id)].timestamp + maxTicks < xTaskGetTickCount();
    return false;
}

void can_send_msg(CAN_TypeDef *CAN, can_msg_t *tx_msg){
    hal_can_send(CAN, tx_msg, 0);
    while(hal_can_send_ready(CAN, 0));
    tx_msg->id = 100;
    hal_can_send(CAN, tx_msg, 0);
    while(hal_can_send_ready(CAN, 0));
}


