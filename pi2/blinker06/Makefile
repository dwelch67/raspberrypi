
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : blinker06.hex blinker06.bin

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

blinker06.o : blinker06.c
	$(ARMGNU)-gcc $(COPS) -c blinker06.c -o blinker06.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

wdog.o : wdog.c
	$(ARMGNU)-gcc $(COPS) -c wdog.c -o wdog.o

blinker06.elf : memmap vectors.o periph.o wdog.o blinker06.o 
	$(ARMGNU)-ld vectors.o periph.o wdog.o blinker06.o -T memmap -o blinker06.elf
	$(ARMGNU)-objdump -D blinker06.elf > blinker06.list

blinker06.bin : blinker06.elf
	$(ARMGNU)-objcopy blinker06.elf -O binary blinker06.bin

blinker06.hex : blinker06.elf
	$(ARMGNU)-objcopy blinker06.elf -O ihex blinker06.hex






