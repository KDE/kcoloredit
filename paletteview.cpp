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

#include <qlayout.h>

#include "kcoloreditview.h"
#include "paletteview.h"

PaletteView::PaletteView(const int defaultCellWidth, const int defaultCellHeight, const int cellSpacing,
	KColorEditView* view, QWidget *parent, const char *name) :
	QFrame(parent, name, QWidget::WResizeNoErase*0) {
	setFrameStyle(StyledPanel|Sunken);
	setLineWidth(2);
	QHBoxLayout* topLayout = new QHBoxLayout(this, 0);
	topLayout->setMargin(2);
	scrollBar = new QScrollBar(this);
	scrolledArea = new PaletteViewScrolledArea(defaultCellWidth,
		defaultCellHeight, cellSpacing, scrollBar, view, this);
	connect(scrollBar, SIGNAL( valueChanged(int) ),
		SLOT( slotRepaintScrolledArea() ));
	topLayout->addWidget(scrolledArea, 10);
	topLayout->addWidget(scrollBar, 0);
}

PaletteView::~PaletteView() {
}

void PaletteView::redraw() {
	slotRepaintScrolledArea();
}

void PaletteView::setScrollBarValue(const int value) {
	scrollBar->setValue(value);
}

void PaletteView::slotViewColorNames(bool viewColorNames) {
	scrolledArea->slotViewColorNames(viewColorNames);
	setScrollBarValue(0);
	scrolledArea->redraw();
}

void PaletteView::slotCursorFollowsChosenColor(bool follows) {
	scrolledArea->slotCursorFollowsChosenColor(follows);
}

void PaletteView::slotRepaintScrolledArea() {
	scrolledArea->redraw();
}
#include "paletteview.moc"
