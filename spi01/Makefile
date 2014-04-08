
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
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f blinker01.s.*
	rm -f blinker02.s.*
	rm -f dumphex

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

spi01.o : spi01.c blinker.h
	$(ARMGNU)-gcc $(COPS) -c spi01.c -o spi01.o

spi01.elf : memmap novectors.o spi01.o 
	$(ARMGNU)-ld novectors.o spi01.o -T memmap -o spi01.elf
	$(ARMGNU)-objdump -D spi01.elf > spi01.list

spi01.bin : spi01.elf
	$(ARMGNU)-objcopy spi01.elf -O binary spi01.bin

spi01.hex : spi01.elf
	$(ARMGNU)-objcopy spi01.elf -O ihex spi01.hex


dumphex : dumphex.c
	gcc -o dumphex dumphex.c

blinker.h : blinker02.s 
	gcc -o dumphex dumphex.c
	avra -fI blinker02.s
	./dumphex blinker02.s.hex


blinker01.s.hex : blinker01.s
	avra -fI blinker01.s

blinker02.s.hex : blinker02.s
	avra -fI blinker02.s


