
#ARMGNU ?= arm-none-eabi
ARMGNU ?= arm-linux-gnueabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : multi00.bin

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

multi00.o : multi00.c
	$(ARMGNU)-gcc $(COPS) -c multi00.c -o multi00.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

multi00.bin : memmap start.o periph.o multi00.o 
	$(ARMGNU)-ld start.o periph.o multi00.o -T memmap -o multi00.elf
	$(ARMGNU)-objdump -D multi00.elf > multi00.list
	$(ARMGNU)-objcopy multi00.elf -O ihex multi00.hex
	$(ARMGNU)-objcopy multi00.elf -O binary multi00.bin



LOPS = -Wall -m32 -emit-llvm
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
LLCOPS = $(LLCOPS1)
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : multi00.clang.bin

multi00.bc : multi00.c
	clang $(LOPS) -c multi00.c -o multi00.bc

periph.bc : periph.c
	clang $(LOPS) -c periph.c -o periph.bc

multi00.clang.elf : memmap start.o multi00.bc periph.bc
	llvm-link periph.bc multi00.bc -o multi00.nopt.bc
	opt $(OOPS) multi00.nopt.bc -o multi00.opt.bc
	llc $(LLCOPS) multi00.opt.bc -o multi00.clang.s
	$(ARMGNU)-as multi00.clang.s -o multi00.clang.o
	$(ARMGNU)-ld -o multi00.clang.elf -T memmap start.o multi00.clang.o
	$(ARMGNU)-objdump -D multi00.clang.elf > multi00.clang.list

multi00.clang.bin : multi00.clang.elf
	$(ARMGNU)-objcopy multi00.clang.elf multi00.clang.hex -O ihex
	$(ARMGNU)-objcopy multi00.clang.elf multi00.clang.bin -O binary


