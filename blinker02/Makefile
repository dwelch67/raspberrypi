
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : blinker02.hex blinker02.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

blinker02.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -c blinker02.c -o blinker02.o

blinker02.elf : memmap novectors.o blinker02.o 
	$(ARMGNU)-ld novectors.o blinker02.o -T memmap -o blinker02.elf
	$(ARMGNU)-objdump -D blinker02.elf > blinker02.list

blinker02.bin : blinker02.elf
	$(ARMGNU)-objcopy blinker02.elf -O binary blinker02.bin

blinker02.hex : blinker02.elf
	$(ARMGNU)-objcopy blinker02.elf -O ihex blinker02.hex

