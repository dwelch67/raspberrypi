
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : notmain.hex

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


novectors.o : novectors.s
	$(ARMGNU)-as novectors.s -o novectors.o

notmain.o : notmain.c
	$(ARMGNU)-gcc $(COPS) -c notmain.c -o notmain.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

notmain.hex : memmap novectors.o periph.o notmain.o 
	$(ARMGNU)-ld novectors.o periph.o notmain.o -T memmap -o notmain.elf
	$(ARMGNU)-objdump -D notmain.elf > notmain.list
	$(ARMGNU)-objcopy notmain.elf -O ihex notmain.hex










LOPS = -Wall -m32 -emit-llvm
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
LLCOPS = $(LLCOPS1)
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : notmain.bin

notmain.bc : notmain.c
	clang $(LOPS) -c notmain.c -o notmain.bc

periph.bc : periph.c
	clang $(LOPS) -c periph.c -o periph.bc

notmain.clang.elf : loader novectors.o notmain.bc periph.bc
	llvm-link periph.bc notmain.bc -o notmain.nopt.bc
	opt $(OOPS) notmain.nopt.bc -o notmain.opt.bc
	llc $(LLCOPS) notmain.opt.bc -o notmain.clang.s
	$(ARMGNU)-as notmain.clang.s -o notmain.clang.o
	$(ARMGNU)-ld -o notmain.clang.elf -T loader novectors.o notmain.clang.o
	$(ARMGNU)-objdump -D notmain.clang.elf > notmain.clang.list

notmain.bin : notmain.clang.elf
	$(ARMGNU)-objcopy notmain.clang.elf notmain.clang.bin -O binary


