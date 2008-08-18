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
    if (m_items.isEmpty())
        return QString();

    if (itemType(0) == PaletteItem::CommentType)
        return commentItem(0)->comment();

    return QString();

/*        

        
    {
        if (m_items.count() == 1)
        {
            if (itemType(0) == PaletteItem::CommentType)
                return commentItem(0)->comment();
            else
                return QString();
        }

        if (m_items.count() > 1)
            if (itemType(1) == PaletteItem::ColorType)
                return commentItem(0)->comment();
    }

    QString tmpDescription;

    int j = 0;

    for (int i = 0; i < m_items.count(); i++)
    {
        if (itemType(i) == PaletteItem::CommentType)
        {
            j++;

            if (i != (j - 1))
                break;

            tmpDescription +=  commentItem(i)->comment() + "\n";
        }
    }

    return tmpDescription;*/
}

QString Palette::comments() const
{
    if (m_items.isEmpty())
        return QString();

    QString tmpFullDescription;

    for (int i = 0; i < m_items.count(); i++)
        if (itemType(i) == PaletteItem::CommentType)
            tmpFullDescription +=  commentItem(i)->comment() + "\n";

    return tmpFullDescription;
}

int Palette::count() const
{
    return m_items.count();
}

bool Palette::isEmpty() const
{
    return m_items.isEmpty();
}

PaletteItem * Palette::item(int index) const
{
    return m_items[index];
}

void Palette::insertItem(int index, PaletteItem * item)
{
    if (!item)
        return;

    m_items.insert(index, item);
}

void Palette::setItem(int index, PaletteItem * item)
{
    if (!item)
        return;

    PaletteItem * wasteItem = m_items[index];

    if (wasteItem)
        delete wasteItem;

    m_items[index] = item;
}

PaletteItem::ItemType Palette::itemType(int index) const
{
    return m_items[index]->type();
}

PaletteColorItem * Palette::colorItem(int index) const
{
    return dynamic_cast<PaletteColorItem *>(item(index));
}

void Palette::appendColorItem(PaletteColorItem * colorItem)
{
    m_items.append(colorItem);
}

void Palette::insertColorItem(int index, PaletteColorItem * colorItem)
{
    m_items.insert(index, colorItem);
}

void Palette::setColorItem(int index, PaletteColorItem * colorItem)
{
    if (itemType(index) != PaletteItem::ColorType)
        return ;

    setItem(index, colorItem);
}

PaletteCommentItem * Palette::commentItem(int index) const
{
    if (itemType(index) != PaletteItem::CommentType)
        return 0;

    return dynamic_cast<PaletteCommentItem *>(item(index));
}

void Palette::appendCommentItem(PaletteCommentItem * commentItem)
{
    m_items.append(commentItem);
}

void Palette::insertCommentItem(int index, PaletteCommentItem * commentItem)
{
    m_items.insert(index, commentItem);
}

void Palette::setCommentItem(int index, PaletteCommentItem * commentItem)
{
    if (itemType(index) != PaletteItem::CommentType)
        return ;

    setItem(index, commentItem);
}

void Palette::moveItem(int index, Palette::MoveOperation operation)
{
    switch (operation)
    {
        case Palette::MoveToPrev:
            if (index == 0)
                break ;

            m_items.swap(index, index - 1);

            break;

        case Palette::MoveToNext:
            if (index == m_items.count() - 1)
                break ;

            m_items.swap(index, index + 1);

            break;

        case Palette::MoveToStart:
            if (index == 0)
                break ;

            m_items.swap(index, 0);

            break;

        case Palette::MoveToEnd:
            if (index == m_items.count() - 1)
                break ;

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


Palette & Palette::operator = (const Palette & palette)
{
    // WARNING TODO test this

    for (int i = 0; i < m_items.count(); i++)
        removeItem(i);

    for (int i = 0; i < palette.count(); i++)
    {
        if (palette.itemType(i) == PaletteItem::ColorType)
            appendColorItem(new PaletteColorItem(palette.colorItem(i)->color(), palette.colorItem(i)->colorName()));
        if (palette.itemType(i) == PaletteItem::CommentType)
            appendCommentItem(new PaletteCommentItem(palette.commentItem(i)->comment()));
    }

    m_name = palette.name();

    return *this;
}

void Palette::swapItem(int i, int j)
{
    m_items.swap(i, j);
}
