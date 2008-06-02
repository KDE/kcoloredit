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

#ifndef PALETTE_DETAIL_VIEW_H
#define PALETTE_DETAIL_VIEW_H

#include <QtCore/QModelIndex>
#include <QtGui/QWidget>

#include "palette.h"

class QTextDocument;
class QTableView;

class KLineEdit;
class KTextEdit;

class PaletteModel;

class PaletteDetailView : public QWidget
{
    Q_OBJECT

    public:
        PaletteDetailView(PaletteModel * model, QWidget * parent = 0);

        void setModel(PaletteModel * model);

        int selectedIndex() const;

        void addColorItem(const QColor & color = Qt::white, const QString & colorName = QString());
        void insertColorItem(int index, const QColor & color = Qt::white, const QString & colorName = QString());

        void addCommentItem(const QString & comment = QString());
        void insertCommentItem(int index, const QString & comment = QString());

        void removeItem(int index);

        void moveItem(int index, Palette::MoveOperation operation);

    public slots:
        void setSelectedItem(int index);
        void slotScrollToItem(int index);

        void updateHeaders(const QModelIndex & topLeft, const QModelIndex & bottomRight);

    private slots:
        void updatePaletteName(const QString & text);
        void setDescriptionMode(int index);
        void updateDescriptions(const QModelIndex & topLeft, const QModelIndex & bottomRight);

    private:
        KLineEdit * m_paletteNameLineEdit;
        QTextDocument * m_briefDescriptionDocument;
        QTextDocument * m_fullDescriptionDocument;
        KTextEdit * m_paletteDescriptionTextEdit;
        QTableView * m_view;
        PaletteModel * m_model;
};

#endif // PALETTE_DETAIL_VIEW_H
