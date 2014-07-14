/*!
 * \file UserData.hpp
 * \author Danilo Treffiletti <urban82@gmail.com>
 * \date 2014-07-05
 * \brief Header for the UserData class
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

#ifndef USERDATA_HPP
#define USERDATA_HPP

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QDate>
#include <QtCore/QList>

#include "UserMeasurement.hpp"

namespace BSM {

/*!
 * \class BSM::UserData
 * \brief Measurements and personal data for the user.
 *
 * This class holds the measurements data and the personal data for each user.
 *
 * The measurements data are presented as a list of UserMeasurement, the personal
 * data are presented as single value: birth date, height, gender and degree of
 * activity.
 */
class UserData : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(UserData)

public:
    /*! Gender enumerator.
     * \sa gender
     */
    enum Gender {
        Unknown,    //!< Unknown gender
        Male,       //!< Male
        Female      //!< Female
    };

    /*! Degree of activity.
     *
     * The values are reported from the Beurer user manual of the scale BF 480 USB.
     * \sa activity
     */
    enum Activity {
        None,       //!< No activity
        Low,        //!< A small amount of light physical effort (e.g. short walk, light garden work, gymnastic exercises)
        Medium,     //!< Physical effort for 30 minutes at least 2 to 4 times a week
        High,       //!< Physical effort for 30 minutes at least 4 to 6 times a week
        VeryHigh,   //!< Intensive physical effort, intensive training or hard physical work for at least one hour daily
    };

private:
    /*! The ID of the user.
     * \sa getId setId
     */
    Q_PROPERTY(uchar id READ getId WRITE setId);
    /*! The birth date of the user.
     * \sa getBirthDate setBirthDate
     */
    Q_PROPERTY(QDate birthDate READ getBirthDate WRITE setBirthDate);
    /*! The height of the user (in cm).
     * \sa getHeight setHeight
     */
    Q_PROPERTY(uchar height READ getHeight WRITE setHeight);
    /*! The gender of the user.
     * \sa Gender getGender setGender getGenderString
     */
    Q_PROPERTY(Gender gender READ getGender WRITE setGender);
    /*! The degree of activity of the user.
     * \sa Activity getActivity setActivity
     */
    Q_PROPERTY(Activity activity READ getActivity WRITE setActivity);
    /*! The measurements of the user.
     * \sa UserMeasurement UserMeasurementList getMeasurements setMeasurements
     */
    Q_PROPERTY(UserMeasurementList measurements READ getMeasurements WRITE setMeasurements);

public:
    /*! Constructor of the class.
     * \param parent the parent QObject
     */
    explicit UserData(QObject* parent = 0);
    virtual ~UserData();

    /*! Getter for the id property.
     * \sa id setId
     */
    uchar getId() const;

    /*! Getter for the birthDate property.
     * \sa birthDate setBirthDate
     */
    QDate getBirthDate() const;

    /*! Getter for the height property.
     * \sa height setHeight
     */
    uchar getHeight() const;

    /*! Getter for the gender property.
     * \sa Gender gender setGender getGenderString
     */
    Gender getGender() const;

    /*! Getter for the activity property.
     * \sa Activity activity setActivity
     */
    Activity getActivity() const;

    /*! Getter for the measurements property.
     * \sa UserMeasurement UserMeasurementList measurements setMeasurements
     */
    UserMeasurementList& getMeasurements();

    /*! Get gender as string.
     * Retrieves the gender as a single character string: \c M or \c F.
     * A \c ? is returned if an invalid gender is set.
     * \return the gender as string
     * \sa Gender gender getGender setGender
     */
    QString getGenderString() const;

public Q_SLOTS:
    /*! Setter for the id property.
     * \param id the new value
     * \sa id getId
     */
    void setId(const uchar& id);

    /*! Setter for the birthDate property.
     * \param birthDate the new value
     * \sa birthDate getBirthDate
     */
    void setBirthDate(const QDate& birthDate);

    /*! Setter for the height property.
     * \param height the new value
     * \sa height getHeight
     */
    void setHeight(const uchar& height);

    /*! Setter for the gender property.
     * \param gender the new value
     * \sa Gender gender getGender getGenderString
     */
    void setGender(const Gender& gender);

    /*! Setter for the activity property.
     * \param activity the new value
     * \sa Activity activity getActivity
     */
    void setActivity(const Activity& activity);

    /*! Setter for the measurements property.
     * \param measurements the new value
     * \sa UserMeasurement UserMeasurementList measurements getMeasurements
     */
    void setMeasurements(const UserMeasurementList& measurements);

protected:
    uchar               m_id;           //!< id property value.             \sa id getId setId
    QDate               m_birthDate;    //!< birthDate property value.      \sa birthDate getBirthDate setBirthDate
    uchar               m_height;       //!< height property value.         \sa height getHeight setHeight
    Gender              m_gender;       //!< gender property value.         \sa Gender gender getGender setGender getGenderString
    Activity            m_activity;     //!< activity property value.       \sa Activity activity getActivity setActivity
    UserMeasurementList m_measurements; //!< measurements property values.  \sa measurements getMeasurements setMeasurements

    friend QDebug operator<<(QDebug dbg, const UserData& ud);
};

/*! QDebug stream operator for UserData.
 * \param dbg the QDebug object
 * \param ud the UserData object
 * \return the QDebug object
 */
QDebug operator<<(QDebug dbg, const UserData& ud);
/*! QDebug stream operator for UserData.
 * \param dbg the QDebug object
 * \param ud the UserData object
 * \return the QDebug object
 */
QDebug operator<<(QDebug dbg, const UserData* ud);

//! List of user data
typedef QList<UserData*> UserDataList;

} // namespace BSM

#endif // USERDATA_HPP
