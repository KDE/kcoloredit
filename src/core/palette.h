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

/**
 * @short This is the palette (a set of color and comment items).
 *
 * This class hold the data of a palette file and , is a wrapper of
 * QList<PaletteItem*>, so this class can store all the subclasses of PaletteItem.
 * <b>Subclasses:</b>\n
 * @li ColorItem.
 * @li CommentItem.
 *
 * Unlike KColorCollection, this class doesn't perform any operation with
 * palette files (PaletteDocument is responsible for load and save palette files).
 * Also, this class suport more features than KColorCollection on storing data.
 * This class has fully support to the GIMP palette specification (like preferred columns)
 * and KDE palette format.
 *
 * @author Percy Camilo Triveño Aucahuasi <orgyforever@gmail.com>
 */
class Palette
{
    public:
        enum MoveOperation { MoveToPrev = 0, MoveToNext, MoveToStart, MoveToEnd };
        static const int MIN_PREFERRED_COLUMNS = 1;
        static const int MAX_PREFERRED_COLUMNS = 50;

        /**
         * Constructs a new Palette.
         */
        Palette();
        /**
         * Destroys the palette and all its data.
         */
        ~Palette();

        /** Returns the name of the palette */
        QString name() const;
        /** Sets the name of the palette */
        void setName(const QString & name);

        /** Returns the description of the palette */
        QString description() const;
        /** Sets the description of the palette */
        void setDescription(const QString & description);

        /** Returns the number of items in the palette */
        int count() const;
        /** Returns true if the palette is empty. */
        bool isEmpty() const;

        /**
         * @param index position of the item.
         * @returns the type of the item.
         */
        PaletteItem::ItemType itemType(int index) const;

        /**
         * @param index position of the color item.
         * @returns a color item.
         */
        ColorItem colorItem(int index) const;
        void appendColorItem(const ColorItem & colorItem);
        void insertColorItem(int index, const ColorItem & colorItem);
        void setColorItem(int index, const ColorItem & colorItem);

        CommentItem commentItem(int index) const;
        void appendCommentItem(const CommentItem & commentItem);
        void insertCommentItem(int index, const CommentItem & commentItem);
        void setCommentItem(int index, const CommentItem & commentItem);

        void setPreferredColumns(int columns);
        int preferredColumns() const;

        void moveItem(int index, Palette::MoveOperation operation);

        void removeItem(int index);

        void clear();

    private:
        void swapItem(int i, int j);

    private:
        QList<PaletteItem *> m_items;
        QString m_name;
        QString m_description;
        int m_preferredColumns;
};

#endif // PALETTE_H
