
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : svc01.hex svc01.bin

all : gcc 

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.opt.s

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

svc01.o : svc01.c
	$(ARMGNU)-gcc $(COPS) -c svc01.c -o svc01.o

svc01.elf : memmap vectors.o svc01.o 
	$(ARMGNU)-ld vectors.o svc01.o -T memmap -o svc01.elf
	$(ARMGNU)-objdump -D svc01.elf > svc01.list

svc01.bin : svc01.elf
	$(ARMGNU)-objcopy svc01.elf -O binary svc01.bin

svc01.hex : svc01.elf
	$(ARMGNU)-objcopy svc01.elf -O ihex svc01.hex






