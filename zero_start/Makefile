
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : uart02.bin blinker05.bin

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

uart02.o : uart02.c
	$(ARMGNU)-gcc $(COPS) -c uart02.c -o uart02.o

uart02.elf : memmap vectors.o uart02.o 
	$(ARMGNU)-ld vectors.o uart02.o -T memmap -o uart02.elf
	$(ARMGNU)-objdump -D uart02.elf > uart02.list

uart02.bin : uart02.elf
	$(ARMGNU)-objcopy uart02.elf -O binary uart02.bin

uart02.hex : uart02.elf
	$(ARMGNU)-objcopy uart02.elf -O ihex uart02.hex





bvectors.o : bvectors.s
	$(ARMGNU)-as bvectors.s -o bvectors.o

blinker05.o : blinker05.c
	$(ARMGNU)-gcc $(COPS) -c blinker05.c -o blinker05.o

blinker05.elf : memmap bvectors.o blinker05.o 
	$(ARMGNU)-ld bvectors.o blinker05.o -T memmap -o blinker05.elf
	$(ARMGNU)-objdump -D blinker05.elf > blinker05.list

blinker05.bin : blinker05.elf
	$(ARMGNU)-objcopy blinker05.elf -O binary blinker05.bin

blinker05.hex : blinker05.elf
	$(ARMGNU)-objcopy blinker05.elf -O ihex blinker05.hex

