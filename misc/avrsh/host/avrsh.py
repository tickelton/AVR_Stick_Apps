#!/usr/bin/env python3
'''
Python client for the avrsh firmware on the AVR Stick

This scripts acts as the client terminal application for
interfacing with avrsh on an AVR Stick over HID feature
reports.

Example usage:

    $ sudo python avrsh.py
    > fcpu
    CPU Freq: 16500000
    > set pin PB3 output
    Set PB3 for output
    > write pin PB3 high
    Wrote logic high to pin PB3
    > write pin PB3 low
    Wrote logic low to pin PB3

'''

# Copyright (c) 2020 tick <tickelton@gmail.com>
# SPDX-License-Identifier: ISC

import os
import sys
import usb1
from usb1 import REQUEST_TYPE_CLASS, RECIPIENT_DEVICE, ENDPOINT_IN, ENDPOINT_OUT

vendor_id = 0x16c0
product_id = 0x05df


def find_device(ctx, vendor, product):
    for device in ctx.getDeviceIterator(skip_on_error=True):
        if (device.getVendorID() == vendor
              and device.getProductID() == product):
            return device
    return None


if __name__ == '__main__':
    ret = 0

    with usb1.USBContext() as ctx:

        device = find_device(ctx, vendor_id, product_id);
    
        if device:
            # print(device)
            handle = device.open()
            if not handle:
                print('Something went wrong')
                sys.exit(1)

            while True:
                try:
                    cmd = input('> ')
                    if len(cmd) < 1:
                        continue
                    cmdlist = cmd.split(' ')

                    #print('send: cmd = "%s" len = %d', (cmd.encode(), len(cmd)))
                    sent = handle.controlWrite(
                        REQUEST_TYPE_CLASS | RECIPIENT_DEVICE | ENDPOINT_OUT,
                        9,
                        3<<8 | 0,
                        0,
                        cmd.encode(),
                        5000
                    )
                    if sent != len(cmd):
                        print('Warning: %d bytes sent, expected %d' % (sent, len(cmd)))

                    while True:
                        buf = handle.controlRead(REQUEST_TYPE_CLASS | RECIPIENT_DEVICE | ENDPOINT_IN, 1, 3<<8 | 0, 0, 32, 2000)
                        #print('recv: buf = "%s" len = %d', (buf, len(buf)))
                        if len(buf) == 0:
                            break
                        else:
                            print(buf.decode(), end='')
                except (KeyboardInterrupt, EOFError):
                    break;

            handle.close()

    sys.exit(ret)


