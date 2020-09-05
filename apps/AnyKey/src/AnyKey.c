#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <avr/pgmspace.h>
#include "usbdrv.h"

#define PIN_BUTTON PINB3
#define PORT_BUTTON PB3
#define DD_BUTTON DDB3

#define KEY_ENTER       0x28
#define KEY_ESC         0x29
#define KEY_BACKSPACE   0x2a
#define KEY_TAB         0x2b
#define KEY_SPACE       0x2c

// Keycode to be sent on key press
#define ANYKEY          KEY_ENTER

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};

typedef struct {
    char    modifier;
    char    key;
} report_t;

typedef struct {
    uchar    buffer[3];
    uint8_t  idx;
} keyBuffer_t;

static keyBuffer_t      keyBuffer;
static report_t         reportBuffer;
static uchar            idleRate;
static uint8_t          button_state;
static uint8_t          button_counter;
static volatile uint8_t key_press;

#ifdef WITH_LED_DEBUGGING

#define LED1 PB1
#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))
#define tbi(var, mask)   ((var) ^= (uint8_t)(1 << mask))

static void blink_led(uint8_t led_no)
{
    uint8_t i;

	sbi(PORTB, led_no);
    for (i = 0; i < 10; i++) {
        _delay_ms(10);
    }
	cbi(PORTB, led_no);
}

static void toggle_led()
{
    tbi(PORTB, LED1);
}
#endif // WITH_LED_DEBUGGING

static void initKeyBuffer(void)
{
    keyBuffer.buffer[0] = ANYKEY;
    keyBuffer.buffer[1] = 0;
    keyBuffer.buffer[2] = 0xff;
    keyBuffer.idx = 0;
}

static void buildReport(void)
{
    reportBuffer.modifier = 0;
    reportBuffer.key = keyBuffer.buffer[keyBuffer.idx];
    keyBuffer.idx++;
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

ISR( TIMER0_OVF_vect )
{
    uint8_t input = PINB & (1 << PIN_BUTTON);

    if (input != button_state) {
        --button_counter;
        if (button_counter == 0xFF) {
            button_counter = 3;
            button_state = input;
            if (!input) { // Negated because button is active high
                key_press = 1;
            }
        }
    } else {
        button_counter = 3;
    }
}

static uint8_t get_key_press()
{
    uint8_t ret;

    cli();
    ret = key_press;
    key_press = 0;
    sei();

    return ret;
}

static void reset_key_buffer()
{
    keyBuffer.idx = 0;
}

int __attribute__((OS_main)) main(void)
{
    uchar i;

    button_state = 1; // Port will be configured as active high, so unpressed state is 1
    button_counter = 0;
    key_press = 0;

#ifdef WITH_LED_DEBUGGING
	DDRB |= 1 << LED1; // output for LED
#endif
    DDRB &= ~(1 << DD_BUTTON); // Set key button as input
    PORTB |= (1 << PORT_BUTTON); // Enable pullup on button pin

    TIFR &= ~(1 << TOV0); // Clear Timer0 overflow interrupt
    TIMSK |= (1 << TOIE0); // Enable Timer0 overflow interrupt
    //TCCR0B |= (1 << CS02); // Start Timer0 with precsaler 256
    TCCR0B |= (1 << CS02 | 1 << CS00); // Start Timer0 with precsaler 1024

    initKeyBuffer();

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

        if (get_key_press()) {
            reset_key_buffer();
        }
        if (usbInterruptIsReady() && keyBuffer.idx < 3) {
            // called after every poll of the interrupt endpoint
            buildReport();
            usbSetInterrupt((void *)&reportBuffer, sizeof(reportBuffer));
        }

    }
}
