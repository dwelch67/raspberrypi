
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : blinker02.hex blinker02.bin

all : gcc clang

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

blinker02.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -c blinker02.c -o blinker02.o

blinker02.elf : memmap vectors.o blinker02.o 
	$(ARMGNU)-ld vectors.o blinker02.o -T memmap -o blinker02.elf
	$(ARMGNU)-objdump -D blinker02.elf > blinker02.list

blinker02.bin : blinker02.elf
	$(ARMGNU)-objcopy blinker02.elf -O binary blinker02.bin

blinker02.hex : blinker02.elf
	$(ARMGNU)-objcopy blinker02.elf -O ihex blinker02.hex






LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=arm -mcpu=arm1176jzf-s
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : blinker02.clang.hex blinker02.clang.bin


blinker02.clang.bc : blinker02.c
	clang $(LOPS) -c blinker02.c -o blinker02.clang.bc

blinker02.clang.opt.elf : memmap vectors.o blinker02.clang.bc
	opt $(OOPS) blinker02.clang.bc -o blinker02.clang.opt.bc
	llc $(LLCOPS) blinker02.clang.opt.bc -o blinker02.clang.opt.s
	$(ARMGNU)-as blinker02.clang.opt.s -o blinker02.clang.opt.o
	$(ARMGNU)-ld -o blinker02.clang.opt.elf -T memmap vectors.o blinker02.clang.opt.o
	$(ARMGNU)-objdump -D blinker02.clang.opt.elf > blinker02.clang.opt.list

blinker02.clang.hex : blinker02.clang.opt.elf
	$(ARMGNU)-objcopy blinker02.clang.opt.elf blinker02.clang.hex -O ihex

blinker02.clang.bin : blinker02.clang.opt.elf
	$(ARMGNU)-objcopy blinker02.clang.opt.elf blinker02.clang.bin -O binary

