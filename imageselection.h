/***************************************************************************
                          imageselection.h  -  description
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

#ifndef IMAGESELECTION_H
#define IMAGESELECTION_H

#include <qwidget.h>

#include "color.h"

/** This is a widget for selecting colors from an image
  * @author Artur Rataj
  */
class ImageSelection : public QWidget  {
   Q_OBJECT

public:
	/** constructs the widget */
	ImageSelection(QWidget *parent=0, const char *name=0);
	~ImageSelection();
	
signals:
	/** A signal that a color value has changed by edition */
	void valueChanged(Color*);

public slots:
	/** Sets a color */
	void slotSetValue(Color* color);

protected:
	/** The selected color */
	Color color;
};

#endif
