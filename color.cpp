/***************************************************************************
                          color.cpp  -  description
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

#include "color.h"

Color::Color(){
	setComponents(0, 0, 0);
	setName("");
}
Color::Color(const int red, const int green, const int blue, const QString& name) {
	setComponents(red, green, blue);
	setName(name);
}
Color::~Color(){
}

void Color::setComponent(const int index, const int value) {
	components[index] = value;
}

void Color::setComponents(const int red, const int green, const int blue) {
	setComponent(RED_INDEX, red);
	setComponent(GREEN_INDEX, green);
	setComponent(BLUE_INDEX, blue);
}

void Color::setName(const QString& name) {
	this->name = name;
}

int Color::getComponent(const int index) const {
	return components[index];
}

int* Color::getComponents() {
	return components;
}

const QString& Color::getName() const {
	return name;
}

bool Color::equals(const Color& color) {
	return getComponent(RED_INDEX) == color.getComponent(RED_INDEX) &&
		getComponent(GREEN_INDEX) == color.getComponent(GREEN_INDEX) &&
		getComponent(BLUE_INDEX) == color.getComponent(BLUE_INDEX);
}

void Color::modifyComponent(const int index, const int value, const double amount) {
	int component = getComponent(index);
	component += (int)(value*amount + 0.5);
	if(component > RGB_MAX_COMPONENT_VALUE)
		component = RGB_MAX_COMPONENT_VALUE;
	else if(component < 0)
		component = 0;
	setComponent(index, component);
}

void Color::modifyComponents(const int red, const int green, const int blue, const double amount) {
	modifyComponent(RED_INDEX, red, amount);
	modifyComponent(GREEN_INDEX, green, amount);
	modifyComponent(BLUE_INDEX, blue, amount);
}

