DEVICE      = attiny85
CLOCK      = 16500000
PROGRAMMER = arduino
TTY        = /dev/ttyUSB0
BAUDRATE   = 19200
OBJECTS    = SelfReset.o
USBDRV     = ../../submodules/v-usb/usbdrv
SRC        = src

AVRDUDE = avrdude -c $(PROGRAMMER) -p $(DEVICE) -b $(BAUDRATE) -P $(TTY)
COMPILE = avr-gcc -Wall -Os -I$(USBDRV) -I$(SRC) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

OBJECTS = usbdrv/usbdrv.o usbdrv/usbdrvasm.o usbdrv/oddebug.o $(SRC)/SelfReset.o

all: $(SRC)/SelfReset.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash: all
	$(AVRDUDE) -U flash:w:$(SRC)/SelfReset.hex:i

load: all
	sudo micronucleus --run $(SRC)/SelfReset.hex

readcal:
	$(AVRDUDE) -U calibration:r:/dev/stdout:i | head -1

clean:
	rm -f $(SRC)/SelfReset.hex $(SRC)/SelfReset.elf $(OBJECTS)

usbdrv:
	cp -r $(USBDRV) usbdrv

$(SRC)/SelfReset.elf: $(OBJECTS)
	$(COMPILE) -o $(SRC)/SelfReset.elf $(OBJECTS)

$(SRC)/SelfReset.hex: usbdrv $(SRC)/SelfReset.elf
	rm -f $(SRC)/SelfReset.hex
	avr-objcopy -j .text -j .data -O ihex $(SRC)/SelfReset.elf $(SRC)/SelfReset.hex
	avr-size --format=avr --mcu=$(DEVICE) $(SRC)/SelfReset.elf

disasm: $(SRC)/SelfReset.elf
	avr-objdump -d $(SRC)/SelfReset.elf

cpp:
	$(COMPILE) -E $(SRC)/SelfReset.c
