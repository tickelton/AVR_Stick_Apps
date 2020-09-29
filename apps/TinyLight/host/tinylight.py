#!/usr/bin/env python3

# Copyright (c) 2020 tick <tickelton@gmail.com>
# SPDX-License-Identifier: ISC

import os
import sys
import usb1
from tkinter import *
from usb1 import REQUEST_TYPE_CLASS, RECIPIENT_DEVICE, ENDPOINT_IN, ENDPOINT_OUT

vendor_id = 0x16c0
product_id = 0x05df

PATH_RES = os.path.dirname(__file__)
PATH_OFF = PATH_RES + '/../res/off.png'
PATH_ON1 = PATH_RES + '/../res/on1.png'
PATH_ON2 = PATH_RES + '/../res/on2.png'
PATH_ON3 = PATH_RES + '/../res/on3.png'


def find_device(ctx, vendor, product):
    for device in ctx.getDeviceIterator(skip_on_error=True):
        if (device.getVendorID() == vendor
              and device.getProductID() == product):
            return device
    return None


def get_led_state():

    ret = -1

    with usb1.USBContext() as ctx:

        device = find_device(ctx, vendor_id, product_id);
    
        if device:
            handle = device.open()
            if not handle:
                print('Something went wrong')
                sys.exit(1)


            buf = handle.controlRead(REQUEST_TYPE_CLASS | RECIPIENT_DEVICE | ENDPOINT_IN, 1, 3<<8 | 0, 0, 32, 2000)
            if len(buf) > 0:
                ret = int(buf[0])
            handle.close()

    return ret


def set_led_state(led_state):
    with usb1.USBContext() as ctx:

        device = find_device(ctx, vendor_id, product_id);
    
        if device:
            #print(device)
            handle = device.open()
            if not handle:
                print('Something went wrong')
                sys.exit(1)

            buf = bytes([led_state])
            #print('send: buf = "%s" len = %d' % (buf, len(buf)))
            sent = handle.controlWrite(
                REQUEST_TYPE_CLASS | RECIPIENT_DEVICE | ENDPOINT_OUT,
                9,
                3<<8 | 0,
                0,
                buf,
                5000
            )
            if sent != len(buf):
                print('Warning: %d bytes sent, expected %d' % (sent, len(buf)))

            handle.close()


class Application(Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.led_state = get_led_state()
        self.led_state_imgs = {
            0: PhotoImage(file=PATH_OFF),
            1: PhotoImage(file=PATH_ON1),
            3: PhotoImage(file=PATH_ON2),
            7: PhotoImage(file=PATH_ON3)
        }
        self.master = master
        self.pack()
        self.create_widgets()

    def create_widgets(self):
        self.led_button = Button(self)
        self.led_button["image"] = self.led_state_imgs[self.led_state]
        self.led_button["relief"] = FLAT
        self.led_button["overrelief"] = FLAT
        self.led_button["padx"] = 0
        self.led_button["pady"] = 0
        self.led_button["bd"] = 0
        self.led_button["highlightthickness"] = 0
        self.led_button["command"] = self.update_led
        self.led_button.pack(side="top")

    def update_led(self):
        set_led_state(((self.led_state << 1) + 1) % 15)
        self.led_state = get_led_state()
        self.led_button.configure(image=self.led_state_imgs[self.led_state])


def gui_main():
    root = Tk()
    app = Application(master=root)
    app.mainloop()


if __name__ == '__main__':
    gui_main()
