
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : kernel.img blinker.bin

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.hex

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

bootloader02.o : bootloader02.c
	$(ARMGNU)-gcc $(COPS) -c bootloader02.c -o bootloader02.o

kernel.img : loader novectors.o bootloader02.o 
	$(ARMGNU)-ld novectors.o bootloader02.o -T loader -o bootloader02.elf
	$(ARMGNU)-objdump -D bootloader02.elf > bootloader02.list
	$(ARMGNU)-objcopy bootloader02.elf -O ihex bootloader02.hex
	$(ARMGNU)-objcopy bootloader02.elf -O binary kernel.img



start.o : start.s
	$(ARMGNU)-as start.s -o start.o

blinker.o : blinker.c
	$(ARMGNU)-gcc $(COPS) -c blinker.c -o blinker.o

blinker.bin : memmap start.o blinker.o 
	$(ARMGNU)-ld start.o blinker.o -T memmap -o blinker.elf
	$(ARMGNU)-objdump -D blinker.elf > blinker.list
	$(ARMGNU)-objcopy blinker.elf -O ihex blinker.hex
	$(ARMGNU)-objcopy blinker.elf -O binary blinker.bin


