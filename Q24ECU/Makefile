# Jacob Chisholm
# Makefile for uploading bare metal code
# to STMF446 with BlackMagic Probe + GDB

.PHONY: all build cmake clean format

BUILD_DIR := build
BUILD_TYPE ?= Debug

all: build

${BUILD_DIR}/Makefile:
	cmake \
		-B${BUILD_DIR} \
		-DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
		-DCMAKE_TOOLCHAIN_FILE=gcc-arm-none-eabi.cmake \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \

cmake: ${BUILD_DIR}/Makefile

build: cmake
	$(MAKE) -C ${BUILD_DIR} --no-print-directory

SRCS := $(shell find . -name '*.[ch]' -or -name '*.[ch]pp')
%.format: %
	clang-format -i $<
format: $(addsuffix .format, ${SRCS})

clean:
	rm -rf $(BUILD_DIR)

# use this line to select the flash tool
flash: st-flash


# must run "run" command or restart target to start program
BlackMagic-flash:
	gdb -ex 'target extended-remote $(COMPORT)' \
	-ex 'set mem inaccessible-by-default off' \
	-ex 'monitor swdp' \
	-ex 'attach 1' \
	-ex 'load firmware.elf' \
	-q

st-flash: build/firmware.bin
	./Tools/ST-LINK_CLI.exe -P build/firmware.bin 0x08000000 -V -RST

clean:
	rm -r build

cmsis_core:
	git clone --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@
  
cmsis_f4: 
	git clone --depth 1 -b v2.6.8 https://github.com/STMicroelectronics/cmsis_device_f4 $@
