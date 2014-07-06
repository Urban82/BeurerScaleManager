/*!
 * \file UserMeasurement.hpp
 * \author Danilo Treffiletti <urban82@gmail.com>
 * \date 2014-07-05
 * \brief Header for the UserMeasurement class
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

#ifndef USERMEASUREMENT_HPP
#define USERMEASUREMENT_HPP

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QList>

/*!
 * \class UserMeasurement
 * \brief Measurement for the user.
 *
 * This class holds the measurement data for a single weighing.
 */
class UserMeasurement : public QObject
{
    Q_OBJECT

    /*! The date and the time of the measurement.
     * \sa getDateTime setDateTime
     */
    Q_PROPERTY(QDateTime dateTime READ getDateTime WRITE setDateTime);
    /*! The weight of the measurement (in kg).
     * \sa getWeight setWeight
     */
    Q_PROPERTY(double weight READ getWeight WRITE setWeight);
    /*! The body fat percentage of the measurement.
     * \sa getBodyFatPercent setBodyFatPercent
     */
    Q_PROPERTY(double bodyFatPercent READ getBodyFatPercent WRITE setBodyFatPercent);
    /*! The water percentage of the measurement.
     * \sa getWaterPercent setWaterPercent
     */
    Q_PROPERTY(double waterPercent READ getWaterPercent WRITE setWaterPercent);
    /*! The muscle percentage of the measurement.
     * \sa getMusclePercent setMusclePercent
     */
    Q_PROPERTY(double musclePercent READ getMusclePercent WRITE setMusclePercent);

public:
    /*! Constructor of the class.
     * \param parent the parent QObject
     */
    explicit UserMeasurement(QObject* parent = 0);
    /*! Copy constructor of the class.
     * \param other the other UserMeasurement to copy
     * \param parent the parent QObject
     */
    UserMeasurement(const UserMeasurement& other, QObject* parent = 0);
    virtual ~UserMeasurement();

    /*! Assignment operator.
     * \param other the other UserMeasurement to copy
     */
    virtual UserMeasurement& operator=(const UserMeasurement& other);

    /*! Relative operator (greater than)
     * \param other the other UserMeasurement to compare
     * \return \c true if this UserMeasurement is greater than the \p other, \c false otherwise
     */
    virtual bool operator>(const UserMeasurement& other);
    /*! Relative operator (less than)
     * \param other the other UserMeasurement to compare
     * \return \c true if this UserMeasurement is less than the \p other, \c false otherwise
     */
    virtual bool operator<(const UserMeasurement& other);
    /*! Relative operator (greater or equal than)
     * \param other the other UserMeasurement to compare
     * \return \c true if this UserMeasurement is greater or equal than the \p other, \c false otherwise
     */
    virtual bool operator>=(const UserMeasurement& other);
    /*! Relative operator (less or equal than)
     * \param other the other UserMeasurement to compare
     * \return \c true if this UserMeasurement is less or equal than the \p other, \c false otherwise
     */
    virtual bool operator<=(const UserMeasurement& other);

    /*! Getter for the dateTime property.
     * \sa dateTime setDateTime
     */
    QDateTime getDateTime() const;
    /*! Setter for the dateTime property.
     * \param dateTime the new value
     * \sa dateTime getDateTime
     */
    void setDateTime(const QDateTime& dateTime);

    /*! Getter for the weight property.
     * \sa weight setWeight
     */
    double getWeight() const;
    /*! Setter for the weight property.
     * \param weight the new value
     * \sa weight getWeight
     */
    void setWeight(const double& weight);

    /*! Getter for the bodyFatPercent property.
     * \sa bodyFatPercent setBodyFatPercent
     */
    double getBodyFatPercent() const;
    /*! Setter for the bodyFatPercent property.
     * \param bodyFatPercent the new value
     * \sa bodyFatPercent getBodyFatPercent
     */
    void setBodyFatPercent(const double& bodyFatPercent);

    /*! Getter for the waterPercent property.
     * \sa waterPercent setWaterPercent
     */
    double getWaterPercent() const;
    /*! Setter for the waterPercent property.
     * \param waterPercent the new value
     * \sa waterPercent getWaterPercent
     */
    void setWaterPercent(const double& waterPercent);

    /*! Getter for the musclePercent property.
     * \sa musclePercent setMusclePercent
     */
    double getMusclePercent() const;
    /*! Setter for the musclePercent property.
     * \param musclePercent the new value
     * \sa musclePercent getMusclePercent
     */
    void setMusclePercent(const double& musclePercent);

protected:
    QDateTime   m_dateTime;         //!< dateTime property value.       \sa dateTime getDateTime setDateTime
    double      m_weight;           //!< weight property value.         \sa weight getWeight setWeight
    double      m_bodyFatPercent;   //!< bodyFatPercent property value. \sa bodyFatPercent getBodyFatPercent setBodyFatPercent
    double      m_waterPercent;     //!< waterPercent property value.   \sa waterPercent getWaterPercent setWaterPercent
    double      m_musclePercent;    //!< musclePercent property value.  \sa musclePercent getMusclePercent setMusclePercent
};

//! List of measurements
typedef QList<UserMeasurement> UserMeasurementList;

#endif // USERMEASUREMENT_HPP
