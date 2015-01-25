/*!
 * \file UserMeasurementDB.cpp
 * \author Danilo Treffiletti <urban82@gmail.com>
 * \date 2015-01-25
 * \brief Implementation for the UserMeasurementDB class
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

#include "UserMeasurementDB.hpp"

#include <utils.hpp>
#include <Data/UserData.hpp>

#include <QtSql/QSqlQuery>

namespace BSM {
namespace Data {

const QString UserMeasurementDB::tableName = "UserMeasurement";
const uint UserMeasurementDB::tableVersion = 1;

UserMeasurementDB::UserMeasurementDB(QObject* parent)
    : UserMeasurement(parent)
    , m_userId(0)
{
}

UserMeasurementDB::UserMeasurementDB(UserData* user)
    : UserMeasurement(user)
    , m_userId(user->getId())
{
}

UserMeasurementDB::UserMeasurementDB(const UserMeasurement& measurement, UserData* user)
    : UserMeasurement(user)
    , m_userId(user->getId())
{
    m_dateTime = measurement.getDateTime();
    m_weight = measurement.getWeight();
    m_bodyFatPercent = measurement.getBodyFatPercent();
    m_waterPercent = measurement.getWaterPercent();
    m_musclePercent = measurement.getMusclePercent();
}

UserMeasurementDB::~UserMeasurementDB()
{
}

bool UserMeasurementDB::createTable()
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
    columns.append(Utils::Column("userId", "INTEGER NOT NULL"));
    columns.append(Utils::Column("dateTime", "TEXT NOT NULL"));
    columns.append(Utils::Column("weight", "REAL NOT NULL"));
    columns.append(Utils::Column("bodyFatPercent", "REAL NOT NULL"));
    columns.append(Utils::Column("waterPercent", "REAL NOT NULL"));
    columns.append(Utils::Column("musclePercent", "REAL NOT NULL"));
    Utils::TableConstraintList constraints;
    constraints.append(Utils::TableConstraint(Utils::PrimaryKey, "(userId, dateTime)"));
    constraints.append(Utils::TableConstraint(Utils::ForeignKey, "(userId) REFERENCES UserData(`id`) ON DELETE CASCADE ON UPDATE CASCADE"));
    if (!Utils::createTable(tableName, columns, constraints))
        return false;

    // Save table version
    if (!Utils::setTableVersion(tableName, tableVersion))
        return false;

    return true;
}

UserMeasurementDBList UserMeasurementDB::loadAll(UserData* user)
{
    UserMeasurementDBList list;

    if (!user)
        return list;

    QSqlQuery query;
    if (!query.prepare("SELECT * FROM " + tableName + " WHERE userId = :userId ORDER BY dateTime;")) {
        qCritical() << "Cannot prepare query for UserMeasurementDB::loadAll()";
        return list;
    }
    query.bindValue(":userId", user->getId());
    if (!query.exec()) {
        qCritical() << "Cannot execute query for UserMeasurementDB::loadAll()";
        return list;
    }
    while (query.next()) {
        UserMeasurementDB* um = new UserMeasurementDB(user);
        if (um->parse(query.record())) {
            list.append(um);
        }
        else {
            qWarning() << "Cannot parse record" << query.record();
            delete um;
            um = 0;
        }
    }

    return list;
}

bool UserMeasurementDB::parse(const QSqlRecord& record)
{
    QVariant value;
    bool ok;

    value = record.value("userId");
    if (!value.isValid())
        return false;
    uchar tmp_userId = value.toUInt(&ok);
    if (!ok || tmp_userId != m_userId)
        return false;

    value = record.value("dateTime");
    if (!value.isValid())
        return false;
    m_dateTime = value.toDateTime();

    value = record.value("weight");
    if (!value.isValid())
        return false;
    m_weight = value.toDouble(&ok);
    if (!ok)
        return false;

    value = record.value("bodyFatPercent");
    if (!value.isValid())
        return false;
    m_bodyFatPercent = value.toDouble(&ok);
    if (!ok)
        return false;

    value = record.value("waterPercent");
    if (!value.isValid())
        return false;
    m_waterPercent = value.toDouble(&ok);
    if (!ok)
        return false;

    value = record.value("musclePercent");
    if (!value.isValid())
        return false;
    m_musclePercent = value.toDouble(&ok);
    if (!ok)
        return false;

    return true;
}

uchar UserMeasurementDB::getUserId() const
{
    return m_userId;
}

void UserMeasurementDB::setUserId(const uchar& userId)
{
    m_userId = userId;
}

bool UserMeasurementDB::save()
{
    QSqlQuery query;

    // Check data
    if (m_userId == 0 || !m_dateTime.isValid()) {
        qCritical() << "Could not save" << this;
        return false;
    }

    if (!query.prepare("INSERT OR REPLACE INTO " + tableName +
                               " ( userId,  dateTime,  weight,  bodyFatPercent,  waterPercent,  musclePercent)"
                        " VALUES (:userId, :dateTime, :weight, :bodyFatPercent, :waterPercent, :musclePercent);")) {
        qCritical() << "Cannot prepare query for UserMeasurementDB::save()";
        return false;
    }
    query.bindValue(":userId", m_userId);
    query.bindValue(":dateTime", m_dateTime);
    query.bindValue(":weight", m_weight);
    query.bindValue(":bodyFatPercent", m_bodyFatPercent);
    query.bindValue(":waterPercent", m_waterPercent);
    query.bindValue(":musclePercent", m_musclePercent);
    if (!query.exec()) {
        qCritical() << "Cannot execute query for UserMeasurementDB::save()";
        return false;
    }

    return true;
}

QDebug operator<<(QDebug dbg, const UserMeasurementDB& um)
{
#ifdef QT_NO_DEBUG_OUTPUT
    return dbg;
#else
    dbg.nospace() << "Data::UserMeasurementDB("
                  << um.m_userId << ", "
                  << um.m_dateTime.toString() << " - "
                  << um.m_weight << "kg, "
                  << um.m_bodyFatPercent << "%, "
                  << um.m_waterPercent << "%, "
                  << um.m_musclePercent << "%)";
    return dbg.space();
#endif
}

QDebug operator<<(QDebug dbg, const UserMeasurementDB* um)
{
#ifdef QT_NO_DEBUG_OUTPUT
    return dbg;
#else
    if (um)
        return operator<<(dbg, *um);
    dbg.nospace() << "Data::UserMeasurementDB()";
    return dbg.space();
#endif
}

} // namespace Data
} // namespace BSM
