
ARMGNU ?= arm-none-eabi
#ARMGNU ?= arm-linux-gnueabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : asmdelay.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.s

start.o : start.s
	$(ARMGNU)-as start.s -o start.o

asmdelay.o : asmdelay.c
	$(ARMGNU)-gcc $(COPS) -c asmdelay.c -o asmdelay.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

asmdelay.bin : memmap start.o periph.o asmdelay.o 
	$(ARMGNU)-ld start.o periph.o asmdelay.o -T memmap -o asmdelay.elf
	$(ARMGNU)-objdump -D asmdelay.elf > asmdelay.list
	$(ARMGNU)-objcopy asmdelay.elf -O ihex asmdelay.hex
	$(ARMGNU)-objcopy asmdelay.elf -O binary asmdelay.bin



