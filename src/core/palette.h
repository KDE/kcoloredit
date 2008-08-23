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

#ifndef PALETTE_H
#define PALETTE_H

#include "paletteitem.h"

#include <QtCore/QList>

class Palette
{
    public:
        enum MoveOperation { MoveToPrev = 0, MoveToNext = 1, MoveToStart = 2, MoveToEnd = 3 };

        Palette();
        ~Palette();

        QString name() const;
        void setName(const QString & name);

        QString description() const;
        void setDescription(const QString & description);

        QString comments() const;

        int count() const;
        bool isEmpty() const;

        PaletteItem * item(int index) const;
        void insertItem(int index, PaletteItem * item);
        void setItem(int index, PaletteItem * item);
        PaletteItem::ItemType itemType(int index) const;

        PaletteColorItem * colorItem(int index) const;
        void appendColorItem(PaletteColorItem * colorItem);
        void insertColorItem(int index, PaletteColorItem * colorItem);
        void setColorItem(int index, PaletteColorItem * colorItem);

        PaletteCommentItem * commentItem(int index) const;
        void appendCommentItem(PaletteCommentItem * commentItem);
        void insertCommentItem(int index, PaletteCommentItem * commentItem);
        void setCommentItem(int index, PaletteCommentItem * commentItem);

        void moveItem(int index, Palette::MoveOperation operation);

        void removeItem(int index);

        void clear();

        Palette & operator = (const Palette & palette);

    private:
        void swapItem(int i, int j);

    private:
        QList<PaletteItem *> m_items;
        QString m_name;
        QString m_description; // NOTE set of comments in the beginig of the file
};

#endif // PALETTE_H
