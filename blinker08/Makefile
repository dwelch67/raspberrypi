
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : blinker08.hex blinker08.bin

all : gcc 

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.opt.s

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

blinker08.o : blinker08.c
	$(ARMGNU)-gcc $(COPS) -c blinker08.c -o blinker08.o

blinker08.elf : memmap vectors.o blinker08.o 
	$(ARMGNU)-ld vectors.o blinker08.o -T memmap -o blinker08.elf
	$(ARMGNU)-objdump -D blinker08.elf > blinker08.list

blinker08.bin : blinker08.elf
	$(ARMGNU)-objcopy blinker08.elf -O binary blinker08.bin

blinker08.hex : blinker08.elf
	$(ARMGNU)-objcopy blinker08.elf -O ihex blinker08.hex






