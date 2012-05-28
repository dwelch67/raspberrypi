
ARMGNU ?= arm-none-eabi

COPS = -mthumb -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : uart03.hex uart03.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

uart03.o : uart03.c
	$(ARMGNU)-gcc $(COPS) -c uart03.c -o uart03.o

uart03.elf : memmap novectors.o uart03.o 
	$(ARMGNU)-ld novectors.o uart03.o -T memmap -o uart03.elf
	$(ARMGNU)-objdump -D uart03.elf > uart03.list

uart03.bin : uart03.elf
	$(ARMGNU)-objcopy uart03.elf -O binary uart03.bin

uart03.hex : uart03.elf
	$(ARMGNU)-objcopy uart03.elf -O ihex uart03.hex


