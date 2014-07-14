/*!
 * \file UserMeasurementModel.cpp
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

#include "UserMeasurementModel.hpp"
#include "UserMeasurement.hpp"

#include <QtGui/QApplication>
#include <QtGui/QPalette>

namespace BSM {

UserMeasurementModel::UserMeasurementModel(const UserMeasurementList& list, QObject* parent)
    : QAbstractItemModel(parent)
    , m_list(list)
{}

UserMeasurementModel::~UserMeasurementModel()
{}

QVariant UserMeasurementModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    UserMeasurement* userMeasurement = static_cast<UserMeasurement*>(index.internalPointer());
    if (!userMeasurement)
        return QVariant();

    QLocale locale;
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: // Date
                return userMeasurement->getDateTime().date().toString(Qt::SystemLocaleLongDate);
            case 1: // Time
                return userMeasurement->getDateTime().time().toString(Qt::SystemLocaleShortDate);
            case 2: // Weight
                return locale.toString(userMeasurement->getWeight(), 'f', 1);
            case 3: // Body fat
                return locale.toString(userMeasurement->getBodyFatPercent(), 'f', 1);
            case 4: // Water
                return locale.toString(userMeasurement->getWaterPercent(), 'f', 1);
            case 5: // Muscle
                return locale.toString(userMeasurement->getMusclePercent(), 'f', 1);
        }
    }
    else if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
            case 0: // Date
            case 1: // Time
                return Qt::AlignHCenter + Qt::AlignVCenter;
            case 2: // Weight
            case 3: // Body fat
            case 4: // Water
            case 5: // Muscle
                return Qt::AlignRight + Qt::AlignVCenter;
        }
    }
    else if (role == Qt::BackgroundRole) {
        return QApplication::palette().color((index.row() % 2 == 0) ? QPalette::Base : QPalette::AlternateBase);
    }

    return QVariant();
}

int UserMeasurementModel::columnCount(const QModelIndex& parent) const
{
    return 6;
}

int UserMeasurementModel::rowCount(const QModelIndex& parent) const
{
    return m_list.size();
}

QModelIndex UserMeasurementModel::parent(const QModelIndex& child) const
{
    return QModelIndex();
}

QModelIndex UserMeasurementModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    UserMeasurement* userMeasurement = m_list.at(row);
    if (userMeasurement)
        return createIndex(row, column, userMeasurement);

    return QModelIndex();
}

QVariant UserMeasurementModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("Date");
            case 1:
                return tr("Time");
            case 2:
                return tr("Weight");
            case 3:
                return tr("% Body fat");
            case 4:
                return tr("% Water");
            case 5:
                return tr("% Muscle");
        }
    }

    return QVariant();
}

} // namespace BSM
