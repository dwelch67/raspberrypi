
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

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

bootloader03.o : bootloader03.c
	$(ARMGNU)-gcc $(COPS) -c bootloader03.c -o bootloader03.o

kernel.img : loader vectors.o bootloader03.o 
	$(ARMGNU)-ld vectors.o bootloader03.o -T loader -o bootloader03.elf
	$(ARMGNU)-objdump -D bootloader03.elf > bootloader03.list
	$(ARMGNU)-objcopy bootloader03.elf -O ihex bootloader03.hex
	$(ARMGNU)-objcopy bootloader03.elf -O binary kernel.img



start.o : start.s
	$(ARMGNU)-as start.s -o start.o

blinker.o : blinker.c
	$(ARMGNU)-gcc $(COPS) -c blinker.c -o blinker.o

blinker.bin : memmap start.o blinker.o 
	$(ARMGNU)-ld start.o blinker.o -T memmap -o blinker.elf
	$(ARMGNU)-objdump -D blinker.elf > blinker.list
	$(ARMGNU)-objcopy blinker.elf -O ihex blinker.hex
	$(ARMGNU)-objcopy blinker.elf -O binary blinker.bin


