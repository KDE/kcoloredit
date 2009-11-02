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

#include "colorutil.h"

#include <KColorScheme>

const QColor ColorUtil::DEFAULT_COLOR = QColor(0, 123, 254);

float ColorUtil::luminance(const QColor & color)
{
    return 0.2126*color.red() + 0.7152*color.green() + 0.0722*color.blue();
}

QColor ColorUtil::contrastColor(const QColor & color)
{
    if (luminance(color) > (255.0 / 2.0))
        return Qt::black;

    return Qt::white;
}

QColor ColorUtil::complementColor(const QColor & color)
{
    return QColor::fromHsv(validHueValue(color.hue() + 180), color.saturation(), color.value());
}

QVector<QColor> ColorUtil::triadicColors(const QColor & color)
{
    QVector<QColor> ret;

    ret.append(QColor::fromHsv(validHueValue(color.hue() + 120), color.saturation(), color.value()));
    ret.append(QColor::fromHsv(validHueValue(color.hue() - 120), color.saturation(), color.value()));

    return ret;
}

QVector<QColor> ColorUtil::tetradicColors(const QColor & color)
{
    QVector<QColor> ret;

    ret.append(QColor::fromHsv(validHueValue(color.hue() + 90), color.saturation(), color.value()));
    ret.append(QColor::fromHsv(validHueValue(color.hue() + 180), color.saturation(), color.value()));
    ret.append(QColor::fromHsv(validHueValue(color.hue() + 270), color.saturation(), color.value()));

    return ret;
}

QVector<QColor> ColorUtil::analogousColors(const QColor & color)
{
    QVector<QColor> ret;

    ret.append(QColor::fromHsv(validHueValue(color.hue() - 30), color.saturation(), color.value()));
    ret.append(QColor::fromHsv(validHueValue(color.hue() + 30), color.saturation(), color.value()));

    return ret;
}

QColor ColorUtil::backgroundColorOfWindow()
{
    KColorScheme systemColorScheme(QPalette::Active);

    return systemColorScheme.background(KColorScheme::NormalBackground).color();
}

inline int ColorUtil::validHueValue(int hue)
{
    int ret = hue;

    if (ret > 359)
        ret -= 360;

    if (ret < 0)
        ret += 360;

    return ret;
}
