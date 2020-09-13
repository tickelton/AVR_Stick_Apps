#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

#include <avr/pgmspace.h>
#include "usbdrv.h"

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x80,                    //   REPORT_COUNT (128)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0                           // END_COLLECTION
};

uint16_t request_length;
uint16_t request_bytes_remaining;

#define RQ_BUF_LEN 32
char inbuf[RQ_BUF_LEN];
uint8_t idx_inbuf;
char outbuf[RQ_BUF_LEN];
uint8_t idx_outbuf;

void process_inbuf()
{
    if (idx_inbuf == 3) {
        if(strncmp(inbuf, "foo", 3) == 0) {
            strncpy(outbuf, "bar", 3);
            idx_outbuf = 0;
            request_length = 3;
            request_bytes_remaining = 3;
            return;
        }
    } else if (idx_inbuf == 8) {
        if (strncmp(inbuf, "blafasel", 8) == 0) {
            strncpy(outbuf, "foobar", 6);
            idx_outbuf = 0;
            request_length = 6;
            request_bytes_remaining = 6;
            return;
        }
    }
    idx_outbuf = 0;
    request_length = 0;
    request_bytes_remaining = 0;
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
    if (request_bytes_remaining > 0) {
        if (len > request_bytes_remaining) {
            len = request_bytes_remaining;
        }
        for (uint8_t i = 0; i < len; ++i) {
            data[i] = outbuf[idx_outbuf];
            ++idx_outbuf;
            --request_bytes_remaining;
        }
        return len;
    }

    return 0;
}

/* Receives data from host
 * Returns 0xff on error, 0 if more data is expected, 1 if
 * if all expected data was received.
 * len can be up to 8, the total length of the payload is
 * known from usbFunctionSetup in the rq->wLength parameter.
 */
uchar usbFunctionWrite(uchar *data, uchar len)
{
    if (request_length > RQ_BUF_LEN) {
        return 0xff;
    }
    if (len > request_bytes_remaining) {
        len = request_bytes_remaining;
    }
    for(uint8_t i = 0; i < len; ++i) {
        inbuf[idx_inbuf] = data[i];
        ++idx_inbuf;
        --request_bytes_remaining;
    }

    if (request_bytes_remaining == 0) {
        process_inbuf();
        return 1;
    }
    return 0;
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
            memset(inbuf, 0, RQ_BUF_LEN);
            idx_inbuf = 0;
            request_length = rq->wLength.word;
            request_bytes_remaining = rq->wLength.word;
            return USB_NO_MSG;
        }
    } else {
        // no vendor specific requests implemented
    }
    return 0; // default for not implemented requests: return no data back to host
}

// Main function
int __attribute__((OS_main)) main(void)
{
    uchar i;

    wdt_enable(WDTO_1S);
    usbInit();

    usbDeviceDisconnect(); // enforce re-enumeration, do this while interrupts are disabled!
    i = 0;
    while (--i) { // fake USB disconnect for > 250 ms
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();

    sei();
    for (;;) {
        wdt_reset();
        usbPoll();
    }
}
