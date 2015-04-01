/*!
 * \file UserDataDB.cpp
 * \author Danilo Treffiletti <urban82@gmail.com>
 * \date 2014-07-15
 * \brief Implementation for the UserDataDB class
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
#include <Data/UserMeasurementDB.hpp>
#include <Usb/UsbData.hpp>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

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

UserDataDBList UserDataDB::loadAll()
{
    UserDataDBList list;

    QSqlQuery query;
    if (!query.prepare("SELECT * FROM " + tableName + " ORDER BY name;")) {
        qCritical() << "Cannot prepare query for UserDataDB::loadAll()" << query.lastError().text();
        return list;
    }
    if (!query.exec()) {
        qCritical() << "Cannot execute query for UserDataDB::loadAll()" << query.lastError().text();
        return list;
    }
    while (query.next()) {
        UserDataDB* ud = new UserDataDB();
        if (ud->parse(query.record())) {
            list.append(ud);
        }
        else {
            qWarning() << "Cannot parse record" << query.record();
            delete ud;
            ud = 0;
        }
    }

    return list;
}

bool UserDataDB::parse(const QSqlRecord& record)
{
    QVariant value;
    bool ok;

    value = record.value("id");
    if (!value.isValid())
        return false;
    m_id = value.toUInt(&ok);
    if (!ok)
        return false;

    value = record.value("name");
    if (!value.isValid())
        return false;
    m_name = value.toString();

    value = record.value("birthDate");
    if (!value.isValid())
        return false;
    m_birthDate = value.toDate();

    value = record.value("height");
    if (!value.isValid())
        return false;
    m_height = value.toUInt(&ok);
    if (!ok)
        return false;

    value = record.value("gender");
    if (!value.isValid())
        return false;
    unsigned gender = value.toUInt(&ok);
    if (!ok)
        return false;
    if (gender < UserData::Male || gender > UserData::Female)
        return false;
    m_gender = (UserData::Gender) gender;

    value = record.value("activity");
    if (!value.isValid())
        return false;
    unsigned activity = value.toUInt(&ok);
    if (!ok)
        return false;
    if (activity > UserData::VeryHigh)
        return false;
    m_activity = (UserData::Activity) activity;

    value = record.value("lastDownload");
    if (!value.isValid())
        return false;
    m_lastDownload = value.toDateTime();

    return true;
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

UserMeasurementDBList UserDataDB::getMeasurements()
{
    return UserMeasurementDB::loadAll(this);
}

bool UserDataDB::merge(const QDateTime& scaleDateTime, const UserData& userData, const UserMeasurementList& userMeasurements)
{
    if (userData.getId()        != m_id        ||
        userData.getBirthDate() != m_birthDate ||
        userData.getGender()    != m_gender
    )
        return false; // Not the correct user, something changed on the scale?

    // Update user parameters
    m_height = userData.getHeight();
    m_activity = userData.getActivity();

    // Import measurements
    foreach(UserMeasurement* m, userMeasurements) {
        if (m->getDateTime() > m_lastDownload) {
            // Save new UserMeasurementDB for this user
            UserMeasurementDB* measurement_db = new UserMeasurementDB(*m, this);
            measurement_db->save();
        }
    }

    // Save lastDownload
    m_lastDownload = scaleDateTime;

    // Save new data
    return save();
}

bool UserDataDB::save() const
{
    QSqlQuery query;
    if (!query.prepare("INSERT OR REPLACE INTO " + tableName +
                               " ( id,  name,  birthDate,  height,  gender,  activity,  lastDownload)"
                        " VALUES (:id, :name, :birthDate, :height, :gender, :activity, :lastDownload);")) {
        qCritical() << "Cannot prepare query for UserDataDB::save()" << query.lastError().text();
        return false;
    }
    query.bindValue(":id", m_id);
    query.bindValue(":name", m_name);
    query.bindValue(":birthDate", m_birthDate);
    query.bindValue(":height", m_height);
    query.bindValue(":gender", m_gender);
    query.bindValue(":activity", m_activity);
    query.bindValue(":lastDownload", m_lastDownload);
    if (!query.exec()) {
        qCritical() << "Cannot execute query for UserDataDB::save()" << query.lastError().text();
        return false;
    }

    return true;
}

QDebug operator<<(QDebug dbg, const UserDataDB& ud)
{
#ifdef QT_NO_DEBUG_OUTPUT
    return dbg;
#else
    dbg.nospace() << "Data::UserDataDB("
                  << ud.m_id << ", "
                  << ud.m_name << ", "
                  << ud.m_birthDate.toString() << ", "
                  << ud.m_height << "cm, ";
    switch (ud.m_gender) {
        case UserData::Male:
            dbg.nospace() << "M";
            break;
        case UserData::Female:
            dbg.nospace() << "F";
            break;
        default:
            dbg.nospace() << "?";
            break;
    }
    dbg.nospace() << ", "
                  << (int) ud.m_activity << ", "
                  << ud.m_lastDownload.toString();
    dbg.nospace() << ")";
    return dbg.space();
#endif
}

QDebug operator<<(QDebug dbg, const UserDataDB* ud)
{
#ifdef QT_NO_DEBUG_OUTPUT
    return dbg;
#else
    if (ud)
        return operator<<(dbg, *ud);
    dbg.nospace() << "Data::UserDataDB()";
    return dbg.space();
#endif
}

} // namespace Data
} // namespace BSM
