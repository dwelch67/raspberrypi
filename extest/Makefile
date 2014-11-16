


ARMGNU ?= arm-linux-gnueabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : notmain.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.hex
	rm -f *.list

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

notmain.o : notmain.c 
	$(ARMGNU)-gcc $(COPS) -c notmain.c -o notmain.o

uart.o : uart.c 
	$(ARMGNU)-gcc $(COPS) -c uart.c -o uart.o

notmain.bin : memmap vectors.o uart.o notmain.o 
	$(ARMGNU)-ld -T memmap vectors.o notmain.o uart.o -o notmain.elf
	$(ARMGNU)-objdump -D notmain.elf > notmain.list
	$(ARMGNU)-objcopy notmain.elf -O ihex notmain.hex 
	$(ARMGNU)-objcopy notmain.elf -O binary notmain.bin 








