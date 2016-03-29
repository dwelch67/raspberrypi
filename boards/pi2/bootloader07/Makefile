
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : kernel7.img 

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.s


vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

bootloader07.o : bootloader07.c
	$(ARMGNU)-gcc $(COPS) -c bootloader07.c -o bootloader07.o

periph.o : periph.c 
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

kernel7.img : loader vectors.o periph.o bootloader07.o 
	$(ARMGNU)-ld vectors.o periph.o bootloader07.o -T loader -o bootloader07.elf
	$(ARMGNU)-objdump -D bootloader07.elf > bootloader07.list
	$(ARMGNU)-objcopy bootloader07.elf -O ihex bootloader07.hex
	$(ARMGNU)-objcopy bootloader07.elf -O binary kernel7.img


