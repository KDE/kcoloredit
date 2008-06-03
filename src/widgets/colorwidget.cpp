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

#include "colorwidget.h"

#include <QtGui/QVBoxLayout>

#include <KColorPatch>

ColorWidget::ColorWidget(QWidget * parent)
    : QWidget(parent)
{
    m_colorPatch = new KColorPatch(this);
    setMinimumHeight(64);
    m_colorPatch->setColor(Qt::red); // default color

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(m_colorPatch);

    connect(m_colorPatch, SIGNAL( colorChanged(QColor) ), this, SLOT( updateColor(QColor) ));
}

QColor ColorWidget::color() const
{
    return m_color;
}

void ColorWidget::setColor(const QColor & color)
{
    m_color = color;

    m_colorPatch->setColor(m_color);

    emit colorChanged(m_color);
}

void ColorWidget::updateColor(const QColor & color)
{
    m_color = color;

    emit colorChanged(m_color);
}

#include "colorwidget.moc"
