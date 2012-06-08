
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : kernel.img blinker.bin

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

bootloader04.o : bootloader04.c
	$(ARMGNU)-gcc $(COPS) -c bootloader04.c -o bootloader04.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

kernel.img : loader vectors.o periph.o bootloader04.o 
	$(ARMGNU)-ld vectors.o periph.o bootloader04.o -T loader -o bootloader04.elf
	$(ARMGNU)-objdump -D bootloader04.elf > bootloader04.list
	$(ARMGNU)-objcopy bootloader04.elf -O ihex bootloader04.hex
	$(ARMGNU)-objcopy bootloader04.elf -O binary kernel.img





start.o : start.s
	$(ARMGNU)-as start.s -o start.o

blinker.o : blinker.c
	$(ARMGNU)-gcc $(COPS) -c blinker.c -o blinker.o

blinker.bin : memmap start.o blinker.o 
	$(ARMGNU)-ld start.o blinker.o -T memmap -o blinker.elf
	$(ARMGNU)-objdump -D blinker.elf > blinker.list
	$(ARMGNU)-objcopy blinker.elf -O ihex blinker.hex
	$(ARMGNU)-objcopy blinker.elf -O binary blinker.bin






LOPS = -Wall -m32 -emit-llvm
LLCOPS0 = -march=arm 
LLCOPS1 = -march=arm -mcpu=arm1176jzf-s
LLCOPS = $(LLCOPS1)
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

clang : bootloader04.bin

bootloader04.bc : bootloader04.c
	clang $(LOPS) -c bootloader04.c -o bootloader04.bc

periph.bc : periph.c
	clang $(LOPS) -c periph.c -o periph.bc

bootloader04.clang.elf : loader vectors.o bootloader04.bc periph.bc
	llvm-link periph.bc bootloader04.bc -o bootloader04.nopt.bc
	opt $(OOPS) bootloader04.nopt.bc -o bootloader04.opt.bc
	llc $(LLCOPS) bootloader04.opt.bc -o bootloader04.clang.s
	$(ARMGNU)-as bootloader04.clang.s -o bootloader04.clang.o
	$(ARMGNU)-ld -o bootloader04.clang.elf -T loader vectors.o bootloader04.clang.o
	$(ARMGNU)-objdump -D bootloader04.clang.elf > bootloader04.clang.list

bootloader04.bin : bootloader04.clang.elf
	$(ARMGNU)-objcopy bootloader04.clang.elf bootloader04.clang.bin -O binary


