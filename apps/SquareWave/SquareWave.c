/*
 * Copyright (c) 2020 tick <tickelton@gmail.com>
 * SPDX-License-Identifier: ISC
 */

#include <avr/io.h>
#include <avr/interrupt.h>
 
uint8_t tcount;

ISR( TIMER0_OVF_vect )
{
//    if (tcount % 2 == 0) {
        PORTB ^= (1<<PB3);
//    }
    ++tcount;
}

void initTimer(void)
{
    TIFR &= ~(1 << TOV0); // Clear Timer0 overflow interrupt
    TIMSK |= (1 << TOIE0); // Enable Timer0 overflow interrupt
    TCCR0B |= (1 << CS01); // Start Timer0 with no prescaler
    //TCCR0B |= (1 << CS02); // Start Timer0 with precsaler 256
    //TCCR0B |= (1 << CS02 | 1 << CS00); // Start Timer0 with precsaler 1024
}

void initIO(void)
{
    DDRB |= (1 << PB3);
}
int main (void)
{
    tcount = 0;

    initIO();
    initTimer();

    sei();
    while (1) {
    }

    return 1;
}
