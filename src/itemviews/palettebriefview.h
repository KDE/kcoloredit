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

#ifndef PALETTE_GRID_VIEW_H
#define PALETTE_GRID_VIEW_H

#include <QtCore/QModelIndex>

#include <KColorCells>

class QSlider;
class QCheckBox;

class KAction;
class KMenu;
class KPushButton;

class PaletteModel;

class ColorCellsAdapter : public KColorCells
{
    Q_OBJECT

    public:
        ColorCellsAdapter(QWidget * parent = 0, int row = 0, int column = 0);

        KAction * cutAction() const;
        KAction * copyAction() const;
        KAction * pasteAction() const;

    protected:
        virtual void mousePressEvent(QMouseEvent * event);

    private:
        KAction * m_cutAction;
        KAction * m_copyAction;
        KAction * m_pasteAction;

        KMenu * m_menu;
};

class PaletteBriefView : public QWidget
{
    Q_OBJECT

    public:
        PaletteBriefView(PaletteModel * model, QWidget * parent = 0);

        void setModel(PaletteModel * model);

    public slots:
        void setZoomFactor(int factor);
        void zoomOut();
        void zoomIn();

    signals:
        void selectedItem(int index);
        void trackedItem(int index);
        void trackedColor(const QColor & color);

    private slots:
        void updateWhenInsertItem(const QModelIndex & topLeft, const QModelIndex & bottomRight);
        void updateWhenRemoveItem(const QModelIndex & parent, int start, int end);

        void updateIndex(int row, int column);
        void trackColor(int row, int column);
        void showComments(bool show); // NOTE if show commenten then can't copy/cut/paste

        void cut();
        void copy();
        void paste();

    private:
        void loadDataFromModel();

    private:
        PaletteModel * m_model;

        ColorCellsAdapter * m_colorCellsAdapter;

        QSlider * m_setColumnSlider;
        KPushButton * m_zoomOutButton;
        KPushButton * m_zoomInButton;
        QCheckBox * m_quickNavigationCheckBox;
        QCheckBox * m_showCommentsCheckBox;
};

#endif // PALETTE_GRID_VIEW_H
