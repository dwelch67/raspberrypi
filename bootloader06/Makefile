
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

bootloader06.o : bootloader06.c
	$(ARMGNU)-gcc $(COPS) -c bootloader06.c -o bootloader06.o

periph.o : periph.c
	$(ARMGNU)-gcc $(COPS) -c periph.c -o periph.o

kernel.img : loader vectors.o periph.o bootloader06.o 
	$(ARMGNU)-ld vectors.o periph.o bootloader06.o -T loader -o bootloader06.elf
	$(ARMGNU)-objdump -D bootloader06.elf > bootloader06.list
	$(ARMGNU)-objcopy bootloader06.elf -O ihex bootloader06.hex
	$(ARMGNU)-objcopy bootloader06.elf -O binary kernel.img





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

clang : bootloader06.bin

bootloader06.bc : bootloader06.c
	clang $(LOPS) -c bootloader06.c -o bootloader06.bc

periph.bc : periph.c
	clang $(LOPS) -c periph.c -o periph.bc

bootloader06.clang.elf : loader vectors.o bootloader06.bc periph.bc
	llvm-link periph.bc bootloader06.bc -o bootloader06.nopt.bc
	opt $(OOPS) bootloader06.nopt.bc -o bootloader06.opt.bc
	llc $(LLCOPS) bootloader06.opt.bc -o bootloader06.clang.s
	$(ARMGNU)-as bootloader06.clang.s -o bootloader06.clang.o
	$(ARMGNU)-ld -o bootloader06.clang.elf -T loader vectors.o bootloader06.clang.o
	$(ARMGNU)-objdump -D bootloader06.clang.elf > bootloader06.clang.list

bootloader06.bin : bootloader06.clang.elf
	$(ARMGNU)-objcopy bootloader06.clang.elf bootloader06.clang.bin -O binary


