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

#ifndef PALETTE_DOCUMENT_H
#define PALETTE_DOCUMENT_H

#include <KUrl>

class PaletteModel;

class PaletteDocument : public QObject
{
    Q_OBJECT

    public:
        enum DocumentType { KDEType = 0, GIMPType };

        PaletteDocument(QObject * parent = 0);
        ~PaletteDocument();

        bool openFile(const KUrl & url);
        bool saveFileAs(const KUrl & url);

        KUrl url() const;

        bool isModified() const;
        void setModified(bool modified);

        bool isSaved() const;
        void setSaved(bool saved);

        PaletteDocument::DocumentType type() const;

        PaletteModel * model();

        QString lastErrorString() const;

    signals:
        void modified();

    private slots:
        void updatePaletteDocument();

    private:
        void extractDescriptionFromModel();

    private:
        KUrl m_url;
        PaletteModel * m_model;
        QString m_lastErrorString;
        PaletteDocument::DocumentType m_documentType;
        bool m_modified;
        bool m_saved;
};

#endif // PALETTE_DOCUMENT_H
