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
	m_components[index] = value;
}

void Color::setComponents(const int red, const int green, const int blue) {
	setComponent(RED_INDEX, red);
	setComponent(GREEN_INDEX, green);
	setComponent(BLUE_INDEX, blue);
}

void Color::setName(const QString& name) {
	m_name = name;
}

int Color::component(const int index) const {
	return m_components[index];
}

const int* Color::components() const{
	return m_components;
}

const QString& Color::name() const {
	return m_name;
}

bool Color::equals(const Color& color) {
	return component(RED_INDEX) == color.component(RED_INDEX) &&
		component(GREEN_INDEX) == color.component(GREEN_INDEX) &&
		component(BLUE_INDEX) == color.component(BLUE_INDEX);
}

void Color::modifyComponent(const int index, const int value, const double amount) {
	int comp = component(index);
	comp += (int)(value*amount + 0.5);
	if(comp > RGB_MAX_COMPONENT_VALUE)
		comp = RGB_MAX_COMPONENT_VALUE;
	else if(comp < 0)
		comp = 0;
	setComponent(index, comp);
}

void Color::modifyComponents(const int red, const int green, const int blue, const double amount) {
	modifyComponent(RED_INDEX, red, amount);
	modifyComponent(GREEN_INDEX, green, amount);
	modifyComponent(BLUE_INDEX, blue, amount);
}

