
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

blinker02.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -c blinker02.c -o blinker02.o

kernel.img : memmap novectors.o blinker02.o 
	$(ARMGNU)-ld novectors.o blinker02.o -T memmap -o blinker02.elf
	$(ARMGNU)-objdump -D blinker02.elf > blinker02.list
	$(ARMGNU)-objcopy blinker02.elf -O binary kernel.img






