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
	m_palette(), m_paletteHistory(&m_palette, 0) {
	m_pViewList = new QPtrList<KColorEditView>();
	m_pViewList->setAutoDelete(true);
}

KColorEditDoc::~KColorEditDoc()
{
}

void KColorEditDoc::addView(KColorEditView *view)
{
  m_pViewList->append(view);
}

void KColorEditDoc::removeView(KColorEditView *view)
{
  m_pViewList->remove(view);
}

void KColorEditDoc::setModified(bool b) {
	m_modified = b;

    emit modified( b );
}

void KColorEditDoc::setAbsFilePath(const QString &filename)
{
  m_absFilePath=filename;
}

const QString &KColorEditDoc::absFilePath() const
{
  return m_absFilePath;
}

void KColorEditDoc::setTitle(const QString &_t)
{
  m_title=_t;
}

const QString &KColorEditDoc::title() const
{
  return m_title;
}

void KColorEditDoc::slotRedrawAllViews(KColorEditView *sender, bool newDocument) {
 KColorEditView *w;
  if(m_pViewList)
  {
    for(w=m_pViewList->first(); w!=0; w=m_pViewList->next())
    {
      if(w!=sender)
        w->redraw(newDocument);
    }
  }
}

void KColorEditDoc::slotChangeViewMode(bool viewColorNames) {
 KColorEditView *w;
  if(m_pViewList)
  {
    for(w=m_pViewList->first(); w!=0; w=m_pViewList->next())
    {
      w->slotViewColorNames(viewColorNames);
    }
  }
}

bool KColorEditDoc::saveModified()
{
  bool completed=true;

  if(m_modified)
  {
    KColorEditApp *window=(KColorEditApp *) parent();
    int want_save = KMessageBox::warningYesNoCancel(window,
                                         i18n("The current file has been modified.\n"
                                              "Do you want to save it?"), QString::null, KStdGuiItem::save(), i18n("Do Not Save"));
    switch(want_save)
    {
      case KMessageBox::Yes:
           if (title() == i18n("Untitled"))
           {
             completed = window->slotFileSaveAs();
           }
           else
           {
             completed = saveDocument(absFilePath());
       	   };
       	   if(!completed)
							KMessageBox::sorry(0, errorString());
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
  setAbsFilePath( QDir::homeDirPath() );
  setTitle( i18n("Untitled") );
  setPaletteCursorPos(0);
  setPaletteSelection(0, 0);
  slotRedrawAllViews(0, true);
  return true;
}

bool KColorEditDoc::openDocument(const QString& filename) {
	if(filename.isEmpty())
		return newDocument();
	else {
		deleteContents();
		QFileInfo fileInfo(filename);
		setAbsFilePath( fileInfo.absFilePath() );
		if(!m_palette.load( absFilePath() )) {
			setErrorString(m_palette.errorString());
	  		return false;
		}
		setModified(false);
		setTitle( fileInfo.fileName() );
		setPaletteCursorPos(m_palette.length());
		setPaletteSelection(0, 0);
		slotRedrawAllViews(0, true);
        KColorEditApp *window=(KColorEditApp*)parent();
        window->setCaption(m_title);
	}
	return true;
}

bool KColorEditDoc::saveDocument(const QString& filename) {
	if(!m_palette.save( filename )) {
	 	setErrorString(m_palette.errorString());
	 	return false;
	}
  setModified(false);
  return true;
}

void KColorEditDoc::deleteContents() {
	m_palette.deleteContents();
}

void KColorEditDoc::setErrorString(const QString& string) {
	m_errorString = string;
}

const QString& KColorEditDoc::errorString() const {
	return m_errorString;
}

PaletteHistory* KColorEditDoc::paletteHistory() {
	return &m_paletteHistory;
}

void KColorEditDoc::setPaletteCursorPos(const int pos) {
  m_paletteCursorPos = pos;

  emit paletteAvailable( pos < m_palette.length() );
}

int KColorEditDoc::paletteCursorPos() {
	return m_paletteCursorPos;
}

void KColorEditDoc::setPaletteSelection(const int begin, const int end) {
	m_paletteSelectionBegin = begin;
	m_paletteSelectionEnd = end;

    emit selectionChanged( begin, end );
}

int KColorEditDoc::paletteSelectionBegin() const {
	return m_paletteSelectionBegin;
}

int KColorEditDoc::paletteSelectionEnd() const {
	return m_paletteSelectionEnd;
}

void KColorEditDoc::copyToClipboard(Palette& palette) {
	QString text;
	QTextOStream stream(&text);
	palette.save(stream, 0, false);
	KApplication::clipboard()->setText(text);

    emit clipboardChanged();
}

void KColorEditDoc::copy() {
	Palette paletteCopy = m_palette.copy(paletteSelectionBegin(),
		paletteSelectionEnd() - paletteSelectionBegin());
	copyToClipboard(paletteCopy);
}

void KColorEditDoc::cut() {
	Palette paletteCut = m_paletteHistory.cut(paletteSelectionBegin(),
		paletteSelectionEnd() - paletteSelectionBegin());
	copyToClipboard(paletteCut);
	setPaletteCursorPos(paletteSelectionBegin());
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
		m_paletteHistory.paste(paletteCursorPos(), palettePaste);
		setPaletteSelection(paletteCursorPos(), paletteCursorPos() +
			palettePaste.length());
		setModified(true);
		slotRedrawAllViews(0);
	}
}

void KColorEditDoc::insert(int index, const Color& color) {
	Palette paletteInsert;
	Color* insertColor = new Color(color);
	paletteInsert.append(insertColor);
	m_paletteHistory.paste(index, paletteInsert);
    setPaletteCursorPos( index );
	setPaletteSelection(0, 0);
	setModified(true);
	slotRedrawAllViews(0);
}

void KColorEditDoc::replace(int index, const Color& color) {
	Palette paletteReplace;
	Color* replaceColor = new Color(color);
	paletteReplace.append(replaceColor);
	m_paletteHistory.replace(index, paletteReplace);
	setPaletteSelection(0, 0);
	setModified(true);
	slotRedrawAllViews(0);
}
#include "kcoloreditdoc.moc"
