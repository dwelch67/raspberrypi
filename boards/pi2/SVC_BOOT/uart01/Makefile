
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : uart01.bin 

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

uart01.o : uart01.c
	$(ARMGNU)-gcc $(COPS) -c uart01.c -o uart01.o

periph.o : periph.c 
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

uart01.bin : loader vectors.o periph.o uart01.o 
	$(ARMGNU)-ld vectors.o periph.o uart01.o -T loader -o uart01.elf
	$(ARMGNU)-objdump -D uart01.elf > uart01.list
	$(ARMGNU)-objcopy uart01.elf -O ihex uart01.hex
	$(ARMGNU)-objcopy uart01.elf -O srec --srec-forceS3 uart01.srec
	$(ARMGNU)-objcopy uart01.elf -O binary uart01.bin


