/***************************************************************************
                          paletteviewscrolledarea.h
                             -------------------
    begin                : Sun Jul 17 2000
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

#ifndef PALETTEVIEWSCROLLEDAREA_H
#define PALETTEVIEWSCROLLEDAREA_H

#include <qframe.h>
#include <qscrollbar.h>
#include <qevent.h>

#include "palette.h"

class KColorEditDoc;
class KColorEditView;

/** This widget draws the palette view scrolled area.
 *  It has paste and cut features.
 *  @author Artur Rataj
 */
class PaletteViewScrolledArea : public QFrame {
	Q_OBJECT

public:
	/** Constructs the widget */
	PaletteViewScrolledArea(const int defaultCellWidth, const int defaultCellHeight,
		const int cellSpacing, QScrollBar* scrollBar, KColorEditView* view,
		QWidget* parent = 0, const char* name = 0);
	~PaletteViewScrolledArea();
	/** Sets cells sizes and then calls repaintPalette() */
	void redraw();

public slots:
	/** Sets if the cursor follows a chosen color */
	void slotCursorFollowsChosenColor(bool follows);
	
protected:
	/** The scrollbar widget */
	QScrollBar* scrollBar;
	/** Pointer to the document view */
	KColorEditView* view;
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
	
	/** @return The viewed palette */
	Palette* getPalette() const;
	/** Computes the size of cell, number of cells in a row and
	 *  height of cells table, depending on colors number and
	 *  visible area width
	 */
	void setCellsSizes();
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
	/** The widget painting */
	void paintEvent(QPaintEvent* event);
	/** Used to get mouse press events coordinates */
	void mousePressEvent(QMouseEvent* event);
	/** Used to get mouse move events coordinates */
	void mouseMoveEvent(QMouseEvent* event);
	/** Used to get mouse release events coordinates */
	void mouseReleaseEvent(QMouseEvent* event);
	/** If there is a scroll timeout, scrolls palette,
	 *  sets scrollTimeout to false and resets scroll timeout timer
	 */
	void scrollPalette(const int amount, const int timeout);
	/** @return The viewed document */
	KColorEditDoc* getDocument() const;
	/** Chooses a color to the color selector */
	void chooseColor(Color* const color);
	/** scrolls the scrolled area */
	void scrollBy(const int y);
	
protected slots:
	/** called if there is a scroll timeout, sets scrollTimeout to true */
	void slotScrollTimeout();
};

#endif /* !defined( PALETTEVIEWSCROLLEDAREA_H ) */
