/***************************************************************************
                          kzcolorselector.h  -  description
                             -------------------
    begin                : Fri Jul 14 2000
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

#ifndef KZCOLORSELECTOR_H
#define KZCOLORSELECTOR_H

#include "qcolor.h"
#include "qpixmap.h"
#include "kselect.h"

#include "color.h"

/** A widget for selecting a color component
 * @author Artur Rataj
 */
class KZColorSelector : public KSelector {
   Q_OBJECT
	
public:
	/** Selector type constants */
	enum { TYPE_H = 0,
	       TYPE_S = 1,
	       TYPE_V = 2 };

	/** Constructs the widget */
	KZColorSelector(Orientation o, QWidget *parent=0, const char *name=0);
	~KZColorSelector();
	/** Sets the selector type */
	void setType(const int type);
	/** Update the pixmap */
	void updateContents();
	/** Set the global components */
	void setBaseColor(const QColor& color);
	/** Set the global components using HSV components */
	void setBaseColorHsv(const int colorH, const int colorS, const int colorV);
	/** Updates a pointer position due to the base color */
	void updatePointerPos();
	
protected:
	/** The number of colors that are used to dither the pixmap
	 *  if number of color planes <= 8. The palette returned by
	 *  getStandardColorsPalette() is of the size.
	 */
	enum { STANDARD_PALETTE_SIZE = 17 };
	
	/** A type of the selector */
	int type;
	/** A base color H component */
	int baseColorH;
	/** A base color S component */
	int baseColorS;
	/** A base color V component */
	int baseColorV;
	
  /** Draws the contents of the widget on a pixmap,
   *  which is used for buffering.
   */
  virtual void drawPalette( QPixmap *pixmap );
  /** @reimplemented */
  virtual void resizeEvent( QResizeEvent * );
	/** Draws a color gradient in the selector */
  virtual void drawContents( QPainter *painter );
  /** Sets a color at a given coordinate */
	virtual void setColor(QColor* const color, const int y);
	/** @return STANDARD_PALETTE_SIZE colors used to dither the
	 *  pixmap if number of color planes <= 8
	 */
	QColor* getStandardColorsPalette();

private:
	/* The buffering pixmap */
  QPixmap pixmap;
};

#endif
