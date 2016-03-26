
ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 



gcc : blinker05.hex blinker05.bin

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

blinker05.o : blinker05.c
	$(ARMGNU)-gcc $(COPS) -c blinker05.c -o blinker05.o

blinker05.elf : memmap vectors.o blinker05.o 
	$(ARMGNU)-ld vectors.o blinker05.o -T memmap -o blinker05.elf
	$(ARMGNU)-objdump -D blinker05.elf > blinker05.list

blinker05.bin : blinker05.elf
	$(ARMGNU)-objcopy blinker05.elf -O binary blinker05.bin

blinker05.hex : blinker05.elf
	$(ARMGNU)-objcopy blinker05.elf -O ihex blinker05.hex






LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=arm -mcpu=arm1176jzf-s
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : blinker05.clang.hex blinker05.clang.bin


blinker05.clang.bc : blinker05.c
	clang $(LOPS) -c blinker05.c -o blinker05.clang.bc

blinker05.clang.opt.elf : memmap vectors.o blinker05.clang.bc
	opt $(OOPS) blinker05.clang.bc -o blinker05.clang.opt.bc
	llc $(LLCOPS) blinker05.clang.opt.bc -o blinker05.clang.opt.s
	$(ARMGNU)-as blinker05.clang.opt.s -o blinker05.clang.opt.o
	$(ARMGNU)-ld -o blinker05.clang.opt.elf -T memmap vectors.o blinker05.clang.opt.o
	$(ARMGNU)-objdump -D blinker05.clang.opt.elf > blinker05.clang.opt.list

blinker05.clang.hex : blinker05.clang.opt.elf
	$(ARMGNU)-objcopy blinker05.clang.opt.elf blinker05.clang.hex -O ihex

blinker05.clang.bin : blinker05.clang.opt.elf
	$(ARMGNU)-objcopy blinker05.clang.opt.elf blinker05.clang.bin -O binary


