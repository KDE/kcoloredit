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

#ifndef BLENDER_COLOR_SELECTOR_H
#define BLENDER_COLOR_SELECTOR_H

#include "colorselector.h"

class KColorButton;

class BlenderColorSelector : public ColorSelector
{
    Q_OBJECT

    public:
        BlenderColorSelector(QWidget * parent = 0);

    public slots:
        void setColor(const QColor & color);

    private slots:
        void updateMixWhenChangeBias(int factor);
        void updateMixWhenChangeColor(const QColor & color);

    private:
        void performMix();

    private:
        float m_bias;

        KColorButton * m_baseColor;
        KColorButton * m_overlayedColor;
};

#endif // BLENDER_COLOR_SELECTOR_H
