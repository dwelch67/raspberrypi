
ARMGNU ?= arm-none-eabi
#ARMGNU ?= arm-linux-gnueabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : uart05.bin

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

uart05.o : uart05.c
	$(ARMGNU)-gcc $(COPS) -c uart05.c -o uart05.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

uart05.bin : memmap start.o periph.o uart05.o 
	$(ARMGNU)-ld start.o periph.o uart05.o -T memmap -o uart05.elf
	$(ARMGNU)-objdump -D uart05.elf > uart05.list
	$(ARMGNU)-objcopy uart05.elf -O ihex uart05.hex
	$(ARMGNU)-objcopy uart05.elf -O binary uart05.bin



