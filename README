
This repo is getting a bit messy starting with the first released to
the public pi and then all that came after.  I want to rework it but
it is going slowly.  Recently started creating directories pi1, piaplus
and so on to target the examples specifically to the different cards.
Again that is slow going.  Likewise this README will be rewritten perhaps
but for now...

--------------

This repo serves as a collection of low level examples.  No operating
system, embedded or low level embedded or deeply embedded or bare metal,
whatever your term is for this.

I am in no way shape or form associated with the raspberry pi organization
nor broadcom.  I just happen to own one (some) and am sharing my
experiences.  The raspberry pi is about education, and I feel bare
metal education is just as important as Python programming.

So I started this years ago when got my first ARM11 based raspberry pi
maybe we call that a raspberry pi 1, I dont know a good term.  But
now there is a growing number of variations.

ARM11 based (BCM2835)
Raspberry Pi B
Raspberry Pi B2
Raspberry Pi A+
Raspberry Pi B+
Raspberry Pi Zero
Cortex-A7 based (BCM2836)
Raspberry Pi 2 B
Cortex-A53 based (BCM2837)
Raspberry Pi 3 B

There is also the compute module but I dont have one of those.

General differences that we care about for these examples.  The amount
of ram varies from board to board.  The peripheral base address is
different between the BCM2835 and BCM2836.  The BCM2835 looks for the
file kernel.img the BCM2836 looks for kernel7.img.  The ARM11 based
Zero is a B with stuff removed and a new layout, but they up/over
clocked the processor from 750MHz to 1000MHz, one led on gpio 16.  The
A+ and B+ they moved the led (or put two) on gpio 35 and 47.  The
raspberry pi 2 is B+ like but with the different chip, supposedly the
BCM2836 is  BCM2835 with the ARM11 core removed and replaced with
the Cortex A7 and for the most part it appears to be.  The raspberry
pi 3 is Cortex A8 based, 64 bit.  And they moved the LED (the leds)
to an i2c gpio expander.

As of this writing I am adding plus and pi2 versions of the examples
as many of them are based on the early/original.  No guarantees I will
do that, just looking at the differences between the blinker01 examples
should show you what to do to convert these yourself.  In some cases
I am intentionally not trying to have one code base build for all
three with ifdefs and such, keep it simple stupid then complicate it
as needed.  The text may say kernel.img but substitute with kernel7.img
as needed.

I still have a number of raspberry pi 2 examples to port, and now a
bunch of examples to port to the raspberry pi 3.  The raspberry pi 3
as of this writing, without a config.txt, is switched to 32 bit
compatibility mode.  See the aarch64 directory for 64 bit ARM examples.

From what we know so far there is a gpu on chip which:

1) boots off of an on chip rom of some sort
2) reads the sd card and looks for additional gpu specific boot files
bootcode.bin and start.elf in the root dir of the first partition
(fat32 formatted, loader.bin no longer used/required)
3) in the same dir it looks for config.txt which you can do things like
change the arm speed, or change the address where to load kernel.img,
and many others
4) it reads kernel.img the arm boot binary file and copies it to memory
5) releases reset on the arm such that it runs from the address where
the kernel.img data was written

The memory is split between the GPU and the ARM, I believe the default
is to split the memory in half.  And there are ways to change that
split (to give the ARM more)(using config.txt).  Not going to worry
about that here.

From the ARMs perspective the kernel.img file is loaded, by default,
to address 0x8000.  (there are ways to change that, not going to worry
about that right now).

Hardware and programming information:

You will want to go here
http://elinux.org/RPi_Hardware
And the datasheet and schematic.  These are moving targets the above
elinux link has the datasheet and errata which is important.  They
didnt give us a full datasheet for the BCM2836 have to go with the
BCM2835.
You will want to go to
http://raspberrypi.org and then the forum tab then slide down to
the Bare Metal forum, the first (only) Sticky topic is Bare Metal
Resources.  There are many more links there for good information.
Also go to
http://infocenter.arm.com and get the Architectural Reference Manual
and the Techincal Reference Manual for the ARM1176JZF-S (BCM2835)
and/or the Cortex-A7 (BCM2836).

Early in the BCM2835 document you see a memory map.  I am going to
operate based on the middle map, this is how the ARM comes up.  The
left side is the system which we dont have direct access to in that
form, the gpu probably, not the ARM.  The ARM comes up with a memory
space that is basically 0x40000000 bytes in size as it mentions in
the middle chart.  The bottom of this picture shows total system
sdram (memory) and somewhere between zero and the top of ram is a
split between sdram for the ARM on the bottom and a chunk of that
for the VC SDRAM, basically memory for the gpu and memory shared
between the ARM and GPU to allow the ARM to ask the GPU to draw stuff
on the video screen.  256MBytes is 0x10000000, and 512MBytes is
0x20000000.  Some models of raspberry pi have 256MB, newer models have
512MB total ram which is split between the GPU and the ARM.  Assume
the ARM gets at least half of this.  Peripherals (uart, gpio, etc)
are mapped into arm address space at 0x20000000.  When you see
0x7Exxxxxx in the manual replace that with 0x20xxxxxx as your ARM
physical address.  Experimentally I have seen the memory repeats every
0x40000000, read 0x40008000 and you see the data from 0x8000.  From the
Broadcom doc this looks to be giving us access to the memory with
different caching schemes (cached, uncached, etc) depending on which
upper address bits you use.  Most likely to allow more room for RAM
the Raspberry Pi 2 uses a peripheral base address of 0x3Fxxxxxx instead
of the 0x20xxxxxx.

I do not normally zero out .bss or use .data so if you do this to my
examples

int x;
fun()
{
  static int y;
}

dont assume x and y are zero when your program starts. Nor if you do
this

int x=5;
fun()
{
  static int y=7;
}

will x=5 or y=7.

See the bssdata directory for more information, you can most likely
use the linker script to solve the problem for you since .text, .data,
.bss, (.rodata), everything lives in ram.

Nor do I use gcc libraries nor C libraries so you can build most if not
all of my examples using a gcc cross compiler.  Basically it doesnt
matter if you use arm-none-linux-gnueabi or arm-none-eabi.  I have not
looked in a while but formerly codesourcery.com (now a part of Mentor
Graphics) had a free LITE version of their toolchain which was pretty
easy to come by.  An even easier place is here
https://launchpad.net/gcc-arm-embedded
to get a cross compiler.  Building your own toolchain from gnu sources
(binutils and gcc) is fairly straight forward see my build_gcc
repository for a build script (Linux only but from that you might get
other platforms to build).  And also rememeber that you can run linux
on the pi and on that it has a native, not cross, gnu toolchain.

As far as we know so far the Raspberry Pi is not "brickable".  Normally
what brickable means is the processor relies on a boot flash and with
that flash it is possible to change/erase it such that the processor will
not boot normally.  Brickable and bricked sometimes excludes things
like jtag or special programming headers.  From the customers perspective
a bricked board is...bricked.  But on return to the vendor they may
have other equipment that is able to recover the board without doing
any soldering, perhaps plugging in jtag or some other cable on pins/pads
they have declared as reserved.  Take apart a tv remote control or
calculator, etc and you may see some holes or pads on the circuit board,
for some of these devices just opening the battery case you have a view
of some of the pcboard.  This is no doubt a programming header.  Long
story short, so far as I know the Raspberry Pi is not brickable because
it boots off of an sd card which we can easily remove and replace
ourselves.  I dont know for sure, a lot more info is out about the
GPU since I started with this, but I assume that there is some GPU code
that boots off of an internal rom, I doubt with two on chip processors
they created pure logic to read the sd card, wade through the filesystem
to find a specific bootcode.bin file, load that into ram and run it.
If that assumption is true is that on chip rom one time programmable
or can it be erased/reprogrammed, and if the latter how lucky do we have
to be with a broken program to erase that?  So I am not 100% sure but
almost 100% sure the Raspberry Pi is not brickable.  This is actually
a big deal for bare metal programming, in particular if it is your first
platform.  With decades of experience I still make mistakes from time
to time and brick a board, never to be recovered.

To use my samples you do not need a huge sd card.  Nor do you need nor
want to download one of the linux images, takes a while to download,
takes a bigger sd card to use, and takes forever to write to the sd card.
I use the firmware from http://github.com/raspberrypi.  The minimum
configuration you need to get started at this level is:

go to https://github.com/raspberrypi, you DO NOT need to download
the repo, they have some large files in there you will not need (for
my examples).  go to the firmware directory and then the boot directory.
For each of these files, bootcode.bin and start.elf (NOT kernel.img,
dont need it, too big)(loader.bin is no longer used/required).  Click
on the file name, it will go to another page then click on View Raw and
it will let you download the file.  For reference, I do not use nor
have a config.txt file on my sd card.  I only have the minimum number
of files on the sd card, bootcode.bin, start.elf and either kernel.img
or kernel7.img (or sometimes both).

My examples are basically the kernel.img file.  Not a linux kernel,
just bare metal programs.  Since the GPU bootloader is looking for
that file name, you use that file name.  The kernel.img file is just a
blob that is copied to memory, dont worry about what they named it.

What I do is setup the sd card with a single partition, fat32.  And
copy the above files in the root directory.  bootcode.bin and start.elf.
From there you take .bin files from my examples and place them on the sd
card with the name kernel.img.  It wont take you very long to figure out
this is going to get painful.

1) power off raspi
2) remove sd card
3) insert sd card in reader
4) plug reader into computer
5) mount/wait
6) copy binary file to kernel.img
7) sync/wait
8) unmount
9) insert sd card in raspi
10) power raspi
11) repeat

There are ways to avoid this, one is jtag, which is not as expensive
as it used to be.  It used to be in the thousands of dollars, now it
is under $50 and the software tools are free.  Now the raspi does have
jtag on the arm, getting the jtag connected requires soldering on older
of the older models, but unless you were an early adopter, you dont
need to worry about that all the signals are on the P1 connector. How
to use the jtag and how to hook it up is described later and in
the armjtag sample.

Another method is a bootloader, typically you use a serial port connected
to the target processor.  That processor boots a bootloader program that
in some way, shape, or form allows you to interact with the bootloader
and load programs into memory (that the bootloader is not using itself)
and then the bootloader branches/jumps to your program.  If your program
is still being debugged and you want to try again, you reboot the processor
the bootloader comes up and you can try again without having to move any
sd cards, etc.  The sd card dance above is now replaced with the
bootloader dance:

1) power off raspi
2) power on raspi
3) type command to load and start new program

Or if you solder on a reset button

1) reset raspi
2) type command to load and start new program

I have working bootloader examples.  bootloader05 is currently the last
of the xmodem based ones (that basically take a kernel.img file),
personally I use bootloader07 which takes an intel hex formatted file
which these examples also build.  The .bin file would be used with
bootloader05, the .hex with bootloader07.  But you need more hardware
(no soldering is required).  For those old enough to know what a serial
port is, you CANNOT connect your raspberry pi directly to this port,
you will fry the raspberry pi.  You need some sort of serial port at
3.3V either a level shifter of some sort (transceiver like a MAX232) or
a usb serial port where the signals are 3.3V (dont need to use RS232
just stay at the logic level).  The solution I recommend is a non-solder
solution:

A recent purchase, experimentally white is RX and green is TX, black GND
http://www.nexuscyber.com/usb-to-ttl-serial-debug-console-cable-for-raspberry-pi
Sparkfun has one
https://www.sparkfun.com/products/12977
As does Adafruit
https://www.adafruit.com/products/954
The above, assuming you figure out rx from tx, are all you need.  The
ones below you may need to solder or may need some jumper wires.

http://www.sparkfun.com/products/9873
plus some male/female wire
http://www.sparkfun.com/products/9140

Solutions that may involve soldering
http://www.sparkfun.com/products/718
http://www.sparkfun.com/products/8430

Or this for level shifting to a real com port.
http://www.sparkfun.com/products/449

Or see the pitopi (pi to pi) directory.  This talks about how to take
two raspberry pi's and connect them together.  One being the
host/development platform (a raspberry pi running linux is a native
arm development platform, no need to find/get/build a cross compiler)
the other being the target that runs your bare metal programs.

Lastly and perhaps the best solution IMO, is the FT4232H or FT2232H
mini module from FTDI.  It gives you UART and JTAG for under $30.
See the armjtag directory README for more and you will want some
female/female wire from sparkfun or adafruit or elsewhere
https://www.sparkfun.com/products/8430
(I use these F/F wires for most projects, buy/bought the 100 pack)

---- connecting to the uart pins ----

On the raspberry pi, the connector with two rows of a bunch of pins is
P1.  Starting at that corner of the board, the outside corner pin is
pin 2.  From pin 2 heading toward the yellow rca connector the pins
are 2, 4, 6, 8, 10.  Pin 6 connect to gnd on the usb to serial board
pin 8 is TX out of the raspi connect that to RX on the usb to serial
board.  Pin 10 is RX into the raspi, connect that to TX on the usb to
serial board.  Careful not to have metal items on the usb to serial
board touch metal items on the raspberry pi (other than the three
connections described).  On your host computer you will want to use
some sort of dumb terminal program, minicom, putty, etc.  Set the
serial port (usb to serial board) to 115200 baud, 8 data bits, no
parity, 1 stop bit.  NO flow control.  With minicom to get no flow
control you normally have to save the config, exit minicom, then
restart minicom and load the config in order for flow control
changes to take effect.  Once you have a saved config you dont have
to mess with it any more.

2  outer corner
4
6  ground
8  TX out
10 RX in

ground is not necessarily needed if both the raspberry pi and the
usb to serial board are powered by the same computer (I recommend
you do that) as they will ideally have the same ground.

Read more about the bootloaders in their local README files.  Likewise
if you interested in jtag see the armjatag README file.  Other than
chewing up a few more GPIO pins, and another thing you have to buy, the
jtag solution is the most powerful and useful.  My typical setup is the
armjtag binary as kernel.img, a usb to jtag board like the amontec
jtag-tiny and a usb to serial using minicom.

Update, amontec is history their website is gone.  But you can
get j-link (or clone) boxes from asia on ebay for around $11, so far
I have tried them with ARM JTAG and ARM SWD.  Very pleased so far.

If you can solder, the A+, B+, Zero and Pi 2 all have a pair of holes
sometimes with the text RUN next to them.  I use buttons like this
https://www.sparkfun.com/products/97
with two of the legs broken off then the others twisted and adjusted
to fit in the holes and soldered down.

As far as these samples go I recommend starting with blinker01 then
follow the discovery of the chip into uart01, etc.

The bssdata and baremetal directories attempt to explain a little
bit about taking control of the gnu toolchain to build bare metal
programs like these examples.  As with any bare metal programmer I have
my ways of doing things and these two directories hopefully will show
you some basics, get you thinking about how these tools really work,
take the fear away from using them, as well as some comments on why
I take the approach I take (not worrying about .bss nor .data).  Since
the raspberry pi is from our perspective RAM based (the GPU loads our
whole binary into memory), we dont have to deal with the things we
would deal with on a FLASH/PROM + RAM system.  This RAM only approach
makes life a lot easier, but leaves out some important bare metal
experiences that you will have to find elsewhere.

-----------
