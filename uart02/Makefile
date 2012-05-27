
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

uart02.o : uart02.c
	$(ARMGNU)-gcc $(COPS) -c uart02.c -o uart02.o

kernel.img : memmap novectors.o uart02.o 
	$(ARMGNU)-ld novectors.o uart02.o -T memmap -o uart02.elf
	$(ARMGNU)-objdump -D uart02.elf > uart02.list
	$(ARMGNU)-objcopy uart02.elf -O binary kernel.img






