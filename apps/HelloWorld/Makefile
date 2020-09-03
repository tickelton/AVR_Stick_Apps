DEVICE      = attiny85
CLOCK      = 16500000
PROGRAMMER = -c arduino
OBJECTS    = HelloWorld.o

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

all: HelloWorld.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash: all
	$(AVRDUDE) -U flash:w:HelloWorld.hex:i

load: all
	sudo micronucleus --run HelloWorld.hex

clean:
	rm -f HelloWorld.hex HelloWorld.elf $(OBJECTS)

HelloWorld.elf: $(OBJECTS)
	$(COMPILE) -o HelloWorld.elf $(OBJECTS)

HelloWorld.hex: HelloWorld.elf
	rm -f HelloWorld.hex
	avr-objcopy -j .text -j .data -O ihex HelloWorld.elf HelloWorld.hex
	avr-size --format=avr --mcu=$(DEVICE) HelloWorld.elf

disasm: HelloWorld.elf
	avr-objdump -d HelloWorld.elf

cpp:
	$(COMPILE) -E HelloWorld.c
