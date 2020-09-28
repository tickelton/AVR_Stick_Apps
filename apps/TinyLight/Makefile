DEVICE     = attiny85
CLOCK      = 16500000
PROGRAMMER = arduino
TTY        = /dev/ttyUSB0
BAUDRATE   = 19200
OBJECTS    = TinyLight.o
USBDRV     = ../../submodules/v-usb/usbdrv
SRC        = src

AVRDUDE = avrdude -c $(PROGRAMMER) -p $(DEVICE) -b $(BAUDRATE) -P $(TTY)
COMPILE = avr-gcc -Wall -Os -I$(USBDRV) -I$(SRC) -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

OBJECTS = usbdrv/usbdrv.o usbdrv/usbdrvasm.o usbdrv/oddebug.o $(SRC)/TinyLight.o

all: $(SRC)/TinyLight.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash: all
	$(AVRDUDE) -U flash:w:$(SRC)/TinyLight.hex:i

load: all
	sudo micronucleus --run $(SRC)/TinyLight.hex

clean:
	rm -f $(SRC)/TinyLight.hex $(SRC)/TinyLight.elf $(OBJECTS)

usbdrv:
	cp -r $(USBDRV) usbdrv

$(SRC)/TinyLight.elf: $(OBJECTS)
	$(COMPILE) -o $(SRC)/TinyLight.elf $(OBJECTS)

$(SRC)/TinyLight.hex: usbdrv $(SRC)/TinyLight.elf
	rm -f $(SRC)/TinyLight.hex
	avr-objcopy -j .text -j .data -O ihex $(SRC)/TinyLight.elf $(SRC)/TinyLight.hex
	avr-size --format=avr --mcu=$(DEVICE) $(SRC)/TinyLight.elf

disasm: $(SRC)/TinyLight.elf
	avr-objdump -d $(SRC)/TinyLight.elf

cpp:
	$(COMPILE) -E $(SRC)/TinyLight.c
