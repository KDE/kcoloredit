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

#ifndef PALETTE_DELEGATE_EDITORS_H
#define PALETTE_DELEGATE_EDITORS_H

#include <QtGui/QWidget>

class KColorButton;
class KLineEdit;

class ColorItemEditor : public QWidget
{
    public:
        ColorItemEditor(QWidget * parent = 0);

        QColor color() const;
        void setColor(const QColor & color);

        QString colorName() const;
        void setColorName(const QString & colorName);

    private:
        KColorButton * m_color;
        KLineEdit * m_colorName;
};

class CommentItemEditor : public QWidget
{
    public:
        CommentItemEditor(QWidget * parent = 0);

        QString comment() const;
        void setComment(const QString & comment);

    private:
        KLineEdit * m_comment;
};

#endif // PALETTE_DELEGATE_EDITORS_H
