
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : uartx01.hex uartx01.bin

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

uartx01.o : uartx01.c
	$(ARMGNU)-gcc $(COPS) -c uartx01.c -o uartx01.o

uartx01.elf : memmap vectors.o uartx01.o 
	$(ARMGNU)-ld vectors.o uartx01.o -T memmap -o uartx01.elf
	$(ARMGNU)-objdump -D uartx01.elf > uartx01.list

uartx01.bin : uartx01.elf
	$(ARMGNU)-objcopy uartx01.elf -O binary uartx01.bin

uartx01.hex : uartx01.elf
	$(ARMGNU)-objcopy uartx01.elf -O ihex uartx01.hex







LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=arm -mcpu=arm1176jzf-s
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : uartx01.clang.hex uartx01.clang.bin


uartx01.clang.bc : uartx01.c
	clang $(LOPS) -c uartx01.c -o uartx01.clang.bc

uartx01.clang.opt.elf : memmap vectors.o uartx01.clang.bc
	opt $(OOPS) uartx01.clang.bc -o uartx01.clang.opt.bc
	llc $(LLCOPS) uartx01.clang.opt.bc -o uartx01.clang.opt.s
	$(ARMGNU)-as uartx01.clang.opt.s -o uartx01.clang.opt.o
	$(ARMGNU)-ld -o uartx01.clang.opt.elf -T memmap vectors.o uartx01.clang.opt.o
	$(ARMGNU)-objdump -D uartx01.clang.opt.elf > uartx01.clang.opt.list

uartx01.clang.hex : uartx01.clang.opt.elf
	$(ARMGNU)-objcopy uartx01.clang.opt.elf uartx01.clang.hex -O ihex

uartx01.clang.bin : uartx01.clang.opt.elf
	$(ARMGNU)-objcopy uartx01.clang.opt.elf uartx01.clang.bin -O binary





