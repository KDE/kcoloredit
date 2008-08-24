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

#include "colorutil.h"

#include <KColorScheme>

float ColorUtils::luminance(const QColor & color)
{
    return 0.2126*color.red() + 0.7152*color.green() + 0.0722*color.blue();
}

QColor ColorUtils::contrastColor(const QColor & color)
{
    if (luminance(color) > (255.0 / 2.0))
        return Qt::black;

    return Qt::white;
}

QColor ColorUtils::complementColor(const QColor & color)
{
    return QColor::fromHsv(validHueValue(color.hue() + 180), color.saturation(), color.value());
}

QVector<QColor> ColorUtils::triadicColors(const QColor & color)
{
    QVector<QColor> ret;

    ret.append(QColor::fromHsv(validHueValue(color.hue() + 120), color.saturation(), color.value()));
    ret.append(QColor::fromHsv(validHueValue(color.hue() - 120), color.saturation(), color.value()));

    return ret;
}

QVector<QColor> ColorUtils::tetradicColors(const QColor & color)
{
    QVector<QColor> ret;

    ret.append(QColor::fromHsv(validHueValue(color.hue() + 90), color.saturation(), color.value()));
    ret.append(QColor::fromHsv(validHueValue(color.hue() + 180), color.saturation(), color.value()));
    ret.append(QColor::fromHsv(validHueValue(color.hue() + 270), color.saturation(), color.value()));

    return ret;
}

QVector<QColor> ColorUtils::analogousColors(const QColor & color)
{
    QVector<QColor> ret;

    ret.append(QColor::fromHsv(validHueValue(color.hue() - 30), color.saturation(), color.value()));
    ret.append(QColor::fromHsv(validHueValue(color.hue() + 30), color.saturation(), color.value()));

    return ret;
}

QColor ColorUtils::backgroundColorOfWindow()
{
    KColorScheme systemColorScheme(QPalette::Active);

    return systemColorScheme.background(KColorScheme::NormalBackground).color();
}

inline int ColorUtils::validHueValue(int hue)
{
    int tmpHue = hue;

    if (tmpHue > 359)
        tmpHue -= 360;

    if (tmpHue < 0)
        tmpHue += 360;

    return tmpHue;
}
