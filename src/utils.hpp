/*!
 * \file utils.hpp
 * \author Danilo Treffiletti <urban82@gmail.com>
 * \date 2014-07-17
 * \brief Application global utility functions.
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

#ifndef UTILS_HPP
#define UTILS_HPP

#include <QtCore/QString>

#include <QtSql/QSqlDatabase>

namespace BSM {
namespace Utils {

//! Instance of the DB.
extern QSqlDatabase db;

//! Load the translation for the current language.
void loadTranslation();

//! Check user folder for saving.
bool checkUserDirectory();

//! Retrieve the user folder for saving.
QString getSavingDirectory();

//! Open the DB and check for tables.
bool openDdAndCheckTables();

//! Close the DB
void closeDb();

/*! Execute query on the DB.
 * \param sql the SQL query
 * \return \c true on success or \c false on failure
 */
bool executeQuery(QString sql);

} // namespace Utils
} // namespace BSM

#endif // UTILS_HPP
