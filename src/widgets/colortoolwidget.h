/*********************************************************************************
*  Copyright (C) 2009 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>   *
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

#ifndef COLOR_TOOL_WIDGET_H
#define COLOR_TOOL_WIDGET_H

#include <QtGui/QWidget>

class QLabel;
class QCheckBox;
class QSlider;

class KColorPatch;

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
        void changeBrightnessValue(int);
        void setupDefaultBrightness();

        void changeSaturationValue(int);
        void setupDefaultSaturation();

        void setColor(const QColor & color);

    signals:
        void colorSelected(const QColor & color);

    protected:
        virtual void keyPressEvent(QKeyEvent * event);
        virtual void mouseMoveEvent(QMouseEvent * event);
        virtual void mousePressEvent(QMouseEvent * event);

    private slots:
        void pickColorFromDesktop();

    private:
        void grabPicking();
        void releasePicking();

        QColor grabColor(const QPoint & p);

    private:
        QColor m_color;
        QColor m_oldColor; // original color without any modification

        QLabel * m_brightnessPercentage;
        QLabel * m_saturationPercentage;

        QSlider *m_brightnessSlider;
        QSlider *m_saturationSlider;

        bool m_colorPicking;

        KColorPatch * m_colorView;

#ifdef Q_WS_X11

        QCheckBox * m_checkBoxHideWindow;

        KCDPickerFilter * m_filter;

#endif
};

#endif // COLOR_TOOL_WIDGET_H
