
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : uart04.hex uart04.bin

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

uart04.o : uart04.c
	$(ARMGNU)-gcc $(COPS) -c uart04.c -o uart04.o

uart04.elf : memmap vectors.o uart04.o 
	$(ARMGNU)-ld vectors.o uart04.o -T memmap -o uart04.elf
	$(ARMGNU)-objdump -D uart04.elf > uart04.list

uart04.bin : uart04.elf
	$(ARMGNU)-objcopy uart04.elf -O binary uart04.bin

uart04.hex : uart04.elf
	$(ARMGNU)-objcopy uart04.elf -O ihex uart04.hex







LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=arm -mcpu=arm1176jzf-s
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : uart04.clang.hex uart04.clang.bin


uart04.clang.bc : uart04.c
	clang $(LOPS) -c uart04.c -o uart04.clang.bc

uart04.clang.opt.elf : memmap vectors.o uart04.clang.bc
	opt $(OOPS) uart04.clang.bc -o uart04.clang.opt.bc
	llc $(LLCOPS) uart04.clang.opt.bc -o uart04.clang.opt.s
	$(ARMGNU)-as uart04.clang.opt.s -o uart04.clang.opt.o
	$(ARMGNU)-ld -o uart04.clang.opt.elf -T memmap vectors.o uart04.clang.opt.o
	$(ARMGNU)-objdump -D uart04.clang.opt.elf > uart04.clang.opt.list

uart04.clang.hex : uart04.clang.opt.elf
	$(ARMGNU)-objcopy uart04.clang.opt.elf uart04.clang.hex -O ihex

uart04.clang.bin : uart04.clang.opt.elf
	$(ARMGNU)-objcopy uart04.clang.opt.elf uart04.clang.bin -O binary





