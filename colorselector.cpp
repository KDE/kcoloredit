/***************************************************************************
                          colorselector.cpp  -  description
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

#include <math.h>

#include <qlayout.h>
/* #include <qtabwidget.h> */
#include <qradiobutton.h>
#include <qlabel.h>
#include <qslider.h>
#include <kseparator.h>
#include <klocale.h>

#include "main.h"
#include "imageselection.h"
#include "texteditselection.h"
#include "colorselector.h"

ColorSelector::ColorSelector(QWidget *parent, const char *name ) : QWidget(parent, name) {
	fComponentsMode = false;
	QVBoxLayout* topLayout = new QVBoxLayout(this, 4);
	/*
	QTabWidget* pages = new QTabWidget(this);
	 */
	gradientSelection = new GradientSelection(this);
	connect(gradientSelection, SIGNAL( valueChanged(Color*) ),
		SLOT( slotGradientSelectionChangeColor(Color*) ));
	connect(gradientSelection, SIGNAL( synchronizeColor() ),
		SLOT( slotGradientSelectionSynchronizeColor() ));
	connect(this, SIGNAL( valueChanged(Color*) ), gradientSelection, SLOT( slotSetValue(Color*) ));
	/*
	pages->addTab(gradientSelection, i18n( "Gradient" ));
	ImageSelection* imageSelection = new ImageSelection(this);
	connect(imageSelection, SIGNAL( valueChanged(Color*) ), SLOT( slotSetColor(Color*) ));
	connect(this, SIGNAL( valueChanged(Color*) ), imageSelection, SLOT( slotSetValue(Color*) ));
	pages->addTab(imageSelection, i18n( "Image" ));
	topLayout->addWidget(pages, 10);
	 */
	topLayout->addWidget(gradientSelection, 10);
	KSeparator* hLine = new KSeparator(KSeparator::HLine, this);
	topLayout->addWidget(hLine);
	QHBoxLayout* layout = new QHBoxLayout();
	TextEditSelection* textEditSelection = new TextEditSelection(this);
	connect(textEditSelection, SIGNAL( valueChanged(Color*) ), SLOT( slotSetColor(Color*) ));
	connect(this, SIGNAL( valueChanged(Color*) ), textEditSelection, SLOT( slotSetValue(Color*) ));
	QVBoxLayout* colorChangeLayout = new QVBoxLayout();
	colorChangeLayout->setMargin(2);
	QRadioButton* replaceButton = new QRadioButton(i18n( "Replace" ), this);
	connect(replaceButton, SIGNAL( clicked() ), SLOT( slotColorReplace() ));
	replaceButton->setChecked(true);
	colorChangeButtons.insert(replaceButton);
	colorChangeLayout->addWidget(replaceButton);
	QRadioButton* changeButton = new QRadioButton(i18n( "Change" ) + ":", this);
	connect(changeButton, SIGNAL( clicked() ), SLOT( slotColorChange() ));
	colorChangeButtons.insert(changeButton);
	colorChangeLayout->addWidget(changeButton);
	colorChangeValue = 0;
	colorChangeSliderWidget = new QWidget(this);
	QVBoxLayout* colorChangeSliderLayout = new QVBoxLayout(colorChangeSliderWidget, 1);
	colorChangeSliderLayout->setMargin(0);
	QSlider* colorChangeSlider = new QSlider(0, MAX_COLOR_CHANGE_VALUE,
		MAX_COLOR_CHANGE_VALUE/4, colorChangeValue, QSlider::Horizontal, colorChangeSliderWidget);
	colorChangeSlider->setTickInterval(colorChangeSlider->pageStep());
	colorChangeSlider->setTickmarks(QSlider::Above);
	connect(colorChangeSlider, SIGNAL( valueChanged(int) ), SLOT( slotColorChangeValueChanged(int) ));
	colorChangeSliderLayout->addWidget(colorChangeSlider);
	QHBoxLayout* colorChangeSliderLabelsLayout = new QHBoxLayout(0);
	QLabel* subtractLabel = new QLabel(i18n( "0" ), colorChangeSliderWidget);
	colorChangeSliderLabelsLayout->addWidget(subtractLabel);
	colorChangeSliderLabelsLayout->addStretch(10);
	QLabel* addLabel = new QLabel(" " + i18n( "Replace" ), colorChangeSliderWidget);
	colorChangeSliderLabelsLayout->addWidget(addLabel);
	colorChangeSliderLayout->addLayout(colorChangeSliderLabelsLayout);
	colorChangeLayout->addStretch(10);
	colorChangeLayout->addWidget(colorChangeSliderWidget);
	colorChangeLayout->addStretch(10);
	layout->addLayout(colorChangeLayout, 10);
	m_color.setComponents(RGB_MAX_COMPONENT_VALUE, RGB_MAX_COMPONENT_VALUE, RGB_MAX_COMPONENT_VALUE);
	slotColorReplace();
	KSeparator* vLine = new KSeparator(KSeparator::VLine, this);
	layout->addWidget(vLine);
	layout->addWidget(textEditSelection, 1);
	colorPatch = new KColorPatch(this);
	connect(colorPatch, SIGNAL( colorChanged(const QColor&) ), SLOT( slotSetColor(const QColor&) ));
	colorPatch->setMinimumSize(80, 64);
	layout->addWidget(colorPatch, 10);
	topLayout->addLayout(layout);
}
ColorSelector::~ColorSelector() {
}

void ColorSelector::slotSetColor(Color color) {
	m_color = color;
	colorPatch->setColor(QColor( m_color.component(Color::RED_INDEX),
		m_color.component(Color::GREEN_INDEX),
		m_color.component(Color::BLUE_INDEX) ));
	fComponentsMode = false;
	emit valueChanged(&m_color);
}

void ColorSelector::slotSetColor(Color* color) {
	slotSetColor(*color);
}

void ColorSelector::slotSetColor(const QColor& color) {
	m_color.setComponent(Color::RED_INDEX, color.red());
	m_color.setComponent(Color::GREEN_INDEX, color.green());
	m_color.setComponent(Color::BLUE_INDEX, color.blue());
	fComponentsMode = false;
	emit valueChanged(&m_color);
}

void ColorSelector::scaledComponent(double* const component, const double componentDiff) {
	double scaledComponentDiff = componentDiff*
		pow(colorChangeValue*1.0/MAX_COLOR_CHANGE_VALUE, 2.0);
	*component += scaledComponentDiff;
	if(*component > RGB_MAX_COMPONENT_VALUE)
		*component = RGB_MAX_COMPONENT_VALUE;
	else if(*component < 0)
		*component = 0;
}

void ColorSelector::slotGradientSelectionChangeColor(Color* gradientSelectionColor) {
	switch(colorChangeMode) {
		case MODE_REPLACE:
			slotSetColor(gradientSelectionColor);
			break;
			
		case MODE_CHANGE:
			gradientSelection->slotIgnoreSetValue(true);
			double rComponent;
			double gComponent;
			double bComponent;
			if(fComponentsMode) {
				rComponent = fRComponent;
				gComponent = fGComponent;
				bComponent = fBComponent;
			} else {
				rComponent = m_color.component(Color::RED_INDEX);
				gComponent = m_color.component(Color::GREEN_INDEX);
				bComponent = m_color.component(Color::BLUE_INDEX);
			}
			double rDiff = gradientSelectionColor->component(Color::RED_INDEX) - rComponent;
			double gDiff = gradientSelectionColor->component(Color::GREEN_INDEX) - gComponent;
			double bDiff = gradientSelectionColor->component(Color::BLUE_INDEX) - bComponent;
			/*
		 	printf("base color = (%i %i %i) gradient color = (%i %i %i)\n",
		 		rComponent, gComponent, bComponent,
		 		gradientSelectionColor->getComponent(Color::RED_INDEX),
		 		gradientSelectionColor->getComponent(Color::GREEN_INDEX),
		 		gradientSelectionColor->getComponent(Color::BLUE_INDEX));
		 	 */
			scaledComponent(&rComponent, rDiff);
			scaledComponent(&gComponent, gDiff);
			scaledComponent(&bComponent, bDiff);
			Color newColor((int)( rComponent + 0.5 ),
				(int)( gComponent + 0.5 ), (int)( bComponent + 0.5 ), "");
			slotSetColor(newColor);
			fRComponent = rComponent;
			fGComponent = gComponent;
			fBComponent = bComponent;
			fComponentsMode = true;
			/*
		 	printf("output color = (%i %i %i)\n",
		 		rComponent, gComponent, bComponent);
		 	 */
			gradientSelection->slotIgnoreSetValue(false);
			break;
	}
}

void ColorSelector::slotGradientSelectionSynchronizeColor() {
	/** Notify the gradient selector to update its color */
	emit valueChanged(&m_color);
}

const Color& ColorSelector::color() {
	return m_color;
}

void ColorSelector::slotColorReplace() {
	colorChangeMode = MODE_REPLACE;
	slotGradientSelectionSynchronizeColor();
	gradientSelection->enableSynchronizeColorButton(false);
	colorChangeSliderWidget->setEnabled(false);
}

void ColorSelector::slotColorChange() {
	colorChangeMode = MODE_CHANGE;
	gradientSelection->enableSynchronizeColorButton(true);
	colorChangeSliderWidget->setEnabled(true);
}

void ColorSelector::slotColorChangeValueChanged(int value) {
	colorChangeValue = value;
}

#include "colorselector.moc"
