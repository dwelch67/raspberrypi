
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostartfiles -ffreestanding

LIB = -L /opt/gnuarm/arm-none-eabi/lib/ -L/opt/gnuarm/lib/gcc/arm-none-eabi/4.7.2

gcc : uart02.hex uart02.bin

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

uart02.o : uart02.c
	$(ARMGNU)-gcc $(COPS) -c uart02.c -o uart02.o

uart02.elf : memmap vectors.o uart02.o syscalls.o
	$(ARMGNU)-ld vectors.o uart02.o syscalls.o -T memmap -o uart02.elf $(LIB) -lc -lgcc
	$(ARMGNU)-objdump -D uart02.elf > uart02.list

uart02.bin : uart02.elf
	$(ARMGNU)-objcopy uart02.elf -O binary uart02.bin

uart02.hex : uart02.elf
	$(ARMGNU)-objcopy uart02.elf -O ihex uart02.hex

syscalls.o : syscalls.c
	$(ARMGNU)-gcc $(COPS) -c $(COPS) syscalls.c -o syscalls.o






