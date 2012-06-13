
ARMGNU = arm-none-eabi


ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=arm1176jzf-s -march=armv6
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=arm1176jzf-s -mtune=arm1176jzf-s -mhard-float

all : float02.bin 

vectors.o : vectors.s
	$(ARMGNU)-as $(AOPS) -mfpu=vfp vectors.s -o vectors.o

float02.o : float02.c slowfloat.c uart.h
	$(ARMGNU)-gcc $(COPS) -c float02.c -o float02.o

uart.o : uart.c uart.h
	$(ARMGNU)-gcc $(COPS) -c uart.c -o uart.o

float02.bin : memmap vectors.o uart.o float02.o
	$(ARMGNU)-ld -o float02.elf -T memmap vectors.o uart.o float02.o
	$(ARMGNU)-objdump -D float02.elf > float02.list
	$(ARMGNU)-objcopy float02.elf float02.bin -O binary


clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list


