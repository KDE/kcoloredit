/***************************************************************************
                          kxycolorselector.cpp  -  description
                             -------------------
    begin                : Fri Jul 7 2000
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

#include <qpainter.h>
#include <qpixmap.h>
#include <qimage.h>
#include <kimageeffect.h>
#include <kpalette.h>

#include "kxycolorselector.h"

KXYColorSelector::KXYColorSelector(QWidget *parent, const char *name) : KXYSelector(parent,name) {
	setBackgroundMode(NoBackground);
	setRange(0, 0, COMPONENT_SELECTION_RESOLUTION - 1, COMPONENT_SELECTION_RESOLUTION - 1);
	setType(TYPE_NONE);
	setGlobalComponent(0);
	setMinimumSize(RGB_MAX_COMPONENT_VALUE/2 + 4, RGB_MAX_COMPONENT_VALUE/2 + 4);
	setMaximumSize(512, 512);
	updateContents();
}
KXYColorSelector::~KXYColorSelector()	{

}

void KXYColorSelector::setType(const int type) {
	this->type = type;
}

void KXYColorSelector::updateContents() {
	drawPalette(&pixmap);
	repaint();
}

void KXYColorSelector::resizeEvent(QResizeEvent*) {
	setValues(xValue(), yValue());
	updateContents();
}

void KXYColorSelector::drawContents(QPainter* painter) {
	painter->drawPixmap(contentsRect().x(), contentsRect().y(), pixmap);
}

void KXYColorSelector::drawCursor(QPainter* painter, int x, int y) {
	QColor color;
	int colorX = x - contentsRect().x();
	int colorY = y - contentsRect().y();
	if(colorX < 0)
		colorX = 0;
	if(colorY < 0)
		colorY = 0;
	if(colorX > contentsRect().width() - 1)
		colorX = contentsRect().width() - 1;
	if(colorY > contentsRect().height() - 1)
		colorY = contentsRect().height() - 1;
	setColor(&color, colorX, colorY);
	QColor cursorColor;
	if(( 2*color.red() + 4*color.green() + 1*color.blue() )*1.0/
		( 2*255 + 4*255 + 1*255 ) > 0.65)
		cursorColor = Qt::black;
	else
		cursorColor = Qt::white;
	painter->setPen(QPen( cursorColor ));
	const int lineBegin = 2;
	const int lineEnd = 6;
	painter->drawLine(x + lineBegin, y - lineBegin, x + lineEnd, y - lineEnd);
	painter->drawLine(x + lineBegin, y + lineBegin, x + lineEnd, y + lineEnd);
	painter->drawLine(x - lineBegin, y + lineBegin, x - lineEnd, y + lineEnd);
	painter->drawLine(x - lineBegin, y - lineBegin, x - lineEnd, y - lineEnd);
}

void KXYColorSelector::setGlobalComponent(const int component) {
	m_globalComponent = component;
}

int KXYColorSelector::globalComponent() const{
	return m_globalComponent;
}

void KXYColorSelector::setColor(QColor* const color, const int x, const int y) {
	int xSize = contentsRect().width();
	int ySize = contentsRect().height();
	switch(type) {
		case TYPE_HS:
			color->setHsv(360*x/xSize, 256*( ySize - 1 - y )/ySize,
                    globalComponent());
			break;

		case TYPE_VS:
			color->setHsv(globalComponent(), 256*( ySize - 1 - y )/ySize,
                    256*x/xSize);
			break;

		case TYPE_HV:
			color->setHsv(360*x/xSize, globalComponent(),
                    256*( ySize - 1 - y )/ySize);
			break;

		case TYPE_RG:
			color->setRgb(x/xSize, 256*( ySize - 1 - y )/ySize,
                    globalComponent());
			break;

		case TYPE_GB:
			color->setRgb(globalComponent(), 256*x/xSize,
                    256*( ySize - 1 - y )/ySize);
			break;

		case TYPE_BR:
			color->setRgb(256*( ySize - 1 - y )/ySize, globalComponent(),
                    256*x/xSize);
			break;

		case TYPE_NONE:
			color->setRgb(192, 192, 192);
			break;

	}
}

QColor* KXYColorSelector::standardColorsPalette() {
	QColor* palette = new QColor[STANDARD_PALETTE_SIZE];
	int i = 0;
	palette[i++] = Qt::red;
	palette[i++] = Qt::green;
	palette[i++] = Qt::blue;
	palette[i++] = Qt::cyan;
	palette[i++] = Qt::magenta;
	palette[i++] = Qt::yellow;
	palette[i++] = Qt::darkRed;
	palette[i++] = Qt::darkGreen;
	palette[i++] = Qt::darkBlue;
	palette[i++] = Qt::darkCyan;
	palette[i++] = Qt::darkMagenta;
	palette[i++] = Qt::darkYellow;
	palette[i++] = Qt::white;
	palette[i++] = Qt::lightGray;
	palette[i++] = Qt::gray;
	palette[i++] = Qt::darkGray;
	palette[i++] = Qt::black;
	return palette;
}

void KXYColorSelector::drawPalette(QPixmap* pixmap) {
	int xSize = contentsRect().width();
	int ySize = contentsRect().height();
	QImage image(xSize, ySize, 32);
	QColor color;
	int x;
	int y;

	if(type != TYPE_NONE) {
		for (y = 0; y < ySize; ++y)
		{
			unsigned int* p = (unsigned int*)image.scanLine(y);
			for(x = 0; x < xSize; ++x)
			{
				setColor(&color, x, y);
				*p = color.rgb();
				++p;
			}
		}
		if (QColor::numBitPlanes() <= 8)
		{
	    QColor* standardPalette = standardColorsPalette();
			KImageEffect::dither(image, standardPalette, STANDARD_PALETTE_SIZE);
			delete[] standardPalette;
		}
	}
	pixmap->convertFromImage(image);
}

#include "kxycolorselector.moc"
