
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : blinker01.hex blinker01.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

blinker01.o : blinker01.c
	$(ARMGNU)-gcc $(COPS) -c blinker01.c -o blinker01.o

blinker01.elf : memmap novectors.o blinker01.o 
	$(ARMGNU)-ld novectors.o blinker01.o -T memmap -o blinker01.elf
	$(ARMGNU)-objdump -D blinker01.elf > blinker01.list

blinker01.bin : blinker01.elf
	$(ARMGNU)-objcopy blinker01.elf -O binary blinker01.bin

blinker01.hex : blinker01.elf
	$(ARMGNU)-objcopy blinker01.elf -O ihex blinker01.hex






