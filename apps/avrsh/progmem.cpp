////////////////////////////////////////////////////////////////////////////
// Name: progmem.cpp
// Author: B. Gian James <gian@BattleDroids.net>
// Description: This file is an effort to fix gcc's broken PROGMEM attribute
//              in C++.  
//
// $Id: progmem.cpp,v 1.3 2009/09/05 04:06:17 gian Exp $
////////////////////////////////////////////////////////////////////////////
#include <avr/pgmspace.h>
#include "progmem.h"

char const	strCmdRead1[] = "Pin: %s is HIGH\r\n";
char const 	strCmdRead2[] = "Pin: %s is LOW\r\n";

char const	pmUnkCmd[] = "<<ERROR>> %s: unknown command.\r\n";

char const 	endl[] = "\r\n";

// Scripting
char const 	pmHelp[] = "help";
char const	pmMenu[] = "menu";
char const 	pmEna[] = "ena";
char const	pmVars[] = "vars";
char const 	pmScript[] = "script";
char const 	pmEcho[] = "echo";
char const 	pmCat[] = "cat";
char const 	pmRead[] = "read";
char const 	pmWrite[] = "write";
char const 	pmSet[] = "set";
char const 	pmFCPU[] = "fcpu";
char const 	pmTimer[] = "timer";
char const 	pmAuth[] = "auth";
char const 	pmPasswd[] = "passwd";
char const 	pmPrint[] = "print";
char const 	pmPowerup[] = "powerup";
char const 	pmPowerdown[] = "powerdown";
char const 	pmEnaSvc[] = "Service: %s enabled.\r\n";


// PortPins
char const	pmPB0[] = "PB0";
char const	pmPB1[] = "PB1";
char const	pmPB2[] = "PB2";
char const	pmPB3[] = "PB3";
char const	pmPB4[] = "PB4";
char const	pmPB5[] = "PB5";
char const	pmPB6[] = "PB6";
char const	pmPB7[] = "PB7";
// Registers
char const	pmPORTB[] = "PORTB";
char const 	pmDDRB[] ="DDRB" ;
char const	pmPINB[] = "PINB";
char const 	pmSREG[] = "SREG";
char const	pmGPIOR0[] = "GPIOR0";
char const 	pmGPIOR1[] = "GPIOR1";
char const 	pmGPIOR2[] = "GPIOR2";
char const 	pmTCCR0B[] = "TCCR0B";
char const 	pmOCR0A[] =  "OCR0A";
char const	pmACSR[] = "ACSR";
char const	pmMCUSR[] = "MCUSR";
char const	pmCLKPR[] = "CLKPR";
char const	pmOSCCAL[] = "OSCCAL";
char const	pmADCL[] = "ADCL";
char const	pmADCSRA[] = "ADCSRA";
char const	pmADMUX[] = "ADMUX";
char const	pmEECR[] = "EECR";
char const	pmEEDR[] = "EEDR";
char const	pmEEARL[] = "EEARL";
char const	pmEEARH[] = "EEARH";
char const	pmGTCCR[] = "GTCCR";
char const	pmTCCR0A[] = "TCCR0A";
char const	pmTCNT0[] = "TCNT0";
char const	pmOCR0B[] = "OCR0B";
char const	pmMCUCR[] = "MCUCR";
char const	pmPRR[] = "PRR";
char const	pmADCH[] = "ADCH";
char const	pmADCSRB[] = "ADCSRB";
char const	pmDIDR0[] = "DIDR0";



