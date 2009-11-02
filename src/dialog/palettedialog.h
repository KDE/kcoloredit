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

#ifndef PALETTE_DIALOG_H
#define PALETTE_DIALOG_H

#include <KColorDialog>

class QLabel;
class QStringList;

class KFileWidget;
class KTabWidget;

class CollectionGrid : public KColorCells
{
    Q_OBJECT

    public:
        explicit CollectionGrid(const QString & collection, QWidget * parent = 0);

        bool isEmpty() const;

    signals:
        void collectionClicked(const QString & collection);

    private:
        void mousePressEvent(QMouseEvent * event);

    private:
        QString m_collection;
        bool m_isEmpty;
};

class CollectionsWidget : public QWidget
{
    Q_OBJECT

    public:
        CollectionsWidget(QWidget * parent = 0);
        virtual ~CollectionsWidget();

    signals:
        void selectedCollection(const QString & collection);

    private slots:
        void chooseCollection(const QString & collection);

    private:
        QList<QLabel *> m_headers;
        QList<CollectionGrid *> m_collections;
};

class PaletteDialog : public KDialog
{
    Q_OBJECT

    public:
        static QString filter();
        static KUrl getOpenUrl(QWidget * parent = 0);

        explicit PaletteDialog(QWidget * parent = 0, Qt::WFlags flags = 0);
        virtual ~PaletteDialog();

        KUrl paletteUrl() const;
        bool isKdePalette() const;

    private slots:
        void updateDialogBtns(int tabIndex);
        void selectKdePalette(const QString & kdePalette);
        void selectUrl();

    private:
        KTabWidget * m_mainWidget;

        CollectionsWidget * m_collectionsWidget;
        KFileWidget * m_fileWidget;

        KUrl m_paletteUrl; // the url of the palette file (custom palette or KDE palette)
        bool m_isKdePalette; // tell me if m_paletteUrl.path() is a kdepalette or a sigle file name
};

#endif // PALETTE_DIALOG_H
