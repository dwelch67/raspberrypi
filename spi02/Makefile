
ARMGNU ?= arm-none-eabi

COPS = -mthumb -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : spi02.hex spi02.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

spi02.o : spi02.c fontdata.h
	$(ARMGNU)-gcc $(COPS) -c spi02.c -o spi02.o

spi02.elf : memmap novectors.o spi02.o 
	$(ARMGNU)-ld novectors.o spi02.o -T memmap -o spi02.elf
	$(ARMGNU)-objdump -D spi02.elf > spi02.list

spi02.bin : spi02.elf
	$(ARMGNU)-objcopy spi02.elf -O binary spi02.bin

spi02.hex : spi02.elf
	$(ARMGNU)-objcopy spi02.elf -O ihex spi02.hex


