// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include <sys/stat.h>
#include <stm32f4xx.h>
#include "drivers/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "string.h"

int _fstat(int fd, struct stat *st) {
  if (fd < 0) return -1;
  st->st_mode = S_IFCHR;
  return 0;
}

// my _sbrk function -> works with my linker script
// void *_sbrk(int incr) {
//   extern char _end;
//   static unsigned char *heap = NULL;
//   unsigned char *prev_heap;
//   if (heap == NULL) heap = (unsigned char *) &_end;
//   prev_heap = heap;
//   heap += incr;
//   return prev_heap;
// }

// STM Cube _sbrk function
static uint8_t *__sbrk_heap_end = NULL;
void *_sbrk(ptrdiff_t incr)
{
  extern uint8_t _end; /* Symbol defined in the linker script */
  extern uint8_t _estack; /* Symbol defined in the linker script */
  extern uint32_t _Min_Stack_Size; /* Symbol defined in the linker script */
  const uint32_t stack_limit = (uint32_t)&_estack - (uint32_t)&_Min_Stack_Size;
  const uint8_t *max_heap = (uint8_t *)stack_limit;
  uint8_t *prev_heap_end;

  /* Initialize heap end at first call */
  if (NULL == __sbrk_heap_end)
  {
    __sbrk_heap_end = &_end;
  }

  /* Protect heap from growing into the reserved MSP stack */
  if (__sbrk_heap_end + incr > max_heap)
  {
    return (void *)-1;
  }

  prev_heap_end = __sbrk_heap_end;
  __sbrk_heap_end += incr;

  return (void *)prev_heap_end;
}

int _open(const char *path) {
  (void) path;
  return -1;
}

int _close(int fd) {
  (void) fd;
  return -1;
}

int _isatty(int fd) {
  (void) fd;
  return 1;
}

int _lseek(int fd, int ptr, int dir) {
  (void) fd, (void) ptr, (void) dir;
  return 0;
}

void _exit(int status) {
  (void) status;
  for (;;) __asm__ volatile("BKPT #0");
}

void _kill(int pid, int sig) {
  (void) pid, (void) sig;
}

int _getpid(void) {
  return -1;
}

int _write(int fd, char *ptr, int len) {
  (void) fd, (void) ptr, (void) len;
  if (fd == 1 || fd == 2){
    #ifdef DEBUG_PRINTF_TASK_NAME
    char * callerID = NULL;
    // Get the name of the task calling printf - Only run if scheduler has been started
    if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) callerID = pcTaskGetName(NULL);
    #endif
    if(Serial2.pUART == NULL)          return -1;
    if(Serial2.writeHandler == NULL)     return -1;
    // Take over the debug usart
    if(xSemaphoreTake(Serial2.writeHandler, (TickType_t) 10) == pdTRUE){
      #ifdef DEBUG_PRINTF_TASK_NAME
      // Write caller ID, followed by ": ", then the argument given to printf
      if(callerID != NULL){
        hal_uart_write_buf(Serial2.pUART, callerID, strlen(callerID));
        hal_uart_write_buf(Serial2.pUART, ": ", 3);
      }
      #endif
      hal_uart_write_buf(Serial2.pUART, ptr, (size_t) len);
      xSemaphoreGive(Serial2.writeHandler);
    }
    // hal_uart_write_buf(UART_DEBUG, ptr, (size_t) len);
  } //hal_uart_write_buf(UART_DEBUG, ptr, (size_t) len);
  return -1;
}

int _read(int fd, char *ptr, int len) {
  (void) fd, (void) ptr, (void) len;
  // There is no filesystem
  return -1;
}

int _link(const char *a, const char *b) {
  (void) a, (void) b;
  return -1;
}

int _unlink(const char *a) {
  (void) a;
  return -1;
}

int _stat(const char *path, struct stat *st) {
  (void) path, (void) st;
  return -1;
}

int mkdir(const char *path, mode_t mode) {
  (void) path, (void) mode;
  return -1;
}

// void _init(void) {
// }
