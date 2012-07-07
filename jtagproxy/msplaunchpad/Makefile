

COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 



AOPS = --warn --fatal-warnings

OOPS = -std-compile-opts -strip-debug -disable-simplify-libcalls

LOPS = -Wall -m32 -emit-llvm -disable-simplify-libcalls

HLOPS = -Wall -emit-llvm 

LLCOPS = -march=msp430

all : out.hex

clean :
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.hex
	rm -f *.norm.s
	rm -f *.opt.s




startup.o : startup.s
	msp430-as startup.s -o startup.o

reset.o : reset.s
	msp430-as reset.s -o reset.o


jtagproxy.o : jtagproxy.c
	msp430-gcc $(COPS) -c jtagproxy.c -o jtagproxy.o


jtagproxy.elf : memmap startup.o reset.o jtagproxy.o
	msp430-ld -T memmap reset.o jtagproxy.o startup.o -o jtagproxy.elf

out.hex : jtagproxy.elf
	msp430-objdump -D jtagproxy.elf > jtagproxy.list
	msp430-objcopy -O ihex jtagproxy.elf out.hex


