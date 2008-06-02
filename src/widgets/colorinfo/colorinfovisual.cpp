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

#include "colorinfovisual.h"

#include <QtGui/QHBoxLayout>

#include <KColorPatch>

ColorInfoVisual::ColorInfoVisual(QWidget * parent) : ColorInfo(parent), m_mainColorPatch(0)
{
}

ColorInfoVisual::~ColorInfoVisual()
{
}

//BEGIN public class ColorInfoVisualSingle

ColorInfoVisualSingle::ColorInfoVisualSingle(QWidget * parent) : ColorInfoVisual(parent)
{
    m_mainColorPatch = new KColorPatch(this);
    m_mainColorPatch->setMinimumSize(100, 100); // NOTE this valuessss ?
    m_mainColorPatch->setColor(Qt::red); // default color

    QHBoxLayout * mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_mainColorPatch);

    connect(m_mainColorPatch, SIGNAL( colorChanged(QColor) ), this, SLOT( setColor(QColor) ));
}

ColorInfoVisualSingle::~ColorInfoVisualSingle()
{
}

void ColorInfoVisualSingle::setColor(const QColor & color)
{
    m_mainColorPatch->setColor(color);
}

//END public class ColorInfoVisualSingle

//BEGIN public class ColorInfoVisualComplement

ColorInfoVisualComplement::ColorInfoVisualComplement(QWidget * parent) : ColorInfoVisual(parent)
{
    m_mainColorPatch = new KColorPatch(this);
    m_mainColorPatch->setMinimumSize(100, 100); // NOTE this valuessss ?
    m_mainColorPatch->setColor(Qt::red); // default color

    m_complementColorPatch = new KColorPatch(this);
    m_complementColorPatch->setMinimumSize(100, 100); // NOTE this valuessss ?
    m_complementColorPatch->setColor(QColor(0, 255, 255)); // default complement color

    QHBoxLayout * mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_mainColorPatch);
    mainLayout->addWidget(m_complementColorPatch);

    connect(m_mainColorPatch, SIGNAL( colorChanged(QColor) ), this, SLOT( setColor(QColor) ));
    connect(m_complementColorPatch, SIGNAL( colorChanged(QColor) ), this, SLOT( setComplementColor(QColor) ));
}

ColorInfoVisualComplement::~ColorInfoVisualComplement()
{
}

void ColorInfoVisualComplement::setColor(const QColor & color)
{
    // WARNING OPTIMIZE THIS
    m_mainColorPatch->setColor(color);
    m_complementColorPatch->setColor(QColor(255 - color.red(), 255 - color.green(), 255 - color.blue()));
}

void ColorInfoVisualComplement::setComplementColor(const QColor & color)
{
    m_mainColorPatch->setColor(QColor(255 - color.red(), 255 - color.green(), 255 - color.blue()));
    m_complementColorPatch->setColor(color);
}

//BEGIN public class ColorInfoVisualComplement

#include "colorinfovisual.moc"
