/***************************************************************************
                          kcoloredit.cpp  -  description
                             -------------------
    begin                : Sat Jul  8 09:57:28 CEST 2000
    copyright            : (C) 2000 by Artur Rataj
    email                : art@zeus.polsl.gliwice.pl
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// include files for QT
#include <qdir.h>
#include <qprinter.h>
#include <qpainter.h>
//Added by qt3to4:
#include <QMouseEvent>

// include files for KDE
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kcolordialog.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kstandardaction.h>
// application specific includes
#include "kcoloredit.h"
#include "kcoloreditview.h"
#include "kcoloreditdoc.h"
#include "loadpalettedlg.h"
#include "resource.h"
#include <kmenu.h>
#include <kstatusbar.h>
#include <kglobal.h>


KColorEditApp::KColorEditApp() : KMainWindow(0) {
  config=KGlobal::config();

  ///////////////////////////////////////////////////////////////////
  // call inits to invoke all other construction parts
  initActions();
  initStatusBar();
  initDocument();
  initView();

  resize(606,400);
  setupGUI();

  readOptions();

  gettingColorFromScreen = false;
}

KColorEditApp::~KColorEditApp() {
}

void KColorEditApp::initActions()
{
  // File actiojns
  KStandardAction::openNew( this, SLOT( slotFileNew() ), actionCollection() );
  KStandardAction::open( this, SLOT( slotFileOpen() ), actionCollection() );
  KStandardAction::saveAs( this, SLOT( slotFileSaveAs() ), actionCollection() );
  KStandardAction::close( this, SLOT( slotClose() ), actionCollection() );
  KStandardAction::quit( this, SLOT( slotQuit() ), actionCollection() );
  m_actSave = KStandardAction::save( this, SLOT( slotFileSave() ),
          actionCollection() );
  m_actRecent = KStandardAction::openRecent( this,
          SLOT( slotFileOpenRecent( const KUrl& ) ), actionCollection() );

  KAction *action = new KAction( i18n("New &Window"), actionCollection(), "file_new_window" );
  connect(action, SIGNAL(triggered(bool)), SLOT( slotFileNewWindow() ));
  static_cast<QAction*>(action)->setIcon(qApp->windowIcon().pixmap(IconSize(K3Icon::Small),IconSize(K3Icon::Small)));

  // Edit actions
  m_actCut = KStandardAction::cut( this, SLOT( slotEditCut() ),
          actionCollection() );
  m_actCopy = KStandardAction::copy( this, SLOT( slotEditCopy() ),
          actionCollection() );
  m_actPaste = KStandardAction::paste( this, SLOT( slotEditPaste() ),
          actionCollection() );

  m_actPaste->setEnabled( false );

  // Color Menu
  m_actNames = new KToggleAction( i18n("Show &Color Names"), actionCollection(), "color_view_names" );
  connect(m_actNames, SIGNAL(triggered(bool) ), SLOT( slotViewColorNames() ));
  m_actNames->setCheckedState(KGuiItem(i18n("Hide &Color Names")));
  m_actPalette = new KAction( i18n("From &Palette"), actionCollection(), "color_from_palette" );
  connect(m_actPalette, SIGNAL(triggered(bool) ), SLOT( slotColorFromPalette() ));
  action = new KAction( i18n("From &Screen"), actionCollection(), "color_from_screen" );
  connect(action, SIGNAL(triggered(bool)),SLOT( slotColorFromScreen() ));
}

void KColorEditApp::initStatusBar()
{
  statusBar()->insertItem(i18n("Ready."), ID_STATUS_MSG, 1);
  statusBar()->setItemAlignment( ID_STATUS_MSG, Qt::AlignLeft );
}

void KColorEditApp::initDocument()
{
  doc = new KColorEditDoc(this);
  doc->newDocument();

  connect( doc, SIGNAL( selectionChanged( int, int ) ),
          SLOT( slotSelectionChanged( int, int ) ) );
  connect( doc, SIGNAL( clipboardChanged() ),
          SLOT( slotClipboardChanged() ) );
  connect( doc, SIGNAL( modified( bool ) ),
          SLOT( slotModified( bool ) ) );
  connect( doc, SIGNAL( paletteAvailable( bool ) ),
          SLOT( slotPaletteAvailable( bool ) ) );
}

void KColorEditApp::initView()
{
  ////////////////////////////////////////////////////////////////////
  // create the main widget here that is managed by KMainWindow's view-region
  // and connect the widget to your document to display document contents.

  view = new KColorEditView(this);
  doc->addView(view);
  setCentralWidget(view);
  setCaption(doc->title());
}

void KColorEditApp::openDocumentFile(const char* _cmdl)
{
  doc->openDocument(_cmdl);
}


KColorEditDoc *KColorEditApp::document() const
{
  return doc;
}

void KColorEditApp::saveOptions()
{
  saveMainWindowSettings( config, "MainWindowSettings" );
  m_actRecent->saveEntries( config );

  config->setGroup("KColorEdit Options");
  config->writeEntry("ColorNames", viewColorNames);
}

void KColorEditApp::readOptions()
{
  applyMainWindowSettings( config, "MainWindowSettings" );
  m_actRecent->loadEntries( config );

  config->setGroup("KColorEdit Options");

  viewColorNames = config->readEntry("ColorNames", false);
  m_actNames->setChecked(viewColorNames);
  doc->slotChangeViewMode(viewColorNames);
}

bool KColorEditApp::queryClose()
{
  return doc->saveModified();
}

bool KColorEditApp::queryExit()
{
  saveOptions();
  return true;
}

/////////////////////////////////////////////////////////////////////
// SLOT IMPLEMENTATION
/////////////////////////////////////////////////////////////////////

void KColorEditApp::slotSelectionChanged( int begin, int end )
{
  m_actCut->setEnabled( begin != end );
  m_actCopy->setEnabled( begin != end );
}

void KColorEditApp::slotClipboardChanged()
{
  m_actPaste->setEnabled( true );
}

void KColorEditApp::slotModified( bool b )
{
  m_actSave->setEnabled( b );
}

void KColorEditApp::slotPaletteAvailable( bool b )
{
  m_actPalette->setEnabled( b );
}

void KColorEditApp::slotFileNewWindow()
{
  KColorEditApp *new_window= new KColorEditApp();
  new_window->show();
}

void KColorEditApp::slotFileNew()
{
  if(doc->saveModified()) {
    doc->newDocument();

    setCaption(doc->title());
  }
}

void KColorEditApp::slotFileOpen() {
  if(doc->saveModified()) {
    LoadPaletteDlg dialog(this);
    if(dialog.exec()) {
      QString fileToOpen = dialog.getFileName();
      if(!fileToOpen.isEmpty())
      {
        if(!doc->openDocument( fileToOpen )) {
          KMessageBox::sorry(0, doc->errorString());
        } else {
          setCaption(doc->title());
          m_actRecent->addUrl( KUrl::fromPathOrUrl( fileToOpen ) );
        }
      }
    }
  }
}

void KColorEditApp::slotFileOpenRecent( const KUrl & url )
{
  if(doc->saveModified()) {
    doc->openDocument( url.path() );
    setCaption(doc->title());
  }
}

void KColorEditApp::slotFileSave()
{
  if(!doc->saveDocument( doc->absFilePath() ))
      slotFileSaveAs();
      //KMessageBox::sorry(0, doc->getErrorString());
}

bool KColorEditApp::slotFileSaveAs()
{
    bool result = true;

    while(result) {
        QString newName=KFileDialog::getSaveFileName(lastSavePaletteAsFileDir,
                  "*|" + i18n("All Files"), this, i18n("Save As"));
        if(newName.isEmpty())
            result = false;
        else {
      QFileInfo saveAsInfo(newName);
      if(!saveAsInfo.exists() ||
               KMessageBox::questionYesNo( this,
                       i18n("A Document with this name already exists.\n"
                            "Do you want to overwrite it?"),
                       i18n("Warning"), KGuiItem(i18n("Overwrite")), KStdGuiItem::cancel() ) == KMessageBox::Yes) {
                if(!doc->saveDocument( newName )) {
                    KMessageBox::sorry(0, doc->errorString());
                    result = false;
                } else {
                    doc->setTitle(saveAsInfo.fileName());
                    doc->setAbsFilePath(saveAsInfo.absoluteFilePath());
                    setCaption(doc->title());
                    lastSavePaletteAsFileDir = saveAsInfo.absoluteFilePath();
                    m_actRecent->addUrl( KUrl( newName ) );
                    break;
                }
            }
        }
    }

    return result;
}

void KColorEditApp::slotClose()
{
  close();
}

void KColorEditApp::slotFilePrint()
{
  QPrinter printer;
  if (printer.setup(this))
  {
    view->print(&printer);
  }
}

void KColorEditApp::slotQuit()
{
  saveOptions();
  // close the first window, the list makes the next one the first again.
  // This ensures that queryClose() is called on each window to ask for closing
  KMainWindow* w;
  if(!memberList().isEmpty())
  {
	for (int i = 0; i < memberList().size(); ++i)
    {
	  w = memberList().at(i);
      // only close the window if the closeEvent is accepted. If the user presses Cancel on the saveModified() dialog,
      // the window and the application stay open.
      if(!w->close())
  break;
    }
  }
}

void KColorEditApp::slotEditCut()
{
  doc->cut();
}

void KColorEditApp::slotEditCopy()
{
  doc->copy();
}

void KColorEditApp::slotEditPaste()
{
  doc->paste();
}

void KColorEditApp::slotColorFromPalette() {
  view->chooseColor(doc->paletteHistory()->editableStream()->
  color( doc->paletteCursorPos() ));
}

void KColorEditApp::slotColorFromScreen() {
  gettingColorFromScreen = true;
  grabMouse(Qt::CrossCursor);
  grabKeyboard();
}

void KColorEditApp::slotViewColorNames()
{
  viewColorNames = m_actNames->isChecked();
  doc->slotChangeViewMode(viewColorNames);
}

void KColorEditApp::mouseReleaseEvent(QMouseEvent* event) {
	if(gettingColorFromScreen) {
		gettingColorFromScreen = false;
		releaseMouse();
		releaseKeyboard();
		QColor rgbColor =  KColorDialog::grabColor(event->globalPos());
		color.setComponents(rgbColor.red(), rgbColor.green(), rgbColor.blue());
		view->chooseColor(&color);
	} else
		KMainWindow::mouseReleaseEvent(event);
}

#include "kcoloredit.moc"
