/*********************************************************************************
*  Copyright (C) 2008 by Percy Camilo Triveño Aucahuasi <orgyforever@gmail.com>  *
*  this class is based on kdeui: module colors KColorDialog.h                    *
*  Copyright (C) 1997 Martin Jones (mjones@kde.org)                              *
*  Copyright (C) 2007 Roberto Raggi (roberto@kdevelop.org)                       *
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

#ifndef RECTANGLE_COLOR_WIDGET_H
#define RECTANGLE_COLOR_WIDGET_H

#include <QtGui/QWidget>

#include <kcolorchoosermode.h>

class KHueSaturationSelector;
class KColorValueSelector;

class RectangleColorWidget : public QWidget
{
    Q_OBJECT

    public:
        RectangleColorWidget(QWidget * parent = 0);
        ~RectangleColorWidget();

        QColor color() const;

        KColorChooserMode chooserMode() const;
        void setChooserMode(KColorChooserMode chooserMode);

    public slots:
        void setColor(const QColor & color);

    signals:
        void colorSelected( const QColor & color);

    private slots:
        void slotSetHs(int x, int y);
        void slotSetV(int v);

    private:
        void changeColor(const QColor & color);
        void updateColor(const QColor & color);

        KColorChooserMode m_chooserMode;

        KHueSaturationSelector * m_hsSelector;
        KColorValueSelector * m_vSelector;
        QColor m_selColor;
};

#endif // RECTANGLE_COLOR_WIDGET_H
