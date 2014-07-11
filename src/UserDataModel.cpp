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
#include "UserData.hpp"

UserDataModel::UserDataModel(const UserDataList& list, QObject* parent)
    : QAbstractItemModel(parent)
    , m_list(list)
{}

UserDataModel::~UserDataModel()
{}

QVariant UserDataModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    UserData* userData = static_cast<UserData*>(index.internalPointer());
    if (!userData)
        return QVariant();

    if (role == Qt::DisplayRole)
        return QString("P%1 - %2 - %3 cm - %4")
                .arg(userData->getId())
                .arg(userData->getGenderString())
                .arg(userData->getHeight())
                .arg(userData->getBirthDate().toString(Qt::SystemLocaleShortDate))
        ;

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

    UserData* userData = m_list.at(row);
    if (userData)
        return createIndex(row, column, userData);

    return QModelIndex();
}
