
#ARMGNU ?= arm-none-eabi
ARMGNU ?= arm-linux-gnueabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : uart05.bin

all : gcc clang

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.clang.s

start.o : start.s
	$(ARMGNU)-as start.s -o start.o

uart05.o : uart05.c
	$(ARMGNU)-gcc $(COPS) -c uart05.c -o uart05.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

uart05.bin : memmap start.o periph.o uart05.o 
	$(ARMGNU)-ld start.o periph.o uart05.o -T memmap -o uart05.elf
	$(ARMGNU)-objdump -D uart05.elf > uart05.list
	$(ARMGNU)-objcopy uart05.elf -O ihex uart05.hex
	$(ARMGNU)-objcopy uart05.elf -O binary uart05.bin



LOPS = -Wall -m32 -emit-llvm
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
LLCOPS = $(LLCOPS1)
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : uart05.clang.bin

uart05.bc : uart05.c
	clang $(LOPS) -c uart05.c -o uart05.bc

periph.bc : periph.c
	clang $(LOPS) -c periph.c -o periph.bc

uart05.clang.elf : memmap start.o uart05.bc periph.bc
	llvm-link periph.bc uart05.bc -o uart05.nopt.bc
	opt $(OOPS) uart05.nopt.bc -o uart05.opt.bc
	llc $(LLCOPS) uart05.opt.bc -o uart05.clang.s
	$(ARMGNU)-as uart05.clang.s -o uart05.clang.o
	$(ARMGNU)-ld -o uart05.clang.elf -T memmap start.o uart05.clang.o
	$(ARMGNU)-objdump -D uart05.clang.elf > uart05.clang.list

uart05.clang.bin : uart05.clang.elf
	$(ARMGNU)-objcopy uart05.clang.elf uart05.clang.hex -O ihex
	$(ARMGNU)-objcopy uart05.clang.elf uart05.clang.bin -O binary


