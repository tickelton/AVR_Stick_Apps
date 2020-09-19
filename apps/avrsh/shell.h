#ifndef _AVRSH_SHELL_H_
#define _AVRSH_SHELL_H_
////////////////////////////////////////////////////////////////////////////
// Name: shell.h
// Author: B. Gian James <gian@BattleDroids.net>
// Description: The shell file for the AVR Shell. Written to be used with
//              the AVR Telnet Gateway.  Contains the shell functionality.
//
// $Id: shell.h,v 1.5 2009/09/02 12:55:17 gian Exp $
////////////////////////////////////////////////////////////////////////////
#include "common.h"

#define EE_ROOT 0x01
#define MAX_RXBUFF	32
#define MAX_TXBUFF	32

class AVRShell
{
public:
	AVRShell() { timeSec = 0; timect=0; bIsAuthorized = 0; bIsFSOn = 0; }
	
	uint8	ExecCmd(char *, char **);
	uint8	Process(const char * b);
	inline void AddSec() volatile { ++timeSec; }
	
	// Commands
	void	cmdFCPU();
	void	cmdSet(char **);
	void	cmdClock(char **);
	void	cmdTimer(char **);
	void	cmdRead(char **);
	void	cmdWrite(char **);
	void	cmdPowerUp(char **);
	void	cmdPowerDown(char **);
	void	cmdEna(char **);
	void	cmdDir(char **);
	
	
	volatile uint8 timect;
	
private:
	volatile uint8	timeSec;
	uint8			bIsAuthorized;
	uint8			bIsFSOn;

};
#endif	/* _AVRSH_SHELL_H_ */
