/***************************************************************************
                          kcoloreditview.cpp  -  description
                             -------------------
    begin                : Sat Jul  8 09:57:28 CEST 2000
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

// include files for Qt
#include <qpainter.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qvgroupbox.h>
#include <qlineedit.h>
#include <qcolor.h>
#include <qprinter.h>

// include files for KDE
#include <kseparator.h>
#include <klocale.h>

// application specific includes
#include "main.h"
#include "kcoloreditview.h"
#include "kcoloreditdoc.h"
#include "kcoloredit.h"
#include "palette.h"

KColorEditView::KColorEditView(QWidget *parent, const char *name) : QSplitter(parent, name) {
	colorSelector = new ColorSelector(this);
	colorSelector->slotSetColor(
		Color( RGB_MAX_COMPONENT_VALUE, RGB_MAX_COMPONENT_VALUE, RGB_MAX_COMPONENT_VALUE, "" ));
	QWidget* paletteViewArea = new QWidget(this);
	QVBoxLayout* paletteLayout = new QVBoxLayout(paletteViewArea);
	paletteView = new PaletteView(16, 16, 2, this, paletteViewArea);
	paletteLayout->addWidget(paletteView, 10);
	QHBoxLayout* layout = new QHBoxLayout();
	QVBoxLayout* addColorLayout = new QVBoxLayout(4);
	addColorLayout->setMargin(8);
	QHBoxLayout* buttonsLayout = new QHBoxLayout(4);
	QPushButton* addColorButton = new QPushButton(i18n( "Add Color" ), paletteViewArea);
	connect(addColorButton, SIGNAL( clicked() ), SLOT( slotAddColor() ));
	buttonsLayout->addWidget(addColorButton);
	buttonsLayout->addStretch(10);
	addColorLayout->addLayout(buttonsLayout);
	QCheckBox* atCursorCheckBox = new QCheckBox(i18n( "At cursor" ), paletteViewArea);
	connect(atCursorCheckBox, SIGNAL( toggled(bool) ), SLOT( slotAddColorAtCursor(bool) ));
	addColorLayout->addWidget(atCursorCheckBox);
	overwriteCheckBox = new QCheckBox(i18n( "Overwrite" ), paletteViewArea);
	connect(overwriteCheckBox, SIGNAL( toggled(bool) ), SLOT( slotAddColorOverwrite(bool) ));
	slotAddColorAtCursor(false);
	slotAddColorOverwrite(false);
	addColorLayout->addWidget(overwriteCheckBox);
	addColorLayout->addStretch(10);
	//KSeparator* hLine = new KSeparator(KSeparator::HLine, paletteViewArea);
	//addColorLayout->addWidget(hLine);
	//addColorLayout->addStretch(10);
	//QCheckBox* cursorFollowsChosenColor = new QCheckBox(i18n( "Cursor follows" ), paletteViewArea);
	//addColorLayout->addWidget(cursorFollowsChosenColor);
	//connect(cursorFollowsChosenColor, SIGNAL( toggled(bool) ),
	//	paletteView, SLOT( slotCursorFollowsChosenColor(bool) ));
	//cursorFollowsChosenColor->toggle();
	paletteView->slotCursorFollowsChosenColor(true);
	layout->addLayout(addColorLayout, 0);
	QVGroupBox* colorAtCursorFrame = new QVGroupBox(i18n("Color at Cursor"), paletteViewArea);
	QWidget* colorAtCursorFrameArea = new QWidget(colorAtCursorFrame);
	QVBoxLayout* colorAtCursorLayout = new QVBoxLayout(colorAtCursorFrameArea, 4);
	QHBoxLayout* colorNameLayout = new QHBoxLayout(0);
	QLabel* nameLabel = new QLabel(i18n( "Name" ) + ": ", colorAtCursorFrameArea);
	colorNameLayout->addWidget(nameLabel, 0);
	colorName = new QLineEdit(colorAtCursorFrameArea);
	connect(colorName, SIGNAL( textChanged(const QString&) ), SLOT( slotSetColorName(const QString&) ));
	colorNameLayout->addWidget(colorName, 10);
	colorAtCursorLayout->addLayout(colorNameLayout);
	QGridLayout* colorAtCursorComponentsLayout = new QGridLayout(3, 6, 4);
	colorAtCursorLayout->addLayout(colorAtCursorComponentsLayout);
	colorAtCursorComponentsLayout->setColStretch(1, 10);
	colorAtCursorComponentsLayout->addColSpacing(2, 8);
	colorAtCursorComponentsLayout->setColStretch(4, 10);
	colorAtCursorComponentsLayout->setColStretch(5, 10);
	QLabel* hLabel = new QLabel("H: ", colorAtCursorFrameArea);
	colorAtCursorComponentsLayout->addWidget(hLabel, 0, 0);
	colorAtCursorHValueLabel = new QLabel("", colorAtCursorFrameArea);
	setColorAtCursorComponentValueLabelSizes(colorAtCursorHValueLabel);
	colorAtCursorComponentsLayout->addWidget(colorAtCursorHValueLabel, 0, 1);
	QLabel* sLabel = new QLabel("S: ", colorAtCursorFrameArea);
	colorAtCursorComponentsLayout->addWidget(sLabel, 1, 0);
	colorAtCursorSValueLabel = new QLabel("", colorAtCursorFrameArea);
	setColorAtCursorComponentValueLabelSizes(colorAtCursorSValueLabel);
	colorAtCursorComponentsLayout->addWidget(colorAtCursorSValueLabel, 1, 1);
	QLabel* vLabel = new QLabel("V: ", colorAtCursorFrameArea);
	colorAtCursorComponentsLayout->addWidget(vLabel, 2, 0);
	colorAtCursorVValueLabel = new QLabel("", colorAtCursorFrameArea);
	setColorAtCursorComponentValueLabelSizes(colorAtCursorVValueLabel);
	colorAtCursorComponentsLayout->addWidget(colorAtCursorVValueLabel, 2, 1);
	QLabel* rLabel = new QLabel("R: ", colorAtCursorFrameArea);
	colorAtCursorComponentsLayout->addWidget(rLabel, 0, 3);
	colorAtCursorRValueLabel = new QLabel("", colorAtCursorFrameArea);
	setColorAtCursorComponentValueLabelSizes(colorAtCursorRValueLabel);
	colorAtCursorComponentsLayout->addWidget(colorAtCursorRValueLabel, 0, 4);
	QLabel* gLabel = new QLabel("G: ", colorAtCursorFrameArea);
	colorAtCursorComponentsLayout->addWidget(gLabel, 1, 3);
	colorAtCursorGValueLabel = new QLabel("", colorAtCursorFrameArea);
	setColorAtCursorComponentValueLabelSizes(colorAtCursorGValueLabel);
	colorAtCursorComponentsLayout->addWidget(colorAtCursorGValueLabel, 1, 4);
	QLabel* bLabel = new QLabel("B: ", colorAtCursorFrameArea);
	colorAtCursorComponentsLayout->addWidget(bLabel, 2, 3);
	colorAtCursorBValueLabel = new QLabel("", colorAtCursorFrameArea);
	setColorAtCursorComponentValueLabelSizes(colorAtCursorBValueLabel);
	colorAtCursorComponentsLayout->addWidget(colorAtCursorBValueLabel, 2, 4);
	QHBoxLayout* colorAtCursorRgbStringLayout = new QHBoxLayout();
	QLabel* colorAtCursorRgbStringLabel =
		new QLabel("RGB " + i18n( "hex." ) + ": ", colorAtCursorFrameArea);
	colorAtCursorRgbStringLayout->addWidget(colorAtCursorRgbStringLabel);
	colorAtCursorRgbStringValueLabel = new QLabel("", colorAtCursorFrameArea);
	colorAtCursorRgbStringValueLabel->setFixedWidth(
		colorAtCursorRgbStringValueLabel->fontMetrics().width( QString("8888888") ));
	colorAtCursorRgbStringLayout->addWidget(colorAtCursorRgbStringValueLabel);
	colorAtCursorRgbStringLayout->addStretch();
	colorAtCursorLayout->addLayout(colorAtCursorRgbStringLayout);
	layout->addWidget(colorAtCursorFrame, 10);
	layout->addSpacing(8);
	paletteLayout->addSpacing(4);
	paletteLayout->addLayout(layout);
	paletteLayout->addSpacing(4);
	inColorNameChanging = false;
	doNotUpdateColorLabels = false;
}

KColorEditView::~KColorEditView() {
}

void KColorEditView::setColorAtCursorComponentValueLabelSizes(QLabel* const label) {
	label->setMinimumWidth(label->fontMetrics().width( QString("888") ));
	label->setMaximumWidth(label->fontMetrics().width( QString("88888") ));
}

KColorEditDoc *KColorEditView::document() const {
  KColorEditApp *theApp=(KColorEditApp *) parentWidget();

  return theApp->document();
}

void KColorEditView::print(QPrinter *pPrinter) {
  QPainter printpainter;
  printpainter.begin(pPrinter);

  // TODO: add your printing code here

  printpainter.end();
}

void KColorEditView::chooseColor(Color* const color) {
	colorSelector->slotSetColor(color);
}

void KColorEditView::slotCursorPosChanged(int position) {
	Palette* palette = document()->paletteHistory()->editableStream();
	if(position < palette->length()) {
		Color* color = palette->color(position);
		QString string;
		inColorNameChanging = true;
		colorName->setText(color->name());
		colorName->setEnabled(true);
		inColorNameChanging = false;
		int rComponent = color->component(Color::RED_INDEX);
		int gComponent = color->component(Color::GREEN_INDEX);
		int bComponent = color->component(Color::BLUE_INDEX);
		colorAtCursorRValueLabel->setText(string.setNum( rComponent ));
		colorAtCursorGValueLabel->setText(string.setNum( gComponent ));
		colorAtCursorBValueLabel->setText(string.setNum( bComponent ));
		QColor hsvColor(rComponent, gComponent, bComponent);
		int hComponent;
		int sComponent;
		int vComponent;
		hsvColor.hsv(&hComponent, &sComponent, &vComponent);
		colorAtCursorHValueLabel->setText(string.setNum( hComponent ));
		colorAtCursorSValueLabel->setText(string.setNum( sComponent ));
		colorAtCursorVValueLabel->setText(string.setNum( vComponent ));
		colorAtCursorRgbStringValueLabel->setText(string.sprintf( "%02x%02x%02x",
			rComponent, gComponent, bComponent ));
	} else {
		colorName->setText("");
		colorName->setEnabled(false);
		colorAtCursorHValueLabel->setText("");
		colorAtCursorSValueLabel->setText("");
		colorAtCursorVValueLabel->setText("");
		colorAtCursorRValueLabel->setText("");
		colorAtCursorGValueLabel->setText("");
		colorAtCursorBValueLabel->setText("");
		colorAtCursorRgbStringValueLabel->setText("");
	}
}

void KColorEditView::slotViewColorNames(bool viewColorNames) {
	paletteView->slotViewColorNames(viewColorNames);
}

void KColorEditView::updateColorValueLabels() {
	if(!doNotUpdateColorLabels)
		slotCursorPosChanged(document()->paletteCursorPos());
}

void KColorEditView::redraw(bool newDocument) {
	if(newDocument)
		paletteView->setScrollBarValue(0);
	paletteView->redraw();
	updateColorValueLabels();
}

void KColorEditView::slotAddColor() {
    Color color = colorSelector->color();
    Palette* palette = document()->paletteHistory()->editableStream();
    color.setName("");
    int index;
    if(addColorAtCursor)
        index = document()->paletteCursorPos();
    else
        index = palette->length();
    switch(addColorMode)
    {
        case INSERT_COLOR:
            document()->insert(index, color);
            break;

        case REPLACE_COLOR:
            if(index < palette->length())
                document()->replace(index, color);
            else
                document()->insert(index, color);
            break;

    }
}

void KColorEditView::slotAddColorAtCursor(bool atCursor) {
	addColorAtCursor = atCursor;
	overwriteCheckBox->setEnabled(addColorAtCursor);
}

void KColorEditView::slotAddColorOverwrite(bool overwrite) {
	if(overwrite)
		addColorMode = REPLACE_COLOR;
	else
		addColorMode = INSERT_COLOR;
}

void KColorEditView::slotSetColorName(const QString& name) {
	if(!inColorNameChanging) {
		/*
		Palette* palette = getDocument()->getPaletteHistory()->getEditableStream();
		int cursorPos = getDocument()->getPaletteCursorPos();
		if(cursorPos < palette->length()) {
			palette->getColor(cursorPos)->setName(name);
			getDocument()->setModified(true);
			getDocument()->slotRedrawAllViews(this);
		}
		 */
		Palette* palette = document()->paletteHistory()->editableStream();
		int cursorPos = document()->paletteCursorPos();
		if(cursorPos < palette->length()) {
			Color newColor(
				palette->color(cursorPos)->component(Color::RED_INDEX),
				palette->color(cursorPos)->component(Color::GREEN_INDEX),
				palette->color(cursorPos)->component(Color::BLUE_INDEX),
				name);
			doNotUpdateColorLabels = true;
			document()->replace(cursorPos, newColor);
			doNotUpdateColorLabels = false;
		}
	}
}
#include "kcoloreditview.moc"
