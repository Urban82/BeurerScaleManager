/*!
 * \file UsbData.hpp
 * \author Danilo Treffiletti <urban82@gmail.com>
 * \date 2014-07-07
 * \brief Header for the UsbData class
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

#ifndef USBDATA_HPP
#define USBDATA_HPP

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QByteArray>

#include <Data/UserData.hpp>

namespace BSM {
namespace Usb {

/*!
 * \class BSM::Usb::UsbData
 * \brief Data received via USB from the scale.
 *
 * This class holds the data received from the scale via USB.
 * Some received data are note decoded ore used: for now only users measurements,
 * user parameters (birth date, height, gender and degree of activity) and
 * current date and time from the scale are decoded.
 */
class UsbData : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(UsbData)

    /*! The date and the time of the scale.
     * \sa getDateTime
     */
    Q_PROPERTY(QDateTime dateTime READ getDateTime);
    /*! The data for all the users.
     * \sa getUserData
     */
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    typedef Data::UserDataList Data_UserDataList;
    Q_PROPERTY(Data_UserDataList userData READ getUserData);
#else
    Q_PROPERTY(Data::UserDataList userData READ getUserData); // This one is gust used by doxygen
#endif

public:
    /*! Constructor of the class.
     * \param parent the parent QObject
     */
    explicit UsbData(QObject* parent = 0);
    virtual ~UsbData();

    /*! Getter for the dateTime property.
     * \sa dateTime
     */
    QDateTime getDateTime() const;

    /*! Getter for the userData property.
     * \sa userData
     */
    Data::UserDataList& getUserData();

public Q_SLOTS:
    /*! \brief Parse the USB data.
     *
     * The data received from the scale are parsed to collect each measurement
     * for each user and the extra data the scale send in the USB transfer.
     * \param data the QByteArray containing the USB data
     * \return \c true if the data were parsed successfully, \c false otherwise
     */
    bool parse(const QByteArray& data);

private:
    QDateTime           m_dateTime;
    Data::UserDataList  m_userData;

    friend QDebug operator<<(QDebug dbg, const UsbData& ud);
};

/*! QDebug stream operator for UsbData.
 * \param dbg the QDebug object
 * \param ud the UsbData object
 * \return the QDebug object
 */
QDebug operator<<(QDebug dbg, const UsbData& ud);
/*! QDebug stream operator for UsbData.
 * \param dbg the QDebug object
 * \param ud the UsbData object
 * \return the QDebug object
 */
QDebug operator<<(QDebug dbg, const UsbData* ud);

} // namespace Usb
} // namespace BSM

#endif // USBDATA_HPP
