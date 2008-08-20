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

#include "gtkcolorselector.h"

#include <QtGui/QVBoxLayout>

#include "trianglecolorwidget.h"

GtkColorSelector::GtkColorSelector(QWidget* parent) : ColorSelector(parent)
{
    m_triangleColorWidget = new TriangleColorWidget(this);

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_triangleColorWidget);

    connect(m_triangleColorWidget, SIGNAL(colorChanged(QColor)), this, SLOT( updateColor(QColor) ));
}

void GtkColorSelector::setColor(const QColor & color)
{
    m_triangleColorWidget->setColor(color);
}

void GtkColorSelector::updateColor(const QColor & color)
{
    emit colorSelected(color);
}

#include "gtkcolorselector.moc"
