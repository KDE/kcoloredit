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

#ifndef KCOLOREDIT_WIDGET_H
#define KCOLOREDIT_WIDGET_H

#include <QtGui/QWidget>

class PaletteModel;

class ColorWidget;

class KdeColorSelector;
class GtkColorSelector;
class BlenderColorSelector;

class ColorInfoVisualComplement;
class ColorInfoVisualTriadic;
class ColorInfoVisualTetradic;
class ColorInfoVisualAnalogous;

class KColorEditWidget : public QWidget
{
    Q_OBJECT

    public:
        KColorEditWidget(QWidget * parent = 0);

        void setModel(PaletteModel * model);

        QColor selectedColor() const;

    public slots:
        void setColor(const QColor & color);

    private slots:
        void addAllColorRange();
        void addLowestColorRange();
        void addHighestColorRange();

        void addColorFromSchemes(const QColor color);

    private:
        PaletteModel * m_model;

        ColorWidget * m_colorDispatcher;

        KdeColorSelector * m_kdeColorSelector;
        GtkColorSelector * m_gtkColorSelector;
        BlenderColorSelector * m_blenderColorSelector;

        ColorInfoVisualComplement * m_colorInfoVisualComplement;
        ColorInfoVisualTriadic * m_colorInfoVisualTriadic;
        ColorInfoVisualTetradic * m_colorInfoVisualTetradic;
        ColorInfoVisualAnalogous * m_colorInfoVisualAnalogous;
};

#endif // KCOLOREDIT_WIDGET_H
