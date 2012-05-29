
ARMGNU ?= arm-none-eabi

COPS = -mthumb -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : zlibtest.hex zlibtest.bin

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
	$(ARMGNU)-ld $(GOBJLIST) -T memmap -o zlibtest.elf libgcc.a
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



















