
ARMGNU ?= arm-none-eabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

all : kernel.img 

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

cpuid.o : cpuid.c
	$(ARMGNU)-gcc $(COPS) -c cpuid.c -o cpuid.o

kernel.img : loader vectors.o cpuid.o 
	$(ARMGNU)-ld vectors.o cpuid.o -T loader -o cpuid.elf
	$(ARMGNU)-objdump -D cpuid.elf > cpuid.list
	$(ARMGNU)-objcopy cpuid.elf -O ihex cpuid.hex
	$(ARMGNU)-objcopy cpuid.elf -O binary kernel.img


