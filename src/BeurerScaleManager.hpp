/*!
 * \file BeurerScaleManager.hpp
 * \author Danilo Treffiletti <urban82@gmail.com>
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

#ifndef BEURERSCALEMANAGER_HPP
#define BEURERSCALEMANAGER_HPP

#include <QtGui/QWidget>

namespace Ui
{
class BeurerScaleManager;
}

class UsbDownloader;

/*!
 * \class BeurerScaleManager
 * \brief QWidget for the main window.
 *
 * This class implements the QWidget for the main window.
 */
class BeurerScaleManager : public QWidget
{
    Q_OBJECT

public:
    /*! Constructor of the class.
     * \param parent the parent QWidget
     * \param f the WindowFlags for the widget
     */
    explicit BeurerScaleManager(QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~BeurerScaleManager();

protected slots:
    //! The "Start download" button was clicked
    void startDownload();
    //! The download was completed
    void downloadCompleted(const QByteArray& data);
    //! The download was not completed for an error
    void downloadError();

protected:
    //! The UsbDownloader object
    UsbDownloader* usb;

private:
    Ui::BeurerScaleManager* ui;
};

#endif // BEURERSCALEMANAGER_HPP
