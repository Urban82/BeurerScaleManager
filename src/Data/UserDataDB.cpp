/*!
 * \file UserDataDB.cpp
 * \author Danilo Treffiletti <urban82@gmail.com>
 * \date 2014-07-15
 * \brief Header for the UserDataDB class
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

#include "UserDataDB.hpp"

#include <Usb/UsbData.hpp>

namespace BSM {
namespace Data {

UserDataDB::UserDataDB(QObject* parent)
    : UserData(parent)
{
}

UserDataDB::~UserDataDB()
{
}

QString UserDataDB::getName() const
{
    return m_name;
}

void UserDataDB::setName(const QString& name)
{
    m_name = name;
}

QDateTime UserDataDB::getLastDownload() const
{
    return m_lastDownload;
}

void UserDataDB::setLastDownload(const QDateTime& lastDownload)
{
    m_lastDownload = lastDownload;
}

bool UserDataDB::merge(const QDateTime& scaleDateTime, BSM::Data::UserData& userData)
{
    if (userData.getId()        != m_id        ||
        userData.getBirthDate() != m_birthDate ||
        userData.getHeight()    != m_height    ||
        userData.getGender()    != m_gender    ||
        userData.getActivity()  != m_activity
    )
        return false; // Not the correct user, something changed on the scale?

    // Import measurements
    foreach(UserMeasurement* m, userData.getMeasurements()) {
        if (m->getDateTime() > m_lastDownload)
            m_measurements.append(new UserMeasurement(m, this));
    }
    // Save lastDownload
    m_lastDownload = scaleDateTime;

    // TODO Save new data

    return false;
}

} // namespace Data
} // namespace BSM
