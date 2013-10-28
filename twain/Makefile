
ARMGNU ?= arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=arm1176jzf-s -march=armv6
COPS0 = -Wall -O1 -nostdlib -nostartfiles -ffreestanding 
COPS1 = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 
COPS2 = -Wall -O3 -nostdlib -nostartfiles -ffreestanding 
COPS3 = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=arm1176jzf-s 
COPS4 = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=arm1176jzf-s -mtune=arm1176jzf-s
COPS5 = -Wall -O3 -nostdlib -nostartfiles -ffreestanding -mcpu=arm1176jzf-s -mtune=arm1176jzf-s
COPS = $(COPS4)

gcc : twain.gcc.hex twain.gcc.bin

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


vectors.o : vectors.s
	$(ARMGNU)-as $(AOPS) vectors.s -o vectors.o


ZOBJSDEF = compress.o deflate.o trees.o
ZOBJS = uncompr.o inflate.o crc32.o adler32.o inffast.o inftrees.o zutil.o $(ZOBJSDEF)

twain.o : twain.c twaindef.h
	$(ARMGNU)-gcc $(COPS) -c twain.c -o twain.o

uart.o : uart.c
	$(ARMGNU)-gcc $(COPS) -c uart.c -o uart.o


uncompr.o : uncompr.c
	$(ARMGNU)-gcc $(COPS) -c uncompr.c -o uncompr.o

inflate.o : inflate.c
	$(ARMGNU)-gcc $(COPS) -c inflate.c -o inflate.o

crc32.o : crc32.c
	$(ARMGNU)-gcc $(COPS) -c crc32.c -o crc32.o

adler32.o : adler32.c
	$(ARMGNU)-gcc $(COPS) -c adler32.c -o adler32.o

inffast.o : inffast.c
	$(ARMGNU)-gcc $(COPS) -c inffast.c -o inffast.o

inftrees.o : inftrees.c
	$(ARMGNU)-gcc $(COPS) -c inftrees.c -o inftrees.o

zutil.o : zutil.c
	$(ARMGNU)-gcc $(COPS) -c zutil.c -o zutil.o



compress.o : compress.c
	$(ARMGNU)-gcc $(COPS) -c compress.c -o compress.o

deflate.o : deflate.c
	$(ARMGNU)-gcc $(COPS) -c deflate.c -o deflate.o

trees.o : trees.c
	$(ARMGNU)-gcc $(COPS) -c trees.c -o trees.o


twain.gcc.elf : memmap vectors.o uart.o twain.o $(ZOBJS)
	$(ARMGNU)-ld vectors.o uart.o twain.o $(ZOBJS) -T memmap -o twain.gcc.elf
	$(ARMGNU)-objdump -D twain.gcc.elf > twain.gcc.list

twain.gcc.bin : twain.gcc.elf
	$(ARMGNU)-objcopy twain.gcc.elf -O binary twain.gcc.bin

twain.gcc.hex : twain.gcc.elf
	$(ARMGNU)-objcopy twain.gcc.elf -O ihex twain.gcc.hex



ZBCSDEF = compress.bc deflate.bc trees.bc
ZBCS = uncompr.bc inflate.bc crc32.bc adler32.bc inffast.bc inftrees.bc zutil.bc $(ZBCSDEF)


#LOPS = -Wall -m32 -emit-llvm -ccc-host-triple $(ARMGNU) 
LOPS = -Wall -m32 -emit-llvm -target $(ARMGNU) 
LLCOPS0 = -march=arm -mtriple=$(ARMGNU)
LLCOPS1 = -march=arm -mtriple=$(ARMGNU) -mcpu=arm1176jzf-s
LLCOPS = $(LLCOPS1)


OOPS = -std-compile-opts -disable-simplify-libcalls

clang : twain.clang.hex twain.clang.bin 



twain.bc : twain.c twaindef.h
	clang $(LOPS) -c twain.c -o twain.bc

uart.bc : uart.c
	clang $(LOPS) -c uart.c -o uart.bc


uncompr.bc : uncompr.c
	clang $(LOPS) -c uncompr.c -o uncompr.bc

inflate.bc : inflate.c
	clang $(LOPS) -c inflate.c -o inflate.bc

crc32.bc : crc32.c
	clang $(LOPS) -c crc32.c -o crc32.bc

adler32.bc : adler32.c
	clang $(LOPS) -c adler32.c -o adler32.bc

inffast.bc : inffast.c
	clang $(LOPS) -c inffast.c -o inffast.bc

inftrees.bc : inftrees.c
	clang $(LOPS) -c inftrees.c -o inftrees.bc

zutil.bc : zutil.c
	clang $(LOPS) -c zutil.c -o zutil.bc



compress.bc : compress.c
	clang $(LOPS) -c compress.c -o compress.bc

deflate.bc : deflate.c
	clang $(LOPS) -c deflate.c -o deflate.bc

trees.bc : trees.c
	clang $(LOPS) -c trees.c -o trees.bc


twain.clang.bc : twain.bc uart.bc $(ZBCS)
	llvm-link -o twain.clang.bc twain.bc uart.bc $(ZBCS)

twain.clang.elf : memmap vectors.o twain.clang.bc
	opt $(OOPS) twain.clang.bc -o twain.clang.bc
	llc $(LLCOPS) twain.clang.bc -o twain.clang.s
	$(ARMGNU)-as $(AOPS) twain.clang.s -o twain.clang.o
	$(ARMGNU)-ld -o twain.clang.elf -T memmap vectors.o twain.clang.o
	$(ARMGNU)-objdump -D twain.clang.elf > twain.clang.list

twain.clang.hex : twain.clang.elf
	$(ARMGNU)-objcopy twain.clang.elf twain.clang.hex -O ihex

twain.clang.bin : twain.clang.elf
	$(ARMGNU)-objcopy twain.clang.elf twain.clang.bin -O binary





