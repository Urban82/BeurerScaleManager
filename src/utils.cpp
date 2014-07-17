/*!
 * \file utils.cpp
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

#include "utils.hpp"

#include <QtCore/QDebug>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>
#include <QtCore/QLibraryInfo>

#include <QtGui/QApplication>

#include <config.hpp>

namespace BSM {
namespace Utils {

void loadTranslation()
{
    // '-' is added to default delimiters because it is used on Mac OS X instead of '_'.
    const QString searchDelimiters(QLatin1String("_.-"));

    // Get name of locale
    QString localeName = QLocale::system().name();
    qDebug() << "Loading translation for" << localeName;

    // Get translation for qt dialogues
    QTranslator* qtTr = new QTranslator(qApp);
    if (!localeName.startsWith(QLatin1String("en")) &&
        !qtTr->load("qt_" + localeName, QLibraryInfo::location(QLibraryInfo::TranslationsPath), searchDelimiters) &&
        !qtTr->load("qt_" + localeName, "translations", searchDelimiters)
    ) {
        qWarning() << "Cannot load QT translation for" << localeName;
    }
    qApp->installTranslator(qtTr);

    // Get translation for application
    QTranslator* bsmTr = new QTranslator(qApp);
    if (!localeName.startsWith(QLatin1String("en")) &&
        !bsmTr->load(localeName, BSM_CFG_TRANSLATIONS_PATH, searchDelimiters) &&
        !bsmTr->load(localeName, "translations", searchDelimiters)
    ) {
        qWarning() << "Cannot load translation for" << localeName;
    }
    qApp->installTranslator(bsmTr);
}

} // namespace Utils
} // namespace BSM
