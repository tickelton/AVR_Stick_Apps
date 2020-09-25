DEVICE      = attiny85
CLOCK      = 16500000
PROGRAMMER = -c arduino
OBJECTS    = SquareWave.o

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

all: SquareWave.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash: all
	$(AVRDUDE) -U flash:w:SquareWave.hex:i

load: all
	sudo micronucleus --run SquareWave.hex

clean:
	rm -f SquareWave.hex SquareWave.elf $(OBJECTS)

SquareWave.elf: $(OBJECTS)
	$(COMPILE) -o SquareWave.elf $(OBJECTS)

SquareWave.hex: SquareWave.elf
	rm -f SquareWave.hex
	avr-objcopy -j .text -j .data -O ihex SquareWave.elf SquareWave.hex
	avr-size --format=avr --mcu=$(DEVICE) SquareWave.elf

disasm: SquareWave.elf
	avr-objdump -d SquareWave.elf

cpp:
	$(COMPILE) -E SquareWave.c
