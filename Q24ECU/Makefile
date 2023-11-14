# Jacob Chisholm
# Makefile for uploading bare metal code
# to STMF446 with BlackMagic Probe + GDB

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