
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : kernel.img

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list
	rm -f *.img

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

blinker03.o : blinker03.c
	$(ARMGNU)-gcc $(COPS) -c blinker03.c -o blinker03.o

kernel.img : memmap novectors.o blinker03.o 
	$(ARMGNU)-ld novectors.o blinker03.o -T memmap -o blinker03.elf
	$(ARMGNU)-objdump -D blinker03.elf > blinker03.list
	$(ARMGNU)-objcopy blinker03.elf -O binary kernel.img






