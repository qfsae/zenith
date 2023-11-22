/**
 * @file uart2_reciever.c
 * @author Jacob Chisholm (https://jchisholm204.github.io/)
 * @brief USART 2 Receiver Task
 * @version 0.1
 * @date 2023-11-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "taskHandlers.h"
#include "main.h"

void tsk_USART2_Handler(void *param){
    (void)param;
    for(;;){
        uint8_t buf[64];
        size_t bytes = xStreamBufferReceive(debug.stream, (void*) &buf, 64, portMAX_DELAY);
        printf("%s (%d)\n", buf, bytes);
        // reset the stream buffer
        for (int i = 0; i < 64; i++)
        {
            buf[i] = 0;
        }
        
    }
}