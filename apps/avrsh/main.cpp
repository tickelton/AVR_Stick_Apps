////////////////////////////////////////////////////////////////////////////
// Name: main.cpp
// Author: B. Gian James <gian@BattleDroids.net>
// Description: The main file for the AVR Shell. Written to be used with
//              the AVR Telnet Gateway.
//
// $Id: main.cpp,v 1.5 2009/09/02 12:55:17 gian Exp $
////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "common.h"
#include "shell.h"
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

uchar uart_tx_buff[MAX_TXBUFF];
uchar uart_rx_buff[MAX_RXBUFF];

static AVRShell	shell;
uint8_t cmd_received = 0;
uint8_t cmd_to_send = 0;
uint8_t idx_outbuf;
uint8_t idx_inbuf;
uint16_t request_length;
uint16_t request_bytes_remaining;

uchar usbFunctionRead(uchar *data, uchar len)
{
    if (cmd_to_send > 0) {
        if (len > cmd_to_send) {
            len = cmd_to_send;
        }
        for (uint8_t i = 0; i < len; ++i) {
            data[i] = uart_tx_buff[idx_outbuf];
            ++idx_outbuf;
            --cmd_to_send;
        }
        return len;
    }

    return 0;
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
    if (request_length > MAX_RXBUFF) {
        return 0xff;
    }
    if (len > request_bytes_remaining) {
        len = request_bytes_remaining;
    }
    for(uint8_t i = 0; i < len; ++i) {
	uart_rx_buff[idx_inbuf++] = data[i];
        --request_bytes_remaining;
    }

    if (request_bytes_remaining == 0) {
	cmd_received = 1;
	idx_outbuf = 0;
	idx_inbuf = 0;
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
	    memset(uart_rx_buff, ' ', MAX_RXBUFF);
            request_length = rq->wLength.word;
            request_bytes_remaining = rq->wLength.word;
            return USB_NO_MSG;
        }
    } else {
        // no vendor specific requests implemented
    }
    return 0; // default for not implemented requests: return no data back to host
}
int main(void)
{
	uchar i;
	wdt_disable();
	idx_inbuf = 0;

	usbInit();
	usbDeviceDisconnect(); // enforce re-enumeration, do this while interrupts are disabled!
	i = 0;
	while (--i) { // fake USB disconnect for > 250 ms
		_delay_ms(1);
	}
	usbDeviceConnect();

	sei();
	
	for (;;) 
	{	
		usbPoll();
		if (cmd_received) {
			
			int	ret;
			if (  (ret = shell.Process(uart_rx_buff)) ) {
			}
			cmd_received = 0;
		}
	}
}

void	WriteLine(const char * msg)
{
	strncpy(uart_tx_buff, pgm_read_ptr(msg), MAX_TXBUFF);
	uart_tx_buff[MAX_TXBUFF-1] = 0;
	cmd_to_send = (strlen(msg)) > MAX_TXBUFF ? MAX_TXBUFF : (strlen(msg));
}

void	WriteRAM(const char * msg)
{
	strncpy(uart_tx_buff, msg, MAX_TXBUFF);
	uart_tx_buff[MAX_TXBUFF-1] = 0;
	cmd_to_send = (strlen(msg)) > MAX_TXBUFF ? MAX_TXBUFF : (strlen(msg));
}


