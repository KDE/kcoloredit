/*********************************************************************************
*  Copyright (C) 2008 by Percy Camilo Triveño Aucahuasi <orgyforever@gmail.com>  *
*  this class is based on kdeui: module colors KColorDialog.cpp                  *
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


#include "rectanglecolorwidget.h"

#include <QtGui/QLayout>

#include <KColorValueSelector>
#include <khuesaturationselect.h>

RectangleColorWidget::RectangleColorWidget(QWidget * parent) : QWidget(parent)
{
    m_hsSelector = new KHueSaturationSelector(this);
    m_hsSelector->setMinimumSize(96, 96);
    connect(m_hsSelector, SIGNAL(valueChanged(int, int)), SLOT(slotSetHs(int, int)));

    m_vSelector = new KColorValueSelector(this);
    m_vSelector->setMinimumSize(26, 70);
    m_vSelector->setMaximumWidth(30);
    m_vSelector->setIndent(false);
    m_vSelector->setArrowDirection(Qt::RightArrow);
    connect(m_vSelector, SIGNAL(valueChanged(int)), SLOT(slotSetV(int)));

    QHBoxLayout * lyt = new QHBoxLayout(this);
    lyt->addWidget(m_hsSelector);
    lyt->addWidget(m_vSelector);

    QColor col;

    col.setHsv(0, 0, 255);
    changeColor(col);

    m_hsSelector->setAcceptDrops(true);

    setChooserMode(ChooserValue); // NOTE IMPORTANT
}

RectangleColorWidget::~RectangleColorWidget()
{
}

KColorChooserMode RectangleColorWidget::chooserMode() const
{
    return m_chooserMode;
}

void RectangleColorWidget::setChooserMode(KColorChooserMode chooserMode)
{
    m_chooserMode = chooserMode;
    m_hsSelector->setChooserMode(m_chooserMode);
    m_vSelector->setChooserMode(m_chooserMode);

    m_vSelector->updateContents();
    m_hsSelector->updateContents();
    m_vSelector->repaint();
    m_hsSelector->repaint();
}

QColor RectangleColorWidget::color() const
{
    return m_selColor;
}

void RectangleColorWidget::setColor(const QColor & color)
{
    changeColor(color);
}

void RectangleColorWidget::slotSetHs(int x, int y)
{
    int _h, _s, _v, _r, _g, _b;

    _h = m_selColor.hue();
    _s = m_selColor.saturation();
    _v = m_selColor.value();
    _r = m_selColor.red();
    _g = m_selColor.green();
    _b = m_selColor.blue();

    QColor col;

    switch (chooserMode()) {
    case ChooserRed:
        col.setRgb(_r, x, y);
        break;

    case ChooserGreen:
        col.setRgb(x, _g, y);
        break;

    case ChooserBlue:
        col.setRgb(y, x, _b);
        break;
    case ChooserHue:

        col.setHsv(_h, x, y);
        break;
    case ChooserSaturation:

        col.setHsv(x, _s, y);
        break;
    case ChooserValue:
        col.setHsv(x, y, _v);
        break;

    case ChooserClassic:
        col.setHsv(x, y, _v);
        break;
    }
    changeColor(col);
}

void RectangleColorWidget::slotSetV(int v)
{
    int _h, _s, _v, _r, _g, _b;

    _h = m_selColor.hue();
    _s = m_selColor.saturation();
    _v = m_selColor.value();
    _r = m_selColor.red();
    _g = m_selColor.green();
    _b = m_selColor.blue();

    QColor col;

    switch (chooserMode())
    {
        case ChooserHue:
            col.setHsv(v, _s, _v);
            break;
        case ChooserSaturation:
            col.setHsv(_h, v, _v);
            break;
        case ChooserRed:
            col.setRgb(v, _g, _b);
            break;
        case ChooserGreen:
            col.setRgb(_r, v, _b);
            break;
        case ChooserBlue:
            col.setRgb(_r, _g, v);
            break;
        case ChooserValue:
            col.setHsv(_h, _s, v);
            break;
        case ChooserClassic:
            col.setHsv(_h, _s, v);
            break;
    }

    changeColor(col);
}

void RectangleColorWidget::changeColor(const QColor & color)
{
    if (color.isValid()) {

        m_selColor = color;
    } else {

        //m_selColor = defaultColor;
    }

    updateColor(m_selColor);

    emit colorSelected(m_selColor);
}

// show but don't set into m_selColor, nor emit colorSelected
void RectangleColorWidget::updateColor(const QColor & color)
{

    switch (chooserMode()) {
    case ChooserSaturation:
        m_hsSelector->setValues(color.hue(), color.value());
        m_vSelector->setValue(color.saturation());
        break;
    case ChooserValue:
        m_hsSelector->setValues(color.hue(), color.saturation());
        m_vSelector->setValue(color.value());
        break;
    case ChooserRed:
        m_hsSelector->setValues(color.green(), color.blue());
        m_vSelector->setValue(color.red());
        break;
    case ChooserGreen:
        m_hsSelector->setValues(color.red(), color.blue());
        m_vSelector->setValue(color.green());
        break;
    case ChooserBlue:
        m_hsSelector->setValues(color.green(), color.red());
        m_vSelector->setValue(color.blue());
        break;
    case ChooserHue:
        m_hsSelector->setValues(color.saturation(), color.value());
        m_vSelector->setValue(color.hue());
        break;
    case ChooserClassic:
        m_hsSelector->setValues(color.hue(), color.saturation());
        m_vSelector->setValue(color.value());
        break;
    }

    bool blocked = m_vSelector->blockSignals(true);

    m_vSelector->setHue(color.hue());
    m_vSelector->setSaturation(color.saturation());
    m_vSelector->setColorValue(color.value());
    m_vSelector->updateContents();
    m_vSelector->blockSignals(blocked);
    m_vSelector->repaint();

    blocked = m_hsSelector->blockSignals(true);

    m_hsSelector->setHue(color.hue());
    m_hsSelector->setSaturation(color.saturation());
    m_hsSelector->setColorValue(color.value());
    m_hsSelector->updateContents();
    m_hsSelector->blockSignals(blocked);
    m_hsSelector->repaint();
}

#include "rectanglecolorwidget.moc"
