/***************************************************************************
                          kcoloreditdoc.cpp  -  description
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

// include files for Qt
#include <qdir.h>
#include <qfileinfo.h>
#include <qwidget.h>
#include <qclipboard.h>

// include files for KDE
#include <klocale.h>
#include <kmessagebox.h>

// application specific includes
#include "kcoloreditdoc.h"
#include "kcoloredit.h"
#include "kcoloreditview.h"
#include "resource.h"

KColorEditDoc::KColorEditDoc(QWidget *parent, const char *name) : QObject(parent, name),
	palette(), paletteHistory(&palette, 0) {
	pViewList = new QList<KColorEditView>();
	pViewList->setAutoDelete(true);
}

KColorEditDoc::~KColorEditDoc()
{
}

void KColorEditDoc::addView(KColorEditView *view)
{
  pViewList->append(view);
}

void KColorEditDoc::removeView(KColorEditView *view)
{
  pViewList->remove(view);
}

void KColorEditDoc::setModified(bool modified) {
	this->modified = modified;
	KColorEditApp *window=(KColorEditApp*)parent();
	if(this->modified)
		window->enableCommand(ID_FILE_SAVE);
	else
		window->disableCommand(ID_FILE_SAVE);
}

void KColorEditDoc::setAbsFilePath(const QString &filename)
{
  absFilePath=filename;
}

const QString &KColorEditDoc::getAbsFilePath() const
{
  return absFilePath;
}

void KColorEditDoc::setTitle(const QString &_t)
{
  title=_t;
}

const QString &KColorEditDoc::getTitle() const
{
  return title;
}

void KColorEditDoc::slotRedrawAllViews(KColorEditView *sender) {
 KColorEditView *w;
  if(pViewList)
  {
    for(w=pViewList->first(); w!=0; w=pViewList->next())
    {
      if(w!=sender)
        w->redraw();
    }
  }
}

bool KColorEditDoc::saveModified()
{
  bool completed=true;

  if(modified)
  {
    KColorEditApp *window=(KColorEditApp *) parent();
    int want_save = KMessageBox::warningYesNoCancel(window,
                                         i18n("The current file has been modified.\n"
                                              "Do you want to save it?"));
    switch(want_save)
    {
      case KMessageBox::Yes:
           if (title == i18n("Untitled"))
           {
             completed = window->slotFileSaveAs();
           }
           else
           {
             completed = saveDocument(getAbsFilePath());
       	   };
       	   if(!completed)
							KMessageBox::sorry(0, getErrorString());
           break;

      case KMessageBox::No:
           completed=true;
           break;	

      case KMessageBox::Cancel:
           completed=false;
           break;

      default:
           completed=false;
           break;
    }
  }

  return completed;
}

void KColorEditDoc::closeDocument()
{
  deleteContents();
}

bool KColorEditDoc::newDocument()
{
  deleteContents();
	setModified(false);
  absFilePath=QDir::homeDirPath();
  title=i18n("Untitled");
  setPaletteCursorPos(0);
  setPaletteSelection(0, 0);
  slotRedrawAllViews(0);
  return true;
}

bool KColorEditDoc::openDocument(const QString& filename) {
	if(filename.isEmpty())
		return newDocument();
	else {
		deleteContents();
		QFileInfo fileInfo(filename);
		absFilePath=fileInfo.absFilePath();	
		if(!palette.load( absFilePath )) {
			setErrorString(palette.getErrorString());
	  		return false;
		}
		setModified(false);
		title = fileInfo.fileName();
		setPaletteCursorPos(palette.length());
		setPaletteSelection(0, 0);
		slotRedrawAllViews(0);
	}
	return true;
}

bool KColorEditDoc::saveDocument(const QString& filename) {
	if(!palette.save( filename )) {
	 	setErrorString(palette.getErrorString());
	 	return false;
	}
  setModified(false);
  return true;
}

void KColorEditDoc::deleteContents() {
	palette.deleteContents();
}
		
void KColorEditDoc::setErrorString(const QString& string) {
	errorString = string;
}

const QString& KColorEditDoc::getErrorString() const {
	return errorString;
}

PaletteHistory* KColorEditDoc::getPaletteHistory() {
	return &paletteHistory;
}

void KColorEditDoc::setPaletteCursorPos(const int pos) {
	paletteCursorPos = pos;
	KColorEditApp *window=(KColorEditApp*)parent();
	if(paletteCursorPos == palette.length())
		window->disableCommand(ID_COLOR_FROM_PALETTE);
	else
		window->enableCommand(ID_COLOR_FROM_PALETTE);
}

int KColorEditDoc::getPaletteCursorPos() {
	return paletteCursorPos;
}

void KColorEditDoc::setPaletteSelection(const int begin, const int end) {
	paletteSelectionBegin = begin;
	paletteSelectionEnd = end;
	KColorEditApp *window=(KColorEditApp*)parent();
	if(paletteSelectionBegin == paletteSelectionEnd) {
	  window->disableCommand(ID_EDIT_CUT);
	  window->disableCommand(ID_EDIT_COPY);
	} else {
	  window->enableCommand(ID_EDIT_CUT);
	  window->enableCommand(ID_EDIT_COPY);
	}
}

int KColorEditDoc::getPaletteSelectionBegin() {
	return paletteSelectionBegin;
}

int KColorEditDoc::getPaletteSelectionEnd() {
	return paletteSelectionEnd;
}

void KColorEditDoc::copyToClipboard(Palette& palette) const {
	QString text;
	QTextOStream stream(&text);
	palette.save(stream, 0, false);
	KApplication::clipboard()->setText(text);
}

void KColorEditDoc::copy() {
	Palette paletteCopy = palette.copy(getPaletteSelectionBegin(),
		getPaletteSelectionEnd() - getPaletteSelectionBegin());
	copyToClipboard(paletteCopy);
}

void KColorEditDoc::cut() {
	Palette paletteCut = paletteHistory.cut(getPaletteSelectionBegin(),
		getPaletteSelectionEnd() - getPaletteSelectionBegin());
	copyToClipboard(paletteCut);
	setPaletteCursorPos(getPaletteSelectionBegin());
	setPaletteSelection(0, 0);
	setModified(true);
	slotRedrawAllViews(0);
}

void KColorEditDoc::paste() {
	Palette palettePaste;
	QString text;
	QTextIStream stream(&text);
	text = KApplication::clipboard()->text();
	if(palettePaste.load( stream, false )) {
		paletteHistory.paste(getPaletteCursorPos(), palettePaste);
		setPaletteSelection(getPaletteCursorPos(), getPaletteCursorPos() +
			palettePaste.length());
		setModified(true);
		slotRedrawAllViews(0);
	}
}

void KColorEditDoc::insert(int index, const Color& color) {
	Palette paletteInsert;
	Color* insertColor = new Color(color);
	paletteInsert.append(insertColor);
	paletteHistory.paste(index, paletteInsert);
	setPaletteSelection(0, 0);
	setModified(true);
	slotRedrawAllViews(0);
}

void KColorEditDoc::replace(int index, const Color& color) {
	Palette paletteReplace;
	Color* replaceColor = new Color(color);
	paletteReplace.append(replaceColor);
	paletteHistory.replace(index, paletteReplace);
	setPaletteSelection(0, 0);
	setModified(true);
	slotRedrawAllViews(0);
}
#include "kcoloreditdoc.moc"
