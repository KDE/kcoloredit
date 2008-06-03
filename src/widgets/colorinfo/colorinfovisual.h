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

#ifndef COLOR_INFO_VISUAL_H
#define COLOR_INFO_VISUAL_H

#include "colorinfo.h"

class KColorPatch;

class ColorInfoVisual : public ColorInfo
{
    Q_OBJECT

    public:
        ColorInfoVisual(QWidget * parent = 0);
        virtual ~ColorInfoVisual();
};

class ColorInfoVisualComplement : public ColorInfoVisual
{
    Q_OBJECT

    public:
        ColorInfoVisualComplement(QWidget * parent = 0);
        ~ColorInfoVisualComplement();

    public slots:
        void setColor(const QColor & color);

    private:
        KColorPatch * m_complementColorPatch;
};

#endif // COLOR_INFO_VISUAL_H
