
ARMGNU ?= aarch64-none-elf

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : uart03.bin

all : gcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list


vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart03.o : uart03.c
	$(ARMGNU)-gcc $(COPS) -c uart03.c -o uart03.o

periph7.o : periph.c 
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph7.o

uart03.bin : memmap vectors.o periph7.o uart03.o 
	$(ARMGNU)-ld vectors.o periph7.o uart03.o -T memmap -o uart03.elf
	$(ARMGNU)-objdump -D uart03.elf > uart03.list
	$(ARMGNU)-objcopy uart03.elf -O ihex uart03.hex
	$(ARMGNU)-objcopy uart03.elf -O binary uart03.bin


