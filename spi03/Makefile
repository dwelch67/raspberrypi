
ARMGNU ?= arm-none-eabi

COPS = -mthumb -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : spi03.hex spi03.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

spi03.o : spi03.c fontdata.h
	$(ARMGNU)-gcc $(COPS) -c spi03.c -o spi03.o

spi03.elf : memmap novectors.o spi03.o 
	$(ARMGNU)-ld novectors.o spi03.o -T memmap -o spi03.elf
	$(ARMGNU)-objdump -D spi03.elf > spi03.list

spi03.bin : spi03.elf
	$(ARMGNU)-objcopy spi03.elf -O binary spi03.bin

spi03.hex : spi03.elf
	$(ARMGNU)-objcopy spi03.elf -O ihex spi03.hex


