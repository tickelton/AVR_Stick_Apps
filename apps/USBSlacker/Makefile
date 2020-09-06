DEVICE      = attiny85
CLOCK      = 16500000
PROGRAMMER = arduino
TTY        = /dev/ttyUSB0
BAUDRATE   = 19200
OBJECTS    = USBSlacker.o
USBDRV     = ../../submodules/v-usb/usbdrv
SRC        = src

AVRDUDE = avrdude -c $(PROGRAMMER) -p $(DEVICE) -b $(BAUDRATE) -P $(TTY)
COMPILE = avr-gcc -Wall -Os -I$(USBDRV) -I$(SRC) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

OBJECTS = usbdrv/usbdrv.o usbdrv/usbdrvasm.o usbdrv/oddebug.o $(SRC)/USBSlacker.o

all: $(SRC)/USBSlacker.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash: all
	$(AVRDUDE) -U flash:w:$(SRC)/USBSlacker.hex:i

load: all
	sudo micronucleus --run $(SRC)/USBSlacker.hex

readcal:
	$(AVRDUDE) -U calibration:r:/dev/stdout:i | head -1

clean:
	rm -f $(SRC)/USBSlacker.hex $(SRC)/USBSlacker.elf $(OBJECTS)

usbdrv:
	cp -r $(USBDRV) usbdrv

$(SRC)/USBSlacker.elf: $(OBJECTS)
	$(COMPILE) -o $(SRC)/USBSlacker.elf $(OBJECTS)

$(SRC)/USBSlacker.hex: usbdrv $(SRC)/USBSlacker.elf
	rm -f $(SRC)/USBSlacker.hex
	avr-objcopy -j .text -j .data -O ihex $(SRC)/USBSlacker.elf $(SRC)/USBSlacker.hex
	avr-size --format=avr --mcu=$(DEVICE) $(SRC)/USBSlacker.elf

disasm: $(SRC)/USBSlacker.elf
	avr-objdump -d $(SRC)/USBSlacker.elf

cpp:
	$(COMPILE) -E $(SRC)/USBSlacker.c
