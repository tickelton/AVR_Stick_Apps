help:
	@echo "available targets:"
	@echo ""
	@echo "  bootloader    :  build the micronucleus bootloader"
	@echo "  micronucleus  :  build the micronucleus command line tool"
	@echo ""
	@echo "  AnyKey        :  build the AnyKey app"
	@echo "  HelloWorld    :  build the HelloWorld app"
	@echo "  HID-RT        :  build the USBSlacker app"
	@echo "  USBSlacker    :  build the USBSlacker app"
	@echo ""

bootloader: misc/micronucleus/firmware submodules/micronucleus/firmware/
	@cp -r misc/micronucleus/firmware/* submodules/micronucleus/firmware/
	@cd submodules/micronucleus/firmware && \
		make CONFIG=AVR_Stick

micronucleus: submodules/micronucleus/commandline/
	@cd submodules/micronucleus/commandline && make

AnyKey HelloWorld HID-RT USBSlacker:
	@cd apps/$@ && make


