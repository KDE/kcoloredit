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
	QGridLayout* topLayout = new QGridLayout(this, 2, 2);
	topLayout->setMargin(2);
	topLayout->setRowStretch(0, 10);
	topLayout->setRowStretch(1, 0);
	topLayout->setColStretch(0, 10);
	topLayout->setColStretch(1, 0);
	scrollBar = new QScrollBar(this);
	hScrollBar = new QScrollBar(0, 1, 1, 1, 0, QScrollBar::Horizontal, this);
	scrolledArea = new PaletteViewScrolledArea(defaultCellWidth,
		defaultCellHeight, cellSpacing, scrollBar, hScrollBar, view, this);
	connect(scrollBar, SIGNAL( valueChanged(int) ),
		SLOT( slotRepaintScrolledArea() ));
	topLayout->addWidget(scrolledArea, 0, 0);
	connect(hScrollBar, SIGNAL( valueChanged(int) ),
		SLOT( slotRepaintScrolledArea() ));
	QHBoxLayout* hScrollBarLayout = new QHBoxLayout();
	hScrollBarLayout->addWidget(hScrollBar, 10);
	hScrollBarLayout->addWidget(new QWidget(this), 0);
	topLayout->addLayout(hScrollBarLayout, 1, 0);
	topLayout->addWidget(scrollBar, 0, 1);
}

PaletteView::~PaletteView() {
}

void PaletteView::redraw() {
	slotRepaintScrolledArea();
}

void PaletteView::setScrollBarValue(const int value) {
	scrollBar->setValue(value);
	hScrollBar->setValue(0);
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
