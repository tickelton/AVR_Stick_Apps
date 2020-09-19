#ifndef _AVRSH_COMMON_H_
#define _AVRSH_COMMON_H_
////////////////////////////////////////////////////////////////////////////
/// Name: common.h
/// Author: B. Gian James  <gian@BattleDroids.net>
/// Description: Commonly used things n stuff.
///
/// $Id: common.h,v 1.6 2009/09/05 04:06:17 gian Exp $
////////////////////////////////////////////////////////////////////////////
#ifndef F_CPU
#define F_CPU 16500000
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdint.h>

// Timing & Timers
//////// USART
#define PRESCALER		1024
#define CHECK_TIME		( (F_CPU / PRESCALER) / 4 )				// 250 ms
//#define StartTimer()	BSET(TCCR1B,CS10), BSET(TCCR1B,CS12)	// 1024 prescaler
//#define StopTimer()		BCLR(TCCR1B,CS10), BCLR(TCCR1B,CS12)
//#define ClearTimer()	(TCNT1 = 0)
//////// SPI
#define StartSPITimer()	BSET(TCCR2B,CS20), BSET(TCCR2B,CS22)
#define StopSPITimer()	BCLR(TCCR2B,CS20), BCLR(TCCR2B,CS22)
#define ClearSPITimer()	(TCNT2 = 0)

// This just makes my life easier
#define BIT16(b) ((unsigned long)0x00000001 << (b))
#define	BIT8(b) (0x01 << (b))

// from AVR035: Efficient C Coding for AVR 
#define BSET(ADDRESS,BIT) (ADDRESS |= (unsigned char)(1<<BIT)) 
#define BCLR(ADDRESS,BIT) (ADDRESS &= (unsigned char)~(1<<BIT)) 
#define BTOG(ADDRESS,BIT) (ADDRESS ^= (unsigned char)(1<<BIT)) 
#define BCHK(ADDRESS,BIT) (ADDRESS &  (unsigned char)(1<<BIT)) 

#define BMSET(x,y) (x |= (y)) 
#define BMCLR(x,y) (x &= (~y)) 
#define BMTOG(x,y) (x ^= (y)) 
#define BMCHK(x,y) (x & (y)) 

#define IO_IN	0
#define IO_OUT	1
#define HIGH	1
#define LOW		0

#define _PORTA	0
#define _PORTB	1
#define _PORTC	3
#define _PORTD	4
#define _PORTE	5
#define _PORTF	6
#define _PORTG	7
#define _PORTH	8
#define _PORTJ	9
#define _PORTK	9
#define _PORTL	10

#define PIN0	0
#define PIN1	1
#define PIN2	2
#define PIN3	3
#define PIN4	4
#define PIN5	5
#define PIN6	6
#define PIN7	7

#define NUM_PINS 23

// Typedefs for shorthand
// I wonder how much time programmers around the world have lost from having
// to type "_t"...
typedef uint8_t		uint8;
typedef uint16_t	uint16;
typedef uint32_t	uint32;
typedef unsigned char uchar;

typedef struct
{
	volatile uint8 *	port;
	volatile uint8 *	ddr;
	uint8				pin;
	const char *		name;
	
} PortPin;

extern const PortPin	PortPins[] PROGMEM;


// C++ Sanity Wrappers
#ifdef __cplusplus
extern "C" {
#endif
void __cxa_pure_virtual(void); 
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
void * operator new(size_t size);
void * operator new[] (size_t size);
void operator delete(void * ptr);
void operator delete[] (void * ptr);
#endif

#ifdef __BROKEN_PGMSPACE__
extern char const __attribute__((__progmem__)) conInitialized[];
extern char const __attribute__((__progmem__)) conDoConfig[];
extern char const __attribute__((__progmem__)) *  conMessages[];
#endif

// Utility functions
uint8_t StringCopy(char *, const char *);

extern void WriteLine(const char *);
extern void WriteRAM(const char *);
class ScriptEngine;
extern ScriptEngine & appGetScriptEngine();



#endif	/* _AVRSH_COMMON_H_ */
