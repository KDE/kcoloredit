/*********************************************************************************
*  Copyright (C) 2008 by Percy Camilo Triveño Aucahuasi <orgyforever@gmail.com>  *
*                                                                                *
*  This program is free software; you can redistribute it and/or modify          *
*  it under the terms of the GNU General Public License as published by          *
*  the Free Software Foundation; either version 2 of the License, or             *
*  (at your option) any later version.                                           *
*                                                                                *
*  This program is distributed in the hope that it will be useful,               *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of                *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 *
*  GNU General Public License for more details.                                  *
*                                                                                *
*  You should have received a copy of the GNU General Public License             *
*  along with this program; if not, write to the                                 *
*  Free Software Foundation, Inc.,                                               *
*  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.                 *
*********************************************************************************/

#ifndef PALETTE_MODEL_H
#define PALETTE_MODEL_H

#include <QtCore/QAbstractTableModel>

#include "palette.h"

class PaletteModel : public QAbstractTableModel
{
    Q_OBJECT

    public:
        PaletteModel(QObject * parent = 0);

        Qt::ItemFlags flags(const QModelIndex & index) const;

        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        int rowCount(const QModelIndex & parent = QModelIndex()) const;

        bool insertColorRows(int row, int count, const QModelIndex & parent = QModelIndex());
        bool insertCommentRows(int row, int count, const QModelIndex & parent = QModelIndex());

        bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

        int columnCount(const QModelIndex & parent = QModelIndex()) const;

        void moveItem(const QModelIndex & itemIndex, Palette::MoveOperation operation);

        QString paletteName() const;
        void setPaletteName(const QString & paletteName);

        QString description() const;
        QString comments() const;

        void generateColorNames();
        void completeColorNames();

    private:
        Palette m_palette;
};

#endif // PALETTE_MODEL_H
