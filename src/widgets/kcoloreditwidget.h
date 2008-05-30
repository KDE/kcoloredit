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

class QLabel;

class KLineEdit;
class KComboBox;

class MultiPageWidget;
class ColorInfoVisualSingle;
class ColorInfoVisualComplement;

class KColorEditWidget : public QWidget
{
    Q_OBJECT

    public:
        KColorEditWidget(QWidget * parent = 0);

        QColor selectedColor() const;

    public slots:
        void setColor(const QColor & color);

    private slots:
        void getColorFromColorSelector(const QColor & color);

    private:
        QColor m_color;

        MultiPageWidget * m_selectors;
        MultiPageWidget * m_infoStyles;
        MultiPageWidget * m_infoTextModels;

        ColorInfoVisualSingle * m_colorInfoVisualSingle;
        ColorInfoVisualComplement * m_colorInfoVisualComplement;
};

#endif // KCOLOREDIT_WIDGET_H
