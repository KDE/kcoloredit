/***************************************************************************
                          imageselection.cpp  -  description
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

#include "imageselection.h"

ImageSelection::ImageSelection(QWidget *parent, const char *name ) : QWidget(parent,name) {
}
ImageSelection::~ImageSelection(){
}

void ImageSelection::slotSetValue(Color* color) {
	this->color = *color;
}
#include "imageselection.moc"
