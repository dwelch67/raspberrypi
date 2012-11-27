
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : blinker07.hex blinker07.bin

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

blinker07.o : blinker07.c
	$(ARMGNU)-gcc $(COPS) -c blinker07.c -o blinker07.o

blinker07.elf : memmap vectors.o blinker07.o 
	$(ARMGNU)-ld vectors.o blinker07.o -T memmap -o blinker07.elf
	$(ARMGNU)-objdump -D blinker07.elf > blinker07.list

blinker07.bin : blinker07.elf
	$(ARMGNU)-objcopy blinker07.elf -O binary blinker07.bin

blinker07.hex : blinker07.elf
	$(ARMGNU)-objcopy blinker07.elf -O ihex blinker07.hex






