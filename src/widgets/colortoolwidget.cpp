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

#include "colortoolwidget.h"

#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>

#include <KIcon>
#include <KLocalizedString>
#include <KPushButton>

ColorToolWidget::ColorToolWidget(QWidget * parent)
    : QWidget(parent)
{
    //BEGIN Brightness settings

    QGroupBox * brightnessBox = new QGroupBox(i18n("Brightness"), this);

    KPushButton * decreaseBrightnessButton = new KPushButton(KIcon("arrow-down"), i18n("Decrease"), brightnessBox);
    KPushButton * increaseBrightnessButton = new KPushButton(KIcon("arrow-up"), i18n("Increase"), brightnessBox);

    m_brightnessPercentage = new QLabel("45%", brightnessBox);

    QHBoxLayout * brightnessLayout = new QHBoxLayout(brightnessBox);
    brightnessLayout->addWidget(decreaseBrightnessButton);
    brightnessLayout->addWidget(increaseBrightnessButton);
    brightnessLayout->addWidget(m_brightnessPercentage);

    //END Brightness settings

    //BEGIN Saturation settings

    QGroupBox * saturationBox = new QGroupBox(i18n("Saturation"), this);

    KPushButton * decreaseSaturationButton = new KPushButton(KIcon("arrow-down"), i18n("Decrease"), saturationBox);
    KPushButton * increaseSaturationButton = new KPushButton(KIcon("arrow-up"), i18n("Increase"), saturationBox);

    m_saturationPercentage = new QLabel("15%", saturationBox);

    QHBoxLayout * saturationLayout = new QHBoxLayout(saturationBox);
    saturationLayout->addWidget(decreaseSaturationButton);
    saturationLayout->addWidget(increaseSaturationButton);
    saturationLayout->addWidget(m_saturationPercentage);

    connect(decreaseSaturationButton, SIGNAL( pressed () ), SLOT( decreaseSaturation() ));
    connect(increaseSaturationButton, SIGNAL( pressed () ), SLOT( increaseSaturation() ));

    //END Saturation settings

    //BEGIN Extra color selectors settings

    QGroupBox * extraSelectorsBox = new QGroupBox(i18n("Extra color selectors"), this);

    KPushButton * pickColorButton = new KPushButton(KIcon("color-picker"), i18n("Pick a color"), extraSelectorsBox);
    h_checkBoxHideWindow = new QCheckBox(i18n("Hide window"), extraSelectorsBox);

    QHBoxLayout * pickColorLayout = new QHBoxLayout();
    pickColorLayout->addWidget(pickColorButton);
    pickColorLayout->addWidget(h_checkBoxHideWindow);

    QVBoxLayout * extraSelectorsLayout = new QVBoxLayout(extraSelectorsBox);
    extraSelectorsLayout->addLayout(pickColorLayout);
    //extraSelectorsLayout->addWidget(Choose from image);

    connect(pickColorButton, SIGNAL( pressed () ), this, SLOT( pickColorFromDesktop() ));

    //END Extra color selectors settings

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(brightnessBox);
    mainLayout->addWidget(saturationBox);
    mainLayout->addWidget(extraSelectorsBox);


}

void ColorToolWidget::setColor(const QColor & color)
{
    m_color = color;

    m_saturationPercentage->setText(QString::number(static_cast<int>(m_color.saturation()*100/255)) + "%");
}

void ColorToolWidget::pickColorFromDesktop()
{

}

void ColorToolWidget::decreaseBrightness()
{

}

void ColorToolWidget::increaseBrightness()
{

}

void ColorToolWidget::decreaseSaturation()
{
    int tmpSaturation = qBound(0, m_color.saturation() - 5, 255);

    m_color.setHsv(m_color.hue(), tmpSaturation, m_color.value());

    m_saturationPercentage->setText(QString::number(static_cast<int>(m_color.saturation()*100/255)) + "%");

    emit colorSelected(m_color);
}

void ColorToolWidget::increaseSaturation()
{
    int tmpSaturation = qBound(0, m_color.saturation() + 5, 255);

    m_color.setHsv(m_color.hue(), tmpSaturation, m_color.value());

    m_saturationPercentage->setText(QString::number(static_cast<int>(m_color.saturation()*100/255)) + "%");

    emit colorSelected(m_color);
}

#include "colortoolwidget.moc"
