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

#include <qscrollview.h>
#include <qlist.h>
#include <qevent.h>
#include <qtimer.h>

#include "kcoloreditdoc.h"
#include "palette.h"
#include "palettehistory.h"

class KColorEditView;

/** This is a Palette class view widget. It has paste and cut features.
  * Its parent is KColorEditView
  * @author Artur Rataj
  */
class PaletteView : public QScrollView {
	Q_OBJECT

public:
	/** Constructs a palette view widget, with a default cells sizes and spacing.
	 *  The effective cell sizes may be adjusted to fit the widget sizes.
	 *  After constructing, this widget should have the palette set by calling the setPalette slot
	 */
	PaletteView(const int defaultCellWidth, const int defaultCellHeight, const int cellSpacing,
			KColorEditView* view, QWidget *parent = 0, const char *name=0);
	~PaletteView();
	/** Sets cells sizes and then calls repaintPalette() */
	void redraw();
	
public slots:
	/** Sets the viewed palette. This should be done after constructing the widget */
	void setPaletteHistory(PaletteHistory* paletteHistory);
	/** Sets if the cursor follows a chosen color */
	void slotCursorFollowsChosenColor(bool follows);
	
private:
	/** Pointer to the document view */
	KColorEditView* view;
	/** The viewed palette history */
	PaletteHistory* paletteHistory;
	/** The viewed palette */
	Palette* palette;
	/** Default cell width */
	int defaultCellWidth;
	/** Default cell height */
	int defaultCellHeight;
	/** Cell spacing */
	int cellSpacing;
	/** The number of cells in a row */
	int cellsInRow;
	/** The number of rows */
	int rowsNum;
	/** A row width */
	int rowWidth;
	/** A row height */
	int rowHeight;
	/** Total height of cells table */
	int cellTableHeight;
	/** a position at which selection started */
	int selectionBegin;
	/** a position at which selection ended */
	int selectionEnd;
	/** auto scroll timeout timer */
	QTimer* scrollTimeoutTimer;
	/** whether there is auto scroll timeout */
	bool scrollTimeout;
	/** whether a mouse has been pressed over the palette and not yet released */
	bool mousePressed;
	/** whether the cursor is being positioned */
	bool cursorPositioning;
	/** a color drag point */
	QPoint colorDragPoint;
	/** whether a color has been chosen and it is not dragged */
	bool colorChosen;
	/** Whether the cursor follows a chosen color */
	bool cursorFollowsChosenColor;
	
protected:
  /** @reimplemented */
	void drawContents(QPainter* painter,
		int clipX, int clipY, int clipWidth, int clipHeight);
	/** repaints the palette */
	void repaintPalette();
	/** @return A color index at a given position, -1 if none */
	int getColorIndex(const QPoint& point) const;
	/** @return A color at a given position */
	QColor getColor(const QPoint& point) const;
	/** sets a palette cursor position */
	void setCursorPos(const int pos);
	/** Sets a cursor position due to a mouse position.
	 *  If not in cursor positioning, the cursor is set
	 *  only if mouse points to a possible cursor location
	 *  @return If the mouse points to a cursor location
	 */
	bool setCursorPos(const int x, const int y);
	/** @return a palette cursor position */
	int getCursorPos();
	/** sets a palette selection */
	void setSelection(const int min, const int max);
	/** @return a palette selection lesser position, or equal position
	 *  if no colors are selected
	 */
	int getSelectionMin();
	/** @return a palette selection greater position, or equal position
	 *  if no colors are selected
	 */
	int getSelectionMax();
	/**  checks whether to scrolls the palette if a mouse position
	 *   is outside it
	 */
	void checkSelectionAutoScroll(const int mousePosY);
	/** Used to get mouse press events coordinates */
	void contentsMousePressEvent(QMouseEvent* event);
	/** Used to get mouse move events coordinates */
	void contentsMouseMoveEvent(QMouseEvent* event);
	/** Used to get mouse release events coordinates */
	void contentsMouseReleaseEvent(QMouseEvent* event);
	/** if there is a scroll timeout, scrolls palette,
	 *  sets scrollTimeout to false and resets scroll timeout timer
	 */
	void scrollPalette(const int amount, const int timeout);
	/** returns the viewed document */
	KColorEditDoc* getDocument();
	/** chooses a color to the color selector */
	void chooseColor(Color* const color);
	/** Computes the size of cell, number of cells in a row and
	 *  height of cells table, depending on colors number and
	 *  visible area width
	 */
	void setCellsSizes();
	
protected slots:
	/** called if there is a scroll timeout, sets scrollTimeout to true */
	void slotScrollTimeout();
};

#endif
