////////////////////////////////////////////////////////////////////////////////
/// Name: common.cpp
/// Author: B. Gian James  <gian@BattleDroids.net>
/// Description: Definitions for common things.
///
// $Id: common.cpp,v 1.4 2009/09/02 12:55:17 gian Exp $
////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include <string.h>
#include <stdlib.h>
#include "progmem.h"

const PortPin	PortPins[] = {
	{ &PORTB, &DDRB, PINB0, pmPB0},
	{ &PORTB, &DDRB, PINB1,  pmPB1},
	{ &PORTB, &DDRB, PINB2,  pmPB2},
	{ &PORTB, &DDRB, PINB3,  pmPB3},
	{ &PORTB, &DDRB, PINB4,  pmPB4},
	{ &PORTB, &DDRB, PINB5,  pmPB5},
};

uint8_t
StringCopy(char * dest, const char * source)
{
	uint8_t x;
	for (x = 0; x < strlen(source); x++)
		dest[x] = source[x];		
	dest[x] = '\0';
	return x;
}

void __cxa_pure_virtual(void) {}

void * operator new(size_t size)
{
	return malloc(size);
}

void * operator new [] (size_t size)
{
	return malloc(size);
}

void operator delete(void * ptr)
{
	if (ptr)
		free(ptr);
}

void operator delete [] (void * ptr)
{
	if (ptr)
		free(ptr);
}


//////////////////////////// Strings
