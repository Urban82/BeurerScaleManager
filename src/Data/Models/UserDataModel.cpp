/*!
 * \file UserDataModel.cpp
 * \author Danilo Treffiletti <urban82@gmail.com>
 * \date 2014-07-11
 * \brief Implementation for the UsbDownloader class
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

#include "UserDataModel.hpp"
#include <Data/UserData.hpp>

namespace BSM {
namespace Data {
namespace Models {

UserDataModel::UserDataModel(const UserDataDBList& list, QObject* parent)
    : QAbstractItemModel(parent)
    , m_list(list)
{}

UserDataModel::~UserDataModel()
{}

QVariant UserDataModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    UserDataDB* userData = static_cast<UserDataDB*>(index.internalPointer());
    if (!userData)
        return QVariant();

    if (role == Qt::DisplayRole)
        return userData->getName();

    return QVariant();
}

int UserDataModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

int UserDataModel::rowCount(const QModelIndex& parent) const
{
    return m_list.size();
}

QModelIndex UserDataModel::parent(const QModelIndex& child) const
{
    return QModelIndex();
}

QModelIndex UserDataModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    UserDataDB* userData = m_list.at(row);
    if (userData)
        return createIndex(row, column, userData);

    return QModelIndex();
}

} // namespace Models
} // namespace Data
} // namespace BSM
