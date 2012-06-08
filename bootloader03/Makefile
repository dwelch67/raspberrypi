
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

bootloader03.o : bootloader03.c
	$(ARMGNU)-gcc $(COPS) -c bootloader03.c -o bootloader03.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

kernel.img : loader vectors.o periph.o bootloader03.o 
	$(ARMGNU)-ld vectors.o periph.o bootloader03.o -T loader -o bootloader03.elf
	$(ARMGNU)-objdump -D bootloader03.elf > bootloader03.list
	$(ARMGNU)-objcopy bootloader03.elf -O ihex bootloader03.hex
	$(ARMGNU)-objcopy bootloader03.elf -O binary kernel.img





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

clang : bootloader03.bin

bootloader03.bc : bootloader03.c
	clang $(LOPS) -c bootloader03.c -o bootloader03.bc

periph.bc : periph.c
	clang $(LOPS) -c periph.c -o periph.bc

bootloader03.clang.elf : loader vectors.o bootloader03.bc periph.bc
	llvm-link periph.bc bootloader03.bc -o bootloader03.nopt.bc
	opt $(OOPS) bootloader03.nopt.bc -o bootloader03.opt.bc
	llc $(LLCOPS) bootloader03.opt.bc -o bootloader03.clang.s
	$(ARMGNU)-as bootloader03.clang.s -o bootloader03.clang.o
	$(ARMGNU)-ld -o bootloader03.clang.elf -T loader vectors.o bootloader03.clang.o
	$(ARMGNU)-objdump -D bootloader03.clang.elf > bootloader03.clang.list

bootloader03.bin : bootloader03.clang.elf
	$(ARMGNU)-objcopy bootloader03.clang.elf bootloader03.clang.bin -O binary


