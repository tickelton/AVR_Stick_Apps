DEVICE     = attiny85
CLOCK      = 16500000
PROGRAMMER = arduino
TTY        = /dev/ttyUSB0
BAUDRATE   = 19200
OBJECTS    = HID-RT.o
USBDRV     = ../../submodules/v-usb/usbdrv
SRC        = src

AVRDUDE = avrdude -c $(PROGRAMMER) -p $(DEVICE) -b $(BAUDRATE) -P $(TTY)
COMPILE = avr-gcc -Wall -Os -I$(USBDRV) -I$(SRC) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

OBJECTS = usbdrv/usbdrv.o usbdrv/usbdrvasm.o usbdrv/oddebug.o $(SRC)/HID-RT.o

all: $(SRC)/HID-RT.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash: all
	$(AVRDUDE) -U flash:w:$(SRC)/HID-RT.hex:i

read_eeprom:
	$(AVRDUDE) -U eeprom:r:data.eep:r

write_eeprom:
	$(AVRDUDE) -U eeprom:w:data.eep:r

load: all
	sudo micronucleus --run $(SRC)/HID-RT.hex

readcal:
	$(AVRDUDE) -U calibration:r:/dev/stdout:i | head -1

clean:
	rm -f $(SRC)/HID-RT.hex $(SRC)/HID-RT.elf $(OBJECTS)

usbdrv:
	cp -r $(USBDRV) usbdrv

$(SRC)/HID-RT.elf: $(OBJECTS)
	$(COMPILE) -o $(SRC)/HID-RT.elf $(OBJECTS)

$(SRC)/HID-RT.hex: usbdrv $(SRC)/HID-RT.elf
	rm -f $(SRC)/HID-RT.hex
	avr-objcopy -j .text -j .data -O ihex $(SRC)/HID-RT.elf $(SRC)/HID-RT.hex
	avr-size --format=avr --mcu=$(DEVICE) $(SRC)/HID-RT.elf

disasm: $(SRC)/HID-RT.elf
	avr-objdump -d $(SRC)/HID-RT.elf

cpp:
	$(COMPILE) -E $(SRC)/HID-RT.c
