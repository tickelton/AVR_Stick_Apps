/*
 * Copyright (C) 2020 <tickelton@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

#include <avr/pgmspace.h>
#include "usbdrv.h"

#define LED1 PB1
#define LED2 PB3
#define LED3 PB4

#define LED_OFF(mask)   (PORTB |= (uint8_t)(1 << mask))
#define LED_ON(mask)   (PORTB &= (uint8_t)~(1 << mask))


PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0                           // END_COLLECTION
};


#define RQ_BUF_LEN 8
uint8_t led_state;

void process_leds()
{
    led_state & 0x1 ? LED_ON(LED1) : LED_OFF(LED1);
    led_state & 0x2 ? LED_ON(LED2) : LED_OFF(LED2);
    led_state & 0x4 ? LED_ON(LED3) : LED_OFF(LED3);
}

/* Sends data to host
 * Returns 0xff on error, otherwise the number of bytes read
 * (which should match the parameter 'len' which will usually
 * be 8).
 * If the number of bytes read is less then the number of bytes
 * requested, the request will be considered finished.
 */
uchar usbFunctionRead(uchar *data, uchar len)
{
    data[0] = led_state;
    return 1;
}

/* Receives data from host
 * Returns 0xff on error, 0 if more data is expected, 1 if
 * if all expected data was received.
 * len can be up to 8, the total length of the payload is
 * known from usbFunctionSetup in the rq->wLength parameter.
 */
uchar usbFunctionWrite(uchar *data, uchar len)
{

    led_state = data[0];
    process_leds();

    return 1;
}

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t *rq = (void *)data;

    if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) { // class request type
        if (rq->bRequest == USBRQ_HID_GET_REPORT) { // wValue: ReportType (highbyte), ReportID (lowbyte)
            // Host wants to read data. Calls usbFunctionRead()
            return USB_NO_MSG;
        } else if (rq->bRequest == USBRQ_HID_SET_REPORT) {
            // Host wants to write data. Calls usbFunctionWrite()
            return USB_NO_MSG;
        }
    } else {
        // no vendor specific requests implemented
    }
    return 0; // default for not implemented requests: return no data back to host
}

void initIO(void)
{
    DDRB |= (1 << LED1 | 1 << LED2 | 1 << LED3);
    LED_OFF(LED1);
    LED_OFF(LED2);
    LED_OFF(LED3);
}

// Main function
int __attribute__((OS_main)) main(void)
{
    uchar i;
    led_state = 0;

    wdt_disable();
    initIO();
    usbInit();

    usbDeviceDisconnect(); // enforce re-enumeration, do this while interrupts are disabled!
    i = 0;
    while (--i) { // fake USB disconnect for > 250 ms
        _delay_ms(1);
    }
    usbDeviceConnect();

    sei();
    for (;;) {
        usbPoll();
    }
}
