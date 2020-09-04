#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <avr/pgmspace.h>
#include "usbdrv.h"

#define LED1 3
#define LED2 1
#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xA1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM
    0x29, 0x03,                    //     USAGE_MAXIMUM
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Const,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x09, 0x38,                    //     USAGE (Wheel)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7F,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xC0,                          //   END_COLLECTION
    0xC0,                          // END COLLECTION
};

typedef struct{
    uchar   buttonMask;
    char    dx;
    char    dy;
    char    dWheel;
} report_t;

static report_t reportBuffer;
static uchar    idleRate;   // repeat rate for keyboards, never used for mice
static uint8_t sendEvent = 0;

#ifdef WITH_LED_DEBUGGING
void blink_led(uint8_t led_no)
{
    uint8_t i;

	sbi(PORTB, led_no);
    for (i = 0; i < 10; i++) {
        _delay_ms(10);
    }
	cbi(PORTB, led_no);
}
#endif

#define NUM_CURSOR_STEPS    4
static const int8_t cursor_steps[NUM_CURSOR_STEPS][2] = {
    {  5,  0 },
    {  0, -5 },
    { -5,  0 },
    {  0,  5 }
};

static void advanceCursor(void)
{
    static int step = 0;

    reportBuffer.dx = cursor_steps[step][0];
    reportBuffer.dy = cursor_steps[step][1];

    ++step;
    step %= NUM_CURSOR_STEPS;
    if (step == 0) {
        sendEvent = 0;
    }
}

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t *rq = (void *)data;

    /* The following requests are never used. But since they are required by
     * the specification, they are implemented anyway.
     */
    if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) { // class request type
        if (rq->bRequest == USBRQ_HID_GET_REPORT) { // wValue: ReportType (highbyte), ReportID (lowbyte)
            // we only have one report type, so don't look at wValue
            usbMsgPtr = (void *)&reportBuffer;
            return sizeof(reportBuffer);
        } else if (rq->bRequest == USBRQ_HID_GET_IDLE) {
            usbMsgPtr = &idleRate;
            return 1;
        } else if(rq->bRequest == USBRQ_HID_SET_IDLE) {
            idleRate = rq->wValue.bytes[1];
        }
    } else {
        // no vendor specific requests implemented
    }
    return 0; // default for not implemented requests: return no data back to host
}

static void timerInit(void)
{
    TCCR1 = 0x0b; // select clock: 16.5M/1k -> overflow rate = 16.5M/256k = 62.94 Hz
}

static void timerPoll(void)
{
    static uint16_t timerCnt;

    if (TIFR & (1 << TOV1)) { // This flag is triggered at 60 hz.
        TIFR = (1 << TOV1);
		if (++timerCnt >= 14400) { // ~ 4 minute interval
            timerCnt = 0;
			sendEvent = 1;
		}
	}
}

int __attribute__((OS_main)) main(void)
{
    uchar i;

#ifdef WITH_LED_DEBUGGING
	DDRB |= 1 << LED1 | 1 << LED2 | 1<<4; // output for LED
#endif

    wdt_enable(WDTO_1S);
	timerInit(); //Create a timer that will trigger a flag at a ~60hz rate 
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

        if (usbInterruptIsReady() && sendEvent != 0) {
            // called after every poll of the interrupt endpoint
            advanceCursor();
            usbSetInterrupt((void *)&reportBuffer, sizeof(reportBuffer));
        }

        timerPoll();
    }
}
