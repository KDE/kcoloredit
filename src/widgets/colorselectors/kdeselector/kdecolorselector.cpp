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

#include <QtGui/QLayout>
#include <QtGui/QLabel>

#include <KLocalizedString>
#include <KIcon>
#include <KComboBox>

#include "rectanglecolorwidget.h"

KdeColorSelector::KdeColorSelector(QWidget * parent) : ColorSelector(parent)
{
    m_rectangleColorWidget = new RectangleColorWidget(this);

    KComboBox * chooserModeComboBox = new KComboBox(this);
    chooserModeComboBox->addItem(KIcon("view-filter"), i18n("Hue"));
    chooserModeComboBox->addItem(KIcon("view-filter"), i18n("Saturation"));
    chooserModeComboBox->addItem(KIcon("view-filter"), i18n("Value"));
    chooserModeComboBox->addItem(KIcon("view-filter"), i18n("Red"));
    chooserModeComboBox->addItem(KIcon("view-filter"), i18n("Green"));
    chooserModeComboBox->addItem(KIcon("view-filter"), i18n("Blue"));
    chooserModeComboBox->setCurrentIndex(2);

    QHBoxLayout * chooserModeLayout = new QHBoxLayout();
    chooserModeLayout->addWidget(new QLabel(i18n("Chooser mode:"), this));
    chooserModeLayout->addWidget(chooserModeComboBox, Qt::AlignJustify);

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_rectangleColorWidget);
    mainLayout->addLayout(chooserModeLayout);

    connect(m_rectangleColorWidget, SIGNAL(colorSelected(QColor)), SLOT( updateColor(QColor) ));
    connect(chooserModeComboBox, SIGNAL(  currentIndexChanged(int) ), SLOT( changeChooserMode(int) ));
}

KdeColorSelector::~KdeColorSelector()
{
}

void KdeColorSelector::setColor(const QColor & color)
{
    m_lastSelectedColor = color;

    m_rectangleColorWidget->setColor(m_lastSelectedColor);
}

void KdeColorSelector::updateColor(const QColor & color)
{
    m_lastSelectedColor = color;

    emit colorSelected(m_lastSelectedColor);
}

void KdeColorSelector::changeChooserMode(int index)
{
    switch (index)
    {
        case 0 : m_rectangleColorWidget->setChooserMode(ChooserHue); break;
        case 1 : m_rectangleColorWidget->setChooserMode(ChooserSaturation); break;
        case 2 : m_rectangleColorWidget->setChooserMode(ChooserValue); break;
        case 3 : m_rectangleColorWidget->setChooserMode(ChooserRed); break;
        case 4 : m_rectangleColorWidget->setChooserMode(ChooserGreen); break;
        case 5 : m_rectangleColorWidget->setChooserMode(ChooserBlue); break;
    }

    m_rectangleColorWidget->setColor(m_lastSelectedColor);
}

#include "kdecolorselector.moc"
