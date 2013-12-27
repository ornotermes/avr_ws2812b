# Name: Makefile
# Author: Christian Starkjohann
# Creation Date: 2008-04-07
# Tabsize: 4
# Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
# Copyright: (c) 2012 Rikard Lindström <ornotermes@gmail.com>
# License: GNU GPL v3
# This Revision: $Id: Makefile 692 2008-11-07 15:07:40Z cs $

DEVICE  = at90usb162
F_CPU   = 16000000	# in Hz

CFLAGS  = -I.
OBJECTS = main.o

COMPILE = avr-gcc -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE) -std=c99

# symbolic targets:
help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "make hex ........... to build main.hex"
	@echo "make clean ......... to delete objects and hex file"
	@echo "make program ....... to flash with flip (Windows)"
	@echo "make dfu-program ... to flash with dfu-programmer(Unix/Linux)"

hex: main.hex

# rule for uploading firmware with flip:
program: main.hex
	batchisp -device $(DEVICE) -hardware usb -operation erase f memory flash blankcheck loadbuffer main.hex program verify

# rule for uploading firmware with dfu-programmer:
dfu-program: main.hex
	dfu-programmer $(DEVICE) erase
	dfu-programmer $(DEVICE) flash main.hex

# rule for deleting dependent files (those which can be built by Make):
clean:
	rm -f main.hex main.lst main.obj main.cof main.list main.map main.eep.hex main.elf *.o main.s

# Generic rule for compiling C files:
.c.o:
	$(COMPILE) -c $< -o $@

# Generic rule for assembling Assembler source files:
.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

# Generic rule for compiling C to assembler, used for debugging only.
.c.s:
	$(COMPILE) -S $< -o $@

# file targets:

# Since we don't want to ship the driver multipe times, we copy it into this project:

main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size main.hex

# debugging targets:

disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c
