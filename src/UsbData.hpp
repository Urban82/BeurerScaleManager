/*!
 * \file UsbData.hpp
 * \author Danilo Treffiletti <danilo@treffiletti.it>
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
#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QByteArray>

#include "UserData.hpp"

/*!
 * \class UsbData
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

    /*! The date and the time of the scale.
     * \sa getDateTime
     */
    Q_PROPERTY(QDateTime dateTime READ getDateTime);
    /*! The data for all the users.
     * \sa getUserData
     */
    Q_PROPERTY(QList<UserData> userData READ getUserData);

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
    QList<UserData>& getUserData();

private:
    QDateTime       m_dateTime;
    QList<UserData> m_userData;
};

#endif // USBDATA_HPP
