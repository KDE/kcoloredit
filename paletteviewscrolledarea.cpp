/***************************************************************************
                         paletteviewscrolledarea.cpp
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

#include <stdlib.h>
#include <qlist.h>
#include <qcolor.h>
#include <qbrush.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpen.h>
#include <qfontmetrics.h> 
#include <qtimer.h>
#include <kglobal.h>
#include <kcolordrag.h>

#include "palette.h"
#include "palettehistory.h"
#include "kcoloreditdoc.h"
#include "kcoloreditview.h"
#include "paletteviewscrolledarea.h"

PaletteViewScrolledArea::PaletteViewScrolledArea(const int defaultCellWidth,
	const int defaultCellHeight, const int cellSpacing, QScrollBar* scrollBar,
	KColorEditView* view, QWidget* parent, const char* name)
	: QFrame(parent, name) {
	this->defaultCellWidth = defaultCellWidth;
	this->defaultCellHeight = defaultCellHeight;
	this->cellSpacing = cellSpacing;
	this->scrollBar = scrollBar;
	this->view = view;
	setBackgroundMode(NoBackground);
	scrollTimeoutTimer = new QTimer(this);
	connect(scrollTimeoutTimer, SIGNAL( timeout() ), SLOT( slotScrollTimeout() ));
	scrollTimeout = true;
	mousePressed = false;
	cursorPositioning = false;
	colorChosen = false;
	setCellsSizes();
	slotCursorFollowsChosenColor(false);
	this->scrollBar->setValue(0);
	slotViewColorNames(false);
}

PaletteViewScrolledArea::~PaletteViewScrolledArea() {
}

void PaletteViewScrolledArea::slotCursorFollowsChosenColor(bool follows) {
	cursorFollowsChosenColor = follows;
}

void PaletteViewScrolledArea::slotViewColorNames(bool viewColorNames) {
	this->viewColorNames = viewColorNames;
}

void PaletteViewScrolledArea::redraw() {
	//setCellsSizes();
	repaintPalette();
}

void PaletteViewScrolledArea::repaintPalette() {
	repaint(false);
}

void PaletteViewScrolledArea::checkSelectionAutoScroll(const int mousePosY) {
	if(mousePosY < 0)
		scrollPalette(-8, 50);
	else if(mousePosY > height() - 1)
		scrollPalette(8, 50);
}

void PaletteViewScrolledArea::scrollPalette(const int amount, const int timeout) {
	if(scrollTimeout) {
		scrollBy(amount);
		scrollTimeout = false;
		scrollTimeoutTimer->start(timeout, true);
	}
}

void PaletteViewScrolledArea::slotScrollTimeout() {
	scrollTimeout = true;
	if(mousePressed) {
		QPoint cursorPoint = mapFromGlobal(QCursor::pos());
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

void PaletteViewScrolledArea::setCursorPos(const int pos) {
	int oldCursorPos = getDocument()->getPaletteCursorPos();
	getDocument()->setPaletteCursorPos(pos);
	if(pos != oldCursorPos)
		view->slotCursorPosChanged(pos);
}

bool PaletteViewScrolledArea::setCursorPos(const int x, const int y) {
	int shiftedY = y + scrollBar->value();
	int cursorColumn = (x + rowWidth/cellsInRow/2)*cellsInRow/rowWidth;
	int cursorRow;
	if(shiftedY >= 0)
		cursorRow = (shiftedY + cellSpacing)/rowHeight;
	else
		cursorRow = -1;
	bool atCursorLocation = abs(cursorColumn*rowWidth/cellsInRow - x) < cellSpacing + 1;
	if(!atCursorLocation) {
		atCursorLocation = abs(cursorRow*rowHeight - shiftedY) < cellSpacing + 1;
		if(atCursorLocation)
			cursorColumn = 0;
	}
	if(atCursorLocation || cursorPositioning) {
		if(cursorColumn > cellsInRow - 1) {
			cursorColumn = 0;
			++cursorRow;
		}
		int tmpCursorPos = cursorRow*cellsInRow + cursorColumn;
		if(tmpCursorPos < 0)
			tmpCursorPos = 0;
		else if(tmpCursorPos >= getPalette()->length()) {
			if(tmpCursorPos == getPalette()->length() &&
				cursorColumn == 0 &&
				getSelectionMin() == getSelectionMax())
				/* if cursor has been set after the last color and the color is
				 * also the last in a row, and there is no selection, scroll the
				 * palette to ensure the cursor is visible
				 */
				scrollBy(rowHeight*2);
			if(tmpCursorPos > getPalette()->length())
				tmpCursorPos = getPalette()->length();
		}
		setCursorPos(tmpCursorPos);
	}
	return atCursorLocation;
}

void PaletteViewScrolledArea::setSelection(const int min, const int max) {
	getDocument()->setPaletteSelection(min, max);
}

void PaletteViewScrolledArea::setCellsSizes() {
	rowWidth = width();
	if(viewColorNames)
		cellsInRow = 1;
	else
		cellsInRow = rowWidth/(defaultCellWidth + cellSpacing*2);
	if(viewColorNames)
		rowHeight = defaultCellHeight*2 + cellSpacing*2;
	else
		rowHeight = (int)(rowWidth*1.0/cellsInRow/
			( defaultCellWidth + cellSpacing*2 )*( defaultCellHeight + cellSpacing*2 ) + 0.5);
	rowsNum = (getPalette()->length() + cellsInRow - 1)/cellsInRow;
	cellTableHeight = rowsNum*rowHeight;
	int contentsHeight;
	if(getPalette()->length() != 0 &&
		( getPalette()->length()%cellsInRow ) == 0)
		contentsHeight = cellTableHeight + rowHeight;
	else
		contentsHeight = cellTableHeight;
	int scrollBarRange = contentsHeight - 1 - height();
	if(scrollBarRange < 0)
		scrollBarRange = 0;
	scrollBar->setRange(0, scrollBarRange);
	scrollBar->setSteps(rowHeight, height());
}

Palette* PaletteViewScrolledArea::getPalette() const {
	return getDocument()->getPaletteHistory()->getEditableStream();
}

int PaletteViewScrolledArea::getCursorPos() {
	return getDocument()->getPaletteCursorPos();
}

int PaletteViewScrolledArea::getSelectionMin() {
	return getDocument()->getPaletteSelectionBegin();
}

int PaletteViewScrolledArea::getSelectionMax() {
	return getDocument()->getPaletteSelectionEnd();
}

void PaletteViewScrolledArea::paintEvent(QPaintEvent* event) {
	setCellsSizes();
	int width = rowWidth;
	int posY = scrollBar->value();
	int firstRow = posY/rowHeight;
	int lastRow = (posY + height() - 1 + rowHeight - 1)/rowHeight;
	QPixmap pixmap(size());
	QPainter painter;
	painter.begin(&pixmap, this);
	if(viewColorNames)
		painter.fillRect(0, 0, rowWidth, height(), QBrush( palette().active().base() ));
	QBrush normalBackgroundBrush(palette().active().background());
	QBrush selectedBackgroundBrush(palette().active().highlight());
	QBrush foregroundBrush;
	QBrush cursorBrush(palette().active().foreground());
	QPen backgroundPen(palette().active().foreground());
	int cellHeight = rowHeight - 2*cellSpacing;
	int selectionMin = getSelectionMin();
	int selectionMax = getSelectionMax();
	int fontAscent = painter.fontMetrics().ascent();
	int xBegin = 0;
	for(int x = 0; x < cellsInRow; ++x) {
		int xEnd;
		if(viewColorNames)
			xEnd = defaultCellWidth*2 + cellSpacing*2;
		else
			xEnd = (x + 1)*(width - 1)/cellsInRow;
		int cellWithSpacingWidth = xEnd - xBegin + 1;
		int cellWidth = cellWithSpacingWidth - 2*cellSpacing;
		for(int y = firstRow; y <= lastRow; ++y) {
			int yBegin = y*rowHeight - posY;
			int currCellNum = y*cellsInRow + x;
			QBrush* backgroundBrush;
			if(currCellNum >= selectionMin &&
				currCellNum < selectionMax)
				backgroundBrush = &selectedBackgroundBrush;
			else
				backgroundBrush = &normalBackgroundBrush;
			if(currCellNum < getPalette()->length()) {
				Color* color = getPalette()->getColor(currCellNum);
				QBrush foregroundBrush(QColor(
					color->getComponent(Color::RED_INDEX),
					color->getComponent(Color::GREEN_INDEX),
					color->getComponent(Color::BLUE_INDEX) ));
				painter.fillRect(xBegin, yBegin, cellWithSpacingWidth, cellSpacing,
					*backgroundBrush);
				painter.fillRect(xBegin, yBegin + rowHeight - cellSpacing, cellWithSpacingWidth, cellSpacing,
					*backgroundBrush);
				QBrush* backgroundOrCursorBrush;
				if(getCursorPos() == currCellNum)
					backgroundOrCursorBrush = &cursorBrush;
				else
					backgroundOrCursorBrush = backgroundBrush;
				painter.fillRect(xBegin, yBegin + cellSpacing, cellSpacing, cellHeight,
					*backgroundOrCursorBrush);
				painter.fillRect(xBegin + cellWithSpacingWidth - cellSpacing, yBegin + cellSpacing,
					cellSpacing, cellHeight,
					*backgroundBrush);
				painter.fillRect(xBegin + cellSpacing, yBegin + cellSpacing, cellWidth, cellHeight,
					foregroundBrush);
				if(viewColorNames) {
					painter.setPen(backgroundPen);
					painter.drawText(xBegin + cellWithSpacingWidth + cellSpacing*4,
						yBegin + rowHeight/2 + fontAscent/2, color->getName());
				}
			} else {
				if(getCursorPos() == currCellNum) {
					painter.fillRect(xBegin, yBegin + cellSpacing, cellSpacing, cellHeight,
						cursorBrush);
					painter.fillRect(xBegin, yBegin, cellSpacing, cellSpacing,
						*backgroundBrush);
					painter.fillRect(xBegin, yBegin + rowHeight - cellSpacing, cellSpacing, cellSpacing,
						*backgroundBrush);
					painter.fillRect(xBegin + cellSpacing, yBegin, cellWithSpacingWidth - cellSpacing, rowHeight,
						*backgroundBrush);
				} else
					painter.fillRect(xBegin, yBegin, cellWithSpacingWidth, rowHeight,
						*backgroundBrush);
			}
		}
		xBegin = xEnd + 1;
	}
	painter.end();
	painter.begin(this);
	painter.drawPixmap(0, 0, pixmap);
	painter.end();
}

int PaletteViewScrolledArea::getColorIndex(const QPoint& point) const {
	int colorColumn = point.x()*cellsInRow/rowWidth;
	int colorRow = (point.y() + scrollBar->value())/rowHeight;
	int colorIndex = colorRow*cellsInRow + colorColumn;
	if(colorIndex > getPalette()->length() - 1 ||
		colorIndex < 0)
		colorIndex = -1;
	return colorIndex;
}

QColor PaletteViewScrolledArea::getColor(const QPoint& point) const {
	Color* color = getPalette()->getColor(getColorIndex( point ));
	return QColor(color->getComponent( Color::RED_INDEX ),
		color->getComponent( Color::GREEN_INDEX ),
		color->getComponent( Color::BLUE_INDEX ));
}

void PaletteViewScrolledArea::mousePressEvent(QMouseEvent* event) {
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

void PaletteViewScrolledArea::mouseMoveEvent(QMouseEvent* event) {
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
			if(abs( event->x() - colorDragPoint.x() ) > 2 ||
				abs( event->y() - colorDragPoint.y() ) > 2) {
				QColor draggedColor = getColor(colorDragPoint);
				KColorDrag* colorDrag = KColorDrag::makeDrag(draggedColor, this);
				colorDrag->dragCopy();
			} else
				colorChosen = true;
		}
	}
}

void PaletteViewScrolledArea::mouseReleaseEvent(QMouseEvent* event) {
	if(colorChosen) {
		if(getColorIndex( colorDragPoint ) != -1) {
			int index = getColorIndex(colorDragPoint);
			chooseColor(getPalette()->getColor( index ));
			if(cursorFollowsChosenColor) {
				setCursorPos(index);
				setSelection(getCursorPos(), getCursorPos());
				redraw();
			}
		}
		colorChosen = false;
	}
	mousePressed = false;
}

void PaletteViewScrolledArea::chooseColor(Color* const color) {
	view->chooseColor(color);
}

KColorEditDoc* PaletteViewScrolledArea::getDocument() const {
	return view->getDocument();
}

void PaletteViewScrolledArea::scrollBy(const int y) {
	scrollBar->setValue(scrollBar->value() + y);
}
