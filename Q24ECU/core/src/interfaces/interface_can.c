#include "interfaces/interface_can.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stream_buffer.h"
#include "nvicConfig.h"
#include "taskHandlers.h"

static can_msg_t buffer_rxmsg[64];

static StaticStreamBuffer_t RXStaticBuffer;

StreamBufferHandle_t rx_stream;

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
        tsk_CAN1_RXBufferHandler,
        "CAN1RX",
        2048,
        NULL,
        tskIDLE_PRIORITY + 2,
        &tskh_CAN1RX_Handler
    );
}

void CAN1_RX0_IRQHandler(void){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    can_msg_t rx_msg;
    hal_can_read(CAN1, &rx_msg);
    xStreamBufferSendFromISR(rx_stream, &rx_msg, sizeof(can_msg_t), &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void tsk_CAN1_RXBufferHandler(void *param){
    // Create the Recieve Stream buffer
    rx_stream = xStreamBufferCreateStatic((64*sizeof(can_msg_t)), sizeof(can_msg_t), (uint8_t*)buffer_rxmsg, &RXStaticBuffer);
    //rx_stream = xStreamBufferCreate(64*sizeof(can_msg_t), sizeof(can_msg_t));
    SET_BIT(CAN1->IER, CAN_IER_FMPIE0);
    NVIC_SetPriority(CAN1_RX0_IRQn, NVIC_Priority_MIN-10U);
    NVIC_EnableIRQ(CAN1_RX0_IRQn);
    for(;;){
        while(xStreamBufferBytesAvailable(rx_stream) > 0){
            can_msg_t msg;
            xStreamBufferReceive(rx_stream, (void*)&msg, sizeof(can_msg_t), 10);
            printf("id: %d\n", msg.id);
        }
        vTaskDelay(10);
    }
}

can_msg_t can_fetch(uint32_t id){
    
}


uint8_t can_fetch_byte(uint32_t id, uint8_t byte){

}

uint8_t can_fetch_len(uint32_t id){

}

uint32_t can_fetch_timestamp(uint32_t id){

}

bool can_check_timestamp(uint32_t id){

}

void  can_send_msg(can_msg_t tx_msg){

}
