
ARMGNU = arm-none-eabi


ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=arm1176jzf-s -march=armv6
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=arm1176jzf-s -mtune=arm1176jzf-s -mhard-float

all : float03.bin 

vectors.o : vectors.s
	$(ARMGNU)-as $(AOPS) -mfpu=vfp vectors.s -o vectors.o

float03.o : float03.c uart.h
	$(ARMGNU)-gcc $(COPS) -c float03.c -o float03.o

uart.o : uart.c uart.h
	$(ARMGNU)-gcc $(COPS) -c uart.c -o uart.o

float03.bin : memmap vectors.o uart.o float03.o
	$(ARMGNU)-ld -o float03.elf -T memmap vectors.o uart.o float03.o
	$(ARMGNU)-objdump -D float03.elf > float03.list
	$(ARMGNU)-objcopy float03.elf float03.bin -O binary

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list


