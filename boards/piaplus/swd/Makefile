
ARMGNU ?= arm-none-eabi
#ARMGNU ?= arm-linux-gnueabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : swd.bin

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

swd.o : swd.c
	$(ARMGNU)-gcc $(COPS) -c swd.c -o swd.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

swd.bin : memmap start.o periph.o swd.o 
	$(ARMGNU)-ld start.o periph.o swd.o -T memmap -o swd.elf
	$(ARMGNU)-objdump -D swd.elf > swd.list
	$(ARMGNU)-objcopy swd.elf -O ihex swd.hex
	$(ARMGNU)-objcopy swd.elf -O binary swd.bin



