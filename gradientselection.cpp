/***************************************************************************
                          gradientselection.cpp  -  description
                             -------------------
    begin                : Wed Jul 12 2000
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
#include <qframe.h>
#include <qradiobutton.h>
#include <qcolor.h>
#include <klocale.h>

#include "main.h"
#include "gradientselection.h"

GradientSelection::GradientSelection(QWidget *parent, const char *name ) : QWidget(parent,name) {
	QGridLayout* topLayout = new QGridLayout(this, 2, 2, 0);
	QHBoxLayout* layout = new QHBoxLayout(0);
	layout->setMargin(3);
	xyColorSelector = new KXYColorSelector(this);
	connect(xyColorSelector, SIGNAL( valueChanged(int, int) ),
		SLOT( slotXyColorSelectorPosChanged(int, int) ));
	layout->addWidget(xyColorSelector);
	topLayout->addLayout(layout, 0, 0);
	topLayout->setRowStretch(0, 10);
	topLayout->setRowStretch(1, 0);
	QVBoxLayout* xyColorSelectorLayout = new QVBoxLayout();
	QHBoxLayout* checkBoxLayout = new QHBoxLayout();
	checkBoxLayout->setMargin(0);
	variableCheckBox = new QCheckBox(i18n( "Variable" ), this);
	variableGlobalComponent = false;
	connect(variableCheckBox, SIGNAL( toggled(bool) ), SLOT( slotSetVariableGlobalComponent(bool) ));
	checkBoxLayout->addSpacing(2);
	checkBoxLayout->addWidget(variableCheckBox);
	xyColorSelectorLayout->addLayout(checkBoxLayout);
	xyColorSelectorLayout->addStretch(10);
	QHBoxLayout* buttonsLayout = new QHBoxLayout();
	synchronizeColorButton = new QPushButton(i18n( "Synchronize" ), this);
	connect(synchronizeColorButton, SIGNAL( clicked() ), SLOT( slotSynchronizeColor() ));
	buttonsLayout->addSpacing(2);
	buttonsLayout->addWidget(synchronizeColorButton);
	buttonsLayout->addStretch(10);
	xyColorSelectorLayout->addLayout(buttonsLayout);
	xyColorSelectorLayout->addSpacing(2);
	topLayout->addLayout(xyColorSelectorLayout, 1, 0);
	zColorSelector = new KZColorSelector(KZColorSelector::Vertical, this);
	connect(zColorSelector, SIGNAL( valueChanged(int) ),
		SLOT( slotZColorSelectorPosChanged(int) ));
	zColorSelector->setFixedWidth(36);
	topLayout->addWidget(zColorSelector, 0, 1);
	QVBoxLayout* zColorSelectorLayout = new QVBoxLayout(0);
	connect(&hsvButtons, SIGNAL( clicked(int) ), SLOT( slotSetColorSelectionMode(int) ));
	QRadioButton* hRadioButton = new QRadioButton("H", this);
	hsvButtons.insert(hRadioButton, H_COMPONENT);
	zColorSelectorLayout->addWidget(hRadioButton);
	QRadioButton* sRadioButton = new QRadioButton("S", this);
	hsvButtons.insert(sRadioButton, S_COMPONENT);
	zColorSelectorLayout->addWidget(sRadioButton);
	QRadioButton* vRadioButton = new QRadioButton("V", this);
	hsvButtons.insert(vRadioButton, V_COMPONENT);
	vRadioButton->toggle();
	zColorSelectorLayout->addWidget(vRadioButton);
	topLayout->addLayout(zColorSelectorLayout, 1, 1);
	color.setComponents(RGB_MAX_COMPONENT_VALUE, RGB_MAX_COMPONENT_VALUE, RGB_MAX_COMPONENT_VALUE);
	hComponent = -1;
	sComponent = 0;
	vComponent = HSV_MAX_V_VALUE;
	slotIgnoreSetValue(false);
	slotSetColorSelectionMode(V_COMPONENT);
}
GradientSelection::~GradientSelection(){
}

void GradientSelection::slotSetValue(Color* color) {
  if(!ignoreSetValue && !color->equals( this->color )) {
		this->color = *color;
		int newHComponent;
		int newSComponent;
		int newVComponent;
		QColor hsvColor(this->color.component(Color::RED_INDEX),
			this->color.component(Color::GREEN_INDEX),
			this->color.component(Color::BLUE_INDEX));
		hsvColor.hsv(&newHComponent, &newSComponent, &newVComponent);
		hComponent = newHComponent;
		sComponent = newSComponent;
		vComponent = newVComponent;
		updateXyColorSelector(false);
		updateZColorSelector();
	}
}

void GradientSelection::slotIgnoreSetValue(bool ignore) {
	ignoreSetValue = ignore;
}

void GradientSelection::updateXyColorSelector(const bool modeChanged) {
	int xPos;
	int yPos;
	int component;
	switch(zColorSelectorComponentIndex) {
		case H_COMPONENT:
			xPos = (int)(vComponent*( KXYColorSelector::COMPONENT_SELECTION_RESOLUTION - 1 )/
				HSV_MAX_V_VALUE + 0.5);
			yPos = (int)(sComponent*( KXYColorSelector::COMPONENT_SELECTION_RESOLUTION - 1 )/
				HSV_MAX_S_VALUE + 0.5);
			component = hComponent;
			break;
			
		case S_COMPONENT:
			xPos = (int)(hComponent*( KXYColorSelector::COMPONENT_SELECTION_RESOLUTION - 1 )/
				HSV_MAX_H_VALUE + 0.5);
			yPos = (int)(vComponent*( KXYColorSelector::COMPONENT_SELECTION_RESOLUTION - 1 )/
				HSV_MAX_V_VALUE + 0.5);
			if(variableGlobalComponent)
				component = sComponent;
			else
				component = 240;
			break;
			
		case V_COMPONENT:
			xPos = (int)(hComponent*( KXYColorSelector::COMPONENT_SELECTION_RESOLUTION - 1 )/
				HSV_MAX_H_VALUE + 0.5);
			yPos = (int)(sComponent*( KXYColorSelector::COMPONENT_SELECTION_RESOLUTION - 1 )/
				HSV_MAX_S_VALUE + 0.5);
			if(variableGlobalComponent)
				component = vComponent;
			else
				component = 192;
			break;
		
		default:
			xPos = 0;
			yPos = 0;
			component = 0;
			break;
			
	}
	if(xPos < 0)
		xPos = 0;
	if(yPos < 0)
		yPos = 0;
	if(modeChanged || xyColorSelector->globalComponent() != component) {
		xyColorSelector->setGlobalComponent(component);
		xyColorSelector->updateContents();
	}
	xyColorSelector->setValues(xPos, yPos);
}

void GradientSelection::updateZColorSelector() {
	zColorSelector->setBaseColorHsv(hComponent, sComponent, vComponent);
	zColorSelector->updatePointerPos();
	zColorSelector->updateContents();
}

void GradientSelection::slotSetColorSelectionMode(int mode) {
	zColorSelectorComponentIndex = mode;
	xyColorSelector->setType(zColorSelectorComponentIndex);
	updateXyColorSelector(true);
	switch(zColorSelectorComponentIndex) {
		case H_COMPONENT:
			zColorSelector->setType(KZColorSelector::TYPE_H);
			variableCheckBox->setEnabled(false);
			break;
			
		case S_COMPONENT:
			zColorSelector->setType(KZColorSelector::TYPE_S);
			variableCheckBox->setEnabled(true);
			break;
			
		case V_COMPONENT:
			zColorSelector->setType(KZColorSelector::TYPE_V);
			variableCheckBox->setEnabled(true);
			break;
			
	}
	updateZColorSelector();
}

void GradientSelection::slotSetVariableGlobalComponent(bool variable) {
	variableGlobalComponent = variable;
	updateXyColorSelector(false);
}

void GradientSelection::slotXyColorSelectorPosChanged(int x, int y) {
	switch(zColorSelectorComponentIndex) {
		case H_COMPONENT:
			vComponent = (int)(x*1.0*HSV_MAX_V_VALUE/( KXYColorSelector::COMPONENT_SELECTION_RESOLUTION - 1 ) + 0.5);
			sComponent = (int)(y*1.0*HSV_MAX_S_VALUE/( KXYColorSelector::COMPONENT_SELECTION_RESOLUTION - 1 ) + 0.5);
			break;
	
		case S_COMPONENT:
			hComponent = (int)(x*1.0*HSV_MAX_H_VALUE/( KXYColorSelector::COMPONENT_SELECTION_RESOLUTION - 1 ) + 0.5);
			vComponent = (int)(y*1.0*HSV_MAX_V_VALUE/( KXYColorSelector::COMPONENT_SELECTION_RESOLUTION - 1 ) + 0.5);
			break;
	
		case V_COMPONENT:
			hComponent = (int)(x*1.0*HSV_MAX_H_VALUE/( KXYColorSelector::COMPONENT_SELECTION_RESOLUTION - 1 ) + 0.5);
			sComponent = (int)(y*1.0*HSV_MAX_S_VALUE/( KXYColorSelector::COMPONENT_SELECTION_RESOLUTION - 1 ) + 0.5);
			break;
	
	}
	QColor rgbColor;
	rgbColor.setHsv(hComponent, sComponent, vComponent);
	color.setComponents(rgbColor.red(), rgbColor.green(), rgbColor.blue());
	updateZColorSelector();
	emit valueChanged(&color);
}

void GradientSelection::slotZColorSelectorPosChanged(int y) {
	bool repaintZColorSelector = false;
	switch(zColorSelectorComponentIndex) {
		case H_COMPONENT:
			hComponent = y;
			break;
	
		case S_COMPONENT:
			sComponent = y;
			break;
	
		case V_COMPONENT:
			vComponent = y;
			break;
	
	}
	QColor rgbColor;
	rgbColor.setHsv(hComponent, sComponent, vComponent);
	color.setComponents(rgbColor.red(), rgbColor.green(), rgbColor.blue());
	updateXyColorSelector(false);
	if(repaintZColorSelector)
		updateZColorSelector();
	emit valueChanged(&color);
}

void GradientSelection::slotSynchronizeColor() {
	emit synchronizeColor();
}

void GradientSelection::enableSynchronizeColorButton(bool enable) {
	synchronizeColorButton->setEnabled(enable);
}
#include "gradientselection.moc"
