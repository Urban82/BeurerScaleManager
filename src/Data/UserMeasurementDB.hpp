/*!
 * \file UserMeasurementDB.hpp
 * \author Danilo Treffiletti <urban82@gmail.com>
 * \date 2015-01-25
 * \brief Header for the UserMeasurementDB class
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

#ifndef USERMEASUREMENTDB_HPP
#define USERMEASUREMENTDB_HPP

#include <Data/UserMeasurement.hpp>

#include <QtSql/QSqlRecord>

namespace BSM {
namespace Data {

class UserData;

class UserMeasurementDB;
//! List of user data
typedef QList<UserMeasurementDB*> UserMeasurementDBList;

/*!
 * \class BSM::Data::UserMeasurementDB
 * \brief Extension of UserMeasurement to manage extra data saved to DB.
 *
 * This class is intended to save on the DB the data presents in the class UserMeasurement,
 * adding some extra data like the id of the measurement and the id of the user who owns
 * the measurement.
 */
class UserMeasurementDB : public UserMeasurement
{
    Q_OBJECT
    Q_DISABLE_COPY(UserMeasurementDB)

    /*! The user who owns the measurement.
     * \sa getUserId setUserId
     */
    Q_PROPERTY(uchar userId READ getUserId WRITE setUserId)

public:
    /*! Constructor of the class.
     * \param parent the parent QObject
     */
    explicit UserMeasurementDB(QObject* parent = 0);
    /*! Constructor of the class
     * \param user the parent UserData
     */
    explicit UserMeasurementDB(UserData* user);
    /*! Conversion constructor of the class
     * \param measurement the UserMeasurement value
     * \param user the parent UserData
     */
    explicit UserMeasurementDB(const UserMeasurement& measurement, UserData* user);
    virtual ~UserMeasurementDB();

    /*! Create the DB table.
     * \return \c true on success or \c false on failure
     */
    static bool createTable();

    //! Name of the DB table.
    static const QString tableName;

    //! Version of the table
    static const uint tableVersion;

    /*! Load all user data from the DB
     * \return the list of user data as UserMeasurementDBList
     */
    static UserMeasurementDBList loadAll(UserData* user);

    /*! Getter for the userId property.
     * \sa userId setUserId
     */
    uchar getUserId() const;

    /*! Save data on DB.
     * \return \c true on success or \c false on failure
     */
    bool save();

public slots:
    /*! Setter for the id property.
     * \param userId the new value
     * \sa id getId
     */
    void setUserId(const uchar& userId);

protected:
    uchar       m_userId;   //!< user id property value.    \sa userId getUserId setUserId

    /*! Parse a QSqlRecord into the UserMeasurementDB object
     * \param record the QSqlRecord to parse
     * \return \c true on success or \c false on failure
     */
    bool parse(const QSqlRecord& record);

    friend QDebug operator<<(QDebug dbg, const UserMeasurementDB& um);
};

/*! QDebug stream operator for UserMeasurement.
 * \param dbg the QDebug object
 * \param um the UserMeasurementDB object
 * \return the QDebug object
 */
QDebug operator<<(QDebug dbg, const UserMeasurementDB& um);
/*! QDebug stream operator for UserMeasurement.
 * \param dbg the QDebug object
 * \param um the UserMeasurementDB object
 * \return the QDebug object
 */
QDebug operator<<(QDebug dbg, const UserMeasurementDB* um);

} // namespace Data
} // namespace BSM

#endif // USERMEASUREMENTDB_HPP
