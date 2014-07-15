/*!
 * \file main.cpp
 * \author Danilo Treffiletti <urban82@gmail.com>
 * \date 2014-06-29
 * \brief Starting point for the application.
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

#include <QtCore/QDebug>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>
#include <QtCore/QLibraryInfo>

#include <QtGui/QApplication>

#include "BeurerScaleManager.hpp"

/*! Starting point for the application.
 * \param argc the number of command-line arguments
 * \param argv the array of command-line arguments
 * \return the exit status value: \c 0 if no errors
 */
int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    // Get translation for qt dialogs
    QTranslator qtTranslator;
    qDebug() << "Loading translation for" << QLocale::system().name();
    if (!qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        if (!qtTranslator.load("qt_" + QLocale::system().name(), "translations")) {
            qWarning() << "Cannot load QT translation for" << QLocale::system().name();
        }
    }
    // TODO Check translations in global path (/usr/share/
    if (!qtTranslator.load(QLocale::system().name(), "translations")) {
        qWarning() << "Cannot load translation for" << QLocale::system().name();
    }
    app.installTranslator(&qtTranslator);

    BSM::BeurerScaleManager win;
    win.show();

    return app.exec();
}
