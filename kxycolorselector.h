/***************************************************************************
                          kxycolorselector.h  -  description
                             -------------------
    begin                : Fri Jul 7 2000
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

#ifndef KXYCOLORSELECTOR_H
#define KXYCOLORSELECTOR_H

#include <qwidget.h>
#include <kselect.h>

#include "main.h"

/** A widget for selecting two of color components
  * @author Artur Rataj
  */
class KXYColorSelector : public KXYSelector  {
   Q_OBJECT

public:
	/** The types of the selector. They specify a color component subset */
	enum { TYPE_NONE = 0,
	       TYPE_HS = 1,
	       TYPE_VS = 2,
	       TYPE_HV = 3,
	       TYPE_RG = 4,
	       TYPE_GB = 5,
	       TYPE_BR = 6,
	       TYPE_ = 7,
	/** Component selection resolution */
	       COMPONENT_SELECTION_RESOLUTION = 10000 };

public:
	/** Constructs a two-dimensional color component selector widget,
	 *  with a type TYPE_NONE and ranges 0 .. MAX_COLOR_COMPONENT_VALUE
	 */
	KXYColorSelector(QWidget *parent=0, const char *name=0);
	~KXYColorSelector();
	/** Set the type of the selector */
	void setType(const int type);
	/** Update the pixmap */
  void updateContents();
	/** Set the global component */
  void setGlobalComponent(const int component);
	/** @return The global component */
  int globalComponent() const;

protected:
	/** The number of colors that are used to dither the pixmap
	 *  if number of color planes <= 8. The palette returned by
	 *  getStandardColorsPalette() is of the size.
	 */
	enum { STANDARD_PALETTE_SIZE = 17 };

	/** A type of the selector */
	int type;
	/** The global component value */
	int m_globalComponent;

  /** Draws the contents of the widget on a pixmap,
   *  which is used for buffering.
   */
  virtual void drawPalette( QPixmap *pixmap );
  /** @reimplemented */
  virtual void resizeEvent( QResizeEvent * );
  /** Reimplemented from KXYSelector. This drawing is
   *  buffered in a pixmap here. As real drawing
   *  routine, drawPalette() is used.
   */
  virtual void drawContents( QPainter *painter );
  /** Draws the cursor */
  virtual void  drawCursor(QPainter* painter, int x, int y);
  /** set a color at a given coordinate */
	virtual void setColor(QColor* const color, const int x, const int y);
	/** @return STANDARD_PALETTE_SIZE colors used to dither the
	 *  pixmap if number of color planes <= 8
	 */
	QColor* standardColorsPalette();

private:
	/* The buffering pixmap */
  QPixmap pixmap;
};

#endif
