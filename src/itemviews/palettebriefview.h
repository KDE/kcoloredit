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

#ifndef PALETTE_BRIEF_VIEW_H
#define PALETTE_BRIEF_VIEW_H

#include <QtGui/QWidget>

class QSlider;
class QCheckBox;

class KPushButton;
class KColorCells;

class PaletteModel;

class PaletteBriefView : public QWidget
{
    Q_OBJECT

    public:
        explicit PaletteBriefView(PaletteModel * model, QWidget * parent = 0);

        void setModel(PaletteModel * model);

    public slots:
        void setZoomFactor(int factor);
        void zoomOut();
        void zoomIn();

    signals:
        void itemSelected(int index);
        void colorSelected(const QColor & color);
        void itemTracked(int index);
        void colorTracked(const QColor & color);

    private slots:
        void updatePaletteView();

        void updateIndex(int row, int column);
        void trackColor(int row, int column);
        void showComments(bool show);

    private:
        void setupPreferredColumns();

    private:
        PaletteModel * m_model;

        KColorCells * m_colorCells;

        QSlider * m_setColumnSlider;
        KPushButton * m_zoomOutButton;
        KPushButton * m_zoomInButton;
        QCheckBox * m_quickNavigationCheckBox;
        QCheckBox * m_showCommentsCheckBox;
};

#endif // PALETTE_BRIEF_VIEW_H
