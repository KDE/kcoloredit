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

// include files for KDE
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <kmenubar.h>
#include <klocale.h>
#include <kconfig.h>

// application specific includes
#include "kcoloredit.h"
#include "kcoloreditview.h"
#include "kcoloreditdoc.h"
#include "loadpalettedlg.h"
#include "resource.h"


KColorEditApp::KColorEditApp() {
  config=kapp->config();

  ///////////////////////////////////////////////////////////////////
  // call inits to invoke all other construction parts
  initMenuBar();
  initToolBar();
  initStatusBar();
  initKeyAccel();
  initDocument();
  initView();
	
  readOptions();

  ///////////////////////////////////////////////////////////////////
  // disable menu and toolbar items at startup
  disableCommand(ID_FILE_SAVE);
  //disableCommand(ID_FILE_SAVE_AS);
  //disableCommand(ID_FILE_PRINT);
 	
  disableCommand(ID_EDIT_CUT);
  disableCommand(ID_EDIT_COPY);
  //disableCommand(ID_EDIT_PASTE);
}

KColorEditApp::~KColorEditApp() {

}

void KColorEditApp::initKeyAccel() {
  keyAccel = new KAccel(this);
	
  // fileMenu accelerators
  keyAccel->connectItem(KStdAccel::New, this, SLOT(slotFileNew()));
  keyAccel->connectItem(KStdAccel::Open, this, SLOT(slotFileOpen()));
  keyAccel->connectItem(KStdAccel::Save, this, SLOT(slotFileSave()));
  keyAccel->connectItem(KStdAccel::Close, this, SLOT(slotFileClose()));
  //keyAccel->connectItem(KStdAccel::Print, this, SLOT(slotFilePrint()));
  keyAccel->connectItem(KStdAccel::Quit, this, SLOT(slotFileQuit()));
  // editMenu accelerators
  keyAccel->connectItem(KStdAccel::Cut, this, SLOT(slotEditCut()));
  keyAccel->connectItem(KStdAccel::Copy, this, SLOT(slotEditCopy()));
  keyAccel->connectItem(KStdAccel::Paste, this, SLOT(slotEditPaste()));

  keyAccel->connectItem(KStdAccel::Help, this, SLOT(appHelpActivated()));
			
  keyAccel->changeMenuAccel(fileMenu, ID_FILE_NEW, KStdAccel::New);
  keyAccel->changeMenuAccel(fileMenu, ID_FILE_OPEN, KStdAccel::Open);
  keyAccel->changeMenuAccel(fileMenu, ID_FILE_SAVE, KStdAccel::Save);
  keyAccel->changeMenuAccel(fileMenu, ID_FILE_CLOSE, KStdAccel::Close);
  keyAccel->changeMenuAccel(fileMenu, ID_FILE_PRINT, KStdAccel::Print);
  keyAccel->changeMenuAccel(fileMenu, ID_FILE_QUIT, KStdAccel::Quit);

  keyAccel->changeMenuAccel(editMenu, ID_EDIT_CUT, KStdAccel::Cut);
  keyAccel->changeMenuAccel(editMenu, ID_EDIT_COPY, KStdAccel::Copy);
  keyAccel->changeMenuAccel(editMenu, ID_EDIT_PASTE, KStdAccel::Paste);

  keyAccel->readSettings();	
}

void KColorEditApp::initMenuBar() {
  ///////////////////////////////////////////////////////////////////
  // MENUBAR
  recentFilesMenu = new QPopupMenu();
  connect(recentFilesMenu, SIGNAL(activated(int)), SLOT(slotFileOpenRecent(int)));

  ///////////////////////////////////////////////////////////////////
  // menuBar entry fileMenu
  fileMenu = new QPopupMenu();
  fileMenu->insertItem(kapp->miniIcon(), i18n("New &Window"), ID_FILE_NEW_WINDOW);
  fileMenu->insertSeparator();
  fileMenu->insertItem(BarIcon("filenew"), i18n("&New"), ID_FILE_NEW);
  fileMenu->insertItem(BarIcon("fileopen"), i18n("&Open..."), ID_FILE_OPEN);
  fileMenu->insertItem(i18n("Open &recent"), recentFilesMenu, ID_FILE_OPEN_RECENT);

  fileMenu->insertItem(i18n("&Close"), ID_FILE_CLOSE);
  fileMenu->insertSeparator();
  fileMenu->insertItem(BarIcon("filesave") ,i18n("&Save"), ID_FILE_SAVE);
  fileMenu->insertItem(i18n("Save &As..."), ID_FILE_SAVE_AS);
  //fileMenu->insertSeparator();
  //fileMenu->insertItem(BarIcon("fileprint"), i18n("&Print..."), ID_FILE_PRINT);
  fileMenu->insertSeparator();
  fileMenu->insertItem(i18n("&Quit"), ID_FILE_QUIT);
	
  ///////////////////////////////////////////////////////////////////
  // menuBar entry editMenu
  editMenu = new QPopupMenu();
  editMenu->insertItem(BarIcon("editcut"), i18n("Cu&t"), ID_EDIT_CUT);
  editMenu->insertItem(BarIcon("editcopy"), i18n("&Copy"), ID_EDIT_COPY);
  editMenu->insertItem(BarIcon("editpaste"), i18n("&Paste"), ID_EDIT_PASTE);

  ///////////////////////////////////////////////////////////////////
  // menuBar entry viewMenu
  viewMenu = new QPopupMenu();
  viewMenu->setCheckable(true);
  viewMenu->insertItem(i18n("&Toolbar"), ID_VIEW_TOOLBAR);
  viewMenu->insertItem(i18n("&Statusbar"), ID_VIEW_STATUSBAR);

  ///////////////////////////////////////////////////////////////////
  // menuBar entry helpMenu
  helpMenu_ = helpMenu("KColorEdit 0.1\n\n(c) 2000 by\nArtur Rataj\nart@zeus.polsl.gliwice.pl");

  ///////////////////////////////////////////////////////////////////
  // MENUBAR CONFIGURATION
  // insert your popup menus with the according menubar entries in the order
  // they will appear later from left to right
  menuBar()->insertItem(i18n("&File"), fileMenu);
  menuBar()->insertItem(i18n("&Edit"), editMenu);
  menuBar()->insertItem(i18n("&View"), viewMenu);

  menuBar()->insertSeparator();
  menuBar()->insertItem(i18n("&Help"), helpMenu_);

  ///////////////////////////////////////////////////////////////////
  // CONNECT THE MENU SLOTS WITH SIGNALS
  // for execution slots and statusbar messages

  connect(fileMenu, SIGNAL(activated(int)), SLOT(commandCallback(int)));
  connect(fileMenu, SIGNAL(highlighted(int)), SLOT(statusCallback(int)));

  connect(editMenu, SIGNAL(activated(int)), SLOT(commandCallback(int)));
  connect(editMenu, SIGNAL(highlighted(int)), SLOT(statusCallback(int)));

  connect(viewMenu, SIGNAL(activated(int)), SLOT(commandCallback(int)));
  connect(viewMenu, SIGNAL(highlighted(int)), SLOT(statusCallback(int)));

}

void KColorEditApp::initToolBar()
{

  ///////////////////////////////////////////////////////////////////
  // TOOLBAR
  toolBar()->insertButton(BarIcon("filenew"), ID_FILE_NEW, true, i18n("New File"));
  toolBar()->insertButton(BarIcon("fileopen"), ID_FILE_OPEN, true, i18n("Open File"));
  toolBar()->insertButton(BarIcon("filesave"), ID_FILE_SAVE, true, i18n("Save File"));
  //toolBar()->insertButton(BarIcon("fileprint"), ID_FILE_PRINT, true, i18n("Print"));
  toolBar()->insertSeparator();
  toolBar()->insertButton(BarIcon("editcut"), ID_EDIT_CUT, true, i18n("Cut"));
  toolBar()->insertButton(BarIcon("editcopy"), ID_EDIT_COPY, true, i18n("Copy"));
  toolBar()->insertButton(BarIcon("editpaste"), ID_EDIT_PASTE, true, i18n("Paste"));
  toolBar()->insertSeparator();
  toolBar()->insertButton(BarIcon("help"), ID_HELP_CONTENTS, SIGNAL(clicked()),
  				this, SLOT(appHelpActivated()), true,i18n("Help"));

  ///////////////////////////////////////////////////////////////////
  // INSERT YOUR APPLICATION SPECIFIC TOOLBARS HERE WITH toolBar(n)


  ///////////////////////////////////////////////////////////////////
  // CONNECT THE TOOLBAR SLOTS WITH SIGNALS - add new created toolbars by their according number
  // connect for invoking the slot actions
  connect(toolBar(), SIGNAL(clicked(int)), SLOT(commandCallback(int)));
  // connect for the status help on holing icons pressed with the mouse button
  connect(toolBar(), SIGNAL(pressed(int)), SLOT(statusCallback(int)));

}

void KColorEditApp::initStatusBar()
{
  ///////////////////////////////////////////////////////////////////
  // STATUSBAR
  // TODO: add your own items you need for displaying current application status.
  statusBar()->insertItem(i18n(IDS_STATUS_DEFAULT), ID_STATUS_MSG);
}

void KColorEditApp::initDocument()
{
  doc = new KColorEditDoc(this);
  doc->newDocument();
}

void KColorEditApp::initView()
{ 
  ////////////////////////////////////////////////////////////////////
  // create the main widget here that is managed by KTMainWindow's view-region and
  // connect the widget to your document to display document contents.

  view = new KColorEditView(this);
  doc->addView(view);
  setView(view);	
  setCaption(doc->getTitle());

}

void KColorEditApp::enableCommand(int id_)
{
  ///////////////////////////////////////////////////////////////////
  // enable menu and toolbar functions by their ID's
  menuBar()->setItemEnabled(id_, true);
  toolBar()->setItemEnabled(id_, true);
}

void KColorEditApp::disableCommand(int id_)
{
  ///////////////////////////////////////////////////////////////////
  // disable menu and toolbar functions by their ID's
  menuBar()->setItemEnabled(id_, false);
  toolBar()->setItemEnabled(id_, false);
}

void KColorEditApp::addRecentFile(const QString &file)
{
  if(recentFiles.find(file) == -1)
  {
    if( recentFiles.count() < 5)
    {
      recentFiles.insert(0, file);
    }
    else
    {
      recentFiles.remove(4);
      recentFiles.insert(0, file);
    }
    recentFilesMenu->clear();
    for ( int i=0 ; i < (int) recentFiles.count(); i++)
    {
      recentFilesMenu->insertItem(recentFiles.at(i));
    }
  }
}

void KColorEditApp::openDocumentFile(const char* _cmdl)
{
  slotStatusMsg(i18n("Opening file..."));

  doc->openDocument(_cmdl);
  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}


KColorEditDoc *KColorEditApp::getDocument() const
{
  return doc;
}

void KColorEditApp::saveOptions()
{	
  config->setGroup("General Options");
  config->writeEntry("Geometry", size());
  config->writeEntry("Show Toolbar", toolBar()->isVisible());
  config->writeEntry("Show Statusbar",statusBar()->isVisible());
  config->writeEntry("ToolBarPos", (int) toolBar()->barPos());
  config->writeEntry("Recent Files", recentFiles);
}


void KColorEditApp::readOptions()
{
	
  config->setGroup("General Options");

  // bar status settings
  bool bViewToolbar = config->readBoolEntry("Show Toolbar", true);
  viewMenu->setItemChecked(ID_VIEW_TOOLBAR, bViewToolbar);
  if(!bViewToolbar)
  {
     enableToolBar(KToolBar::Hide);
  }
	
  bool bViewStatusbar = config->readBoolEntry("Show Statusbar", true);
  viewMenu->setItemChecked(ID_VIEW_STATUSBAR, bViewStatusbar);
  if(!bViewStatusbar)
  {
    enableStatusBar(KStatusBar::Hide);
  }

  // bar position settings
  KToolBar::BarPosition toolBarPos;
  toolBarPos=(KToolBar::BarPosition) config->readNumEntry("ToolBarPos", KToolBar::Top);
  toolBar()->setBarPos(toolBarPos);
	
  // initialize the recent file list
  recentFiles.setAutoDelete(TRUE);
  config->readListEntry("Recent Files", recentFiles);
	
  for (int i=0; i < (int) recentFiles.count(); i++)
  {
    recentFilesMenu->insertItem(recentFiles.at(i));
  }

  QSize size=config->readSizeEntry("Geometry");
  if(!size.isEmpty())
  {
    resize(size);
  }
  else
  	resize(620, 460);
}

void KColorEditApp::saveProperties(KConfig *_cfg)
{
  if(doc->getTitle()!=i18n("Untitled") && !doc->isModified())
  {
    // saving to tempfile not necessary

  }
  else
  {
    QString filename=doc->getAbsFilePath();	
    _cfg->writeEntry("filename", filename);
    _cfg->writeEntry("modified", doc->isModified());
		
    QString tempname = kapp->tempSaveName(filename);
    doc->saveDocument(tempname);
  }
}


void KColorEditApp::readProperties(KConfig* _cfg)
{
  QString filename = _cfg->readEntry("filename", "");
  bool modified = _cfg->readBoolEntry("modified", false);
  if(modified)
  {
    bool canRecover;
    QString tempname = kapp->checkRecoverFile(filename, canRecover);
  	
    if(canRecover)
    {
      doc->openDocument(tempname);
      doc->setModified(true);
      QFileInfo info(filename);
      doc->setAbsFilePath(info.absFilePath());
      doc->setTitle(info.fileName());
      QFile::remove(tempname);
    }
  }
  else
  {
    if(!filename.isEmpty())
    {
      doc->openDocument(filename);
    }
  }

  QString caption=kapp->caption();	
  setCaption(doc->getTitle());
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

void KColorEditApp::slotFileNewWindow()
{
  slotStatusMsg(i18n("Opening a new application window..."));
	
  KColorEditApp *new_window= new KColorEditApp();
  new_window->show();

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void KColorEditApp::slotFileNew()
{
  slotStatusMsg(i18n("Creating new document..."));

  if(doc->saveModified()) {
    doc->newDocument();		

    setCaption(doc->getTitle());
  }

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void KColorEditApp::slotFileOpen() {
  slotStatusMsg(i18n("Opening file..."));
	
  if(doc->saveModified()) {
    LoadPaletteDlg dialog;
    if(dialog.exec()) {
    	QString fileToOpen = dialog.getFileName();
	    if(!fileToOpen.isEmpty())
	    {
	      if(!doc->openDocument( fileToOpen )) {
	      	KMessageBox::sorry(0, doc->getErrorString());
	      } else {
		      setCaption(doc->getTitle());
		      addRecentFile(fileToOpen);
		    }
	    }
	  }
  }

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void KColorEditApp::slotFileOpenRecent(int id_)
{
  slotStatusMsg(i18n("Opening file..."));
	
  if(doc->saveModified()) {
    doc->openDocument(recentFiles.at(id_));
    setCaption(doc->getTitle());
  }

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void KColorEditApp::slotFileSave()
{
  slotStatusMsg(i18n("Saving file..."));
	
  if(!doc->saveDocument( doc->getAbsFilePath() ))
		KMessageBox::sorry(0, doc->getErrorString());

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

bool KColorEditApp::slotFileSaveAs()
{
	bool result = true;
  slotStatusMsg(i18n("Saving file with a new filename..."));

	while(result) {
	  QString newName=KFileDialog::getSaveFileName(lastSavePaletteAsFileDir,
	                                               i18n("*|All files"), this, i18n("Save as..."));
	  if(newName.isEmpty())
	  	result = false;
	  else {
	    QFileInfo saveAsInfo(newName);
	    if(!saveAsInfo.exists() ||
	    	KMessageBox::questionYesNo( this,
	  		i18n("A Document with this name already exists.\nDo you want to overwrite it?"),
	  		i18n("Warning") ) == KMessageBox::Yes) {
		    if(!doc->saveDocument( newName )) {
					KMessageBox::sorry(0, doc->getErrorString());
					result = false;
				} else {
			    doc->setTitle(saveAsInfo.fileName());
			    doc->setAbsFilePath(saveAsInfo.absFilePath());
			    addRecentFile(newName);
		
			    setCaption(doc->getTitle());
		
			    lastSavePaletteAsFileDir = saveAsInfo.absFilePath();
			
			    break;
			  }
			}
	  }
	}

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));

  return result;
}

void KColorEditApp::slotFileClose()
{
  slotStatusMsg(i18n("Closing file..."));
	
  close();
  //slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void KColorEditApp::slotFilePrint()
{
  slotStatusMsg(i18n("Printing..."));

  QPrinter printer;
  if (printer.setup(this))
  {
    view->print(&printer);
  }

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void KColorEditApp::slotFileQuit()
{
  slotStatusMsg(i18n("Exiting..."));
  saveOptions();
  // close the first window, the list makes the next one the first again.
  // This ensures that queryClose() is called on each window to ask for closing
  KMainWindow* w;
  if(memberList)
  {
    for(w=memberList->first(); w!=0; w=memberList->first())
    {
      // only close the window if the closeEvent is accepted. If the user presses Cancel on the saveModified() dialog,
      // the window and the application stay open.
      if(!w->close())
	break;
    }
  }	
  //slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void KColorEditApp::slotEditCut()
{
  slotStatusMsg(i18n("Cutting selection..."));
	doc->cut();
  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void KColorEditApp::slotEditCopy()
{
  slotStatusMsg(i18n("Copying selection to clipboard..."));
	doc->copy();
  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void KColorEditApp::slotEditPaste()
{
  slotStatusMsg(i18n("Inserting clipboard contents..."));
	doc->paste();
  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void KColorEditApp::slotViewToolBar()
{
  slotStatusMsg(i18n("Toggle the toolbar..."));
  ///////////////////////////////////////////////////////////////////
  // turn Toolbar on or off
  if( viewMenu->isItemChecked(ID_VIEW_TOOLBAR))
  {
    viewMenu->setItemChecked(ID_VIEW_TOOLBAR, false);
    enableToolBar(KToolBar::Hide);
  }
  else
  {
    viewMenu->setItemChecked(ID_VIEW_TOOLBAR, true);
    enableToolBar(KToolBar::Show);
  }		

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}

void KColorEditApp::slotViewStatusBar()
{
  slotStatusMsg(i18n("Toggle the statusbar..."));
  ///////////////////////////////////////////////////////////////////
  //turn Statusbar on or off
  if( viewMenu->isItemChecked(ID_VIEW_STATUSBAR))
  {
    viewMenu->setItemChecked(ID_VIEW_STATUSBAR, false);
    enableStatusBar(KStatusBar::Hide);
  }
  else
  {
    viewMenu->setItemChecked(ID_VIEW_STATUSBAR, true);
    enableStatusBar(KStatusBar::Show);
  }

  slotStatusMsg(i18n(IDS_STATUS_DEFAULT));
}


void KColorEditApp::slotStatusMsg(const QString &text)
{
  ///////////////////////////////////////////////////////////////////
  // change status message permanently
  statusBar()->clear();
  statusBar()->changeItem(text, ID_STATUS_MSG);
}


void KColorEditApp::slotStatusHelpMsg(const QString &text)
{
  ///////////////////////////////////////////////////////////////////
  // change status message of whole statusbar temporary (text, msec)
  statusBar()->message(text, 2000);
}



void KColorEditApp::commandCallback(int id_)
{
  switch (id_)
  {
    case ID_FILE_NEW_WINDOW:
         slotFileNewWindow();
    	 break;

    case ID_FILE_NEW:
    	 slotFileNew();
         break;

    case ID_FILE_OPEN:
         slotFileOpen();
         break;

    case ID_FILE_SAVE:
         slotFileSave();
         break;

    case ID_FILE_SAVE_AS:
         slotFileSaveAs();
         break;

    case ID_FILE_CLOSE:
         slotFileClose();
         break;

    case ID_FILE_PRINT:
         slotFilePrint();
         break;

    case ID_FILE_QUIT:
         slotFileQuit();
         break;

    case ID_EDIT_CUT:
         slotEditCut();
         break;

    case ID_EDIT_COPY:
         slotEditCopy();
         break;

    case ID_EDIT_PASTE:
         slotEditPaste();
         break;
  
    case ID_VIEW_TOOLBAR:
         slotViewToolBar();
         break;

    case ID_VIEW_STATUSBAR:
         slotViewStatusBar();
         break;

    default:
         break;
  }
}

void KColorEditApp::statusCallback(int id_)
{
  switch (id_)
  {
    case ID_FILE_NEW_WINDOW:
         slotStatusHelpMsg(i18n("Opens a new application window"));
         break;

    case ID_FILE_NEW:
         slotStatusHelpMsg(i18n("Creates a new document"));
         break;

    case ID_FILE_OPEN:
         slotStatusHelpMsg(i18n("Opens an existing document"));
         break;

    case ID_FILE_OPEN_RECENT:
         slotStatusHelpMsg(i18n("Opens a recently used file"));
         break;

    case ID_FILE_SAVE:
         slotStatusHelpMsg(i18n("Saves the actual document"));
         break;

    case ID_FILE_SAVE_AS:
         slotStatusHelpMsg(i18n("Saves the actual document as..."));
         break;

    case ID_FILE_CLOSE:
         slotStatusHelpMsg(i18n("Closes the actual document"));
         break;

    case ID_FILE_PRINT:
         slotStatusHelpMsg(i18n("Prints out the actual document"));
         break;

    case ID_FILE_QUIT:
         slotStatusHelpMsg(i18n("Quits the application"));
         break;

    case ID_EDIT_CUT:
         slotStatusHelpMsg(i18n("Cuts the selected section and puts it to the clipboard"));
         break;

    case ID_EDIT_COPY:
         slotStatusHelpMsg(i18n("Copies the selected section to the clipboard"));
         break;

    case ID_EDIT_PASTE:
         slotStatusHelpMsg(i18n("Pastes the clipboard contents to actual position"));
         break;

    case ID_VIEW_TOOLBAR:
         slotStatusHelpMsg(i18n("Enables/disables the toolbar"));
         break;

    case ID_VIEW_STATUSBAR:
         slotStatusHelpMsg(i18n("Enables/disables the statusbar"));
         break;

    default:
         break;
  }
}
