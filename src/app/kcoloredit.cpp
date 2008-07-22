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

#include "kcoloredit.h"

#include <QtGui/QDockWidget>

#include <KLocalizedString>
#include <KIO/NetAccess>
#include <KApplication>
#include <KMessageBox>
#include <KAction>
#include <KActionCollection>
#include <KFileDialog>

#include "palettedocument.h"
#include "palettemodel.h"
#include "palettedetailview.h"
#include "palettebriefview.h"
#include "kcoloreditwidget.h"
#include "palettedialog.h"

KColorEditMainWnd::KColorEditMainWnd(QWidget * parent, Qt::WindowFlags f) : KXmlGuiWindow(parent, f)
{
    setupWidgets();
    setupActions();

    setupGUI();
}

KColorEditMainWnd::~KColorEditMainWnd()
{
}

void KColorEditMainWnd::openPaletteFile(const QString & file)
{
    QString fileNameFromDialog = file;

    QString tmpFile;

    if (!fileNameFromDialog.isEmpty())
    {
        if (KIO::NetAccess::download(fileNameFromDialog, tmpFile, this))
        {
            if (m_paletteDocument->openPaletteFile(tmpFile))
            {
                m_paletteDetailView->setModel(m_paletteDocument->model());
                m_paletteDetailView->updateHeaders(QModelIndex(), QModelIndex());

                m_paletteBriefView->setModel(m_paletteDocument->model());

                m_kColorEditWidget->setModel(m_paletteDocument->model());

                // NOTE little hack to force the signal dataChange to update the gridView
                if (m_paletteDocument->model()->rowCount() > 0)
                    m_paletteDocument->model()->setData(m_paletteDocument->model()->index(0, 0), m_paletteDocument->model()->index(0, 0).data());

                updateTittleWhenOpenSaveDoc();
            }
            else
                KMessageBox::error(this, m_paletteDocument->lastErrorString());

            KIO::NetAccess::removeTempFile(tmpFile);
        }
        else
            KMessageBox::error(this, KIO::NetAccess::lastErrorString());
    }

}

//BEGIN public slots

void KColorEditMainWnd::newFile()
{
}

void KColorEditMainWnd::openFile()
{
    QString fileNameFromDialog = PaletteDialog::getOpenPaletteName();

    QString tmpFile;

    if (!fileNameFromDialog.isEmpty())
    {
        if (KIO::NetAccess::download(fileNameFromDialog, tmpFile, this))
        {
            if (m_paletteDocument->openPaletteFile(tmpFile))
            {
                m_paletteDetailView->setModel(m_paletteDocument->model());
                m_paletteDetailView->updateHeaders(QModelIndex(), QModelIndex());

                m_paletteBriefView->setModel(m_paletteDocument->model());

                m_kColorEditWidget->setModel(m_paletteDocument->model());

                // NOTE little hack to force the signal dataChange to update the gridView
                if (m_paletteDocument->model()->rowCount() > 0)
                    m_paletteDocument->model()->setData(m_paletteDocument->model()->index(0, 0), m_paletteDocument->model()->index(0, 0).data());

                updateTittleWhenOpenSaveDoc();
            }
            else
                KMessageBox::error(this, m_paletteDocument->lastErrorString());

            KIO::NetAccess::removeTempFile(tmpFile);
        }
        else
            KMessageBox::error(this, KIO::NetAccess::lastErrorString());
    }
}

void KColorEditMainWnd::saveFile()
{
    if(!m_paletteDocument->fullPathFileName().isEmpty())
    {
        if (!m_paletteDocument->saveFileAs(m_paletteDocument->fullPathFileName()))
            KMessageBox::error(this, m_paletteDocument->lastErrorString());
        else
            updateTittleWhenOpenSaveDoc();
    }
    else
        saveFileAs();
}

void KColorEditMainWnd::saveFileAs()
{
    // same code in palettedialog ... utils.h ?
    QString allSupportedStr = i18n("All Supported Files");
    QString kdePaletteStr = i18n("KDE Palette");
    QString gimpPaletteStr = i18n("GIMP Palette");

    KUrl url = KUrl(QDir::homePath() + QLatin1Char('/'));
    QString filter = QString("*.colors *.gpl|") + allSupportedStr + QString("\n*.colors|") + kdePaletteStr +
        QString(" (*.colors)\n*.gpl|") + gimpPaletteStr + QString(" (*.gpl)");

    QString paletteFile = KFileDialog::getSaveFileName(url, filter);

    if (!paletteFile.isEmpty())
    {
        if (!m_paletteDocument->saveFileAs(paletteFile))
            KMessageBox::error(this, m_paletteDocument->lastErrorString());
        else
            updateTittleWhenOpenSaveDoc();
    }
}

void KColorEditMainWnd::newWindow()
{
    KColorEditMainWnd * newWnd = new KColorEditMainWnd();
    newWnd->show();
}

/*
// TODO
void KColorEditMainWnd::settingsPreferences()
{
    KConfigDialog dialog(this, QString("TODO :P"), new KConfigSkeleton());

    dialog.exec();
}
*/

void KColorEditMainWnd::cleanPalette()
{
    if (m_paletteDocument->model()->rowCount() > 0)
        if (KMessageBox::questionYesNo(this
            , i18n("This action will delete all items (colors and comments) of the current palette.")
            , QString()
            , KGuiItem(i18n("Clean Palette"), KIcon("edit-clear"))
            , KStandardGuiItem::cancel())
            == KMessageBox::Yes)
            m_paletteDocument->model()->removeRows(0, m_paletteDocument->model()->rowCount());
}

void KColorEditMainWnd::generateColorNames()
{
    if (m_paletteDocument->model()->rowCount() > 0)
        if (KMessageBox::questionYesNo(this
        , i18n("This action will replace the name of all color items with names in this format #RRGGBB.")
        , QString()
        , KGuiItem(i18n("Generate Color Names"), KIcon("format-stroke-color"))
        , KStandardGuiItem::cancel())
        == KMessageBox::Yes)
            m_paletteDocument->model()->generateColorNames();
}

void KColorEditMainWnd::completeColorNames()
{
    if (m_paletteDocument->model()->rowCount() > 0)
        m_paletteDocument->model()->completeColorNames();
}

void KColorEditMainWnd::addColorItem()
{
    m_paletteDetailView->addColorItem(m_kColorEditWidget->selectedColor());
}

void KColorEditMainWnd::addCommentItem()
{
    m_paletteDetailView->addCommentItem();
}

void KColorEditMainWnd::insertColorItem()
{
    m_paletteDetailView->insertColorItem(m_paletteDetailView->selectedIndex(), m_kColorEditWidget->selectedColor());
}

void KColorEditMainWnd::insertCommentItem()
{
    m_paletteDetailView->insertCommentItem(m_paletteDetailView->selectedIndex());
}

void KColorEditMainWnd::removeItem()
{
    m_paletteDetailView->removeItem(m_paletteDetailView->selectedIndex());
}

void KColorEditMainWnd::moveNext()
{
    m_paletteDetailView->moveItem(m_paletteDetailView->selectedIndex(), Palette::MoveToPrev);
}

void KColorEditMainWnd::movePrev()
{
    m_paletteDetailView->moveItem(m_paletteDetailView->selectedIndex(), Palette::MoveToNext);
}

void KColorEditMainWnd::moveBegin()
{
    m_paletteDetailView->moveItem(m_paletteDetailView->selectedIndex(), Palette::MoveToStart);
}

void KColorEditMainWnd::moveEnd()
{
    m_paletteDetailView->moveItem(m_paletteDetailView->selectedIndex(), Palette::MoveToEnd);
}

//END public slots

void KColorEditMainWnd::updateTittleWhenChangeDocState()
{
    setWindowTitle(QString("KColorEdit") + " - " + m_paletteDocument->fileName() + i18n(" [modified]"));
}

void KColorEditMainWnd::setupWidgets()
{
    //init viewers

    m_paletteDocument = new PaletteDocument(this);

    m_paletteDetailView = new PaletteDetailView(m_paletteDocument->model(), this);

    m_paletteBriefView = new PaletteBriefView(m_paletteDocument->model(), this);

    //init central widget

    m_kColorEditWidget = new KColorEditWidget(this);
    m_kColorEditWidget->setModel(m_paletteDocument->model());

    setCentralWidget(m_kColorEditWidget);

    m_paletteTableDockWidget = new QDockWidget(i18n("Brief view"), this);
    m_paletteTableDockWidget->setObjectName(QString("brief-view-dock"));
    m_paletteTableDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
    m_paletteTableDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_paletteTableDockWidget->setWidget(m_paletteBriefView);
    m_paletteTableDockWidget->setMinimumWidth(310);

    m_paletteListDockWidget = new QDockWidget(i18n("Detail view"), this);
    m_paletteListDockWidget->setObjectName(QString("detail-view-dock"));
    m_paletteListDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
    m_paletteListDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_paletteListDockWidget->setWidget(m_paletteDetailView);
    m_paletteListDockWidget->setMinimumWidth(310);

    addDockWidget(Qt::RightDockWidgetArea, m_paletteListDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, m_paletteTableDockWidget);

    tabifyDockWidget(m_paletteListDockWidget, m_paletteTableDockWidget);

    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);

    connect(m_paletteDocument, SIGNAL( modified() ), this, SLOT( updateTittleWhenChangeDocState() ));

    connect(m_paletteBriefView, SIGNAL( trackedColor(QColor) ), m_kColorEditWidget, SLOT( setColor(QColor) ));
    connect(m_paletteBriefView, SIGNAL( trackedItem(int) ), m_paletteDetailView, SLOT( slotScrollToItem(int) ));

    connect(m_paletteBriefView, SIGNAL( selectedItem(int) ), m_paletteDetailView, SLOT( setSelectedItem(int) ));

    // NOTE setup default colors colors
    m_kColorEditWidget->setColor(Qt::blue);
}

void KColorEditMainWnd::setupActions()
{
    KAction * tmpAction = 0;

    /// palette menu

    tmpAction = actionCollection()->addAction("clean-palette");
    tmpAction->setIcon(KIcon("edit-clear"));
    tmpAction->setText(i18n("Clean Palette"));

    tmpAction = actionCollection()->addAction("generate-color-names");
    tmpAction->setIcon(KIcon("format-stroke-color"));
    tmpAction->setText(i18n("Generate Color Names"));

    tmpAction = actionCollection()->addAction("complete-color-names");
    tmpAction->setIcon(KIcon("format-stroke-color"));
    tmpAction->setText(i18n("Complete Color Names"));

    /// palette toolbar

    tmpAction = actionCollection()->addAction("add-color");
    tmpAction->setIcon(KIcon("list-add"));
    tmpAction->setText(i18n("Add Color"));

    tmpAction = actionCollection()->addAction("add-comment");
    tmpAction->setIcon(KIcon("insert-text"));
    tmpAction->setText(i18n("Add Comment"));

    tmpAction = actionCollection()->addAction("insert-color");
    tmpAction->setIcon(KIcon("list-add"));
    tmpAction->setText(i18n("Insert Color"));

    tmpAction = actionCollection()->addAction("insert-comment");
    tmpAction->setIcon(KIcon("insert-text"));
    tmpAction->setText(i18n("Insert Comment"));

    tmpAction = actionCollection()->addAction("remove-item");
    tmpAction->setIcon(KIcon("list-remove"));
    tmpAction->setText(i18n("Remove Item"));

    tmpAction = actionCollection()->addAction("move-next");
    tmpAction->setIcon(KIcon("go-up"));
    tmpAction->setText(i18n("Prev position"));

    tmpAction = actionCollection()->addAction("move-prev");
    tmpAction->setIcon(KIcon("go-down"));
    tmpAction->setText(i18n("Next position"));

    tmpAction = actionCollection()->addAction("move-begin");
    tmpAction->setIcon(KIcon("go-top"));
    tmpAction->setText(i18n("First position"));

    tmpAction = actionCollection()->addAction("move-end");
    tmpAction->setIcon(KIcon("go-bottom"));
    tmpAction->setText(i18n("Last position"));

    /// palette menu
    connect(dynamic_cast<KAction *>(actionCollection()->action("clean-palette"))        , SIGNAL( triggered(bool) ), this, SLOT( cleanPalette() ));
    connect(dynamic_cast<KAction *>(actionCollection()->action("generate-color-names")) , SIGNAL( triggered(bool) ), this, SLOT( generateColorNames() ));
    connect(dynamic_cast<KAction *>(actionCollection()->action("complete-color-names")) , SIGNAL( triggered(bool) ), this, SLOT( completeColorNames() ));

    /// palette toolbar

    connect(dynamic_cast<KAction *>(actionCollection()->action("add-color"))     , SIGNAL( triggered(bool) ), this, SLOT( addColorItem()       ));
    connect(dynamic_cast<KAction *>(actionCollection()->action("add-comment"))   , SIGNAL( triggered(bool) ), this, SLOT( addCommentItem()     ));
    connect(dynamic_cast<KAction *>(actionCollection()->action("insert-color"))  , SIGNAL( triggered(bool) ), this, SLOT( insertColorItem()    ));
    connect(dynamic_cast<KAction *>(actionCollection()->action("insert-comment")), SIGNAL( triggered(bool) ), this, SLOT( insertCommentItem()  ));
    connect(dynamic_cast<KAction *>(actionCollection()->action("remove-item"))   , SIGNAL( triggered(bool) ), this, SLOT( removeItem()         ));
    connect(dynamic_cast<KAction *>(actionCollection()->action("move-next"))     , SIGNAL( triggered(bool) ), this, SLOT( moveNext()           ));
    connect(dynamic_cast<KAction *>(actionCollection()->action("move-prev"))     , SIGNAL( triggered(bool) ), this, SLOT( movePrev()           ));
    connect(dynamic_cast<KAction *>(actionCollection()->action("move-begin"))    , SIGNAL( triggered(bool) ), this, SLOT( moveBegin()          ));
    connect(dynamic_cast<KAction *>(actionCollection()->action("move-end"))      , SIGNAL( triggered(bool) ), this, SLOT( moveEnd()            ));

    // NOTE docks's actinos
    actionCollection()->addAction("show-detail-view", m_paletteTableDockWidget->toggleViewAction());
    actionCollection()->addAction("show-brief-view", m_paletteListDockWidget->toggleViewAction());

    KStandardAction::open   (this, SLOT( openFile()   ), actionCollection());
    KStandardAction::save   (this, SLOT( saveFile()   ), actionCollection());
    KStandardAction::saveAs (this, SLOT( saveFileAs() ), actionCollection());

    KStandardAction::openNew(this, SLOT( newWindow()       ), actionCollection());
    // TODO print
    //KStandardAction::print      (this, SLOT( slotFilePrint()           ), actionCollection());
    // TODO settings
    //KStandardAction::preferences(this, SLOT( settingsPreferences() ), actionCollection());
    KStandardAction::quit       (kapp, SLOT( quit() ), actionCollection());
}

void KColorEditMainWnd::updateTittleWhenOpenSaveDoc()
{
    // setup the window title acording to the file name
    setWindowTitle(QString("KColorEdit") + " - " + m_paletteDocument->fileName());
}

#include "kcoloredit.moc"
