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

#include "palettedelegateeditors.h"

#include <QtGui/QLayout>

#include <KColorButton>
#include <KLineEdit>

//BEGIN public class ColorItemEditor

ColorItemEditor::ColorItemEditor(QWidget * parent)
    : QWidget(parent)
{
    m_color = new KColorButton(this);
    m_color->setMinimumWidth(92);

    m_colorName = new KLineEdit(this);
    m_colorName->setClearButtonShown(true);

    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(m_color);
    layout->addWidget(m_colorName);

    setMinimumHeight(40);
}

QColor ColorItemEditor::color() const
{
    return m_color->color();
}

void ColorItemEditor::setColor(const QColor & color)
{
    m_color->setColor(color);
}

QString ColorItemEditor::colorName() const
{
    return m_colorName->text();
}

void ColorItemEditor::setColorName(const QString & colorName)
{
    m_colorName->setText(colorName);
}

//END public class ColorItemEditor

//BEGIN public class CommentItemEditor

CommentItemEditor::CommentItemEditor(QWidget * parent)
    : QWidget(parent)
{
    m_comment = new KLineEdit(this);
    m_comment->setClearButtonShown(true);

    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(m_comment);

    setMinimumHeight(40);
}

QString CommentItemEditor::comment() const
{
    return m_comment->text();
}

void CommentItemEditor::setComment(const QString & comment)
{
    m_comment->setText(comment);
}

//END public class CommentItemEditor
