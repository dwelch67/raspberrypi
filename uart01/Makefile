
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : uart01.hex uart01.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

uart01.o : uart01.c
	$(ARMGNU)-gcc $(COPS) -c uart01.c -o uart01.o

uart01.elf : memmap novectors.o uart01.o 
	$(ARMGNU)-ld novectors.o uart01.o -T memmap -o uart01.elf
	$(ARMGNU)-objdump -D uart01.elf > uart01.list

uart01.bin : uart01.elf
	$(ARMGNU)-objcopy uart01.elf -O binary uart01.bin

uart01.hex : uart01.elf
	$(ARMGNU)-objcopy uart01.elf -O ihex uart01.hex




