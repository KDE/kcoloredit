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

#ifndef KCOLOREDIT_H
#define KCOLOREDIT_H

#include <KXmlGuiWindow>

class QDockWidget;

class PaletteDocument;
class PaletteDetailView;
class PaletteGridView;

class KColorEditWidget;

class KColorEditMainWnd : public KXmlGuiWindow
{
    Q_OBJECT

    public:
        KColorEditMainWnd(QWidget * parent = 0, Qt::WindowFlags f = KDE_DEFAULT_WINDOWFLAGS);
        ~KColorEditMainWnd();

    public slots:
        void newFile();
        void openFile();
        void saveFile();
        void saveFileAs();

        void newWindow();
        // TODO settings
        //void settingsPreferences();

        void cleanPalette();
        void generateColorNames();
        void completeColorNames();

        void addColorItem();
        void addCommentItem();
        void insertColorItem();
        void insertCommentItem();
        void removeItem();
        void moveNext();
        void movePrev();
        void moveBegin();
        void moveEnd();

    private slots:
        void updateTittleWhenChangeDocState();
        void updateTittleWhenOpenSaveDoc();

    private:
        void setupWidgets();
        void setupActions();

        QString appName() const;

    private:
        PaletteDocument * m_paletteDocument;

        PaletteGridView * m_paletteGridView;
        PaletteDetailView * m_paletteDetailView;

        KColorEditWidget * m_kColorEditWidget;

        QDockWidget * m_paletteTableDockWidget;
        QDockWidget * m_paletteListDockWidget;

        QString m_appName; // save memory
};

#endif // KCOLOREDIT_H
