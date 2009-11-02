/*********************************************************************************
*  Copyright (C) 2009 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>   *
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

#ifndef BUILDER_COLOR_SELECTOR_H
#define BUILDER_COLOR_SELECTOR_H

#include "colorselector.h"

#include "ui_builderwidget.h"

class BuilderColorSelector : public ColorSelector
{
    Q_OBJECT

    public:
        BuilderColorSelector(QWidget * parent = 0);

        // textInput = false then olor components will be insert through SpinBox's controls
        void setInputType(bool textInput);

    public slots:
        void setColor(const QColor & color);

    private slots:
        // this send a color to current color widget by emit colorSelected signal
        void buildColor();
        void setCurrentSource(int tabIndex);
//         void dispatcher(int value);

    private:

    private:
        int m_currentSource; // 0 RGB , 1 HSV , 2 CMY , 3 Textual

        Ui::BuilderForm m_uiBuilderForm;
};

#endif // BUILDER_COLOR_SELECTOR_H
