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
#include <qptrlist.h>
#include <qcolor.h>
#include <qcursor.h>
#include <qbrush.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpen.h>
#include <qfontmetrics.h>
#include <qtimer.h>
#include <kglobal.h>
#include <kcolordrag.h>
#include <qscrollbar.h>

#include "palette.h"
#include "palettehistory.h"
#include "kcoloreditdoc.h"
#include "kcoloreditview.h"
#include "paletteviewscrolledarea.h"
#include "paletteviewscrolledarea.moc"

PaletteViewScrolledArea::PaletteViewScrolledArea(const int defaultCellWidth,
	const int defaultCellHeight, const int cellSpacing, QScrollBar* scrollBar,
	QScrollBar* hScrollBar, KColorEditView* view, QWidget* parent, const char* name)
	: QFrame(parent, name) {
	this->defaultCellWidth = defaultCellWidth;
	this->defaultCellHeight = defaultCellHeight;
	this->cellSpacing = cellSpacing;
	this->scrollBar = scrollBar;
	this->hScrollBar = hScrollBar;
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
	setMinimumSize(1, 1);
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
		selectionEnd = cursorPos();
		if(selectionEnd >= selectionBegin)
			setSelection(selectionBegin, selectionEnd);
		else
			setSelection(selectionEnd, selectionBegin);
		checkSelectionAutoScroll(cursorPoint.y());
		repaintPalette();
	}
}

void PaletteViewScrolledArea::setCursorPos(const int pos) {
	int oldCursorPos = document()->paletteCursorPos();
	document()->setPaletteCursorPos(pos);
	if(pos != oldCursorPos)
		view->slotCursorPosChanged(pos);
}

bool PaletteViewScrolledArea::setCursorPos(const int x, const int y) {
	int shiftedY = y + scrollBar->value();
	int shiftedX = x + hScrollBar->value();
	int cursorColumn = (shiftedX + rowWidth/cellsInRow/2)*cellsInRow/rowWidth;
	int cursorRow;
	if(shiftedY >= 0)
		cursorRow = (shiftedY + cellSpacing)/rowHeight;
	else
		cursorRow = -1;
	bool atCursorLocation = abs(cursorColumn*rowWidth/cellsInRow - shiftedX) < cellSpacing + 1;
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
		else if(tmpCursorPos >= palette()->length()) {
			if(tmpCursorPos == palette()->length() &&
				cursorColumn == 0 &&
				selectionMin() == selectionMax())
				/* if cursor has been set after the last color and the color is
				 * also the last in a row, and there is no selection, scroll the
				 * palette to ensure the cursor is visible
				 */
				scrollBy(rowHeight*2);
			if(tmpCursorPos > palette()->length())
				tmpCursorPos = palette()->length();
		}
		setCursorPos(tmpCursorPos);
	}
	return atCursorLocation;
}

void PaletteViewScrolledArea::setSelection(const int min, const int max) {
	document()->setPaletteSelection(min, max);
}

void PaletteViewScrolledArea::setCellsSizes() {
	rowWidth = width();
	cellWidth = defaultCellWidth;
	cellHeight = defaultCellHeight;
	if(viewColorNames) {
		QPainter painter;
		painter.begin(this);
		if(cellHeight < painter.fontMetrics().height()) {
			int prevCellHeight = cellHeight;
			cellHeight = painter.fontMetrics().height();
			cellWidth = cellHeight*cellWidth/prevCellHeight;
		}
		painter.end();
	}
	if(viewColorNames)
		cellsInRow = 1;
	else
		cellsInRow = rowWidth/(cellWidth + cellSpacing*2);
	if(viewColorNames)
		rowHeight = cellHeight + cellSpacing*2;
	else
		rowHeight = (int)(rowWidth*1.0/cellsInRow/
			( cellWidth + cellSpacing*2 )*( cellHeight + cellSpacing*2 ) + 0.5);
	cellHeight = rowHeight - cellSpacing*2;
	rowsNum = (palette()->length() + cellsInRow - 1)/cellsInRow;
	cellTableHeight = rowsNum*rowHeight;
	int contentsHeight;
	if(palette()->length() != 0 &&
		( palette()->length()%cellsInRow ) == 0)
		contentsHeight = cellTableHeight + rowHeight;
	else
		contentsHeight = cellTableHeight;
	int scrollBarRange = contentsHeight - 1 - height();
	if(scrollBarRange < 0)
		scrollBarRange = 0;
	scrollBar->setRange(0, scrollBarRange);
	scrollBar->setSteps(rowHeight, height());
}

Palette* PaletteViewScrolledArea::palette() const {
	return document()->paletteHistory()->editableStream();
}

int PaletteViewScrolledArea::cursorPos() const {
	return document()->paletteCursorPos();
}

int PaletteViewScrolledArea::selectionMin() const{
	return document()->paletteSelectionBegin();
}

int PaletteViewScrolledArea::selectionMax() const {
	return document()->paletteSelectionEnd();
}

void PaletteViewScrolledArea::paintEvent(QPaintEvent* /*event*/) {
	setCellsSizes();
	QPixmap pixmap(size());
	QPainter painter;
	painter.begin(&pixmap, this);
	QFontMetrics fontMetrics = painter.fontMetrics();
	int maxLineWidth;
	if(viewColorNames) {
		int maxTextLength = 0;
		for(int index = 0; index < palette()->length(); ++index) {
			int currTextLength = fontMetrics.width(
				palette()->color(index)->name());
			if(currTextLength > maxTextLength)
				maxTextLength = currTextLength;
		}
		maxLineWidth = cellWidth + cellSpacing*2 +
			cellSpacing*3 + maxTextLength + 1;
	} else
		maxLineWidth = rowWidth;
	int width = rowWidth;
	if(maxLineWidth > width) {
		hScrollBar->setRange(0, maxLineWidth - width);
		hScrollBar->setSteps(8, width);
		hScrollBar->show();
	} else {
		hScrollBar->setValue(0);
		hScrollBar->hide();
	}
	int posY = scrollBar->value();
	int firstRow = posY/rowHeight;
	int lastRow = (posY + height() - 1 + rowHeight - 1)/rowHeight;
	if(viewColorNames)
		painter.fillRect(0, 0, rowWidth, height(),
        QBrush( QFrame::palette().active().base() ));
	QBrush normalBackgroundBrush(QFrame::palette().active().background());
	QBrush selectedBackgroundBrush(QFrame::palette().active().highlight());
	QBrush foregroundBrush;
	QBrush cursorBrush(QFrame::palette().active().foreground());
	QPen backgroundPen(QFrame::palette().active().foreground());
	int min = selectionMin();
	int max = selectionMax();
	int fontAscent = fontMetrics.ascent();
	int xBegin = -hScrollBar->value();
	for(int x = 0; x < cellsInRow; ++x) {
		int xEnd = -hScrollBar->value();
		if(viewColorNames)
			xEnd += cellWidth + cellSpacing*2;
		else
			xEnd += (x + 1)*(width - 1)/cellsInRow;
		int cellWithSpacingWidth = xEnd - xBegin + 1;
		int cellWidth = cellWithSpacingWidth - 2*cellSpacing;
		for(int y = firstRow; y <= lastRow; ++y) {
			int yBegin = y*rowHeight - posY;
			int currCellNum = y*cellsInRow + x;
			QBrush* backgroundBrush;
			if(currCellNum >= min && currCellNum < max)
				backgroundBrush = &selectedBackgroundBrush;
			else
				backgroundBrush = &normalBackgroundBrush;
			if(currCellNum < palette()->length()) {
				Color* color = palette()->color(currCellNum);
				QBrush foregroundBrush(QColor(
					color->component(Color::RED_INDEX),
					color->component(Color::GREEN_INDEX),
					color->component(Color::BLUE_INDEX) ));
				painter.fillRect(xBegin, yBegin, cellWithSpacingWidth, cellSpacing,
					*backgroundBrush);
				painter.fillRect(xBegin, yBegin + rowHeight - cellSpacing, cellWithSpacingWidth, cellSpacing,
					*backgroundBrush);
				QBrush* backgroundOrCursorBrush;
				if(cursorPos() == currCellNum)
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
					painter.drawText(xBegin + cellWithSpacingWidth + cellSpacing*3,
						yBegin + rowHeight/2 + fontAscent/2, color->name());
				}
			} else {
				if(cursorPos() == currCellNum) {
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

int PaletteViewScrolledArea::colorIndex(const QPoint& point) const {
	int colorColumn = point.x()*cellsInRow/rowWidth;
	int colorRow = (point.y() + scrollBar->value())/rowHeight;
	int colorIndex = colorRow*cellsInRow + colorColumn;
	if(colorIndex > palette()->length() - 1 ||
		colorIndex < 0)
		colorIndex = -1;
	return colorIndex;
}

QColor PaletteViewScrolledArea::color(const QPoint& point) const {
	Color* color = palette()->color(colorIndex( point ));
	return QColor(color->component( Color::RED_INDEX ),
		color->component( Color::GREEN_INDEX ),
		color->component( Color::BLUE_INDEX ));
}

void PaletteViewScrolledArea::mousePressEvent(QMouseEvent* event) {
	cursorPositioning = false;
	if(( cursorPositioning = setCursorPos(event->x(), event->y()) )) {
		selectionBegin = cursorPos();
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
		selectionEnd = cursorPos();
		if(selectionEnd >= selectionBegin)
			setSelection(selectionBegin, selectionEnd);
		else
			setSelection(selectionEnd, selectionBegin);
		checkSelectionAutoScroll(event->y());
		redraw();
	} else {
		/* check if it is a color drag */
		if(colorIndex( colorDragPoint ) != -1) {
			if(abs( event->x() - colorDragPoint.x() ) > 2 ||
				abs( event->y() - colorDragPoint.y() ) > 2) {
				QColor draggedColor = color(colorDragPoint);
				KColorDrag* colorDrag = KColorDrag::makeDrag(draggedColor, this);
				colorDrag->dragCopy();
			} else
				colorChosen = true;
		}
	}
}

void PaletteViewScrolledArea::mouseReleaseEvent(QMouseEvent* /*event*/) {
	if(colorChosen) {
		if(colorIndex( colorDragPoint ) != -1) {
			int index = colorIndex(colorDragPoint);
			chooseColor(palette()->color( index ));
			if(cursorFollowsChosenColor) {
				setCursorPos(index);
				setSelection(cursorPos(), cursorPos());
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

KColorEditDoc* PaletteViewScrolledArea::document() const {
	return view->document();
}

void PaletteViewScrolledArea::scrollBy(const int y) {
	scrollBar->setValue(scrollBar->value() + y);
}
