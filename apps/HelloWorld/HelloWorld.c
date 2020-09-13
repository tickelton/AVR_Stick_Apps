/*
 * Copyright (c) 2020 tick <tickelton@gmail.com>
 * SPDX-License-Identifier: ISC
 */

#include <avr/io.h>
#include <util/delay.h>
 
int main (void)
{
    DDRB = 0b00001000;

    while (1) {
        PORTB = 0b00001000; 
        _delay_ms(500);
        PORTB = 0b00000000;
        _delay_ms(500);
    }

    return 1;
}
