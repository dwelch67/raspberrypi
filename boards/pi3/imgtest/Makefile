
ARMGNU64 ?= aarch64-none-elf
ARMGNU32 ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : kernel8.img kernel8-32.img kernel7.img kernel.img

all : gcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img

kernel8.img : memmap bs64.s notmain.c
	$(ARMGNU64)-as bs64.s -o bs64.o
	$(ARMGNU64)-gcc $(COPS) -c -DDEFINEME=0x800 notmain.c -o notmain.o
	$(ARMGNU64)-ld bs64.o notmain.o -T memmap -o kernel8.elf
	$(ARMGNU64)-objdump -D kernel8.elf > kernel8.list
	$(ARMGNU64)-objcopy kernel8.elf -O binary kernel8.img

kernel8-32.img : memmap bs32.s notmain.c
	$(ARMGNU32)-as bs32.s -o bs32.o
	$(ARMGNU32)-gcc $(COPS) -c -DDEFINEME=0x832 notmain.c -o notmain.o
	$(ARMGNU32)-ld bs32.o notmain.o -T memmap -o kernel8-32.elf
	$(ARMGNU32)-objdump -D kernel8-32.elf > kernel8-32.list
	$(ARMGNU32)-objcopy kernel8-32.elf -O binary kernel8-32.img

kernel7.img : memmap bs32.s notmain.c
	$(ARMGNU32)-as bs32.s -o bs32.o
	$(ARMGNU32)-gcc $(COPS) -c -DDEFINEME=0x700 notmain.c -o notmain.o
	$(ARMGNU32)-ld bs32.o notmain.o -T memmap -o kernel7.elf
	$(ARMGNU32)-objdump -D kernel7.elf > kernel7.list
	$(ARMGNU32)-objcopy kernel7.elf -O binary kernel7.img

kernel.img : memmap bs32.s notmain.c
	$(ARMGNU32)-as bs32.s -o bs32.o
	$(ARMGNU32)-gcc $(COPS) -c -DDEFINEME=0x000 notmain.c -o notmain.o
	$(ARMGNU32)-ld bs32.o notmain.o -T memmap -o kernel.elf
	$(ARMGNU32)-objdump -D kernel.elf > kernel.list
	$(ARMGNU32)-objcopy kernel.elf -O binary kernel.img


