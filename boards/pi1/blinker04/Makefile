
ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 



gcc : blinker04.hex blinker04.bin

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

blinker04.o : blinker04.c
	$(ARMGNU)-gcc $(COPS) -c blinker04.c -o blinker04.o

blinker04.elf : memmap vectors.o blinker04.o 
	$(ARMGNU)-ld vectors.o blinker04.o -T memmap -o blinker04.elf
	$(ARMGNU)-objdump -D blinker04.elf > blinker04.list

blinker04.bin : blinker04.elf
	$(ARMGNU)-objcopy blinker04.elf -O binary blinker04.bin

blinker04.hex : blinker04.elf
	$(ARMGNU)-objcopy blinker04.elf -O ihex blinker04.hex






LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=arm -mcpu=arm1176jzf-s
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : blinker04.clang.hex blinker04.clang.bin


blinker04.clang.bc : blinker04.c
	clang $(LOPS) -c blinker04.c -o blinker04.clang.bc

blinker04.clang.opt.elf : memmap vectors.o blinker04.clang.bc
	opt $(OOPS) blinker04.clang.bc -o blinker04.clang.opt.bc
	llc $(LLCOPS) blinker04.clang.opt.bc -o blinker04.clang.opt.s
	$(ARMGNU)-as blinker04.clang.opt.s -o blinker04.clang.opt.o
	$(ARMGNU)-ld -o blinker04.clang.opt.elf -T memmap vectors.o blinker04.clang.opt.o
	$(ARMGNU)-objdump -D blinker04.clang.opt.elf > blinker04.clang.opt.list

blinker04.clang.hex : blinker04.clang.opt.elf
	$(ARMGNU)-objcopy blinker04.clang.opt.elf blinker04.clang.hex -O ihex

blinker04.clang.bin : blinker04.clang.opt.elf
	$(ARMGNU)-objcopy blinker04.clang.opt.elf blinker04.clang.bin -O binary


