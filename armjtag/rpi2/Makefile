
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : armjtag.hex armjtag.bin 

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

armjtag.o : armjtag.c
	$(ARMGNU)-gcc $(COPS) -c armjtag.c -o armjtag.o

armjtag.elf : memmap vectors.o armjtag.o 
	$(ARMGNU)-ld vectors.o armjtag.o -T memmap -o armjtag.elf
	$(ARMGNU)-objdump -D armjtag.elf > armjtag.list

armjtag.bin : armjtag.elf
	$(ARMGNU)-objcopy armjtag.elf -O binary armjtag.bin

armjtag.hex : armjtag.elf
	$(ARMGNU)-objcopy armjtag.elf -O ihex armjtag.hex


