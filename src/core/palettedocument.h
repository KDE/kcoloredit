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

#ifndef PALETTE_DOCUMENT_H
#define PALETTE_DOCUMENT_H

#include <QtCore/QObject>
#include <QtCore/QModelIndex>

class PaletteModel;

class PaletteDocument : public QObject
{
    Q_OBJECT

    public:
        PaletteDocument(QObject * parent = 0);
        ~PaletteDocument();

        QString fullPathFileName() const;
        QString fileName() const;

        PaletteModel * model();

        bool openPaletteFile(const QString & fileName);
        bool saveFile();
        bool saveFileAs(const QString & fileName);

        QString lastErrorString() const;

    signals:
        void modified();

    private slots:
        void updateDocStateWhenInsertItem(const QModelIndex & topLeft, const QModelIndex & bottomRight);
        void updateDocStateWhenRemoveItem(const QModelIndex & parent, int start, int end);

    private:
        PaletteModel * m_model;
        QString m_fullPathFile;
        QString m_file;
        QString m_lastErrorString;
};

#endif // PALETTE_DOCUMENT_H
