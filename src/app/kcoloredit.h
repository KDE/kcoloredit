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

#ifndef KCOLOREDIT_H
#define KCOLOREDIT_H

#include <KUrl>
#include <KXmlGuiWindow>

#include "ui_builderconfiguration.h"

class QDockWidget;
class QLabel;

class KRecentFilesAction;

class PaletteDocument;
class PaletteDetailView;
class PaletteBriefView;

class KColorEditWidget;

class KColorEditMainWnd : public KXmlGuiWindow
{
    Q_OBJECT

    public:
        explicit KColorEditMainWnd(QWidget * parent = 0, Qt::WindowFlags f = KDE_DEFAULT_WINDOWFLAGS);
        virtual ~KColorEditMainWnd();

    public slots:
        void openFile(const KUrl & url = KUrl());

    protected:
        bool queryClose();

    private slots:
        void newFile();
        void saveFile();
        void saveFileAs();

        void cleanPalette();

        void generateColorNames();
        void completeColorNames();

        void configureApp();

        void updateTittleWhenChangeDocState();
        void updateTittleWhenOpenSaveDoc();

        void updateInputTypePreviewImage(bool textInput);

    private:
        void setupWidgets();
        void setupActions();

        QString appName() const;

    private:
        KRecentFilesAction *m_recentFilesAction;

        PaletteDocument *m_paletteDocument;

        PaletteBriefView *m_paletteBriefView;
        PaletteDetailView *m_paletteDetailView;

        KColorEditWidget *m_kColorEditWidget;

        QDockWidget *m_paletteBriefViewDockWidget;
        QDockWidget *m_paletteDetailViewDockWidget;

        QLabel *m_inputTypePreviewLabel; // same as the config .ui file

        Ui::BuilderConfigurationForm m_uiBuilderConfigurationForm;
};

#endif // KCOLOREDIT_H
