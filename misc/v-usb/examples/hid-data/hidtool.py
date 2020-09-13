#!/usr/bin/env python3

# Copyright (c) 2020 tick <tickelton@gmail.com>
# SPDX-License-Identifier:	ISC

import os
import sys
import usb1
from usb1 import REQUEST_TYPE_CLASS, RECIPIENT_DEVICE, ENDPOINT_IN, ENDPOINT_OUT

vendor_id = 0x16c0
product_id = 0x05df

def usage(name):
    print('usage:')
    print('  %s read' % name)
    print('  %s write <listofbytes>' % name)
    print('     Example: %s write 0x00 0x4a 0xff' % name)

def find_device(ctx, vendor, product):
    for device in ctx.getDeviceIterator(skip_on_error=True):
        if (device.getVendorID() == vendor
              and device.getProductID() == product):
            return device
    return None


def dump_bytes(buf):
    fill = len(buf)
    while fill > 0:
        for i in range(16):
            print('%#04x ' % buf[-fill], end='')
            fill -= 1
            if fill <= 0:
                break;
        print('')


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
            if len(sys.argv) == 2 and sys.argv[1] == 'read':
                buf = handle.controlRead(REQUEST_TYPE_CLASS | RECIPIENT_DEVICE | ENDPOINT_IN, 1, 3<<8 | 0, 0, 128, 2000)
                if buf:
                    dump_bytes(buf)
            elif len(sys.argv) >= 3 and sys.argv[1] == 'write':
                buf = [int(i, 16) for i in sys.argv[2:]] + [0 for i in range(128 - len(sys.argv[2:]))]
                sent = handle.controlWrite(REQUEST_TYPE_CLASS | RECIPIENT_DEVICE | ENDPOINT_OUT, 9, 3<<8 | 0, 0, buf, 5000)
                if sent != 128:
                    print('Warning: %d bytes sent, expected %d' % (sent, len(sys.argv[2:])))
            else:
                usage(os.path.basename(sys.argv[0]))
                ret = 1
            handle.close()

    sys.exit(ret)


