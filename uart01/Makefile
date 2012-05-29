
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : uart01.hex uart01.bin

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

uart01.o : uart01.c
	$(ARMGNU)-gcc $(COPS) -c uart01.c -o uart01.o

uart01.elf : memmap vectors.o uart01.o 
	$(ARMGNU)-ld vectors.o uart01.o -T memmap -o uart01.elf
	$(ARMGNU)-objdump -D uart01.elf > uart01.list

uart01.bin : uart01.elf
	$(ARMGNU)-objcopy uart01.elf -O binary uart01.bin

uart01.hex : uart01.elf
	$(ARMGNU)-objcopy uart01.elf -O ihex uart01.hex





LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=arm -mcpu=arm1176jzf-s
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : uart01.clang.hex uart01.clang.bin


uart01.clang.bc : uart01.c
	clang $(LOPS) -c uart01.c -o uart01.clang.bc

uart01.clang.opt.elf : memmap vectors.o uart01.clang.bc
	opt $(OOPS) uart01.clang.bc -o uart01.clang.opt.bc
	llc $(LLCOPS) uart01.clang.opt.bc -o uart01.clang.opt.s
	$(ARMGNU)-as uart01.clang.opt.s -o uart01.clang.opt.o
	$(ARMGNU)-ld -o uart01.clang.opt.elf -T memmap vectors.o uart01.clang.opt.o
	$(ARMGNU)-objdump -D uart01.clang.opt.elf > uart01.clang.opt.list

uart01.clang.hex : uart01.clang.opt.elf
	$(ARMGNU)-objcopy uart01.clang.opt.elf uart01.clang.hex -O ihex

uart01.clang.bin : uart01.clang.opt.elf
	$(ARMGNU)-objcopy uart01.clang.opt.elf uart01.clang.bin -O binary






