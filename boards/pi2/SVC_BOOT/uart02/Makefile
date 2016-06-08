
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : uart02.bin 

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.srec
	rm -f *.img

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

uart02.o : uart02.c
	$(ARMGNU)-gcc $(COPS) -c uart02.c -o uart02.o

periph.o : periph.c 
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

uart02.bin : loader vectors.o periph.o uart02.o 
	$(ARMGNU)-ld vectors.o periph.o uart02.o -T loader -o uart02.elf
	$(ARMGNU)-objdump -D uart02.elf > uart02.list
	$(ARMGNU)-objcopy uart02.elf -O ihex uart02.hex
	$(ARMGNU)-objcopy uart02.elf -O srec --srec-forceS3 uart02.srec
	$(ARMGNU)-objcopy uart02.elf -O binary uart02.bin


