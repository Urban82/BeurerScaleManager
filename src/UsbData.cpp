/*!
 * \file UsbData.cpp
 * \author Danilo Treffiletti <danilo@treffiletti.it>
 * \date 2014-07-07
 * \brief Implementation for the UsbData class
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

#include "UsbData.hpp"

UsbData::UsbData(QObject* parent)
    : QObject(parent)
{
}

UsbData::~UsbData()
{
}

QDateTime UsbData::getDateTime() const
{
    return m_dateTime;
}

QList<UserData>& UsbData::getUserData()
{
    return m_userData;
}
