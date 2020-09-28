#!/usr/bin/env python3

# Copyright (c) 2020 tick <tickelton@gmail.com>
# SPDX-License-Identifier: ISC

import os
import sys
import usb1
from usb1 import REQUEST_TYPE_CLASS, RECIPIENT_DEVICE, ENDPOINT_IN, ENDPOINT_OUT

vendor_id = 0x16c0
product_id = 0x05df

def usage(name):
    print('usage:')
    print('  %s DATA' % name)


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

            if len(sys.argv) == 2:
                buf = sys.argv[1].encode()
                #print('send: buf = "%s" len = %d', (buf, len(buf)))
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

                buf = handle.controlRead(REQUEST_TYPE_CLASS | RECIPIENT_DEVICE | ENDPOINT_IN, 1, 3<<8 | 0, 0, 32, 2000)
                #print('recv: buf = "%s" len = %d', (buf, len(buf)))
                if len(buf) > 0:
                    print('Received: "%s"' % buf.decode())
            else:
                usage(os.path.basename(sys.argv[0]))
                ret = 1
            handle.close()

    sys.exit(ret)


