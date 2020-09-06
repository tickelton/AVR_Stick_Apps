DEVICE     = attiny85
CLOCK      = 16500000
PROGRAMMER = arduino
TTY        = /dev/ttyUSB0
BAUDRATE   = 19200
OBJECTS    = AnyKey.o
USBDRV     = ../../submodules/v-usb/usbdrv
SRC        = src

AVRDUDE = avrdude -c $(PROGRAMMER) -p $(DEVICE) -b $(BAUDRATE) -P $(TTY)
COMPILE = avr-gcc -Wall -Os -I$(USBDRV) -I$(SRC) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

OBJECTS = usbdrv/usbdrv.o usbdrv/usbdrvasm.o usbdrv/oddebug.o $(SRC)/AnyKey.o

all: $(SRC)/AnyKey.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash: all
	$(AVRDUDE) -U flash:w:$(SRC)/AnyKey.hex:i

read_eeprom:
	$(AVRDUDE) -U eeprom:r:data.eep:r

write_eeprom:
	$(AVRDUDE) -U eeprom:w:data.eep:r

load: all
	sudo micronucleus --run $(SRC)/AnyKey.hex

readcal:
	$(AVRDUDE) -U calibration:r:/dev/stdout:i | head -1

clean:
	rm -f $(SRC)/AnyKey.hex $(SRC)/AnyKey.elf $(OBJECTS)

usbdrv:
	cp -r $(USBDRV) usbdrv

$(SRC)/AnyKey.elf: $(OBJECTS)
	$(COMPILE) -o $(SRC)/AnyKey.elf $(OBJECTS)

$(SRC)/AnyKey.hex: usbdrv $(SRC)/AnyKey.elf
	rm -f $(SRC)/AnyKey.hex
	avr-objcopy -j .text -j .data -O ihex $(SRC)/AnyKey.elf $(SRC)/AnyKey.hex
	avr-size --format=avr --mcu=$(DEVICE) $(SRC)/AnyKey.elf

disasm: $(SRC)/AnyKey.elf
	avr-objdump -d $(SRC)/AnyKey.elf

cpp:
	$(COMPILE) -E $(SRC)/AnyKey.c
