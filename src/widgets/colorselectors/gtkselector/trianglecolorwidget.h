/*********************************************************************************
*  Copyright (C) 2008 Cyrille Berger <cberger@cberger.net>                       *
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

#ifndef TRIANGLE_COLOR_WIDGET_H
#define TRIANGLE_COLOR_WIDGET_H

#include <QtGui/QWidget>

class TriangleColorWidget : public QWidget
{
    Q_OBJECT

    public:
        TriangleColorWidget(QWidget* parent = 0);
        ~TriangleColorWidget();

    public:
        int hue() const;
        int value() const;
        int saturation() const;
        QColor color() const;

    public slots:
        void setHue(int h);
        void setValue(int v);
        void setSaturation(int s);
        void setHSV(int h, int s, int v);
        void setColor(const QColor& );

    signals:
        void colorChanged(const QColor& );

    protected:
        void paintEvent( QPaintEvent * event );
        void resizeEvent( QResizeEvent * event );
        void mouseReleaseEvent( QMouseEvent * event );
        void mousePressEvent( QMouseEvent * event );
        void mouseMoveEvent( QMouseEvent * event );

    private:
        void tellColorChanged();
        void generateTriangle();
        void generateWheel();
        void updateTriangleCircleParameters();
        void selectColorAt(int x, int y, bool checkInWheel = true);

    private:
        enum CurrentHandle
        {
            NoHandle,
            HueHandle,
            ValueSaturationHandle
        };

        QPixmap m_wheelPixmap;
        QPixmap m_trianglePixmap;
        int m_hue;
        int m_saturation;
        int m_value;
        int m_sizeColorSelector;

        double m_centerColorSelector;
        double m_wheelWidthProportion;
        double m_wheelWidth;
        double m_wheelNormExt;
        double m_wheelNormInt;
        double m_wheelInnerRadius;
        double m_triangleRadius;
        double m_triangleLength;
        double m_triangleHeight;
        double m_triangleBottom;
        double m_triangleTop;
        double m_normExt;
        double m_normInt;
        double m_triangleHandleSize;

        bool m_updateAllowed;
        CurrentHandle m_handle;
};

#endif // TRIANGLE_COLOR_WIDGET_H
