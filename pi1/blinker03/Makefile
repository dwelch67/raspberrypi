
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : blinker03.hex blinker03.bin

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

blinker03.o : blinker03.c
	$(ARMGNU)-gcc $(COPS) -c blinker03.c -o blinker03.o

blinker03.elf : memmap vectors.o blinker03.o 
	$(ARMGNU)-ld vectors.o blinker03.o -T memmap -o blinker03.elf
	$(ARMGNU)-objdump -D blinker03.elf > blinker03.list

blinker03.bin : blinker03.elf
	$(ARMGNU)-objcopy blinker03.elf -O binary blinker03.bin

blinker03.hex : blinker03.elf
	$(ARMGNU)-objcopy blinker03.elf -O ihex blinker03.hex







LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=arm -mcpu=arm1176jzf-s
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : blinker03.clang.hex blinker03.clang.bin


blinker03.clang.bc : blinker03.c
	clang $(LOPS) -c blinker03.c -o blinker03.clang.bc

blinker03.clang.opt.elf : memmap vectors.o blinker03.clang.bc
	opt $(OOPS) blinker03.clang.bc -o blinker03.clang.opt.bc
	llc $(LLCOPS) blinker03.clang.opt.bc -o blinker03.clang.opt.s
	$(ARMGNU)-as blinker03.clang.opt.s -o blinker03.clang.opt.o
	$(ARMGNU)-ld -o blinker03.clang.opt.elf -T memmap vectors.o blinker03.clang.opt.o
	$(ARMGNU)-objdump -D blinker03.clang.opt.elf > blinker03.clang.opt.list

blinker03.clang.hex : blinker03.clang.opt.elf
	$(ARMGNU)-objcopy blinker03.clang.opt.elf blinker03.clang.hex -O ihex

blinker03.clang.bin : blinker03.clang.opt.elf
	$(ARMGNU)-objcopy blinker03.clang.opt.elf blinker03.clang.bin -O binary


