
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

bootloader05.o : bootloader05.c
	$(ARMGNU)-gcc $(COPS) -c bootloader05.c -o bootloader05.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

kernel.img : loader vectors.o periph.o bootloader05.o 
	$(ARMGNU)-ld vectors.o periph.o bootloader05.o -T loader -o bootloader05.elf
	$(ARMGNU)-objdump -D bootloader05.elf > bootloader05.list
	$(ARMGNU)-objcopy bootloader05.elf -O ihex bootloader05.hex
	$(ARMGNU)-objcopy bootloader05.elf -O binary kernel.img





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

clang : bootloader05.bin

bootloader05.bc : bootloader05.c
	clang $(LOPS) -c bootloader05.c -o bootloader05.bc

periph.bc : periph.c
	clang $(LOPS) -c periph.c -o periph.bc

bootloader05.clang.elf : loader vectors.o bootloader05.bc periph.bc
	llvm-link periph.bc bootloader05.bc -o bootloader05.nopt.bc
	opt $(OOPS) bootloader05.nopt.bc -o bootloader05.opt.bc
	llc $(LLCOPS) bootloader05.opt.bc -o bootloader05.clang.s
	$(ARMGNU)-as bootloader05.clang.s -o bootloader05.clang.o
	$(ARMGNU)-ld -o bootloader05.clang.elf -T loader vectors.o bootloader05.clang.o
	$(ARMGNU)-objdump -D bootloader05.clang.elf > bootloader05.clang.list

bootloader05.bin : bootloader05.clang.elf
	$(ARMGNU)-objcopy bootloader05.clang.elf bootloader05.clang.bin -O binary


