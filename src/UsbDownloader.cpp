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
#include <QtCore/QFile>

//! USB Vendor ID for the Beurer USB Scale
#define BSM_VID             0x04d9
//! USB Product ID for the Beurer USB Scale (found on BF 480 USB model)
#define BSM_PID             0x8010
//! USB interface number for control transfer
#define USB_INTERFACE_IN    0x00
//! USB interface number for interrupt transfer
#define USB_INTERFACE_OUT   0x01
//! USB interrupt data length
#define USB_INTR_DATA_LEN   8
//! USB control bRequest - HID set report
#define USB_CTRL_REQUEST    0x09
//! USB control wValue
#define USB_CTRL_VALUE      0x0300
//! USB control data length
#define USB_CTRL_DATA_LEN   8
//! USB control data first byte value (others are 0x00)
#define USB_CTRL_DATA_FIRST 0x10
//! USB expected data length
#define USB_EXPECTED_LEN    8192

//! File to read to simulate USB data (debug)
#define USB_READ_DUMP       "usbdata.txt"
//! File for the dump of the USB data (debug)
#define USB_WRITE_DUMP      "usbdump.txt"

//! \private
struct UsbDownloaderData {
    bool completed;
    QByteArray data;
#ifdef USB_WRITE_DUMP
    QFile dump;
#endif
};

/*!
 * Callback for the USB control transfer.
 * \param transfer the pointer to the the control transfer
 */
void cb_out(libusb_transfer *transfer);
/*!
 * Callback for the USB interrupt transfer.
 * \param transfer the pointer to the the control transfer
 */
void cb_in(libusb_transfer *transfer);

UsbDownloader::UsbDownloader(QObject* parent)
    : QThread(parent)
    , ctx(0)
{
#ifndef READ_DUMP
    // Initialize libusb session
    if (libusb_init(&ctx) < 0) {
        qCritical() << "Failed to initialize libusb";
        ctx = 0;
        return;
    }

    // Set debug-level to INFO
    libusb_set_debug(ctx, LIBUSB_LOG_LEVEL_INFO);
    qDebug() << "libusb initialized";
#endif
}

UsbDownloader::~UsbDownloader()
{
#ifndef READ_DUMP
    // Close libusb session
    libusb_exit(ctx);
    qDebug() << "libusb closed";
#endif
}

void UsbDownloader::run()
{
    bool hasError = true;
#ifndef USB_READ_DUMP
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
        if (libusb_kernel_driver_active(handle, USB_INTERFACE_IN)) {
            qDebug() << "Detaching kernel driver...";
            r = libusb_detach_kernel_driver(handle, USB_INTERFACE_IN);
            if (r < 0) {
                qCritical() << "libusb_detach_kernel_driver error" << r;
                break;
            }
            qDebug() << "Kernel driver detached";
        }

        // Claim interface
        qDebug() << "Claiming interface...";
        r = libusb_claim_interface(handle, USB_INTERFACE_IN);
        if (r < 0) {
            qCritical() << "usb_claim_interface error" << r;
            break;
        }
        qDebug() << "Interface claimed";

        // Prepare to receive data
        qDebug() << "Register for interrupt data";
        libusb_transfer *transfer_receive = libusb_alloc_transfer(0);
        unsigned char buffer_receive[8];
        UsbDownloaderData usb_data;
#ifdef USB_WRITE_DUMP
        usb_data.dump.setFileName(USB_WRITE_DUMP);
        usb_data.dump.open(QIODevice::WriteOnly | QIODevice::Append);
#endif
        libusb_fill_interrupt_transfer(transfer_receive, handle, LIBUSB_ENDPOINT_IN | USB_INTERFACE_OUT, buffer_receive, sizeof(buffer_receive), cb_in, &usb_data, 30000);
        libusb_submit_transfer(transfer_receive);

        // Prepare to send request
        qDebug() << "Send control request";
        libusb_transfer *transfer_send = libusb_alloc_transfer(0);
        unsigned char buffer_send[LIBUSB_CONTROL_SETUP_SIZE + 8] __attribute__ ((aligned (2)));
        libusb_fill_control_setup(buffer_send, LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE, USB_CTRL_REQUEST, USB_CTRL_VALUE, 0, 8);
        buffer_send[LIBUSB_CONTROL_SETUP_SIZE] = 0x10;
        memset(buffer_send + LIBUSB_CONTROL_SETUP_SIZE + 1, 0, 7);
        libusb_fill_control_transfer(transfer_send, handle, buffer_send, cb_out, 0, 3000);
        libusb_submit_transfer(transfer_send);

        // Wait for completion
        while (!usb_data.completed) {
            qDebug() << "Waiting!";
            r = libusb_handle_events_completed(ctx, 0);
            if (r < 0)
                break;
        }

        // Emit completion signal
        if (usb_data.completed) {
            emit completed(usb_data.data);
            hasError = false;
        }
    } while(false);

    // Close USB device
    if (handle) {
        libusb_release_interface(handle, USB_INTERFACE_IN);
        qDebug() << "Released interface";
        libusb_close(handle);
        handle = 0;
        qDebug() << "Closed USB device";
    }
#else
    do { // Error loop
        QFile usb_data_file(USB_READ_DUMP);
        if (!usb_data_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCritical() << "Failed to open" << USB_READ_DUMP;
            break;
        }

        UsbDownloaderData usb_data;
#ifdef USB_WRITE_DUMP
        usb_data.dump.setFileName(USB_WRITE_DUMP);
        usb_data.dump.open(QIODevice::WriteOnly | QIODevice::Append);
#endif

        while (!usb_data_file.atEnd()) {
            char buff[20];
            qint64 s = usb_data_file.readLine(buff, 20);
            if (s < 16 || s > 17)
                break;

            if (s == 17 && buff[16] != '\n')
                break;

            libusb_transfer t;
            unsigned char b[8];
            bool ok;
            t.buffer = b;
            t.actual_length = 8;
            t.status = LIBUSB_TRANSFER_ERROR; // ignored as error, to avoid resubmit
            t.user_data = &usb_data;
            for(int i = 0; i < 8; ++i) {
                b[i] = (unsigned char) QString("%1%2").arg(buff[i * 2]).arg(buff[i * 2 + 1]).toUShort(&ok, 16);
                if (!ok)
                    break;
            }
            if (!ok)
                break;

            cb_in(&t);
        }

        if (usb_data_file.atEnd()) {
            emit completed(usb_data.data);
            hasError = false;
        }

        usb_data_file.close();
    } while(false);
#endif
    // Emit error signal
    if (hasError)
        emit error();
}

void cb_out(struct libusb_transfer *transfer)
{
    qDebug() << "[OUT]" << "status =" << transfer->status << "- actual length =" << transfer->actual_length;
}

void cb_in(struct libusb_transfer *transfer)
{
    qDebug() << "[IN]" << "status =" << transfer->status << "- actual length =" << transfer->actual_length;
    QString s;
    for (int i = 0; i < transfer->actual_length; ++i) {
        s.append(QString("%1 ").arg((short) transfer->buffer[i], 2, 16, QChar('0')));
    }
    qDebug() << "[IN]" << s.toStdString().c_str();

    UsbDownloaderData* usb_data = (UsbDownloaderData*) transfer->user_data;

#ifdef USB_WRITE_DUMP
    if (usb_data->dump.isOpen() && usb_data->dump.isWritable()) {
        QByteArray buffer((char *)transfer->buffer, transfer->actual_length);
        usb_data->dump.write(buffer.toHex());
        usb_data->dump.write("\n");
        usb_data->dump.close();
    }
#endif

    usb_data->data.append((char *)transfer->buffer, transfer->actual_length);
    if (usb_data->data.size() >= USB_EXPECTED_LEN) {
        usb_data->completed = true;
        return;
    }

    if (transfer->status == LIBUSB_TRANSFER_COMPLETED || transfer->status == LIBUSB_TRANSFER_OVERFLOW)
        libusb_submit_transfer(transfer);
}
