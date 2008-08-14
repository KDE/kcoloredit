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

//     m_addColorsAction = new KAction(KIcon("list-add"), i18n("Add Colors"), header()->menu());

//     header()->menu()->addAction(m_addColorsAction);
}

ColorInfoVisual::~ColorInfoVisual()
{
}

KAction * ColorInfoVisual::addColorAction() const
{
    return m_addColorsAction;
}

ColorWidget * ColorInfoVisual::buildColorWidget(QWidget * parent)
{
    ColorWidget * tmpColorWidget = new ColorWidget(parent);
    tmpColorWidget->setMinimumSize(64, 64);
    tmpColorWidget->setAcceptDrops(false);

    return tmpColorWidget;
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
    m_complementColorWidget = buildColorWidget(this);

    setWindowTitle(i18n("Complement"));

    layout()->addWidget(m_complementColorWidget);
}

ColorInfoVisualComplement::~ColorInfoVisualComplement()
{
}

QVector<QColor> ColorInfoVisualComplement::colors() const
{
    QVector<QColor> tmpColors;
    tmpColors.append(m_complementColorWidget->color());

    return tmpColors;
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
    m_triad1ColorWidget = buildColorWidget(this);
    m_triad2ColorWidget = buildColorWidget(this);

    setWindowTitle(i18n("Triadic"));

    layout()->addWidget(m_triad1ColorWidget);
    layout()->addWidget(m_triad2ColorWidget);
}

ColorInfoVisualTriadic::~ColorInfoVisualTriadic()
{
}

QVector<QColor> ColorInfoVisualTriadic::colors() const
{
    QVector<QColor> tmpColors;
    tmpColors.append(m_triad1ColorWidget->color());
    tmpColors.append(m_triad2ColorWidget->color());

    return tmpColors;
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
    m_tetrad1ColorWidget = buildColorWidget(this);
    m_tetrad2ColorWidget = buildColorWidget(this);
    m_tetrad3ColorWidget = buildColorWidget(this);

    setWindowTitle(i18n("Tetradic"));

    layout()->addWidget(m_tetrad1ColorWidget);
    layout()->addWidget(m_tetrad2ColorWidget);
    layout()->addWidget(m_tetrad3ColorWidget);
}

ColorInfoVisualTetradic::~ColorInfoVisualTetradic()
{
}

QVector<QColor> ColorInfoVisualTetradic::colors() const
{
    QVector<QColor> tmpColors;
    tmpColors.append(m_tetrad1ColorWidget->color());
    tmpColors.append(m_tetrad2ColorWidget->color());
    tmpColors.append(m_tetrad3ColorWidget->color());

    return tmpColors;
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
    m_analogous1ColorWidget = buildColorWidget(this);
    m_analogous2ColorWidget = buildColorWidget(this);

    setWindowTitle(i18n("Analogous"));

    layout()->addWidget(m_analogous1ColorWidget);
    layout()->addWidget(m_analogous2ColorWidget);
}

ColorInfoVisualAnalogous::~ColorInfoVisualAnalogous()
{
}

QVector<QColor> ColorInfoVisualAnalogous::colors() const
{
    QVector<QColor> tmpColors;
    tmpColors.append(m_analogous1ColorWidget->color());
    tmpColors.append(m_analogous2ColorWidget->color());

    return tmpColors;
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
