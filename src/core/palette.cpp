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

#include "palette.h"

//BEGIN public constructors

Palette::Palette()
    : m_preferredColumns(Palette::MIN_PREFERRED_COLUMNS)
{
}

//END public constructors

//BEGIN public destructor

Palette::~Palette()
{
    while (!m_items.isEmpty())
    {
        PaletteItem * item = m_items.takeFirst();

        if (item)
            delete item;

        item = 0;
    }
}

//END public destructor

//BEGIN public methods

QString Palette::name() const
{
    return m_name;
}

void Palette::setName(const QString & name)
{
    m_name = name;
}

QString Palette::description() const
{
    return m_description;
}

void Palette::setDescription(const QString & description)
{
    m_description = description;
}

int Palette::count() const
{
    return m_items.count();
}

bool Palette::isEmpty() const
{
    return m_items.isEmpty();
}

PaletteItem::ItemType Palette::itemType(int index) const
{
    return m_items[index]->type();
}

ColorItem Palette::colorItem(int index) const
{
    if (itemType(index) != PaletteItem::ColorType)
        return ColorItem(QColor::Invalid, QString());

    return ColorItem(*dynamic_cast<ColorItem *>(m_items[index]));
}

void Palette::appendColorItem(const ColorItem & colorItem)
{
    m_items.append(new ColorItem(colorItem));
}

void Palette::insertColorItem(int index, const ColorItem & colorItem)
{
    m_items.insert(index, new ColorItem(colorItem));
}

void Palette::setColorItem(int index, const ColorItem & colorItem)
{
    if (itemType(index) != PaletteItem::ColorType)
        return ;

    if (ColorItem * tmpColorItem = dynamic_cast<ColorItem *>(m_items[index]))
    {
        tmpColorItem->setColor(colorItem.color());
        tmpColorItem->setColorName(colorItem.colorName());
    }
}

CommentItem Palette::commentItem(int index) const
{
    if (itemType(index) != PaletteItem::CommentType)
        return CommentItem();

    return CommentItem(*dynamic_cast<CommentItem *>(m_items[index]));
}

void Palette::appendCommentItem(const CommentItem & commentItem)
{
    m_items.append(new CommentItem(commentItem));
}

void Palette::insertCommentItem(int index, const CommentItem & commentItem)
{
    m_items.insert(index, new CommentItem(commentItem));
}

void Palette::setCommentItem(int index, const CommentItem & commentItem)
{
    if (itemType(index) != PaletteItem::CommentType)
        return ;

    if (CommentItem * tmpCommentItem = dynamic_cast<CommentItem *>(m_items[index]))
        tmpCommentItem->setComment(commentItem.comment());
}

void Palette::setPreferredColumns(int columns)
{
    m_preferredColumns = columns;
}

int Palette::preferredColumns() const
{
    return m_preferredColumns;
}

void Palette::moveItem(int index, Palette::MoveOperation operation)
{
    switch (operation)
    {
        case Palette::MoveToPrev:
            if (index == 0)
                return ;

            m_items.swap(index, index - 1);

            break;

        case Palette::MoveToNext:
            if (index == m_items.count() - 1)
                return ;

            m_items.swap(index, index + 1);

            break;

        case Palette::MoveToStart:
            if (index == 0)
                return ;

            m_items.swap(index, 0);

            break;

        case Palette::MoveToEnd:
            if (index == m_items.count() - 1)
                return ;

            m_items.swap(index, m_items.count() - 1);

            break;
    }
}

void Palette::removeItem(int index)
{
    // NOTE safe deleting of items

    PaletteItem * item = m_items[index];

    if (item)
    {
        delete item;

        item = 0;
    }

    m_items.removeAt(index);
}

void Palette::clear()
{
    m_items.clear();
}

//END public methods

//BEGIN private methods

void Palette::adjustPreferredColumns()
{
}

void Palette::swapItem(int i, int j)
{
    m_items.swap(i, j);
}

//END private methods
