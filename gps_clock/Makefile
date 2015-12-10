
ARMGNU ?= arm-none-eabi

COPS = -mthumb -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : notmain.hex notmain.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

notmain.o : notmain.c fontdata.h
	$(ARMGNU)-gcc $(COPS) -c notmain.c -o notmain.o

notmain.elf : memmap novectors.o notmain.o 
	$(ARMGNU)-ld novectors.o notmain.o -T memmap -o notmain.elf
	$(ARMGNU)-objdump -D notmain.elf > notmain.list

notmain.bin : notmain.elf
	$(ARMGNU)-objcopy notmain.elf -O binary notmain.bin

notmain.hex : notmain.elf
	$(ARMGNU)-objcopy notmain.elf -O ihex notmain.hex


