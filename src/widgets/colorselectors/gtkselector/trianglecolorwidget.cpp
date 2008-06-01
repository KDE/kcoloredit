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

#include "trianglecolorwidget.h"

#include <math.h>

#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

void rgb_to_hsv(int R, int G, int B, int *H, int *S, int *V)
{
    QColor colo(R, G, B);

    *H = colo.hue();
    *S = colo.saturation();
    *V = colo.value();
}

void hsv_to_rgb(int H, int S, int V, int *R, int *G, int *B)
{
    QColor col;

    col.setHsv(H, S, V);

    *R = col.red();
    *G = col.green();
    *B = col.blue();
}

TriangleColorWidget::TriangleColorWidget(QWidget* parent)
    : QWidget(parent)
{
    setMinimumHeight( 100 );
    setMinimumWidth( 100 );
    m_hue = 0;
    m_saturation = 0;
    m_value = 0;
    m_updateAllowed = true;
    setMouseTracking( true );
    updateTriangleCircleParameters();
}

TriangleColorWidget::~TriangleColorWidget()
{
}

void TriangleColorWidget::updateTriangleCircleParameters()
{
    m_sizeColorSelector = qMin(width(), height());
    m_centerColorSelector = 0.5 * m_sizeColorSelector;
    m_wheelWidthProportion = 0.3;
    m_wheelWidth = m_centerColorSelector * m_wheelWidthProportion;
    m_wheelNormExt = qAbs( m_centerColorSelector );
    m_wheelNormInt = qAbs( m_centerColorSelector * (1.0 - m_wheelWidthProportion));
    m_wheelInnerRadius = m_centerColorSelector * (1.0 - m_wheelWidthProportion);
    m_triangleRadius = m_wheelInnerRadius * 0.9;
    m_triangleLength = 3.0 / sqrt(3.0) * m_triangleRadius;
    m_triangleHeight = m_triangleLength * sqrt(3.0) * 0.5;
    m_triangleTop = 0.5 * m_sizeColorSelector - m_triangleRadius;
    m_triangleBottom = m_triangleHeight + m_triangleTop;
    m_triangleHandleSize = 10.0;
}

void TriangleColorWidget::paintEvent( QPaintEvent * event )
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::Antialiasing);
    QPointF pos(m_centerColorSelector, m_centerColorSelector);
    p.translate(QPointF( 0.5*width(), 0.5*height()  ) );
    // Draw the wheel
    p.drawPixmap( -pos, m_wheelPixmap );
    // Draw the triangle
    p.save();
    p.rotate( hue() + 150 );
    p.drawPixmap( -pos , m_trianglePixmap );
    // Draw selectors
    p.restore();
    // Draw value,saturation selector
    //   Compute coordinates
    {
        double vs_selector_ypos_ = value() / 255.0;
        double ls_ = (vs_selector_ypos_) * m_triangleLength; // length of the saturation on the triangle
        double vs_selector_xpos_ = ls_ * (saturation() / 255.0 - 0.5);
        // Draw it
        p.save();
        p.setPen( QPen( Qt::white, 1.0) );
        p.setBrush( color() );
        p.rotate( hue() + 150 );
        p.drawEllipse( QRectF( -m_triangleHandleSize*0.5 + vs_selector_xpos_,
                               -m_triangleHandleSize*0.5 - (m_centerColorSelector - m_triangleTop) + vs_selector_ypos_ * m_triangleHeight,
                                m_triangleHandleSize , m_triangleHandleSize ));
    }
    p.restore();
    // Draw Hue selector
    p.save();
    p.setPen( QPen( Qt::white, 1.0) );
    p.rotate( hue() - 90 );
    double hueSelectorWidth_ = 0.8;
    double hueSelectorOffset_ = 0.5 *( 1.0 - hueSelectorWidth_) * m_wheelWidth;
    double hueSelectorSize_ = 0.8 * m_wheelWidth;
    p.drawRect( QRectF( -1.5, -m_centerColorSelector + hueSelectorOffset_, 3.0, hueSelectorSize_ ));
    p.restore();
    p.end();
}

int TriangleColorWidget::hue() const
{
    return m_hue;
}

void TriangleColorWidget::setHue(int h)
{
    h = qBound(0, h, 360);
    m_hue = h;
    tellColorChanged();
    generateTriangle();
    update();
}

int TriangleColorWidget::value() const
{
    return m_value;
}

void TriangleColorWidget::setValue(int v)
{
    v = qBound(0, v, 255);
    m_value = v;
    tellColorChanged();
    generateTriangle();
    update();
}

int TriangleColorWidget::saturation() const
{
    return m_saturation;
}

void TriangleColorWidget::setSaturation(int s)
{
    s = qBound(0, s, 255);
    m_saturation = s;
    tellColorChanged();
    generateTriangle();
    update();
}

void TriangleColorWidget::setHSV(int h, int s, int v)
{
    h = qBound(0, h, 360);
    s = qBound(0, s, 255);
    v = qBound(0, v, 255);
    m_hue = h;
    m_value = v;
    m_saturation = s;
    tellColorChanged();
    generateTriangle();
    update();
}

QColor TriangleColorWidget::color() const
{
    int r,g,b;
    hsv_to_rgb( m_hue, m_saturation, m_value, &r, &g, &b);
    return QColor(r,g,b);
}

void TriangleColorWidget::setQColor(const QColor& c)
{
    if(m_updateAllowed)
    {
        int hue;
        rgb_to_hsv( c.red(), c.green(), c.blue(), &hue, &m_saturation, &m_value);
        if( hue >= 0 && hue <= 360)
            m_hue = hue;
        generateTriangle();
        update();
    }
}

void TriangleColorWidget::resizeEvent( QResizeEvent * event )
{
    QWidget::resizeEvent( event );
    updateTriangleCircleParameters();
    generateWheel();
    generateTriangle();
}

inline double pow2(double v)
{
    return v*v;
}

void TriangleColorWidget::tellColorChanged()
{
    m_updateAllowed = false;
    emit(colorChanged(color()));
    m_updateAllowed = true;
}

void TriangleColorWidget::generateTriangle()
{
    QImage img(m_sizeColorSelector, m_sizeColorSelector, QImage::Format_ARGB32_Premultiplied);
    // Length of triangle
    int hue_ = hue();
    
    for(int y = 0; y < m_sizeColorSelector; y++)
    {
        double ynormalize = ( m_triangleTop - y ) / ( m_triangleTop - m_triangleBottom );
        double v = 255 * ynormalize;
        double ls_ = (ynormalize) * m_triangleLength;
        double startx_ = m_centerColorSelector - 0.5 * ls_;
        for(int x = 0; x < m_sizeColorSelector; x++)
        {
            double s = 255 * (x - startx_) / ls_;
            if(v < -1.0 || v > 256.0 || s < -1.0 || s > 256.0 )
            {
                img.setPixel(x,y, qRgba(0,0,0,0));
            } else {
                double va = 1.0, sa = 1.0;
                if( v < 0.0) { va = 1.0 + v; v = 0; }
                else if( v > 255.0 ) { va = 256.0 - v; v = 255; }
                if( s < 0.0) { sa = 1.0 + s; s = 0; }
                else if( s > 255.0 ) { sa = 256.0 - s; s = 255; }
                int r,g,b;
                hsv_to_rgb(hue_, (int)s, (int)v, &r, &g, &b);
                double coef = va * sa;
                if( coef < 0.999)
                {
                    img.setPixel(x,y, qRgba( (int)(r * coef), (int)(g * coef), (int)(b * coef), (int)(255 * coef)));
                } else {
                    img.setPixel(x,y, qRgba(r, g, b, 255 ));
                }
            }
        }
    }
    
    m_trianglePixmap = QPixmap::fromImage(img);
}

void TriangleColorWidget::generateWheel()
{
    QImage img(m_sizeColorSelector, m_sizeColorSelector, QImage::Format_ARGB32_Premultiplied);
    for(int y = 0; y < m_sizeColorSelector; y++)
    {
        double yc = y - m_centerColorSelector;
        double y2 = pow2( yc );
        for(int x = 0; x < m_sizeColorSelector; x++)
        {
            double xc = x - m_centerColorSelector;
            double norm = sqrt(pow2( xc ) + y2);
            if( norm <= m_wheelNormExt + 1.0 && norm >= m_wheelNormInt - 1.0 )
            {
                double acoef = 1.0;
                if(norm > m_wheelNormExt ) acoef = (1.0 + m_wheelNormExt - norm);
                else if(norm < m_wheelNormInt ) acoef = (1.0 - m_wheelNormInt + norm);
                double angle = atan2(yc, xc);
                int h = (int)((180 * angle / M_PI) + 180);
                int r,g,b;
                hsv_to_rgb(h, 255, 255, &r, &g, &b);
                if( acoef < 0.999)
                {
                    img.setPixel(x,y, qRgba( (int)(r * acoef), (int)(g * acoef), (int)(b * acoef), (int)(255 * acoef)));
                } else {
                    img.setPixel(x,y, qRgba(r, g, b, 255 ));
                }
            } else {
                img.setPixel(x,y, qRgba(0,0,0,0));
            }
        }
    }
    m_wheelPixmap = QPixmap::fromImage(img);
}

void TriangleColorWidget::mouseReleaseEvent( QMouseEvent * event )
{
    if(event->button() == Qt::LeftButton)
    {
        selectColorAt( event->x(), event->y());
        m_handle = NoHandle;
    }
    QWidget::mouseReleaseEvent( event );
}



void TriangleColorWidget::mousePressEvent( QMouseEvent * event )
{
    if(event->button() == Qt::LeftButton)
    {
        m_handle = NoHandle;
        selectColorAt( event->x(), event->y());



    }
    QWidget::mousePressEvent( event );


}

void TriangleColorWidget::mouseMoveEvent( QMouseEvent * event )
{
    if(event->buttons() & Qt::LeftButton)
    {
        selectColorAt( event->x(), event->y(), false );
    }
    QWidget::mouseMoveEvent( event);
}

void TriangleColorWidget::selectColorAt(int _x, int _y, bool /* checkInWheel */)
{
    double x = _x - 0.5*width();
    double y = _y - 0.5*height();
    // Check if the click is inside the wheel
    double norm = sqrt( x * x + y * y);
    if ( ( (norm < m_wheelNormExt) && (norm > m_wheelNormInt) && m_handle == NoHandle )
         || m_handle == HueHandle ) {
        m_handle = HueHandle;
        setHue( (int)(atan2(y, x) * 180 / M_PI ) + 180);
        update();
    }
    else {
    // Compute the s and v value, if they are in range, use them
        double rotation = -(hue() + 150) * M_PI / 180;
        double cr = cos(rotation);
        double sr = sin(rotation);
        double x1 = x * cr - y * sr; // <- now x1 gives the saturation
        double y1 = x * sr + y * cr; // <- now y1 gives the value
        y1 += m_wheelNormExt;
        double ynormalize = (m_triangleTop - y1 ) / ( m_triangleTop - m_triangleBottom );
        if( (ynormalize >= 0.0 && ynormalize <= 1.0 ) || m_handle == ValueSaturationHandle)
        {
            m_handle = ValueSaturationHandle;
            double ls_ = (ynormalize) * m_triangleLength; // length of the saturation on the triangle
            double sat = ( x1 / ls_ + 0.5) ;
            if((sat >= 0.0 && sat <= 1.0) || m_handle == ValueSaturationHandle)
            {
                setHSV( m_hue, sat * 255, ynormalize * 255);
            }
        }
        update();
    }
}

#include "trianglecolorwidget.moc"
