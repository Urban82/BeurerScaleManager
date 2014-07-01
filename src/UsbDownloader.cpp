/*!
 * \file UsbDownloader.cpp
 * \author Danilo Treffiletti <danilo@treffiletti.it>
 * \date 2014-07-01
 * \copyright 2014 (c) Danilo Treffiletti
 *
 *    This file is part of BeurerScaleManager.
 *
 *    BeurerScaleManager is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    BeurerScaleManager is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with BeurerScaleManager.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "UsbDownloader.hpp"

#include <libusb.h>

#include <QtCore/QDebug>

//! USB Vendor ID for the Beurer USB Scale
#define BSM_VID             0x04d9
//! USB Product ID for the Beurer USB Scale (found on BF 480 USB model)
#define BSM_PID             0x8010
//! USB interface number
#define USB_INT_NUM         0x0
//! USB control bRequest - HID set report
#define USB_CTRL_REQUEST    0x09
//! USB control wValue
#define USB_CTRL_VALUE      0x0300

UsbDownloader::UsbDownloader(QObject* parent)
    : QObject(parent)
    , ctx(0)
{
    // Initialize libusb session
    if (libusb_init(&ctx) < 0) {
        qCritical() << "Failed to initialize libusb";
        ctx = 0;
        return;
    }

    // Set debug-level to INFO
    libusb_set_debug(ctx, LIBUSB_LOG_LEVEL_INFO);
    qDebug() << "libusb initialized";
}

UsbDownloader::~UsbDownloader()
{
    // Close libusb session
    libusb_exit(ctx);
    qDebug() << "libusb closed";
}

bool UsbDownloader::start()
{
    bool retval = false;

    libusb_device_handle* handle = 0;

    do { // Error loop
        int r;

        // Open USB device
        handle = libusb_open_device_with_vid_pid(ctx, BSM_VID, BSM_PID);
        if (!handle) {
            qCritical() << "Failed to open the device";
            break;
        }
        qDebug() << "USB device opened";

        // Detach kernel driver
        if (libusb_kernel_driver_active(handle, USB_INT_NUM)) {
            qDebug() << "Detaching kernel driver...";
            r = libusb_detach_kernel_driver(handle, USB_INT_NUM);
            if (r < 0) {
                qCritical() << "libusb_detach_kernel_driver error" << r;
                break;
            }
            qDebug() << "Kernel driver detached";
        }

        // Claim interface
        qDebug() << "Claiming interface...";
        r = libusb_claim_interface(handle, USB_INT_NUM);
        if (r < 0) {
            qCritical() << "usb_claim_interface error" << r;
            break;
        }
        qDebug() << "Interface claimed";

        // TODO prepare to send request

        // TODO prepare to receive data
    } while(false);

    // Close USB device
    if (handle) {
        libusb_release_interface(handle, USB_INT_NUM);
        qDebug() << "Released interface";
        libusb_close(handle);
        handle = 0;
        qDebug() << "Closed USB device";
    }

    return retval;
}
