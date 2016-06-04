
ARMGNU ?= aarch64-none-elf

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : kernel7.img

all : gcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

bootloader07.o : bootloader07.c
	$(ARMGNU)-gcc $(COPS) -c bootloader07.c -o bootloader07.o

periph7.o : periph.c 
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph7.o

kernel7.img : loader vectors.o periph7.o bootloader07.o 
	$(ARMGNU)-ld vectors.o periph7.o bootloader07.o -T loader -o bootloader07.elf
	$(ARMGNU)-objdump -D bootloader07.elf > bootloader07.list
	$(ARMGNU)-objcopy bootloader07.elf -O ihex bootloader07.hex
	$(ARMGNU)-objcopy bootloader07.elf -O binary kernel7.img


