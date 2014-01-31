
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : kernel.img

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
	$(ARMGNU)-as vectors.s -o vectors.o

video01.o : video01.c image_data.h
	$(ARMGNU)-gcc $(COPS) -c video01.c -o video01.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

kernel.img : loader vectors.o periph.o video01.o 
	$(ARMGNU)-ld vectors.o periph.o video01.o -T loader -o video01.elf
	$(ARMGNU)-objdump -D video01.elf > video01.list
	$(ARMGNU)-objcopy video01.elf -O ihex video01.hex
	$(ARMGNU)-objcopy video01.elf -O binary kernel.img






LOPS = -Wall -m32 -emit-llvm
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
LLCOPS = $(LLCOPS1)
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : video01.bin

video01.bc : video01.c image_data.h
	clang $(LOPS) -c video01.c -o video01.bc

periph.bc : periph.c
	clang $(LOPS) -c periph.c -o periph.bc

video01.clang.elf : loader vectors.o video01.bc periph.bc
	llvm-link periph.bc video01.bc -o video01.nopt.bc
	opt $(OOPS) video01.nopt.bc -o video01.opt.bc
	#llc $(LLCOPS) video01.opt.bc -o video01.clang.s
	#$(ARMGNU)-as video01.clang.s -o video01.clang.o
	llc $(LLCOPS) -filetype=obj video01.opt.bc -o video01.clang.o
	$(ARMGNU)-ld -o video01.clang.elf -T loader vectors.o video01.clang.o
	$(ARMGNU)-objdump -D video01.clang.elf > video01.clang.list

video01.bin : video01.clang.elf
	$(ARMGNU)-objcopy video01.clang.elf video01.clang.bin -O binary


