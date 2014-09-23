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

#include <utils.hpp>
#include <Usb/UsbData.hpp>

namespace BSM {
namespace Data {

const QString UserDataDB::tableName = "UserData";
const uint UserDataDB::tableVersion = 1;

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

bool UserDataDB::createTable()
{
    int version = Utils::getTableVersion(tableName);

    // Check if table is already present and updated
    if (version == tableVersion)
        return true;

    // Updates of the table will go here

    // Unknown version: drop and start again!
    // WARNING: all data will be lost, prompt the user?
    if (!Utils::dropTable(tableName))
        return false;

    // Create table
    Utils::ColumnList columns;
    columns.append(Utils::Column("id", "INTEGER PRIMARY KEY NOT NULL"));
    columns.append(Utils::Column("name", "TEXT NOT NULL"));
    columns.append(Utils::Column("birthDate", "TEXT NOT NULL"));
    columns.append(Utils::Column("height", "INTEGER NOT NULL"));
    columns.append(Utils::Column("gender", "INTEGER NOT NULL"));
    columns.append(Utils::Column("activity", "INTEGER NOT NULL"));
    columns.append(Utils::Column("lastDownload", "TEXT NOT NULL"));
    if (!Utils::createTable(tableName, columns))
        return false;

    // Save table version
    if (!Utils::setTableVersion(tableName, tableVersion))
        return false;

    return true;
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
