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

#include "kdecolorselector.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>

#include <KLocalizedString>
#include <KComboBox>

#include "rectanglecolorwidget.h"

KdeColorSelector::KdeColorSelector(QWidget * parent) : ColorSelector(parent)
{
    m_rectangleColorWidget = new RectangleColorWidget(this);

    KComboBox * colorMode = new KComboBox(this);
    colorMode->addItem(i18n("Classic"));
    colorMode->addItem(i18n("Hue"));
    colorMode->addItem(i18n("Saturation"));
    colorMode->addItem(i18n("Value"));
    colorMode->addItem(i18n("Red"));
    colorMode->addItem(i18n("Green"));
    colorMode->addItem(i18n("Blue"));
    colorMode->setCurrentIndex(3);

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_rectangleColorWidget);
    mainLayout->addWidget(colorMode);

    connect(m_rectangleColorWidget, SIGNAL(colorSelected(QColor)), this, SLOT( updateColor(QColor) ));
    connect(colorMode, SIGNAL( currentIndexChanged(int) ), this, SLOT( changeColorMode(int) ));
}

KdeColorSelector::~KdeColorSelector()
{
}

// TODO
//void KdeColorSelector::setColor(const QColor & color)
//{
//  m_rectangleColorWidget->setColor(color);
//}

void KdeColorSelector::updateColor(const QColor & /* color */)
{
    emit colorSelected(m_rectangleColorWidget->color());
}

void KdeColorSelector::changeColorMode(int index)
{
    switch (index)
    {
        case 0: m_rectangleColorWidget->setChooserMode(ChooserClassic); break;
        case 1: m_rectangleColorWidget->setChooserMode(ChooserHue); break;
        case 2: m_rectangleColorWidget->setChooserMode(ChooserSaturation); break;
        case 3: m_rectangleColorWidget->setChooserMode(ChooserValue); break;
        case 4: m_rectangleColorWidget->setChooserMode(ChooserRed); break;
        case 5: m_rectangleColorWidget->setChooserMode(ChooserGreen); break;
        case 6: m_rectangleColorWidget->setChooserMode(ChooserBlue); break;
    }
}

#include "kdecolorselector.moc"
