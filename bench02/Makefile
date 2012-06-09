
ARMGNU ?= arm-none-eabi

COPS = -mthumb-interwork -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : bench02.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

bench02.o : bench02.c
	$(ARMGNU)-gcc $(COPS) -c bench02.c -o bench02.o

uart.o : uart.c
	$(ARMGNU)-gcc $(COPS) -c uart.c -o uart.o

bench02.elf : memmap novectors.o bench02.o uart.o 
	$(ARMGNU)-ld -T memmap novectors.o bench02.o uart.o -o bench02.elf
	$(ARMGNU)-objdump -D bench02.elf > bench02.list

bench02.bin : bench02.elf
	$(ARMGNU)-objcopy bench02.elf -O binary bench02.bin


