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

#include <QtGui/QGridLayout>
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

    KPushButton * decreaseBrightnessButton = new KPushButton(brightnessBox);
    KPushButton * increaseBrightnessButton = new KPushButton(brightnessBox);

    QHBoxLayout * brightnessLayout = new QHBoxLayout(brightnessBox);
    brightnessLayout->addWidget(decreaseBrightnessButton);
    brightnessLayout->addWidget(increaseBrightnessButton);
    brightnessLayout->addWidget(new QLabel("45%", brightnessBox));

    //END Brightness settings

    //BEGIN Saturation settings

    QGroupBox * saturationBox = new QGroupBox(i18n("Saturation"), this);

    KPushButton * decreaseSaturationButton = new KPushButton(saturationBox);
    KPushButton * increaseSaturationButton = new KPushButton(saturationBox);

    QHBoxLayout * saturationLayout = new QHBoxLayout(saturationBox);
    saturationLayout->addWidget(decreaseSaturationButton);
    saturationLayout->addWidget(increaseSaturationButton);
    saturationLayout->addWidget(new QLabel("15%", saturationBox));

    connect(decreaseSaturationButton, SIGNAL( pressed () ), SLOT( decreaseSaturation() ));
    connect(increaseSaturationButton, SIGNAL( pressed () ), SLOT( increaseSaturation() ));

    //END Saturation settings

    //BEGIN Extra color selectors settings

    QGroupBox * extraSelectorsBox = new QGroupBox(i18n("Extra color selectors"), this);

    KPushButton * pickColorButton = new KPushButton(KIcon("pick-color"), i18n("pick a color from desktop"), extraSelectorsBox);
    KPushButton * generateRandomColorButton = new KPushButton(KIcon("roll"), i18n("Generate a random color"), extraSelectorsBox);

    QHBoxLayout * pickColorLayout = new QHBoxLayout();
    pickColorLayout->addWidget(pickColorButton);
    pickColorLayout->addWidget(new QCheckBox(extraSelectorsBox));

    QVBoxLayout * extraSelectorsLayout = new QVBoxLayout(extraSelectorsBox);
    extraSelectorsLayout->addLayout(pickColorLayout);
    extraSelectorsLayout->addWidget(generateRandomColorButton);

    connect(generateRandomColorButton, SIGNAL( pressed () ), this, SLOT( generateRandomColor() ));
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
}

void ColorToolWidget::generateRandomColor()
{
    QColor randColor(qrand() % 255, qrand()  % 255, qrand() % 255);

    emit colorSelected(randColor);
}

void ColorToolWidget::pickColorFromDesktop()
{

}

void ColorToolWidget::decreaseSaturation()
{
    //emit colorSelected(randColor);
}

void ColorToolWidget::increaseSaturation()
{
    static QColor saturatedColor;
    saturatedColor.setHsv(m_color.hue(), m_color.saturation() + 0.05 * 100, m_color.value());

    emit colorSelected(saturatedColor);
}

#include "colortoolwidget.moc"
