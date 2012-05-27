
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : kernel.img blinker.hex prograspi

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.hex
	rm -f prograspi

novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

bootloader01.o : bootloader01.c
	$(ARMGNU)-gcc $(COPS) -c bootloader01.c -o bootloader01.o

kernel.img : loader novectors.o bootloader01.o 
	$(ARMGNU)-ld novectors.o bootloader01.o -T loader -o bootloader01.elf
	$(ARMGNU)-objdump -D bootloader01.elf > bootloader01.list
	$(ARMGNU)-objcopy bootloader01.elf -O binary kernel.img



start.o : start.s
	$(ARMGNU)-as start.s -o start.o

blinker.o : blinker.c
	$(ARMGNU)-gcc $(COPS) -c blinker.c -o blinker.o

blinker.hex : memmap start.o blinker.o 
	$(ARMGNU)-ld start.o blinker.o -T memmap -o blinker.elf
	$(ARMGNU)-objdump -D blinker.elf > blinker.list
	$(ARMGNU)-objcopy blinker.elf -O binary blinker.bin
	$(ARMGNU)-objcopy blinker.elf -O ihex blinker.hex


prograspi : prograspi.c ser.c ser.h
	gcc -O2 -Wall prograspi.c ser.c -o prograspi


