/*!
 * \file UserDataModel.hpp
 * \author Danilo Treffiletti <urban82@gmail.com>
 * \date 2014-07-11
 * \brief Header for the UsbDownloader class
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

#ifndef USERDATAMODEL_HPP
#define USERDATAMODEL_HPP

#include <QtCore/QAbstractItemModel>
#include <QtCore/QVariant>
#include <QtCore/QModelIndex>

#include <Data/UserDataDB.hpp>

namespace BSM {
namespace Data {
namespace Models {

/*!
 * \class BSM::Data::Models::UserDataModel
 * \brief Model for the UserData objects
 *
 * This class is the model to insert a UserDataList in a list-view, like a QComboBox.
 */
class UserDataModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    /*! Constructor of the class.
     * \param list the UserDataList to represents
     * \param parent the parent QObject
     */
    UserDataModel(const UserDataDBList& list, QObject* parent = 0);
    virtual ~UserDataModel();

    //! Returns the data stored under the given \p role for the item referred to by the \p index.
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    //! Returns the number of columns for the children of the given \p parent. This model have only 1 column.
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    //! Returns the number of rows under the given \p parent.
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    //! Returns the parent of the model item with the given \p index. This model is flat, so an invalid QModelIndex is always returned.
    virtual QModelIndex parent(const QModelIndex& child) const;
    //! Returns the index of the item in the model specified by the given \p row, \p column and \p parent index.
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;

private:
    const UserDataDBList  m_list;
};

} // namespace Models
} // namespace Data
} // namespace BSM

#endif // USERDATAMODEL_HPP
