DEVICE      = attiny85
CLOCK      = 16500000
PROGRAMMER = arduino
TTY        = /dev/ttyUSB0
BAUDRATE   = 19200
OBJECTS    = Soldering.o
USBDRV     = ../../submodules/v-usb/usbdrv
SRC        = src

AVRDUDE = avrdude -c $(PROGRAMMER) -p $(DEVICE) -b $(BAUDRATE) -P $(TTY)
COMPILE = avr-gcc -Wall -Os -I$(USBDRV) -I$(SRC) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

OBJECTS = usbdrv/usbdrv.o usbdrv/usbdrvasm.o usbdrv/oddebug.o $(SRC)/Soldering.o

all: $(SRC)/Soldering.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash: all
	$(AVRDUDE) -U flash:w:$(SRC)/Soldering.hex:i

load: all
	sudo micronucleus --run $(SRC)/Soldering.hex

readcal:
	$(AVRDUDE) -U calibration:r:/dev/stdout:i | head -1

clean:
	rm -f $(SRC)/Soldering.hex $(SRC)/Soldering.elf $(OBJECTS)

usbdrv:
	cp -r $(USBDRV) usbdrv

$(SRC)/Soldering.elf: $(OBJECTS)
	$(COMPILE) -o $(SRC)/Soldering.elf $(OBJECTS)

$(SRC)/Soldering.hex: usbdrv $(SRC)/Soldering.elf
	rm -f $(SRC)/Soldering.hex
	avr-objcopy -j .text -j .data -O ihex $(SRC)/Soldering.elf $(SRC)/Soldering.hex
	avr-size --format=avr --mcu=$(DEVICE) $(SRC)/Soldering.elf

disasm: $(SRC)/Soldering.elf
	avr-objdump -d $(SRC)/Soldering.elf

cpp:
	$(COMPILE) -E $(SRC)/Soldering.c
