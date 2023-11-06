cmake_minimum_required(VERSION 3.24)

#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
include(./gcc-arm-none-eabi.cmake)
set(CMAKE_SYSTEM_NAME Generic)

project(blinky-led)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/core)

set(MCU_FAMILY STM32F4xx)
set(MCU_MODEL STM32F446xx)
set(CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    # FPU MUST be enabled for this to work
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    )

set(STARTUP_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/vendor/startup_stm32f446retx.s)
set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/vendor/STM32F446RETX_FLASH.ld)

set(EXECUTABLE ${CMAKE_PROJECT_NAME})
enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis_core/CMSIS/Core/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/cmsis_f4/Include
    ${PROJECT_DIR}/include
)

file(GLOB_RECURSE PROJECT_SOURCES FOLLOW_SYMLINKS
    ${PROJECT_DIR}/src/*.c)

add_executable(${EXECUTABLE}
    ${PROJECT_SOURCES}
    ${STARTUP_SCRIPT})


FILE(GLOB FreeRTOS_src FreeRTOS-Kernel/*.c)
add_library( FreeRTOS STATIC
    ${FreeRTOS_src}
    FreeRTOS-Kernel/portable/GCC/ARM_CM3/port.c
    FreeRTOS-Kernel/portable/MemMang/heap_4.c)
target_include_directories( FreeRTOS PUBLIC
    FreeRTOS-Kernel/portable/GCC/ARM_CM3
    FreeRTOS-Kernel/include
    core/include/)



target_include_directories(${EXECUTABLE} PRIVATE
    ${INCLUDE_DIRECTORIES}
    )

target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    # -Wall
    # -Wextra
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
    # -Wall
    --specs=nano.specs
    -lgcc
    -Wl,--gc-sections
    -Wl,-L./ld
    # Printf floats can be removed in production code
    #-Wl,-u_printf_float
    -Wl,-Map=${PROJECT_NAME}.map
    -Wl,--print-memory-usage
    )
target_link_libraries(${EXECUTABLE}
    FreeRTOS)
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> firmware.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> firmware.bin)
