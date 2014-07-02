/*!
 * \file BeurerScaleManager.cpp
 * \author Danilo Treffiletti <danilo@treffiletti.it>
 * \date 2014-07-02
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

#include "BeurerScaleManager.hpp"
#include "ui_BeurerScaleManager.h"

BeurerScaleManager::BeurerScaleManager(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    ui = new Ui::BeurerScaleManager();
    ui->setupUi(this);
}

BeurerScaleManager::~BeurerScaleManager()
{}
