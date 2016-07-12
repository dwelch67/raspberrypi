
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : uart02.hex uart02.bin

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







