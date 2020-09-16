#!/usr/bin/env python3
''' hidtool.py - a re-implementation of V-USB's 'hidtool' in Python

hidtool originally is the C command line tool provided with V-USB's
'hid-data' example application.
The application demonstrates how to (ab)use HID feature reports
to bi-directionally exchange arbitrary data with an USB HID class
device.
The device acts as a 128 byte data store and the command line tool
lets you read and write data from and to the device.

This script is a re-implementation of the command line tool in
Python for the sake of demonstrating the usage of python-libusb1.

Data from a device is read by calling:

    $ sudo hidtool.py read

Data can be written by calling e.g.:

    $ sudo hidtool.py write 0x01 0x02 0x03

'''

# Copyright (c) 2020 tick <tickelton@gmail.com>
# SPDX-License-Identifier:	ISC

import os
import sys
import usb1
from usb1 import REQUEST_TYPE_CLASS, RECIPIENT_DEVICE, ENDPOINT_IN, ENDPOINT_OUT

VENDOR_ID = 0x16c0
PRODUCT_ID = 0x05df

def usage(name):
    print('usage:')
    print('  %s read' % name)
    print('  %s write <listofbytes>' % name)
    print('     Example: %s write 0x00 0x4a 0xff' % name)


def find_device(context, vendor, product):
    for dev in context.getDeviceIterator(skip_on_error=True):
        if (dev.getVendorID() == vendor
              and dev.getProductID() == product):
            return dev
    return None


def dump_bytes(buf):
    fill = len(buf)
    while fill > 0:
        for i in range(16):
            print('%#04x ' % buf[-fill], end='')
            fill -= 1
            if fill <= 0:
                break
        print('')


if __name__ == '__main__':
    ret = 0

    with usb1.USBContext() as ctx:

        device = find_device(ctx, VENDOR_ID, PRODUCT_ID)

        if device:
            # print(device)
            handle = device.open()
            if not handle:
                print('Something went wrong')
                sys.exit(1)

            if len(sys.argv) == 2 and sys.argv[1] == 'read':

                rbuf = handle.controlRead(
                    REQUEST_TYPE_CLASS | RECIPIENT_DEVICE | ENDPOINT_IN,
                    1,
                    3<<8 | 0,
                    0,
                    128,
                    2000
                )
                if rbuf:
                    dump_bytes(rbuf)

            elif len(sys.argv) >= 3 and sys.argv[1] == 'write':

                wbuf = [int(i, 16) for i in sys.argv[2:]] + [0 for i in range(128 - len(sys.argv[2:]))]
                sent = handle.controlWrite(
                    REQUEST_TYPE_CLASS | RECIPIENT_DEVICE | ENDPOINT_OUT,
                    9,
                    3<<8 | 0,
                    0,
                    wbuf,
                    5000
                )
                if sent != 128:
                    print('Warning: %d bytes sent, expected %d' % (sent, len(sys.argv[2:])))

            else:
                usage(os.path.basename(sys.argv[0]))
                ret = 1

            handle.close()
        else:
            print('error finding DataStore: The specified device was not found')
            ret = 1

    sys.exit(ret)
