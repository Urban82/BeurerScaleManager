/*!
 * \file UserDataDB.hpp
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

#ifndef USERDATADB_HPP
#define USERDATADB_HPP

#include <Data/UserData.hpp>

namespace BSM {

namespace Usb {
    class UsbData;
}

namespace Data {

/*!
 * \class BSM::Data::UserDataDB
 * \brief Extension of UserData to manage extra data saved to DB.
 *
 * This class is intended to save on the DB the data presents in the class UserData,
 * adding some extra data like the name of the user and the date and time of the
 * last download of data from the scale.
 *
 * The class also provide a method to exclude duplicated data from the scale.
 */
class UserDataDB : public UserData
{
    Q_OBJECT
    Q_DISABLE_COPY(UserDataDB)

    /*! The name of the user.
     * \sa getName setName
     */
    Q_PROPERTY(QString name READ getName WRITE setName)
    /*! The date and time of the last download of data from the scale.
     * \sa getLastDownload setLastDownload
     */
    Q_PROPERTY(QDateTime lastDownload READ getLastDownload WRITE setLastDownload)

public:
    /*! Constructor of the class.
     * \param parent the parent QObject
     */
    explicit UserDataDB(QObject* parent = 0);
    virtual ~UserDataDB();

    /*! Getter for the name property.
     * \sa name setName
     */
    QString getName() const;

    /*! Getter for the lastDownload property.
     * \sa lastDownload setLastDownload
     */
    QDateTime getLastDownload() const;

    /*! Merge data from USB.
     *
     * The data received from the USB scale are merged with the current data for
     * the user. The data prior to the last download date and time are ignored.
     * \param scaleDateTime the date and time of the scale for the last download
     * \param userData the user data from the USB scale
     * \return \c true on success or \c false on failure
     * \sa UserData
     */
    bool merge(const QDateTime& scaleDateTime, BSM::Data::UserData& userData);

public slots:
    /*! Setter for the name property.
     * \param name the new value
     * \sa name getName
     */
    void setName(const QString& name);

    /*! Setter for the lastDownload property.
     * \param lastDownload the new value
     * \sa lastDownload getLastDownload
     */
    void setLastDownload(const QDateTime& lastDownload);

protected:
    QString     m_name;         //!< name property value.           \sa name getName setName
    QDateTime   m_lastDownload; //!< lastDownload property value.   \sa lastDownload getLastDownload setLastDownload
};

} // namespace Data
} // namespace BSM

#endif // USERDATADB_HPP
