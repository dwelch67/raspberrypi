
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : bssdata.hex bssdata.bin fun.list

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

bssdata.o : bssdata.c
	$(ARMGNU)-gcc $(COPS) -c bssdata.c -o bssdata.o

bssdata.elf : memmap vectors.o bssdata.o 
	$(ARMGNU)-ld vectors.o bssdata.o -T memmap -o bssdata.elf
	$(ARMGNU)-objdump -D bssdata.elf > bssdata.list

bssdata.bin : bssdata.elf
	$(ARMGNU)-objcopy bssdata.elf -O binary bssdata.bin

bssdata.hex : bssdata.elf
	$(ARMGNU)-objcopy bssdata.elf -O ihex bssdata.hex


fun.list : start.s simple fun.c
	$(ARMGNU)-as start.s -o start.o
	$(ARMGNU)-gcc $(COPS) -c fun.c -o fun.o
	$(ARMGNU)-ld -T simple start.o fun.o -o fun.elf
	$(ARMGNU)-objdump -D fun.elf > fun.list

