/***************************************************************************
                          kzcolorselector.cpp  -  description
                             -------------------
    begin                : Fri Jul 14 2000
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
#include <qimage.h>
#include <kimageeffect.h>
#include <kpalette.h>

#include "main.h"
#include "kzcolorselector.h"

KZColorSelector::KZColorSelector(Orientation o, QWidget *parent, const char *name) :
	KSelector(o, parent, name) {
	baseColorH = -1;
	baseColorS = 0;
	baseColorV = 0;
	pixmap.setOptimization( QPixmap::BestOptim );
}
KZColorSelector::~KZColorSelector() {
}

void KZColorSelector::setType(const int type) {
	this->type = type;
	switch(this->type) {
		case TYPE_H:
			setRange(0, HSV_MAX_H_VALUE);
			break;
			
		case TYPE_S:
			setRange(0, HSV_MAX_S_VALUE);
			break;
			
		case TYPE_V:
			setRange(0, HSV_MAX_V_VALUE);
			break;
			
	}
}

void KZColorSelector::updateContents() {
	drawPalette(&pixmap);
	repaint(false);
}

void KZColorSelector::resizeEvent(QResizeEvent*) {
	updateContents();
}

void KZColorSelector::drawContents(QPainter* painter) {
	painter->drawPixmap(contentsRect().x(), contentsRect().y(), pixmap);
}

void KZColorSelector::setBaseColor(const QColor& color) {
	color.hsv(&baseColorH, &baseColorS, &baseColorV);
}

void KZColorSelector::setBaseColorHsv(const int colorH,
	const int colorS, const int colorV) {
	baseColorH = colorH;
	baseColorS = colorS;
	baseColorV = colorV;
}

void KZColorSelector::updatePointerPos() {
	int pos;
	switch(type) {
		case TYPE_H:
			pos = baseColorH;
			if(pos < 0)
				pos = 0;
			break;
			
		case TYPE_S:
			pos = baseColorS;
			break;
			
		case TYPE_V:
			pos = baseColorV;
			break;
			
		default:
			pos = 0;
			break;
			
	}
	setValue(pos);
}

void KZColorSelector::setColor(QColor* const color, const int y) {
	int ySize = contentsRect().height();
	switch(type) {
		case TYPE_H:
			color->setHsv(( ySize - 1 - y )*360/ySize, baseColorS, baseColorV);
			break;
			
		case TYPE_S:
			color->setHsv(baseColorH, ( ySize - 1 - y )*256/ySize, baseColorV);
			break;
			
		case TYPE_V:
			color->setHsv(baseColorH, baseColorS, ( ySize - 1 - y )*256/ySize);
			break;
			
	}
}

QColor* KZColorSelector::getStandardColorsPalette() {
	QColor* palette = new QColor[( int )STANDARD_PALETTE_SIZE];
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

void KZColorSelector::drawPalette(QPixmap* pixmap) {
	int xSize = contentsRect().width();
	int ySize = contentsRect().height();
	QImage image(xSize, ySize, 32);
	QColor color;
	int x;
	int y;

	for (y = 0; y < ySize; ++y)
	{
		unsigned int* p = (unsigned int*)image.scanLine(y);
		for(x = 0; x < xSize; ++x)
		{
			setColor(&color, y);
			*p = color.rgb();
			++p;
		}
	}
	if (QColor::numBitPlanes() <= 8)
	{
    QColor* standardPalette = getStandardColorsPalette();
		KImageEffect::dither(image, standardPalette, STANDARD_PALETTE_SIZE);
		delete[] standardPalette;
	}
	pixmap->convertFromImage(image);
}
#include "kzcolorselector.moc"
