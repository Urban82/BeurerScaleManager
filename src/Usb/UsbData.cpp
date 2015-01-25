/*!
 * \file UsbData.cpp
 * \author Danilo Treffiletti <urban82@gmail.com>
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

#include <QtCore/QDate>
#include <QtCore/QTime>

namespace BSM {
namespace Usb {

//! Number of users in the scale memory.
#define NUM_USERS        10
//! Number of variables saved for each user.
#define NUM_USER_VARS     6
//! Number of samples in each variable
#define NUM_SAMPLES      60
//! Size in byte of each sample.
#define SAMPLE_LEN        2
//! Size in byte of the variables separator (all with value 0)
#define SEPARATOR_LEN     8

//! Size in byte of a variable
#define VAR_LEN         (NUM_SAMPLES * SAMPLE_LEN + SEPARATOR_LEN)
//! Size in byte of an user block
#define USER_LEN        (NUM_USER_VARS * VAR_LEN)
//! Size in byte of the last block.
#define EXTRA_BLOCK_LEN 512
//! Size in byte of the offset in the last block.
#define EXTRA_BLOCK_OFF (NUM_USERS * USER_LEN + 256)
//! Size in byte of each user block in the extra block
#define EXTRA_USER_LEN    8
//! Size in byte of the offset in the ptr block
#define PTR_BLOCK_OFF   (EXTRA_BLOCK_OFF + NUM_USERS * EXTRA_USER_LEN + 16)

//! Size in byte of the offset of the scale date
#define SCALE_DATE_OFF  (NUM_USERS * USER_LEN + 480)
//! Size in byte of the offset of the scale time
#define SCALE_TIME_OFF  (NUM_USERS * USER_LEN + 483)

//! Expected length in byte for all data.
#define EXPECTED_LEN    (NUM_USERS * USER_LEN + EXTRA_BLOCK_LEN)

/*! Convert two bytes to a unsigned short.
 * \param b1 the higher byte
 * \param b2 the lower byte
 * \return the unsigned short
 */
ushort uchar2ushort(const uchar b1, const uchar b2);

/*! Convert an unsigned short to a QDate.
 *
 * The conversion is done using the first 7 bit for the year (starting from the
 * 1920), 4 bits for the month and 5 bits for the day.
 *
 * \param b1 the higher byte
 * \param b2 the lower byte
 * \return the date, or a \c null QDate if invalid
 */
QDate uchar2QDate(const uchar b1, const uchar b2);

/*! Convert an unsigned short to a QTime
 *
 * The conversion is done using the higher byte for the hours and the lower byte
 * for the minutes.
 *
 * \param b1 the higher byte
 * \param b2 the lower byte
 * \return the date, or a \c null QDate if invalid
 */
QTime uchar2QTime(const uchar b1, const uchar b2);

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

Data::UserDataList& UsbData::getUserData()
{
    return m_userData;
}

Data::UserMeasurementList& UsbData::getUserMeasurements(const Data::UserData& user)
{
    return m_userMeasurements[user.getId()];
}

bool UsbData::parse(const QByteArray& data)
{
    if (data.size() != EXPECTED_LEN)
        return false;

    QDate scale_date = uchar2QDate(data[SCALE_DATE_OFF], data[SCALE_DATE_OFF + 1]);
    QTime scale_time = uchar2QTime(data[SCALE_TIME_OFF], data[SCALE_TIME_OFF + 1]);
    m_dateTime = QDateTime(scale_date, scale_time);

    qDeleteAll(m_userData);
    m_userData.clear();

    for (int user = 0; user < NUM_USERS; ++user) {
        int user_offset = user * USER_LEN;
        int extra_offset = EXTRA_BLOCK_OFF + user * EXTRA_USER_LEN;

        uchar id = data[extra_offset];
        if (id < 1 || id > 10)
            continue;

        Data::UserData* ud = new Data::UserData();
        ud->setId(id);
        ud->setHeight(data[extra_offset + 1]);
        ud->setBirthDate(uchar2QDate(data[extra_offset + 2], data[extra_offset + 3]));
        ud->setGender( ((data[extra_offset + 4] & 0x80) == 0x00) ? Data::UserData::Male : Data::UserData::Female );
        switch (data[extra_offset + 4] & 0x0F) {
            case 0:
                ud->setActivity(Data::UserData::None);
                break;
            case 1:
                ud->setActivity(Data::UserData::Low);
                break;
            case 2:
                ud->setActivity(Data::UserData::Medium);
                break;
            case 3:
                ud->setActivity(Data::UserData::High);
                break;
            case 4:
                ud->setActivity(Data::UserData::VeryHigh);
                break;
            default:
                // Invalid value, set to None
                ud->setActivity(Data::UserData::None);
                break;
        }

        uchar num_samples = data[extra_offset + 5];
        uchar ptr_samples = data[PTR_BLOCK_OFF + user];

        for (int sample = 0; sample < NUM_SAMPLES && sample < num_samples; ++sample) {
            int sample_offset = sample;
            if (num_samples == NUM_SAMPLES)
                sample_offset = (sample + ptr_samples) % NUM_SAMPLES;
            int weight_offset  = user_offset + 0 * VAR_LEN + sample_offset * SAMPLE_LEN;
            int bodyFat_offset = user_offset + 1 * VAR_LEN + sample_offset * SAMPLE_LEN;
            int water_offset   = user_offset + 2 * VAR_LEN + sample_offset * SAMPLE_LEN;
            int muscle_offset  = user_offset + 3 * VAR_LEN + sample_offset * SAMPLE_LEN;
            int date_offset    = user_offset + 4 * VAR_LEN + sample_offset * SAMPLE_LEN;
            int time_offset    = user_offset + 5 * VAR_LEN + sample_offset * SAMPLE_LEN;

            QDate date = uchar2QDate(data[date_offset], data[date_offset + 1]);
            QTime time = uchar2QTime(data[time_offset], data[time_offset + 1]);
            QDateTime dateTime = QDateTime(date, time);
            if (dateTime.isNull() || !dateTime.isValid())
                break;

            Data::UserMeasurement* um = new Data::UserMeasurement();
            um->setWeight(uchar2ushort(data[weight_offset], data[weight_offset + 1]) * 0.1);
            um->setBodyFatPercent(uchar2ushort(data[bodyFat_offset], data[bodyFat_offset + 1]) * 0.1);
            um->setWaterPercent(uchar2ushort(data[water_offset], data[water_offset + 1]) * 0.1);
            um->setMusclePercent(uchar2ushort(data[muscle_offset], data[muscle_offset + 1]) * 0.1);
            um->setDateTime(dateTime);

            m_userMeasurements[id].append(um);
        }

        m_userData.append(ud);
    }

    return true;
}

QDebug operator<<(QDebug dbg, const UsbData& ud)
{
#ifdef QT_NO_DEBUG_OUTPUT
    return dbg;
#else
    dbg.nospace() << "Usb::UsbData("
                  << ud.m_dateTime.toString() << ", "
                  << ud.m_userData.size() << " " << ud.m_userData << ")";
    return dbg.space();
#endif
}

QDebug operator<<(QDebug dbg, const UsbData* ud)
{
#ifdef QT_NO_DEBUG_OUTPUT
    return dbg;
#else
    if (ud)
        return operator<<(dbg, *ud);
    dbg.nospace() << "Usb::UsbData()";
    return dbg.space();
#endif
}

ushort uchar2ushort(const uchar b1, const uchar b2)
{
    ushort word = b1;
    word = (word << 8) + b2;
    return word;
}

QDate uchar2QDate(const uchar b1, const uchar b2)
{
    ushort word = uchar2ushort(b1, b2);
    if (word == 0 || word == 0xFFFF)
        return QDate();
    return QDate(1920 + (word >> 9), (word >> 5) & 0xF, word & 0x1F);
}

QTime uchar2QTime(const uchar b1, const uchar b2)
{
    if (b1 > 23 || b2 > 59)
        return QTime();
    return QTime(b1, b2);
}

} // namespace Usb
} // namespace BSM
