
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : uart02.hex uart02.bin

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

uart02.o : uart02.c
	$(ARMGNU)-gcc $(COPS) -c uart02.c -o uart02.o

uart02.elf : memmap vectors.o uart02.o 
	$(ARMGNU)-ld vectors.o uart02.o -T memmap -o uart02.elf
	$(ARMGNU)-objdump -D uart02.elf > uart02.list

uart02.bin : uart02.elf
	$(ARMGNU)-objcopy uart02.elf -O binary uart02.bin

uart02.hex : uart02.elf
	$(ARMGNU)-objcopy uart02.elf -O ihex uart02.hex







LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=arm -mcpu=arm1176jzf-s
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : uart02.clang.hex uart02.clang.bin


uart02.clang.bc : uart02.c
	clang $(LOPS) -c uart02.c -o uart02.clang.bc

uart02.clang.opt.elf : memmap vectors.o uart02.clang.bc
	opt $(OOPS) uart02.clang.bc -o uart02.clang.opt.bc
	llc $(LLCOPS) uart02.clang.opt.bc -o uart02.clang.opt.s
	$(ARMGNU)-as uart02.clang.opt.s -o uart02.clang.opt.o
	$(ARMGNU)-ld -o uart02.clang.opt.elf -T memmap vectors.o uart02.clang.opt.o
	$(ARMGNU)-objdump -D uart02.clang.opt.elf > uart02.clang.opt.list

uart02.clang.hex : uart02.clang.opt.elf
	$(ARMGNU)-objcopy uart02.clang.opt.elf uart02.clang.hex -O ihex

uart02.clang.bin : uart02.clang.opt.elf
	$(ARMGNU)-objcopy uart02.clang.opt.elf uart02.clang.bin -O binary





