
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : zlibtest.hex zlibtest.bin

all : gcc clang

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc
	rm -f *.opt.s


GOBJLIST = vectors.o zlibtest.o uart.o compress.o deflate.o zutil.o adler32.o crc32.o trees.o inflate.o uncompr.o inftrees.o inffast.o

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

zlibtest.o : zlibtest.c
	$(ARMGNU)-gcc $(COPS) -c zlibtest.c -o zlibtest.o

zlibtest.elf : memmap $(GOBJLIST)
	$(ARMGNU)-ld $(GOBJLIST) -T memmap -o zlibtest.elf
	$(ARMGNU)-objdump -D zlibtest.elf > zlibtest.list

zlibtest.bin : zlibtest.elf
	$(ARMGNU)-objcopy zlibtest.elf -O binary zlibtest.bin

zlibtest.hex : zlibtest.elf
	$(ARMGNU)-objcopy zlibtest.elf -O ihex zlibtest.hex


uart.o : uart.c
	$(ARMGNU)-gcc $(COPS) -c uart.c -o uart.o

compress.o : compress.c
	$(ARMGNU)-gcc $(COPS) -c compress.c -o compress.o

deflate.o : deflate.c
	$(ARMGNU)-gcc $(COPS) -c deflate.c -o deflate.o

zutil.o : zutil.c
	$(ARMGNU)-gcc $(COPS) -c zutil.c -o zutil.o

adler32.o : adler32.c
	$(ARMGNU)-gcc $(COPS) -c adler32.c -o adler32.o

crc32.o : crc32.c
	$(ARMGNU)-gcc $(COPS) -c crc32.c -o crc32.o

trees.o : trees.c
	$(ARMGNU)-gcc $(COPS) -c trees.c -o trees.o

inflate.o : inflate.c
	$(ARMGNU)-gcc $(COPS) -c inflate.c -o inflate.o

uncompr.o : uncompr.c
	$(ARMGNU)-gcc $(COPS) -c uncompr.c -o uncompr.o

inftrees.o : inftrees.c
	$(ARMGNU)-gcc $(COPS) -c inftrees.c -o inftrees.o

inffast.o : inffast.c
	$(ARMGNU)-gcc $(COPS) -c inffast.c -o inffast.o





LOPS = -Wall -m32 -emit-llvm
LLCOPS = -mtriple=arm-none-eabi -march=arm -mcpu=arm1176jzf-s
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : zlibtest.clang.hex zlibtest.clang.bin

COBJLIST = zlibtest.bc uart.bc compress.bc deflate.bc zutil.bc adler32.bc crc32.bc trees.bc inflate.bc uncompr.bc inftrees.bc inffast.bc

zlibtest.bc : zlibtest.c
	clang $(LOPS) -c zlibtest.c -o zlibtest.bc

uart.bc : uart.c
	clang $(LOPS) -c uart.c -o uart.bc

compress.bc : compress.c
	clang $(LOPS) -c compress.c -o compress.bc

deflate.bc : deflate.c
	clang $(LOPS) -c deflate.c -o deflate.bc

zutil.bc : zutil.c
	clang $(LOPS) -c zutil.c -o zutil.bc

adler32.bc : adler32.c
	clang $(LOPS) -c adler32.c -o adler32.bc

crc32.bc : crc32.c
	clang $(LOPS) -c crc32.c -o crc32.bc

trees.bc : trees.c
	clang $(LOPS) -c trees.c -o trees.bc

inflate.bc : inflate.c
	clang $(LOPS) -c inflate.c -o inflate.bc

uncompr.bc : uncompr.c
	clang $(LOPS) -c uncompr.c -o uncompr.bc

inftrees.bc : inftrees.c
	clang $(LOPS) -c inftrees.c -o inftrees.bc

inffast.bc : inffast.c
	clang $(LOPS) -c inffast.c -o inffast.bc

zlibtest.clang.bc : $(COBJLIST)
	llvm-link -o zlibtest.clang.bc $(COBJLIST)

zlibtest.clang.opt.elf : memmap vectors.o zlibtest.clang.bc
	opt $(OOPS) zlibtest.clang.bc -o zlibtest.clang.opt.bc
	llc $(LLCOPS) zlibtest.clang.opt.bc -o zlibtest.clang.opt.s
	$(ARMGNU)-as zlibtest.clang.opt.s -o zlibtest.clang.opt.o
	$(ARMGNU)-ld -o zlibtest.clang.opt.elf -T memmap vectors.o zlibtest.clang.opt.o
	$(ARMGNU)-objdump -D zlibtest.clang.opt.elf > zlibtest.clang.opt.list

zlibtest.clang.hex : zlibtest.clang.opt.elf
	$(ARMGNU)-objcopy zlibtest.clang.opt.elf zlibtest.clang.hex -O ihex

zlibtest.clang.bin : zlibtest.clang.opt.elf
	$(ARMGNU)-objcopy zlibtest.clang.opt.elf zlibtest.clang.bin -O binary




















