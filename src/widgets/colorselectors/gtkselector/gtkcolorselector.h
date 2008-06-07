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

#ifndef GTK_COLOR_SELECTOR
#define GTK_COLOR_SELECTOR

#include "colorselector.h"

class TriangleColorWidget;

class GtkColorSelector : public ColorSelector
{
    Q_OBJECT

    public:
        GtkColorSelector(QWidget* parent);

    public slots:
        void setColor(const QColor & color);

    private slots:
        void updateColor(const QColor & color);

        void generateRandomColor();

    private:
        TriangleColorWidget * m_triangleColorWidget;

        KAction * m_generateRandomColorAction;
};

#endif // GTK_COLOR_SELECTOR
