/***************************************************************************
                          texteditselection.h  -  description
                             -------------------
    begin                : Wed Jul 12 2000
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

#ifndef TEXTEDITSELECTION_H
#define TEXTEDITSELECTION_H

#include <qwidget.h>
#include <qlineedit.h>
#include <qlayout.h>

#include "color.h"

/** Color selection widget using line edit. Either HSV or RGB components
  * can be set.
  * @author Artur Rataj
  */
class TextEditSelection : public QWidget  {
   Q_OBJECT

public:
	/** Constructs the widget */
	TextEditSelection(QWidget *parent=0, const char *name=0);
	~TextEditSelection();
	
signals:
	/** A signal that a color value has changed by edition */
	void valueChanged(Color*);

public slots:
	/** sets a color */
	void slotSetValue(Color* color);
	
protected:
	/** Adds a component line edit */
	void addComponent(const int index, QLineEdit* lineEdit, const int maxValue, const QString& labelString,
		const int row, const int column, QGridLayout* layout);
	/** sets RGB string in rgbStringLineEdit */
	void setRgbString(const int red, const int green, const int blue);
		
protected slots:
	/** Called if one of HSV components has changed. In that case, RGB components are
	 *  also changed to match the HSV ones
	 */
	void slotHsvComponentChanged();
	/** Called if one of RGB components has changed. In that case, HSV components are
	 *  also changed to match the RGB ones
	 */
	void slotRgbComponentChanged();
	/** Called if the RGB string has changed. In that case, RGB and HSV components are
	 *  also changed to match the RGB string
	 */
	void slotRgbStringChanged();

protected:
	/** Indexes of components */
	enum { H_INDEX = 0,
	       S_INDEX = 1,
	       V_INDEX = 2,
	       R_INDEX = 3,
	       G_INDEX = 4,
	       B_INDEX = 5,
	/** A total number of components */
	       COMPONENTS_NUM = 6 };
	
	/** Line edit widgets table */
	QLineEdit* lineEditTable[COMPONENTS_NUM];
	/** RGB hex string line edit widgets table */
	QLineEdit* rgbStringLineEdit;
	/** The selected color */
	Color color;	
	/** A flag that components are matched */
	bool inChangingComponents;
};

#endif
