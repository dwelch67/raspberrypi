
ARMGNU ?= aarch64-none-elf

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : uart04.bin

all : gcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list


vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart04.o : uart04.c
	$(ARMGNU)-gcc $(COPS) -c uart04.c -o uart04.o

periph7.o : periph.c 
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph7.o

uart04.bin : memmap vectors.o periph7.o uart04.o 
	$(ARMGNU)-ld vectors.o periph7.o uart04.o -T memmap -o uart04.elf
	$(ARMGNU)-objdump -D uart04.elf > uart04.list
	$(ARMGNU)-objcopy uart04.elf -O ihex uart04.hex
	$(ARMGNU)-objcopy uart04.elf -O binary uart04.bin


