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

#include "kcoloredit.h"

#include <QDockWidget>

#include <KLocalizedString>
#include <KIO/NetAccess>
#include <KMessageBox>
#include <KRecentFilesAction>
#include <KConfigDialog>
#include <KActionCollection>
#include <KFileDialog>
#include <KStatusBar>
#include <KStandardDirs>

#include "palettedocument.h"
#include "palettemodel.h"
#include "palettedetailview.h"
#include "palettebriefview.h"
#include "kcoloreditwidget.h"
#include "palettedialog.h"
#include "colorutil.h"

#include "kcoloredit_settings.h"

KColorEditMainWnd::KColorEditMainWnd(QWidget * parent, Qt::WindowFlags f)
    : KXmlGuiWindow(parent, f)
{
    setupWidgets();
    setupActions();

    KXmlGuiWindow::StandardWindowOptions stdWndOpts;
    stdWndOpts |= KXmlGuiWindow::Default;
    stdWndOpts &= ~KXmlGuiWindow::StatusBar;

    setupGUI(stdWndOpts);
}

KColorEditMainWnd::~KColorEditMainWnd()
{
    m_recentFilesAction->saveEntries(KConfigGroup(KSharedConfig::openConfig(), "Recent Files"));

    KSharedConfig::openConfig()->sync();
}

void KColorEditMainWnd::openFile(const QUrl & url)
{
    KUrl paletteUrl(url);

    if (url.isEmpty())
        paletteUrl = PaletteDialog::getOpenUrl();

    QString paletteFileName;

    if (!paletteUrl.isEmpty())
    {
        if (KIO::NetAccess::download(paletteUrl, paletteFileName, this))
        {
            if (m_paletteDocument->openFile(KUrl(paletteFileName)))
            {
                m_paletteDetailView->setModel(m_paletteDocument->model());
                m_paletteDetailView->updatePaletteDetails();

                m_paletteBriefView->setModel(m_paletteDocument->model());

                m_kColorEditWidget->setModel(m_paletteDocument->model());

                // Little hack to force emit the signal dataChange in PaletteBriefView to update the KColorCells
                if (m_paletteDocument->model()->rowCount() > 0)
                    m_paletteDocument->model()->setData(m_paletteDocument->model()->index(0, 0), m_paletteDocument->model()->index(0, 0).data());

                updateTittleWhenOpenSaveDoc();

                m_recentFilesAction->addUrl(KUrl(paletteFileName));

                m_paletteDocument->setModified(false);
                m_paletteDocument->setSaved(true);
            }
            else
            {
                m_recentFilesAction->removeUrl(KUrl(paletteFileName));

                KMessageBox::error(this, m_paletteDocument->lastErrorString());
            }

            KIO::NetAccess::removeTempFile(paletteFileName);
        }
        else
            KMessageBox::error(this, KIO::NetAccess::lastErrorString());
    }
}

bool KColorEditMainWnd::queryClose()
{
    if (m_paletteDocument->isModified())
    {
        QString paletteFileName = m_paletteDocument->url().fileName();

        if (paletteFileName.isEmpty())
            paletteFileName = i18n("Untitled");

        switch (KMessageBox::warningYesNoCancel(this,
            i18n( "The document \"%1\" has been modified.\n"
            "Do you want to save your changes or discard them?", paletteFileName),
            i18n( "Close Document" ), KStandardGuiItem::save(), KStandardGuiItem::discard()))
        {
            case KMessageBox::Yes: saveFile(); return m_paletteDocument->isSaved();
            case KMessageBox::No : return true;
            // Case KMessageBox::Cancel
            default : return false;
        }
    }

    return true;
}

void KColorEditMainWnd::newFile()
{
    KColorEditMainWnd * newWnd = new KColorEditMainWnd();
    newWnd->show();
}

void KColorEditMainWnd::saveFile()
{
    if(!m_paletteDocument->url().isEmpty())
    {
        if (!m_paletteDocument->saveFileAs(m_paletteDocument->url()))
            KMessageBox::error(this, m_paletteDocument->lastErrorString());
        else
            updateTittleWhenOpenSaveDoc();
    }
    else
        saveFileAs();
}

void KColorEditMainWnd::saveFileAs()
{
    QString paletteFileName = KFileDialog::getSaveFileName(KUrl(QDir::homePath()), PaletteDialog::filter());

    if (KIO::NetAccess::exists(KUrl(paletteFileName), KIO::NetAccess::DestinationSide, widget()))
        if (KMessageBox::warningContinueCancel(widget(), i18n("A file named \"%1\" already exists. Are you sure you want to overwrite it?", paletteFileName), QString(), KGuiItem(i18n("Overwrite"))) != KMessageBox::Continue)
            return;

    if (!paletteFileName.isEmpty())
    {
        if (!m_paletteDocument->saveFileAs(paletteFileName))
            KMessageBox::error(this, m_paletteDocument->lastErrorString());
        else
            updateTittleWhenOpenSaveDoc();
    }
}

void KColorEditMainWnd::cleanPalette()
{
    if (m_paletteDocument->model()->rowCount() > 0)
        if (KMessageBox::questionYesNo(this
            , i18n("This action will delete all items (colors and comments) of the current palette.")
            , QString()
            , KGuiItem(i18n("Clean Palette"), QIcon::fromTheme("edit-clear"))
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
        , KGuiItem(i18n("Generate Color Names"), QIcon::fromTheme("format-stroke-color"))
        , KStandardGuiItem::cancel())
        == KMessageBox::Yes)
            m_paletteDocument->model()->generateColorNames();
}

void KColorEditMainWnd::completeColorNames()
{
    if (m_paletteDocument->model()->rowCount() > 0)
        m_paletteDocument->model()->completeColorNames();
}

void KColorEditMainWnd::configureApp()
{
    if (KConfigDialog::showDialog("settings"))
        return;

    KConfigDialog *dialog = new KConfigDialog(this, "settings", Settings::self());

    QWidget *generalSettingsDlg = new QWidget;

    m_uiBuilderConfigurationForm.setupUi(generalSettingsDlg);

    updateInputTypePreviewImage(Settings::textInput());

    dialog->addPage(generalSettingsDlg, i18n("Color builder"), "format-stroke-color");
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();

    connect(m_uiBuilderConfigurationForm.kcfg_textInput, SIGNAL(toggled(bool)), SLOT(updateInputTypePreviewImage(bool)));
    connect(dialog, SIGNAL(settingsChanged(QString)), m_kColorEditWidget, SLOT(updateSettings(QString)));
}

void KColorEditMainWnd::updateTittleWhenChangeDocState()
{
    QString paletteFileName = m_paletteDocument->url().fileName();

    if (paletteFileName.isEmpty())
        paletteFileName = i18n("Untitled");

    setWindowTitle(i18n("%1 - KColorEdit [modified]", paletteFileName));
}

void KColorEditMainWnd::updateTittleWhenOpenSaveDoc()
{
    QString paletteFileName = m_paletteDocument->url().fileName();

    if (paletteFileName.isEmpty())
        paletteFileName = i18n("Untitled");

    setWindowTitle(i18n("%1 - KColorEdit", paletteFileName));
}

void KColorEditMainWnd::updateInputTypePreviewImage(bool text_input)
{
    if (text_input)
        m_uiBuilderConfigurationForm.inputTypePreviewLabel->setPixmap(
            QPixmap(KStandardDirs::locate("data","kcoloredit/pics/text-input.png")));
    else
        m_uiBuilderConfigurationForm.inputTypePreviewLabel->setPixmap(
            QPixmap(KStandardDirs::locate("data","kcoloredit/pics/controls-input.png")));
}

void KColorEditMainWnd::setupWidgets()
{
    // NOTE
    // Initialize document

    m_paletteDocument = new PaletteDocument(this);

    updateTittleWhenOpenSaveDoc();

    connect(m_paletteDocument, SIGNAL( modified() ), this, SLOT( updateTittleWhenChangeDocState() ));

    // NOTE
    // Initialize central widget (KColorEditWidget)

    m_kColorEditWidget = new KColorEditWidget(this);
    m_kColorEditWidget->setModel(m_paletteDocument->model());
    m_kColorEditWidget->setColor(ColorUtil::DEFAULT_COLOR);

    setCentralWidget(m_kColorEditWidget);

    // NOTE
    // Initialize viewers

    m_paletteDetailView = new PaletteDetailView(m_paletteDocument->model(), this);
    m_paletteBriefView = new PaletteBriefView(m_paletteDocument->model(), this);

    connect(m_paletteBriefView, SIGNAL( colorTracked(QColor) ), m_kColorEditWidget, SLOT( setColor(QColor) ));
    connect(m_paletteBriefView, SIGNAL( itemTracked(int) ), m_paletteDetailView, SLOT( scrollToItem(int) ));

    connect(m_paletteBriefView, SIGNAL( itemSelected(int) ), m_paletteDetailView, SLOT( setSelectedItem(int) ));
    connect(m_paletteBriefView, SIGNAL( colorSelected(QColor) ), m_kColorEditWidget, SLOT( setColor(QColor) ));

    connect(m_kColorEditWidget, SIGNAL(colorSelected(QColor) ), m_paletteDetailView, SLOT(setCurrentKColorEditColor(QColor)));

    // NOTE
    // Initialize docks

    m_paletteBriefViewDockWidget = new QDockWidget(i18nc("In this view the user can see the whole color pallete", "Preview"), this);
    m_paletteBriefViewDockWidget->setObjectName(QString("quick-view-dock"));
    m_paletteBriefViewDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
    m_paletteBriefViewDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_paletteBriefViewDockWidget->setWidget(m_paletteBriefView);

    m_paletteDetailViewDockWidget = new QDockWidget(i18nc("In this view the user can edit the palette", "Edit view"), this);
    m_paletteDetailViewDockWidget->setObjectName(QString("detail-view-dock"));
    m_paletteDetailViewDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
    m_paletteDetailViewDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_paletteDetailViewDockWidget->setWidget(m_paletteDetailView);
    m_paletteDetailViewDockWidget->setMinimumWidth(310);

    addDockWidget(Qt::LeftDockWidgetArea, m_paletteDetailViewDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, m_paletteBriefViewDockWidget);

    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);
}

void KColorEditMainWnd::setupActions()
{
    // NOTE
    // Standar actions

    KStandardAction::open   (this, SLOT( openFile()   ), actionCollection());
    KStandardAction::save   (this, SLOT( saveFile()   ), actionCollection());
    KStandardAction::saveAs (this, SLOT( saveFileAs() ), actionCollection());
    KStandardAction::openNew(this, SLOT( newFile()    ), actionCollection());
    KStandardAction::preferences(this, SLOT(configureApp()), actionCollection());
    KStandardAction::quit   (this, SLOT( close()      ), actionCollection());

    m_recentFilesAction = KStandardAction::openRecent(this, SLOT( openFile(QUrl) ), actionCollection());
    m_recentFilesAction->loadEntries(KConfigGroup(KSharedConfig::openConfig(), "Recent Files"));

    // NOTE
    // Actions for docks

    actionCollection()->addAction("show-detail-view", m_paletteBriefViewDockWidget->toggleViewAction());
    actionCollection()->addAction("show-brief-view", m_paletteDetailViewDockWidget->toggleViewAction());

    // NOTE
    // Actions for palette menu

    QAction * cleanPaletteAction = actionCollection()->addAction("clean-palette");
    cleanPaletteAction->setIcon(QIcon::fromTheme("edit-clear"));
    cleanPaletteAction->setText(i18n("Clean Palette"));

    QAction * generateColorNamesAction = actionCollection()->addAction("generate-color-names");
    generateColorNamesAction->setIcon(QIcon::fromTheme("format-stroke-color"));
    generateColorNamesAction->setText(i18n("Generate Color Names"));

    QAction * completeColorNamesAction = actionCollection()->addAction("complete-color-names");
    completeColorNamesAction->setIcon(QIcon::fromTheme("format-stroke-color"));
    completeColorNamesAction->setText(i18nc("Complete is a verb here, this is an action that fills missing color names in a palette", "Complete Color Names"));

    connect(cleanPaletteAction, SIGNAL( triggered(bool) ), this, SLOT( cleanPalette() ));

    connect(generateColorNamesAction, SIGNAL( triggered(bool) ), this, SLOT( generateColorNames() ));
    connect(completeColorNamesAction, SIGNAL( triggered(bool) ), this, SLOT( completeColorNames() ));
}
