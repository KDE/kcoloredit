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

#ifndef KDE_COLOR_SELECTOR_H
#define KDE_COLOR_SELECTOR_H

#include "colorselector.h"

class RectangleColorWidget;

class KdeColorSelector : public ColorSelector
{
    Q_OBJECT

    public:
        KdeColorSelector(QWidget * parent = 0);
        ~KdeColorSelector();

    public slots:
        void setColor(const QColor & color);

    private slots:
        void updateColor(const QColor & color);

        void changeChooserMode(int index);

    private:
        QColor m_lastSelectedColor; //  NOTE to update the color

        RectangleColorWidget * m_rectangleColorWidget;
};

#endif // KDE_COLOR_SELECTOR_H
