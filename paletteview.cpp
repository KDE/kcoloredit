/***************************************************************************
                          paletteview.cpp  -  description
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

#include <stdlib.h>
#include <qcolor.h>
#include <qpalette.h>
#include <kglobal.h>
#include <kcolordrag.h>

#include "kcoloreditview.h"
#include "paletteview.h"

PaletteView::PaletteView(const int defaultCellWidth, const int defaultCellHeight, const int cellSpacing,
	KColorEditView* view, QWidget *parent, const char *name) :
	QScrollView(parent, name, QWidget::WResizeNoErase) {
	this->defaultCellWidth = defaultCellWidth;
	this->defaultCellHeight = defaultCellHeight;
	this->cellSpacing = cellSpacing;
	this->view = view;
	paletteHistory = 0;
	palette = 0;
	setVScrollBarMode(AlwaysOn);
	setHScrollBarMode(AlwaysOff);
	setAcceptDrops(true);
	setDragAutoScroll(true);
	scrollTimeoutTimer = new QTimer(this);
	connect(scrollTimeoutTimer, SIGNAL( timeout() ), SLOT( slotScrollTimeout() ));
	scrollTimeout = true;
	mousePressed = false;
	cursorPositioning = false;
	colorChosen = false;
	slotCursorFollowsChosenColor(false);
}

PaletteView::~PaletteView() {
}

void PaletteView::setPaletteHistory(PaletteHistory* paletteHistory) {
	this->paletteHistory = paletteHistory;
	this->palette = this->paletteHistory->getEditableStream();
	verticalScrollBar()->setValue(0);
	redraw();
}

void PaletteView::slotCursorFollowsChosenColor(bool follows) {
	cursorFollowsChosenColor = follows;
}

void PaletteView::setCellsSizes() {
	rowWidth = visibleWidth();
	cellsInRow = rowWidth/(defaultCellWidth + cellSpacing*2);
	rowHeight = (int)(rowWidth*1.0/cellsInRow/
		( defaultCellWidth + cellSpacing*2 )*( defaultCellHeight + cellSpacing*2 ) + 0.5);
	rowsNum = (palette->length() + cellsInRow - 1)/cellsInRow;
	cellTableHeight = rowsNum*rowHeight;
	int contentsHeight;
	if(palette->length() != 0 &&
		( palette->length()%cellsInRow ) == 0)
		contentsHeight = cellTableHeight + rowHeight;
	else
		contentsHeight = cellTableHeight;
	resizeContents(rowWidth, contentsHeight);
}

void PaletteView::redraw() {
	setCellsSizes();
	repaintPalette();
}

void PaletteView::drawContents(QPainter* painter,
	int clipX, int clipY, int clipWidth, int clipHeight ) {
	setCellsSizes();
	int width = rowWidth;
	int firstRow = clipY/rowHeight;
	int lastRow = (clipY + clipHeight - 1 + rowHeight - 1)/rowHeight;
	QBrush normalBackgroundBrush(viewport()->palette().active().background());
	QBrush selectedBackgroundBrush(viewport()->palette().active().highlight());
	QBrush foregroundBrush;
	QBrush cursorBrush(viewport()->palette().active().buttonText());
	int cellHeight = rowHeight - 2*cellSpacing;
	int selectionMin = getSelectionMin();
	int selectionMax = getSelectionMax();
	int xBegin = 0;
	for(int x = 0; x < cellsInRow; ++x) {
		int xEnd = (x + 1)*(width - 1)/cellsInRow;
		int cellWithSpacingWidth = xEnd - xBegin + 1;
		int cellWidth = cellWithSpacingWidth - 2*cellSpacing;
		for(int y = firstRow; y <= lastRow; ++y) {
			int yBegin = y*rowHeight;
			int currCellNum = y*cellsInRow + x;
			QBrush* backgroundBrush;
			if(currCellNum >= selectionMin &&
				currCellNum < selectionMax)
				backgroundBrush = &selectedBackgroundBrush;
			else
				backgroundBrush = &normalBackgroundBrush;
			if(currCellNum < palette->length()) {
				Color* color = palette->getColor(currCellNum);
				QBrush foregroundBrush(QColor(
					color->getComponent(Color::RED_INDEX),
					color->getComponent(Color::GREEN_INDEX),
					color->getComponent(Color::BLUE_INDEX) ));
				painter->fillRect(xBegin, yBegin, cellWithSpacingWidth, cellSpacing,
					*backgroundBrush);
				painter->fillRect(xBegin, yBegin + rowHeight - cellSpacing, cellWithSpacingWidth, cellSpacing,
					*backgroundBrush);
				QBrush* backgroundOrCursorBrush;
				if(getCursorPos() == currCellNum)
					backgroundOrCursorBrush = &cursorBrush;
				else
					backgroundOrCursorBrush = backgroundBrush;
				painter->fillRect(xBegin, yBegin + cellSpacing, cellSpacing, cellHeight,
					*backgroundOrCursorBrush);
				painter->fillRect(xBegin + cellWithSpacingWidth - cellSpacing, yBegin + cellSpacing,
					cellSpacing, cellHeight,
					*backgroundBrush);
				painter->fillRect(xBegin + cellSpacing, yBegin + cellSpacing, cellWidth, cellHeight,
					foregroundBrush);
			} else {
				if(getCursorPos() == currCellNum) {
					painter->fillRect(xBegin, yBegin + cellSpacing, cellSpacing, cellHeight,
						cursorBrush);
					painter->fillRect(xBegin, yBegin, cellSpacing, cellSpacing,
						*backgroundBrush);
					painter->fillRect(xBegin, yBegin + rowHeight - cellSpacing, cellSpacing, cellSpacing,
						*backgroundBrush);
					painter->fillRect(xBegin + cellSpacing, yBegin, cellWithSpacingWidth - cellSpacing, rowHeight,
						*backgroundBrush);
				} else
					painter->fillRect(xBegin, yBegin, cellWithSpacingWidth, rowHeight,
						*backgroundBrush);
			}
		}
		xBegin = xEnd + 1;
	}
}

void PaletteView::repaintPalette() {
	viewport()->repaint(false);
	getDocument()->slotRedrawAllViews(view);
}

void PaletteView::checkSelectionAutoScroll(const int mousePosY) {
	if(mousePosY < contentsY())
		scrollPalette(-8, 50);
	else if(mousePosY > contentsY() + viewport()->height() - 1)
		scrollPalette(8, 50);
}

void PaletteView::scrollPalette(const int amount, const int timeout) {
	if(scrollTimeout) {
		scrollBy(0, amount);
		scrollTimeout = false;
		scrollTimeoutTimer->start(timeout, true);
	}
}

void PaletteView::slotScrollTimeout() {
	scrollTimeout = true;
	if(mousePressed) {
		QPoint cursorPoint = viewportToContents(viewport()->mapFromGlobal( QCursor::pos() ));
		setCursorPos(cursorPoint.x(), cursorPoint.y());
		selectionEnd = getCursorPos();
		if(selectionEnd >= selectionBegin)
			setSelection(selectionBegin, selectionEnd);
		else
			setSelection(selectionEnd, selectionBegin);
		checkSelectionAutoScroll(cursorPoint.y());
		repaintPalette();
	}
}

void PaletteView::setCursorPos(const int pos) {
	int oldCursorPos = getDocument()->getPaletteCursorPos();
	getDocument()->setPaletteCursorPos(pos);
	if(pos != oldCursorPos)
		view->slotCursorPosChanged(pos);
}

bool PaletteView::setCursorPos(const int x, const int y) {
	int cursorColumn = (x + rowWidth/cellsInRow/2)*cellsInRow/rowWidth;
	bool atCursorLocation = abs(cursorColumn*rowWidth/cellsInRow - x) < cellSpacing + 1;
	if(atCursorLocation || cursorPositioning) {
		int cursorRow;
		if(y >= 0)
			cursorRow = y/rowHeight;
		else
			cursorRow = -1;
		if(cursorColumn > cellsInRow - 1) {
			cursorColumn = 0;
			++cursorRow;
		}
		int tmpCursorPos = cursorRow*cellsInRow + cursorColumn;
		if(tmpCursorPos < 0)
			tmpCursorPos = 0;
		else if(tmpCursorPos >= palette->length()) {
			if(tmpCursorPos == palette->length() &&
				cursorColumn == 0 &&
				getSelectionMin() == getSelectionMax())
				/* if cursor has been set after the last color and the color is
				 * also the last in a row, and there is no selection, scroll the
				 * palette to ensure the cursor is visible
				 */
				scrollBy(0, rowHeight*2);
			if(tmpCursorPos > palette->length())
				tmpCursorPos = palette->length();
		}
		setCursorPos(tmpCursorPos);
	}
	return atCursorLocation;
}

int PaletteView::getCursorPos() {
	return getDocument()->getPaletteCursorPos();
}

void PaletteView::setSelection(const int min, const int max) {
	getDocument()->setPaletteSelection(min, max);
}

int PaletteView::getSelectionMin() {
	return getDocument()->getPaletteSelectionBegin();
}

int PaletteView::getSelectionMax() {
	return getDocument()->getPaletteSelectionEnd();
}

int PaletteView::getColorIndex(const QPoint& point) const {
	int colorColumn = point.x()*cellsInRow/rowWidth;
	int colorRow = point.y()/rowHeight;
	int colorIndex = colorRow*cellsInRow + colorColumn;
	if(colorIndex > palette->length() - 1 ||
		colorIndex < 0)
		colorIndex = -1;
	return colorIndex;
}

QColor PaletteView::getColor(const QPoint& point) const {
	Color* color = palette->getColor(getColorIndex( point ));
	return QColor(color->getComponent( Color::RED_INDEX ),
		color->getComponent( Color::GREEN_INDEX ),
		color->getComponent( Color::BLUE_INDEX ));
}

void PaletteView::contentsMousePressEvent(QMouseEvent* event) {
	cursorPositioning = false;
	if(( cursorPositioning = setCursorPos(event->x(), event->y()) )) {
		selectionBegin = getCursorPos();
		setSelection(selectionBegin, selectionBegin);
		redraw();
		colorChosen = false;
	} else {
		colorDragPoint = event->pos();
		colorChosen = true;
	}
	mousePressed = true;
}

void PaletteView::contentsMouseMoveEvent(QMouseEvent* event) {
	if(cursorPositioning) {
		setCursorPos(event->x(), event->y());
		selectionEnd = getCursorPos();
		if(selectionEnd >= selectionBegin)
			setSelection(selectionBegin, selectionEnd);
		else
			setSelection(selectionEnd, selectionBegin);
		checkSelectionAutoScroll(event->y());
		redraw();
	} else {
		/* check if it is a color drag */
		if(getColorIndex( colorDragPoint ) != -1) {
			if(abs( event->x() - colorDragPoint.x() ) > 2/*KGlobal::dndEventDelay()*/ || // !!!
				abs( event->y() - colorDragPoint.y() ) > 2/*KGlobal::dndEventDelay()*/) { // !!!
				QColor draggedColor = getColor(colorDragPoint);
				KColorDrag* colorDrag = KColorDrag::makeDrag(draggedColor, viewport());
				colorDrag->dragCopy();
			} else
				colorChosen = true;
		}
	}
}

void PaletteView::contentsMouseReleaseEvent(QMouseEvent* event) {
	if(colorChosen) {
		if(getColorIndex( colorDragPoint ) != -1) {
			int index = getColorIndex(colorDragPoint);
			chooseColor(palette->getColor( index ));
			if(cursorFollowsChosenColor) {
				setCursorPos(index);
				redraw();
			}
		}
		colorChosen = false;
	}
	mousePressed = false;
}

void PaletteView::chooseColor(Color* const color) {
	view->chooseColor(color);
}

KColorEditDoc* PaletteView::getDocument() {
	return view->getDocument();
}
