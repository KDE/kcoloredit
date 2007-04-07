/***************************************************************************
                          paletteview.h  -  description
                             -------------------
    begin                : Sun Jul 9 2000
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

#ifndef PALETTEVIEW_H
#define PALETTEVIEW_H

#include <qframe.h>
#include <qscrollbar.h>

#include "paletteviewscrolledarea.h"

class KColorEditView;

/** This is a Palette class view widget.
  * Its parent is KColorEditView
  * @author Artur Rataj
  */
class PaletteView : public QFrame {
	Q_OBJECT

public:
	/** Constructs a palette view widget, with a default cells sizes and spacing.
	 *  The effective cell sizes may be adjusted to fit the widget sizes.
	 */
	PaletteView(const int defaultCellWidth, const int defaultCellHeight, const int cellSpacing,
			KColorEditView* view, QWidget *parent = 0, const char *name=0);
	~PaletteView();
	/** Calls redraw() in scrolledArea */
	void redraw();
	/** Sets a scroll bar value */
	void setScrollBarValue(const int value);
	/** Sets whether to view color names */
	void slotViewColorNames(bool viewColorNames);

public slots:
	/** Sets if the cursor follows a chosen color */
	void slotCursorFollowsChosenColor(bool follows);
	/** Repaints the scrolled area */
	void slotRepaintScrolledArea();
	
protected:
	/** The scrolled area */
	PaletteViewScrolledArea* scrolledArea;
	/** The scroll bar widget */
	QScrollBar* scrollBar;
	/** The horizontal scroll bar widget */
	QScrollBar* hScrollBar;
};

#endif
