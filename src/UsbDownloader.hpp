/*!
 * \file UsbDownloader.hpp
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

#ifndef USBDOWNLOADER_HPP
#define USBDOWNLOADER_HPP

#include <QtCore/QThread>
#include <QtCore/QByteArray>

class libusb_context;

/*!
 * \class UsbDownloader
 * \brief Downloader for the data from the scale.
 *
 * This class ask the scale for the data in its memory and then download them.
 * When the download is completed, a signal is emitted. A progress signal is also
 * emitted while downloading.
 */
class UsbDownloader : public QThread
{
    Q_OBJECT

public:
    /*! Constructor of the class.
     * \param parent the parent QObject
     */
    explicit UsbDownloader(QObject* parent = 0);
    virtual ~UsbDownloader();

signals:
    /*! The download was completed.
     * \param data the data downloaded
     */
    void completed(const QByteArray& data);

protected:
    libusb_context* ctx;

    virtual void run();
};

#endif // USBDOWNLOADER_HPP
