/***************************************************************************
                          texteditselection.cpp  -  description
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

#include <qlabel.h>
#include <qvalidator.h>
#include <qcolor.h>

#include <klocale.h>

#include "main.h"
#include "texteditselection.h"

TextEditSelection::TextEditSelection(QWidget *parent, const char *name ) : QWidget(parent,name) {
	inChangingComponents = false;
	QVBoxLayout* topLayout = new QVBoxLayout(this, 4);
	QGridLayout* componentsLayout = new QGridLayout(3, 5, 2);
	topLayout->addLayout(componentsLayout);
	componentsLayout->setColStretch(1, 10);
	componentsLayout->addColSpacing(2, 8);
	componentsLayout->setColStretch(4, 10);
	QLineEdit* lineEdit;
	addComponent(H_INDEX, ( lineEdit = new QLineEdit(this) ), HSV_MAX_H_VALUE, "H:", 0, 0, componentsLayout);
	connect(lineEdit, SIGNAL( textChanged(const QString&) ), SLOT( slotHsvComponentChanged() ));
	addComponent(S_INDEX, ( lineEdit = new QLineEdit(this) ), HSV_MAX_S_VALUE, "S:", 1, 0, componentsLayout);
	connect(lineEdit, SIGNAL( textChanged(const QString&) ), SLOT( slotHsvComponentChanged() ));
	addComponent(V_INDEX, ( lineEdit = new QLineEdit(this) ), HSV_MAX_V_VALUE, "V:", 2, 0, componentsLayout);
	connect(lineEdit, SIGNAL( textChanged(const QString&) ), SLOT( slotHsvComponentChanged() ));
	addComponent(R_INDEX, ( lineEdit = new QLineEdit(this) ), RGB_MAX_COMPONENT_VALUE, "R:", 0, 1, componentsLayout);
	connect(lineEdit, SIGNAL( textChanged(const QString&) ), SLOT( slotRgbComponentChanged() ));
	addComponent(G_INDEX, ( lineEdit = new QLineEdit(this) ), RGB_MAX_COMPONENT_VALUE, "G:", 1, 1, componentsLayout);
	connect(lineEdit, SIGNAL( textChanged(const QString&) ), SLOT( slotRgbComponentChanged() ));
	addComponent(B_INDEX, ( lineEdit = new QLineEdit(this) ), RGB_MAX_COMPONENT_VALUE, "B:", 2, 1, componentsLayout);
	connect(lineEdit, SIGNAL( textChanged(const QString&) ), SLOT( slotRgbComponentChanged() ));
	QHBoxLayout* rgbStringLayout = new QHBoxLayout(2);
	QLabel* rgbStringLabel = new QLabel("RGB " + i18n( "hex." ) + ": ", this);
	rgbStringLayout->addWidget(rgbStringLabel);
	rgbStringLineEdit = new QLineEdit(this);
	rgbStringLineEdit->setMinimumWidth(lineEdit->fontMetrics().width( QString("8888888") ));
	rgbStringLineEdit->setMaximumWidth(lineEdit->fontMetrics().width( QString("888888888") ));
	connect(rgbStringLineEdit, SIGNAL( textChanged(const QString&) ), SLOT( slotRgbStringChanged() ));
	rgbStringLayout->addWidget(rgbStringLineEdit);
	topLayout->addLayout(rgbStringLayout);
}
TextEditSelection::~TextEditSelection(){
}

void TextEditSelection::addComponent(const int index, QLineEdit* lineEdit, const int maxValue,
	const QString& labelString, const int row, const int column, QGridLayout* layout) {
	QLabel* label = new QLabel(labelString, this);
	lineEdit->setValidator(new QIntValidator( 0, maxValue, lineEdit ));
	lineEditTable[index] = lineEdit;
	lineEdit->setMinimumWidth(lineEdit->fontMetrics().width( QString("8888") ));
	lineEdit->setMaximumWidth(lineEdit->fontMetrics().width( QString("8888888") ));
	layout->addWidget(label, row, column*3);
	layout->addWidget(lineEdit, row, column*3 + 1);
}

void TextEditSelection::setRgbString(const int red, const int green, const int blue) {
	QString string;
	string.sprintf("%02x%02x%02x", red, green, blue);
	rgbStringLineEdit->setText(string);
}

void TextEditSelection::slotHsvComponentChanged() {
	if(!inChangingComponents) {
		inChangingComponents = true;
		int hComponent = lineEditTable[H_INDEX]->text().toInt();
		int sComponent = lineEditTable[S_INDEX]->text().toInt();
		int vComponent = lineEditTable[V_INDEX]->text().toInt();
		QColor color;
		color.setHsv(hComponent, sComponent, vComponent);
		int rComponent = color.red();
		int gComponent = color.green();
		int bComponent = color.blue();
		QString string;
		lineEditTable[R_INDEX]->setText(string.setNum( rComponent ));
		lineEditTable[G_INDEX]->setText(string.setNum( gComponent ));
		lineEditTable[B_INDEX]->setText(string.setNum( bComponent ));
		Color oldColor = this->color;
		this->color.setComponent(Color::RED_INDEX, rComponent);
		this->color.setComponent(Color::GREEN_INDEX, gComponent);
		this->color.setComponent(Color::BLUE_INDEX, bComponent);
		if(!this->color.equals( oldColor ))
			emit valueChanged(&this->color);
		setRgbString(rComponent, gComponent, bComponent);
		inChangingComponents = false;
	}
}

void TextEditSelection::slotRgbComponentChanged() {
	if(!inChangingComponents) {
		inChangingComponents = true;
		int rComponent = lineEditTable[R_INDEX]->text().toInt();
		int gComponent = lineEditTable[G_INDEX]->text().toInt();
		int bComponent = lineEditTable[B_INDEX]->text().toInt();
		QColor color;
		color.setRgb(rComponent, gComponent, bComponent);
		int hComponent;
		int sComponent;
		int vComponent;
		color.hsv(&hComponent, &sComponent, &vComponent);
		QString string;
		lineEditTable[H_INDEX]->setText(string.setNum( hComponent ));
		lineEditTable[S_INDEX]->setText(string.setNum( sComponent ));
		lineEditTable[V_INDEX]->setText(string.setNum( vComponent ));
		Color oldColor = this->color;
		this->color.setComponent(Color::RED_INDEX, rComponent);
		this->color.setComponent(Color::GREEN_INDEX, gComponent);
		this->color.setComponent(Color::BLUE_INDEX, bComponent);
		if(!this->color.equals( oldColor ))
			emit valueChanged(&this->color);
		setRgbString(rComponent, gComponent, bComponent);
		inChangingComponents = false;
	}
}

void TextEditSelection::slotRgbStringChanged() {
	if(!inChangingComponents) {
		inChangingComponents = true;
		QString string = rgbStringLineEdit->text().stripWhiteSpace();
		bool result;
		int value = string.toInt(&result, 16);
		if(result) {
			int rComponent = (value >> 16)&0xff;
			int gComponent = (value >> 8)&0xff;
			int bComponent = (value >> 0)&0xff;
			lineEditTable[R_INDEX]->setText(string.setNum( rComponent ));
			lineEditTable[G_INDEX]->setText(string.setNum( gComponent ));
			lineEditTable[B_INDEX]->setText(string.setNum( bComponent ));
			int hComponent;
			int sComponent;
			int vComponent;
			QColor hsvColor;
			hsvColor.hsv(&hComponent, &sComponent, &vComponent);
			lineEditTable[H_INDEX]->setText(string.setNum( hComponent ));
			lineEditTable[S_INDEX]->setText(string.setNum( sComponent ));
			lineEditTable[V_INDEX]->setText(string.setNum( vComponent ));
			Color oldColor = this->color;
			this->color.setComponent(Color::RED_INDEX, rComponent);
			this->color.setComponent(Color::GREEN_INDEX, gComponent);
			this->color.setComponent(Color::BLUE_INDEX, bComponent);
			if(!this->color.equals( oldColor ))
				emit valueChanged(&this->color);
		}
		inChangingComponents = false;
	}
}

void TextEditSelection::slotSetValue(Color* color) {
	if(!color->equals( this->color )) {
		inChangingComponents = true;
		this->color = *color;
		QString string;
		int rComponent = this->color.component(Color::RED_INDEX);
		int gComponent = this->color.component(Color::GREEN_INDEX);
		int bComponent = this->color.component(Color::BLUE_INDEX);
		lineEditTable[R_INDEX]->setText(string.setNum( rComponent ));
		lineEditTable[G_INDEX]->setText(string.setNum( gComponent ));
		lineEditTable[B_INDEX]->setText(string.setNum( bComponent ));
		QColor hsvColor(rComponent, gComponent, bComponent);
		int hComponent;
		int sComponent;
		int vComponent;
		hsvColor.hsv(&hComponent, &sComponent, &vComponent);
		lineEditTable[H_INDEX]->setText(string.setNum( hComponent ));
		lineEditTable[S_INDEX]->setText(string.setNum( sComponent ));
		lineEditTable[V_INDEX]->setText(string.setNum( vComponent ));
		setRgbString(rComponent, gComponent, bComponent);
		inChangingComponents = false;
	}
}

#include "texteditselection.moc"
