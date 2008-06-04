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

#include "colorinfovisual.h"

#include <QtGui/QHBoxLayout>

#include <KColorPatch>
#include <KPushButton>

ColorInfoVisual::ColorInfoVisual(QWidget * parent)
    : ColorInfo(parent)
{
    setLayout(new QHBoxLayout(this));
}

ColorInfoVisual::~ColorInfoVisual()
{
}

KColorPatch * ColorInfoVisual::buildColorPatch(QWidget * parent)
{
    KColorPatch * tmpColorPatch = new KColorPatch(parent);
    tmpColorPatch->setMinimumSize(64, 64);
    tmpColorPatch->setAcceptDrops(false);

    return tmpColorPatch;
}

inline int ColorInfoVisual::validHue(int hue)
{
    int tmpHue = hue;

    if (tmpHue > 359)
        tmpHue -= 360;

    if (tmpHue < 0)
        tmpHue += 360;

    return tmpHue;
}

//BEGIN public class ColorInfoVisualComplement

ColorInfoVisualComplement::ColorInfoVisualComplement(QWidget * parent)
    : ColorInfoVisual(parent)
{
    m_complementColorPatch = buildColorPatch(this);

    setupHeader(KGuiItem(i18n("Complement"), KIcon()));

    layout()->addWidget(m_complementColorPatch);
}

ColorInfoVisualComplement::~ColorInfoVisualComplement()
{
}

void ColorInfoVisualComplement::setColor(const QColor & color)
{
    int hue = color.hue();
    int saturation = color.saturation();
    int value = color.value();

    m_complementColorPatch->setColor(QColor::fromHsv(validHue(hue + 180), saturation, value));
}

//END public class ColorInfoVisualComplement

//BEGIN public class ColorInfoVisualTriadic

ColorInfoVisualTriadic::ColorInfoVisualTriadic(QWidget * parent)
    : ColorInfoVisual(parent)
{
    m_triad1ColorPatch = buildColorPatch(this);
    m_triad2ColorPatch = buildColorPatch(this);

    setupHeader(KGuiItem(i18n("Triadic"), KIcon()));

    layout()->addWidget(m_triad1ColorPatch);
    layout()->addWidget(m_triad2ColorPatch);
}

ColorInfoVisualTriadic::~ColorInfoVisualTriadic()
{
}

void ColorInfoVisualTriadic::setColor(const QColor & color)
{
    int hue = color.hue();
    int saturation = color.saturation();
    int value = color.value();

    m_triad1ColorPatch->setColor(QColor::fromHsv(validHue(hue + 120), saturation, value));
    m_triad2ColorPatch->setColor(QColor::fromHsv(validHue(hue - 120), saturation, value));
}

//END public class ColorInfoVisualTriadic

//BEGIN public class ColorInfoVisualTetradic


ColorInfoVisualTetradic::ColorInfoVisualTetradic(QWidget * parent)
    : ColorInfoVisual(parent)
{
    m_tetrad1ColorPatch = buildColorPatch(this);
    m_tetrad2ColorPatch = buildColorPatch(this);
    m_tetrad3ColorPatch = buildColorPatch(this);

    setupHeader(KGuiItem(i18n("Tetradic"), KIcon()));

    layout()->addWidget(m_tetrad1ColorPatch);
    layout()->addWidget(m_tetrad2ColorPatch);
    layout()->addWidget(m_tetrad3ColorPatch);
}

ColorInfoVisualTetradic::~ColorInfoVisualTetradic()
{
}

void ColorInfoVisualTetradic::setColor(const QColor & color)
{
    int hue = color.hue();
    int saturation = color.saturation();
    int value = color.value();

    m_tetrad1ColorPatch->setColor(QColor::fromHsv(validHue(hue + 90), saturation, value));
    m_tetrad2ColorPatch->setColor(QColor::fromHsv(validHue(hue + 180), saturation, value));
    m_tetrad3ColorPatch->setColor(QColor::fromHsv(validHue(hue + 270), saturation, value));
}

//END public class ColorInfoVisualTetradic

    const short int maxGdkColorValue = 65535;
    /** The highest value that can be assigned to an RGB colorByte */
    const int maxRgbValue = 255;
    /** The highest value that can be assigned to a Hue colorByte */
    const int maxHueValue = 360;
    /** The highest value that can be assigned to a Saturation or Value
     * colorByte */
    const int maxSvValue = 100;
    /** The lowest value that can be assigned to a colorByte */
    const int minColorValue = 0;


//BEGIN public class ColorInfoVisualAnalogous

ColorInfoVisualAnalogous::ColorInfoVisualAnalogous(QWidget * parent)
    : ColorInfoVisual(parent)
{
    m_analogous1ColorPatch = buildColorPatch(this);
    m_analogous2ColorPatch = buildColorPatch(this);

    setupHeader(KGuiItem(i18n("Analogous"), KIcon()));

    layout()->addWidget(m_analogous1ColorPatch);
    layout()->addWidget(m_analogous2ColorPatch);
}

ColorInfoVisualAnalogous::~ColorInfoVisualAnalogous()
{
}

void ColorInfoVisualAnalogous::setColor(const QColor & color)
{
    int hue = color.hue();
    int saturation = color.saturation();
    int value = color.value();

    m_analogous1ColorPatch->setColor(QColor::fromHsv(validHue(hue - 30), saturation, value));
    m_analogous2ColorPatch->setColor(QColor::fromHsv(validHue(hue + 30), saturation, value));
}

//END public class ColorInfoVisualAnalogous

#include "colorinfovisual.moc"
