#ifndef _AVRSH_PROGMEM_H_
#define _AVRSH_PROGMEM_H_
////////////////////////////////////////////////////////////////////////////
// Name: progmem.h
// Author: B. Gian James <gian@BattleDroids.net>
// Description: This file is an effort to fix gcc's broken PROGMEM attribute
//              in C++.  
//
// $Id: progmem.h,v 1.6 2009/09/05 04:06:17 gian Exp $
////////////////////////////////////////////////////////////////////////////
#include <avr/pgmspace.h>

//extern char const __attribute__((__progmem__))  [];

extern char const __attribute__((__progmem__))  strCmdRead1[];
extern char const __attribute__((__progmem__))  strCmdRead2[];

extern char const __attribute__((__progmem__))  pmUnkCmd[];
extern char const __attribute__((__progmem__))  endl[];


// PortPin
extern char const __attribute__((__progmem__))  pmPB0[];
extern char const __attribute__((__progmem__))  pmPB1[];
extern char const __attribute__((__progmem__))  pmPB2[];
extern char const __attribute__((__progmem__))  pmPB3[];
extern char const __attribute__((__progmem__))  pmPB4[];
extern char const __attribute__((__progmem__))  pmPB5[];
extern char const __attribute__((__progmem__))  pmPB6[];
extern char const __attribute__((__progmem__))  pmPB7[];
// Registers
extern char const __attribute__((__progmem__))  pmPORTB[];
extern char const __attribute__((__progmem__))  pmDDRB[];
extern char const __attribute__((__progmem__))  pmPINB[];
extern char const __attribute__((__progmem__))  pmSREG[];
extern char const __attribute__((__progmem__))  pmGPIOR0[];
extern char const __attribute__((__progmem__))  pmGPIOR1[];
extern char const __attribute__((__progmem__))  pmGPIOR2[];
extern char const __attribute__((__progmem__))  pmTCCR0B[];
extern char const __attribute__((__progmem__))  pmOCR0A[];
extern char const __attribute__((__progmem__))  pmACSR[];
extern char const __attribute__((__progmem__))  pmMCUSR[];
extern char const __attribute__((__progmem__))  pmCLKPR[];
extern char const __attribute__((__progmem__))  pmOSCCAL[];
extern char const __attribute__((__progmem__))  pmADCL[];
extern char const __attribute__((__progmem__))  pmADCSRA[];
extern char const __attribute__((__progmem__))  pmADMUX[];
extern char const __attribute__((__progmem__))  pmEECR[];
extern char const __attribute__((__progmem__))  pmEEDR[];
extern char const __attribute__((__progmem__))  pmEEARL[];
extern char const __attribute__((__progmem__))  pmEEARH[];
extern char const __attribute__((__progmem__))  pmGTCCR[];
extern char const __attribute__((__progmem__))  pmTCCR0A[];
extern char const __attribute__((__progmem__))  pmTCNT0[];
extern char const __attribute__((__progmem__))  pmOCR0B[];
extern char const __attribute__((__progmem__))  pmMCUCR[];
extern char const __attribute__((__progmem__))  pmPRR[];
extern char const __attribute__((__progmem__))  pmADCH[];
extern char const __attribute__((__progmem__))  pmADCSRB[];
extern char const __attribute__((__progmem__)) pmDIDR0 [];


// Shell
extern char const __attribute__((__progmem__))  pmHelp[];
extern char const __attribute__((__progmem__))  pmMenu[];
extern char const __attribute__((__progmem__))  pmEna[];
extern char const __attribute__((__progmem__))  pmVars[];
extern char const __attribute__((__progmem__))  pmScript[];
extern char const __attribute__((__progmem__))  pmEcho[];
extern char const __attribute__((__progmem__))  pmCat[];
extern char const __attribute__((__progmem__))  pmRead[];
extern char const __attribute__((__progmem__))  pmWrite[];
extern char const __attribute__((__progmem__))  pmSet[];
extern char const __attribute__((__progmem__))  pmFCPU[];
extern char const __attribute__((__progmem__))  pmTimer[];
extern char const __attribute__((__progmem__))  pmAuth[];
extern char const __attribute__((__progmem__))  pmPasswd[];
extern char const __attribute__((__progmem__))  pmPrint[];
extern char const __attribute__((__progmem__))  pmPowerup[];
extern char const __attribute__((__progmem__))  pmPowerdown[];
extern char const __attribute__((__progmem__))  pmTimer[];
extern char const __attribute__((__progmem__))  pmEnaSvc[];

#endif	/* _AVRSH_PROGMEM_H_ */
