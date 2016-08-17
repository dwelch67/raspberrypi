
ARMGNU ?= arm-none-eabi
#ARMGNU ?= arm-linux-gnueabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : ssd1306a.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.s

start.o : start.s
	$(ARMGNU)-as start.s -o start.o

ssd1306a.o : ssd1306a.c fontdata.h
	$(ARMGNU)-gcc $(COPS) -c ssd1306a.c -o ssd1306a.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

ssd1306a.bin : memmap start.o periph.o ssd1306a.o 
	$(ARMGNU)-ld start.o periph.o ssd1306a.o -T memmap -o ssd1306a.elf
	$(ARMGNU)-objdump -D ssd1306a.elf > ssd1306a.list
	$(ARMGNU)-objcopy ssd1306a.elf -O ihex ssd1306a.hex
	$(ARMGNU)-objcopy ssd1306a.elf -O binary ssd1306a.bin



