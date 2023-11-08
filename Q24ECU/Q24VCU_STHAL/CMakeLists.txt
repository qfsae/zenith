cmake_minimum_required(VERSION 3.24)

#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
include(./gcc-arm-none-eabi.cmake)
# set(CMAKE_SYSTEM_NAME Generic)

project(Q24VCU)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/core)
set(RTOS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source)
set(DRIVERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Drivers)

set(MCU_FAMILY STM32F4xx)
set(MCU_MODEL STM32F446xx)
set(CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    # FPU enable code must be written before it can be used
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    )

set(STARTUP_SCRIPT ${PROJECT_DIR}/Startup/startup_stm32f446retx.s)
set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/STM32F446RETX_FLASH.ld)

set(EXECUTABLE ${CMAKE_PROJECT_NAME})
enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(DRIVER_INCLUDE_DIRS
    ${PROJECT_DIR}/Inc
    ${DRIVERS_DIR}/CMSIS/Device/ST/STM32F4xx/Include
    ${DRIVERS_DIR}/CMSIS/Include
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc/Legacy
)

set(RTOS_INCLUDE_DIRS
    ${RTOS_DIR}/CMSIS_RTOS
    ${RTOS_DIR}/include
    ${RTOS_DIR}/portable/GCC/ARM_CM4F)

set(PROJECT_INCLUDE_DIRS
    ${PROJECT_DIR}/Inc)

file(GLOB RTOS_SOURCES
    ${RTOS_DIR}/*.c)

file(GLOB_RECURSE PROJECT_SOURCES FOLLOW_SYMLINKS
    ${PROJECT_DIR}/Src/*.c)

file(GLOB_RECURSE DRIVERS_SOURCES FOLLOW_SYMLINKS
    ${DRIVERS_DIR}/CMSIS/Device/ST/STM32F4xx/Source/*.c
    ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/*.c)

add_executable(${EXECUTABLE}
    ${RTOS_DIR}/portable/GCC/ARM_CM4F/port.c
    ${RTOS_DIR}/portable/MemMang/heap_4.c
    ${RTOS_DIR}/CMSIS_RTOS/cmsis_os.c
    ${RTOS_SOURCES}
    ${PROJECT_SOURCES}
    ${DRIVERS_SOURCES}
    ${STARTUP_SCRIPT})

target_include_directories(${EXECUTABLE} PRIVATE
    ${DRIVER_INCLUDE_DIRS}
    ${RTOS_INCLUDE_DIRS}
    ${PROJECT_INCLUDE_DIRS})

target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -g
    -fmessage-length=0
    -ffunction-sections
    -fdata-sections
    --specs=nano.specs
    #$<$<CONFIG:Debug>:-Os -g3>
    #$<$<CONFIG:Release>:-Os -g0>
    
    )

target_link_options(${EXECUTABLE} PRIVATE
    -T${MCU_LINKER_SCRIPT}
    ${CPU_PARAMETERS}
    #-nostartfiles
    -Wall
    --specs=nano.specs
    -lgcc
    -Wl,--gc-sections
    -Wl,-L./ld
    # Printf floats can be removed in production code
    -Wl,-u_printf_float
    -Wl,-Map=${PROJECT_NAME}.map
    -Wl,--print-memory-usage
    )

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> firmware.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> firmware.bin)