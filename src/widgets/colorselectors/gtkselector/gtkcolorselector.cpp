/*********************************************************************************
*  Copyright (C) 2009 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>   *
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

#include <QtGui/QLayout>
#include <QtGui/QToolButton>

#include <KIcon>
#include <KLocalizedString>

#include "trianglecolorwidget.h"

GtkColorSelector::GtkColorSelector(QWidget* parent) : ColorSelector(parent)
{
    m_triangleColorWidget = new TriangleColorWidget(this);

    QToolButton * generateRandomColorButton = new QToolButton(m_triangleColorWidget);
    generateRandomColorButton->setMinimumSize(42, 42);
    generateRandomColorButton->setIcon(KIcon("roll"));
    generateRandomColorButton->setToolTip(i18n("Generate a random color"));

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_triangleColorWidget);

    connect(m_triangleColorWidget, SIGNAL(colorChanged(QColor)), SLOT( updateColor(QColor) ));

    connect(generateRandomColorButton, SIGNAL( pressed () ), SLOT( generateRandomColor() ));
}

void GtkColorSelector::setColor(const QColor & color)
{
    m_triangleColorWidget->setColor(color);
}

void GtkColorSelector::updateColor(const QColor & color)
{
    emit colorSelected(color);
}

void GtkColorSelector::generateRandomColor()
{
    QColor randColor(qrand() % 255, qrand()  % 255, qrand() % 255);

    m_triangleColorWidget->setColor(randColor);
    updateColor(randColor);
}

#include "gtkcolorselector.moc"
