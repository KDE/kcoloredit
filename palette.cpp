/***************************************************************************
                          palette.cpp  -  description
                             -------------------
    begin                : Sat Jul 8 2000
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

#include <qstring.h>
#include <qstringlist.h>
#include <qtextstream.h>
#include <qregexp.h>
#include <kglobal.h>
#include <kstandarddirs.h>
#include <klocale.h>

#include "main.h"
#include "color.h"
#include "palette.h"

Palette::Palette() {
	init();
}
Palette::Palette(const Palette& palette) {
	init();
	for(int colorIndex = 0; colorIndex < palette.length(); ++colorIndex) {
		Color* color = new Color(*( (Palette&)palette ).color( colorIndex ));
		append(color);
	}
	setName(palette.name());
}
Palette::~Palette() {
}

void Palette::init() {
	colors.setAutoDelete(true);
}

QStringList Palette::kdePalettes() {
	QStringList paletteList;
	KGlobal::dirs()->findAllResources("config", palettesDir + "/*", false, true, paletteList);
	return paletteList;
}

void Palette::setName(const QString& name) {
	m_name = name;
}

const QString& Palette::name() const {
	return m_name;
}

void Palette::insert(const int index, Color* const color) {
	colors.insert(index, color);
}

void Palette::append(Color* const color) {
	colors.append(color);
}

void Palette::remove(const int index) {
	colors.remove(index);
}

int Palette::length() const {
	return (int)colors.count();
}

Color* Palette::color(const int index) {
	return colors.at(index);
}

Palette Palette::copy(const int index, const int length) {
	Palette newPalette;
	for(int colorIndex = index; colorIndex < index + length; ++colorIndex)
		newPalette.append(new Color( *color(colorIndex) ));
	newPalette.setName(name());
	return newPalette;
}

Palette Palette::cut(const int index, const int length) {
	Palette newPalette;
	for(int colorNum = 0; colorNum < length; ++colorNum) {
		newPalette.append(new Color( *color(index) ));
		remove(index);
	}
	newPalette.setName(name());
	return newPalette;
}

void Palette::paste(const int index, Palette& palette) {
	for(int colorIndex = 0; colorIndex < palette.length(); ++colorIndex)
		insert(index + colorIndex, new Color( *palette.color(colorIndex) ));
}

bool Palette::load(QTextStream& stream, bool loadName /* = true */) {
	bool result = true;
	setName("KDE palette");
	int lineNum = 0;
	while (!stream.atEnd()) {
		QString string = stream.readLine().append(' ');
		if(string.find( QRegExp("[^\\s]") ) == -1 ||
			string.stripWhiteSpace().at( 0 ) == '#' ||
			( loadName && lineNum == 0 )) {
			if(loadName && lineNum == 0)
				setName(string.stripWhiteSpace());
		} else {
			Color* newColor = new Color();
			int position = string.find(QRegExp( "[^\\s]" ));
			for(int componentIndex = 0; componentIndex < Color::COMPONENTS_NUM;
				++componentIndex) {
				if(position == -1) {
					m_errorString = i18n("Invalid format");
					result = false;
					break;
				}
				int endPosition = string.find(QRegExp( "\\s" ), position);
				if(endPosition == -1) {
					m_errorString = i18n("Invalid format");
					result = false;
					break;
				}
				QString componentString = string.mid(position, endPosition - position);
				int componentValue = componentString.toInt(&result);
				if(!result ||
					componentValue < 0 ||
					componentValue > RGB_MAX_COMPONENT_VALUE) {
					m_errorString = i18n("Invalid format");
					result = false;
					break;
				}
				newColor->setComponent(componentIndex, componentValue);
				position = string.find(QRegExp( "[^\\s]" ), endPosition);
			}
			if(!result) {
				delete newColor;
				break;
			}
			if(position != -1)
				newColor->setName(string.mid( position ).stripWhiteSpace());
			colors.append(newColor);
		}
		++lineNum;
	}
	if(!result)
		deleteContents();
	return result;
}

bool Palette::load(const QString& fileName) {
	bool result = true;
	QFile file(fileName);
	if(!file.open( IO_ReadOnly )) {
		m_errorString = i18n("Could not open file");
		result = false;
	} else {
		QTextStream stream(&file);
		result = load(stream);
		file.close();
	}
	return result;
}

bool Palette::save(QTextStream& stream, const QFile* file /* = 0 */,
	bool saveName /* = true */) {
	bool result = true;
	if(saveName)
		stream << name() + QString("\n");
	if(file && file->status() != IO_Ok) {
		m_errorString = i18n("Write error");
		result = false;
	} else
		for(int colorIndex = 0; colorIndex < length(); ++colorIndex) {
			Color* col = color(colorIndex);
			QString redComponentString;
			QString greenComponentString;
			QString blueComponentString;
			redComponentString.setNum(col->component( Color::RED_INDEX ));
			greenComponentString.setNum(col->component( Color::GREEN_INDEX ));
			blueComponentString.setNum(col->component( Color::BLUE_INDEX ));
			QString nameString = col->name();
			if(!nameString.isEmpty())
				nameString.prepend(" ");
			stream << redComponentString + QString(" ") +
				greenComponentString + QString(" ") +
				blueComponentString +
				nameString + QString("\n");
			if(file && file->status() != IO_Ok) {
				m_errorString = i18n("Write error");
				result = false;
				break;
			}
		}
	return result;
}

bool Palette::save(const QString& fileName) {
	bool result = true;
	QFile file(fileName);
	if(!file.open( IO_WriteOnly|IO_Truncate )) {
		m_errorString = i18n("Could not open file for writing");
		result = false;
	} else {
		QTextStream stream(&file);
		result = save(stream);
		file.close();
	}
	return result;
}

void Palette::deleteContents() {
	colors.clear();
}

const QString& Palette::errorString() const {
	return m_errorString;
}
