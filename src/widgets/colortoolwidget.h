/*********************************************************************************
*  Copyright (C) 2008 by Percy Camilo Triveño Aucahuasi <orgyforever@gmail.com>  *
*                                                                                *
*  Filters:                                                                      *
*  Is based on kdeui/colors/kcolordialog                                         *
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

#ifndef COLORTOOL_WIDGET_H
#define COLORTOOL_WIDGET_H

#include <QtGui/QWidget>

class QLabel;

#ifdef Q_WS_X11
class KCDPickerFilter;
#endif

class ColorToolWidget : public QWidget
{
    Q_OBJECT

    public:
        ColorToolWidget(QWidget * parent = 0);
        ~ColorToolWidget();

    public slots:
        void setColor(const QColor & color);

    signals:
        void colorSelected(const QColor & color);

    protected:
        virtual void keyPressEvent(QKeyEvent * event);
        virtual void mouseMoveEvent(QMouseEvent * event);
        virtual void mousePressEvent(QMouseEvent * event);

    private slots:
        void decreaseBrightness();
        void increaseBrightness();

        void decreaseSaturation();
        void increaseSaturation();

        void pickColorFromDesktop();

    private:
        QColor grabColor(const QPoint & p);

    private:
        QColor m_color;

        QLabel * m_brightnessPercentage;
        QLabel * m_saturationPercentage;

#ifdef Q_WS_X11
        KCDPickerFilter * m_filter;
#endif

        bool m_colorPicking;
};

#endif // COLORTOOL_WIDGET_H
