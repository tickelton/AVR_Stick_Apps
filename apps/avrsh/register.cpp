////////////////////////////////////////////////////////////////////////////
// Name: register.cpp
// Author: B. Gian James <gian@BattleDroids.net>
// Description: This file declares the scripting components that allow 
//              the user to check registers in real-time.
//
// $Id: register.cpp,v 1.5 2009/09/02 12:55:17 gian Exp $
////////////////////////////////////////////////////////////////////////////
#include <avr/io.h>
#include <string.h>
#include "progmem.h"
#include "common.h"
#include "register.h"


// If you really need to conserve RAM, you can put each of these strings
// individually into program memory and then put the array there, too.
// Entered strings and struct array into PROGMEM 8.30.09 -gian
const RegisterVars
Register::Registers[] PROGMEM =
	{
		{ &PORTB, pmPORTB },
		{ &DDRB,  pmDDRB},
		{ &PINB,  pmPINB},   { &EECR,  pmEECR},
		{ &EEDR, pmEEDR },
		{ &SREG,  pmSREG},   { &EEARL, pmEEARL },
		{ &GPIOR0, pmGPIOR0 }, { &EEARH, pmEEARH },
		{ &GPIOR1,  pmGPIOR1}, { &GTCCR, pmGTCCR },
		{ &GPIOR2,  pmGPIOR2}, { &TCCR0A, pmTCCR0A },
		{ &TCCR0B, pmTCCR0B}, { &TCNT0, pmTCNT0 },
		{ &OCR0A, pmOCR0A }, { &OCR0B, pmOCR0B },
		{ &ACSR, pmACSR },
		{ &MCUSR, pmMCUSR }, { &MCUCR, pmMCUCR },
		{ &CLKPR, pmCLKPR }, { &PRR, pmPRR },
		{ &OSCCAL,pmOSCCAL  },
		{ &ADCL, pmADCL }, { &ADCH, pmADCH },
		{ &ADCSRA, pmADCSRA }, { &ADCSRB, pmADCSRB },
		{ &ADMUX,  pmADMUX}, { &DIDR0, pmDIDR0 },
		{ NULL, NULL }
	};

void
Register::WriteRegister(uint8 * reg)
{
	char buff[30];
	uint8 byte[8];
	for(uint8 x = 0; x < 8; x++) {
		if (BCHK(*reg,x))
			byte[(7-x)] = 1;
		else
			byte[(7-x)] = 0;
	}
	sprintf_P(buff,PSTR("(0x%x) set to [%d%d%d%d%d%d%d%d]"),
		*reg,byte[0],byte[1],byte[2],byte[3],byte[4],byte[5],byte[6],byte[7]);
	WriteRAM(buff);

}

char * 
Register::getRegNameFromPgmSpace(uint8 idx)
{
	const RegisterVars * reg = &Registers[idx];
	char * rname = (char*)pgm_read_word(&reg->name);
	return rname;
}

void
Register::PrintRegisters()
{ 
	uint8 x = 1;
	char buff[60];
	WriteLine(PSTR("I know about the following registers:\r\n"));
	char * rname = getRegNameFromPgmSpace(x);
	char crname[7]; 
	while(rname != NULL) {
		strcpy_P(crname,rname);
		sprintf_P(buff,PSTR("%-7s    "),crname);
		WriteRAM(buff);
		if (!(x % 6))
			WriteLine(PSTR("\r\n"));
		++x;
		rname = getRegNameFromPgmSpace(x);
	}
	sprintf_P(buff,PSTR("%-7s     \r\n"),crname);
	WriteRAM(buff);
}

uint8 * 
Register::GetRegisterByName(const char * n)
{
	uint8 x = 0;
	char * rname = getRegNameFromPgmSpace(x);

	while(rname != NULL) {
		const RegisterVars * reg = &Registers[x];
		if (!strcmp_P(n,rname))
			return (uint8*)pgm_read_byte(&reg->reg);
		++x;
		rname = getRegNameFromPgmSpace(x);
	}
	return (uint8*)NULL;

}


