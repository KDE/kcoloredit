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

ColorInfoVisual::ColorInfoVisual(QWidget * parent)
    : ColorInfo(parent)
{
}

ColorInfoVisual::~ColorInfoVisual()
{
}

//BEGIN public class ColorInfoVisualComplement

ColorInfoVisualComplement::ColorInfoVisualComplement(QWidget * parent) : ColorInfoVisual(parent)
{
    m_complementColorPatch = new KColorPatch(this);
    m_complementColorPatch->setMinimumSize(64, 64); // NOTE this valuessss ?
    m_complementColorPatch->setColor(QColor(0, 255, 255)); // default complement color
    m_complementColorPatch->setAcceptDrops(false);

    QHBoxLayout * mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_complementColorPatch);
}

ColorInfoVisualComplement::~ColorInfoVisualComplement()
{
}

void ColorInfoVisualComplement::setColor(const QColor & color)
{
    m_complementColorPatch->setColor(QColor(255 - color.red(), 255 - color.green(), 255 - color.blue()));
}


//BEGIN public class ColorInfoVisualComplement

#include "colorinfovisual.moc"
