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

#include <colorwidget.h>

ColorInfoVisual::ColorInfoVisual(QWidget * parent)
    : ColorInfo(parent)
{
    setLayout(new QHBoxLayout(this));
}

ColorInfoVisual::~ColorInfoVisual()
{
}

void ColorInfoVisual::addColor(const QColor color)
{
    emit colorAdded(color);
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

#include <QGridLayout>

ColorInfoVisualComplement::ColorInfoVisualComplement(QWidget * parent)
    : ColorInfoVisual(parent)
{
    setWindowTitle(i18n("Complement"));

    m_complementColorWidget = new ColorWidget(this);

    layout()->addWidget(m_complementColorWidget);

    connect(m_complementColorWidget, SIGNAL( colorAdded(QColor) ), this, SLOT( addColor(QColor) ));
}

ColorInfoVisualComplement::~ColorInfoVisualComplement()
{
}

void ColorInfoVisualComplement::setColor(const QColor & color)
{
    int hue = color.hue();
    int saturation = color.saturation();
    int value = color.value();

    m_complementColorWidget->setColor(QColor::fromHsv(validHue(hue + 180), saturation, value));
}

//END public class ColorInfoVisualComplement

//BEGIN public class ColorInfoVisualTriadic

ColorInfoVisualTriadic::ColorInfoVisualTriadic(QWidget * parent)
    : ColorInfoVisual(parent)
{
    m_triad1ColorWidget = new ColorWidget(this);
    m_triad2ColorWidget = new ColorWidget(this);

    setWindowTitle(i18n("Triadic"));

    layout()->addWidget(m_triad1ColorWidget);
    layout()->addWidget(m_triad2ColorWidget);

    connect(m_triad1ColorWidget, SIGNAL( colorAdded(QColor) ), this, SLOT( addColor(QColor) ));
    connect(m_triad2ColorWidget, SIGNAL( colorAdded(QColor) ), this, SLOT( addColor(QColor) ));
}

ColorInfoVisualTriadic::~ColorInfoVisualTriadic()
{
}

void ColorInfoVisualTriadic::setColor(const QColor & color)
{
    int hue = color.hue();
    int saturation = color.saturation();
    int value = color.value();

    m_triad1ColorWidget->setColor(QColor::fromHsv(validHue(hue + 120), saturation, value));
    m_triad2ColorWidget->setColor(QColor::fromHsv(validHue(hue - 120), saturation, value));
}

//END public class ColorInfoVisualTriadic

//BEGIN public class ColorInfoVisualTetradic


ColorInfoVisualTetradic::ColorInfoVisualTetradic(QWidget * parent)
    : ColorInfoVisual(parent)
{
    m_tetrad1ColorWidget = new ColorWidget(this);
    m_tetrad2ColorWidget = new ColorWidget(this);
    m_tetrad3ColorWidget = new ColorWidget(this);

    setWindowTitle(i18n("Tetradic"));

    layout()->addWidget(m_tetrad1ColorWidget);
    layout()->addWidget(m_tetrad2ColorWidget);
    layout()->addWidget(m_tetrad3ColorWidget);

    connect(m_tetrad1ColorWidget, SIGNAL( colorAdded(QColor) ), this, SLOT( addColor(QColor) ));
    connect(m_tetrad2ColorWidget, SIGNAL( colorAdded(QColor) ), this, SLOT( addColor(QColor) ));
    connect(m_tetrad3ColorWidget, SIGNAL( colorAdded(QColor) ), this, SLOT( addColor(QColor) ));
}

ColorInfoVisualTetradic::~ColorInfoVisualTetradic()
{
}

void ColorInfoVisualTetradic::setColor(const QColor & color)
{
    int hue = color.hue();
    int saturation = color.saturation();
    int value = color.value();

    m_tetrad1ColorWidget->setColor(QColor::fromHsv(validHue(hue + 90), saturation, value));
    m_tetrad2ColorWidget->setColor(QColor::fromHsv(validHue(hue + 180), saturation, value));
    m_tetrad3ColorWidget->setColor(QColor::fromHsv(validHue(hue + 270), saturation, value));
}

//END public class ColorInfoVisualTetradic

//BEGIN public class ColorInfoVisualAnalogous

ColorInfoVisualAnalogous::ColorInfoVisualAnalogous(QWidget * parent)
    : ColorInfoVisual(parent)
{
    m_analogous1ColorWidget = new ColorWidget(this);
    m_analogous2ColorWidget = new ColorWidget(this);

    setWindowTitle(i18n("Analogous"));

    layout()->addWidget(m_analogous1ColorWidget);
    layout()->addWidget(m_analogous2ColorWidget);

    connect(m_analogous1ColorWidget, SIGNAL( colorAdded(QColor) ), this, SLOT( addColor(QColor) ));
    connect(m_analogous2ColorWidget, SIGNAL( colorAdded(QColor) ), this, SLOT( addColor(QColor) ));
}

ColorInfoVisualAnalogous::~ColorInfoVisualAnalogous()
{
}

void ColorInfoVisualAnalogous::setColor(const QColor & color)
{
    int hue = color.hue();
    int saturation = color.saturation();
    int value = color.value();

    m_analogous1ColorWidget->setColor(QColor::fromHsv(validHue(hue - 30), saturation, value));
    m_analogous2ColorWidget->setColor(QColor::fromHsv(validHue(hue + 30), saturation, value));
}

//END public class ColorInfoVisualAnalogous

#include "colorinfovisual.moc"
