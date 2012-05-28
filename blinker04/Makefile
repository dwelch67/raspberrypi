
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : blinker04.hex blinker04.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

blinker04.o : blinker04.c
	$(ARMGNU)-gcc $(COPS) -c blinker04.c -o blinker04.o

blinker04.elf : memmap novectors.o blinker04.o 
	$(ARMGNU)-ld novectors.o blinker04.o -T memmap -o blinker04.elf
	$(ARMGNU)-objdump -D blinker04.elf > blinker04.list

blinker04.bin : blinker04.elf
	$(ARMGNU)-objcopy blinker04.elf -O binary blinker04.bin

blinker04.hex : blinker04.elf
	$(ARMGNU)-objcopy blinker04.elf -O ihex blinker04.hex

