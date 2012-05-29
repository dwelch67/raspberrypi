
ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 



gcc : blinker01.hex blinker01.bin

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

blinker01.o : blinker01.c
	$(ARMGNU)-gcc $(COPS) -c blinker01.c -o blinker01.o

blinker01.elf : memmap vectors.o blinker01.o 
	$(ARMGNU)-ld vectors.o blinker01.o -T memmap -o blinker01.elf
	$(ARMGNU)-objdump -D blinker01.elf > blinker01.list

blinker01.bin : blinker01.elf
	$(ARMGNU)-objcopy blinker01.elf -O binary blinker01.bin

blinker01.hex : blinker01.elf
	$(ARMGNU)-objcopy blinker01.elf -O ihex blinker01.hex






LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=arm -mcpu=arm1176jzf-s
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : blinker01.clang.hex blinker01.clang.bin


blinker01.clang.bc : blinker01.c
	clang $(LOPS) -c blinker01.c -o blinker01.clang.bc

blinker01.clang.opt.elf : memmap vectors.o blinker01.clang.bc
	opt $(OOPS) blinker01.clang.bc -o blinker01.clang.opt.bc
	llc $(LLCOPS) blinker01.clang.opt.bc -o blinker01.clang.opt.s
	$(ARMGNU)-as blinker01.clang.opt.s -o blinker01.clang.opt.o
	$(ARMGNU)-ld -o blinker01.clang.opt.elf -T memmap vectors.o blinker01.clang.opt.o
	$(ARMGNU)-objdump -D blinker01.clang.opt.elf > blinker01.clang.opt.list

blinker01.clang.hex : blinker01.clang.opt.elf
	$(ARMGNU)-objcopy blinker01.clang.opt.elf blinker01.clang.hex -O ihex

blinker01.clang.bin : blinker01.clang.opt.elf
	$(ARMGNU)-objcopy blinker01.clang.opt.elf blinker01.clang.bin -O binary


