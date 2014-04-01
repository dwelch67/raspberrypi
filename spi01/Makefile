
ARMGNU ?= arm-none-eabi

COPS = -mthumb -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : spi01.hex spi01.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

spi01.o : spi01.c
	$(ARMGNU)-gcc $(COPS) -c spi01.c -o spi01.o

spi01.elf : memmap novectors.o spi01.o 
	$(ARMGNU)-ld novectors.o spi01.o -T memmap -o spi01.elf
	$(ARMGNU)-objdump -D spi01.elf > spi01.list

spi01.bin : spi01.elf
	$(ARMGNU)-objcopy spi01.elf -O binary spi01.bin

spi01.hex : spi01.elf
	$(ARMGNU)-objcopy spi01.elf -O ihex spi01.hex


