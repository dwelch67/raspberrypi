
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : kernel.img 

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img



vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

atags.o : atags.c
	$(ARMGNU)-gcc $(COPS) -c atags.c -o atags.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

kernel.img : memmap vectors.o periph.o atags.o 
	$(ARMGNU)-ld vectors.o periph.o atags.o -T memmap -o atags.elf
	$(ARMGNU)-objdump -D atags.elf > atags.list
	$(ARMGNU)-objcopy atags.elf -O ihex atags.hex
	$(ARMGNU)-objcopy atags.elf -O binary kernel.img


