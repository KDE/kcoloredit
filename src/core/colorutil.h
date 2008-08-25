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

#ifndef COLOR_UTILS_H
#define COLOR_UTILS_H

#include <QtGui/QColor>
#include <QtCore/QVector>

class ColorUtil
{
    public:
        static float luminance(const QColor & color);
        static QColor contrastColor(const QColor & color);

        static QColor complementColor(const QColor & color);
        static QVector<QColor> triadicColors(const QColor & color);
        static QVector<QColor> tetradicColors(const QColor & color);
        static QVector<QColor> analogousColors(const QColor & color);

        static QColor backgroundColorOfWindow();

    private:
        static inline int validHueValue(int hue);
};

#endif // COLOR_UTILS_H
