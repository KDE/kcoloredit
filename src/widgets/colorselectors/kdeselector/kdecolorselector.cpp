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

#include <QtGui/QVBoxLayout>

#include <KComboBox>

#include "rectanglecolorwidget.h"

KdeColorSelector::KdeColorSelector(QWidget * parent) : ColorSelector(parent)
{
    m_rectangleColorWidget = new RectangleColorWidget(this);

//     m_HueModeAction = new KAction(KIcon("view-filter"), i18n("Hue"), header()->menu());
//     m_SaturationModeAction = new KAction(KIcon("view-filter"), i18n("Saturation"), header()->menu());
//     m_ValueModeAction = new KAction(KIcon("view-filter"), i18n("Value"), header()->menu());
//     m_RedModeAction = new KAction(KIcon("view-filter"), i18n("Red"), header()->menu());
//     m_GreenModeAction = new KAction(KIcon("view-filter"), i18n("Green"), header()->menu());
//     m_BlueModeAction = new KAction(KIcon("view-filter"), i18n("Blue"), header()->menu());

//     header()->menu()->addAction(m_HueModeAction);
//     header()->menu()->addAction(m_SaturationModeAction);
//     header()->menu()->addAction(m_ValueModeAction);
//     header()->menu()->addAction(m_RedModeAction);
//     header()->menu()->addAction(m_GreenModeAction);
//     header()->menu()->addAction(m_BlueModeAction);

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_rectangleColorWidget);

    connect(m_rectangleColorWidget, SIGNAL(colorSelected(QColor)), this, SLOT( updateColor(QColor) ));

//     connect(m_HueModeAction, SIGNAL( triggered(bool) ), this, SLOT( changeToHueMode() ));
//     connect(m_SaturationModeAction, SIGNAL( triggered(bool) ), this, SLOT( changeToSaturationMode() ));
//     connect(m_ValueModeAction, SIGNAL( triggered(bool) ), this, SLOT( changeToValueMode() ));
//     connect(m_RedModeAction, SIGNAL( triggered(bool) ), this, SLOT( changeToRedMode() ));
//     connect(m_GreenModeAction, SIGNAL( triggered(bool) ), this, SLOT( changeToGreenMode() ));
//     connect(m_BlueModeAction, SIGNAL( triggered(bool) ), this, SLOT( changeToBlueMode() ));
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

void KdeColorSelector::changeToHueMode()
{
    m_rectangleColorWidget->setChooserMode(ChooserHue);
    m_rectangleColorWidget->setColor(m_lastSelectedColor);
}

void KdeColorSelector::changeToSaturationMode()
{
    m_rectangleColorWidget->setChooserMode(ChooserSaturation);
    m_rectangleColorWidget->setColor(m_lastSelectedColor);
}

void KdeColorSelector::changeToValueMode()
{
    m_rectangleColorWidget->setChooserMode(ChooserValue);
    m_rectangleColorWidget->setColor(m_lastSelectedColor);
}

void KdeColorSelector::changeToRedMode()
{
    m_rectangleColorWidget->setChooserMode(ChooserRed);
    m_rectangleColorWidget->setColor(m_lastSelectedColor);
}

void KdeColorSelector::changeToGreenMode()
{
    m_rectangleColorWidget->setChooserMode(ChooserGreen);
    m_rectangleColorWidget->setColor(m_lastSelectedColor);
}

void KdeColorSelector::changeToBlueMode()
{
    m_rectangleColorWidget->setChooserMode(ChooserBlue);
    m_rectangleColorWidget->setColor(m_lastSelectedColor);
}

#include "kdecolorselector.moc"
