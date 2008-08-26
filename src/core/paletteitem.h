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

#ifndef PALETTE_ITEM_H
#define PALETTE_ITEM_H

#include <QtCore/QString>
#include <QtGui/QColor>

class PaletteItem
{
    public:
        enum ItemType { ColorType = 0, CommentType};

    PaletteItem() {}
    virtual ~PaletteItem() {}

    virtual PaletteItem::ItemType type() const = 0;
};

class ColorItem : public PaletteItem
{
    public:
        explicit ColorItem(const QColor & color = Qt::white, const QString & colorName = QString())
            : m_color(color)
            , m_colorName(colorName) {}
        ColorItem(const ColorItem & colorItem)
            : m_color(colorItem.color())
            , m_colorName(colorItem.colorName()) {}
        ~ColorItem() {}

        PaletteItem::ItemType type() const { return PaletteItem::ColorType; }

        QColor color() const { return m_color; }
        void setColor(const QColor & color) { m_color = color; }

        QString colorName() const { return m_colorName; }
        void setColorName(const QString & colorName) { m_colorName = colorName; }

    protected:
        QColor m_color;
        QString m_colorName;
};

class CommentItem : public PaletteItem
{
    public:
        CommentItem(const QString & comment = QString())
            : m_comment(comment) {}
        CommentItem(const CommentItem & commentItem)
            : m_comment(commentItem.comment()) {}
        ~CommentItem() {}

        PaletteItem::ItemType type() const { return PaletteItem::CommentType; }

        QString comment() const { return m_comment; }
        void setComment(const QString comment) { m_comment = comment; }

    protected:
        QString m_comment;
};

#endif // PALETTE_ITEM_H
