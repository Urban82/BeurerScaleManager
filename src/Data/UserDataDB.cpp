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

namespace BSM {
namespace Data {

UserDataDB::~UserDataDB()
{
}

UserDataDB::UserDataDB(QObject* parent)
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

} // namespace Data
} // namespace BSM
