/***************************************************************************
                          palette.h  -  description
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

#ifndef PALETTE_H
#define PALETTE_H

#include <qstring.h>
#include <qtextstream.h>
#include <qfile.h>

#include "color.h"

/**This class holds a palette.
  *@author Artur Rataj
  */
class Palette {
public:
	/** Constructs an empty palette */
	Palette();
	/** The copy constructor */
	Palette(const Palette& palette);
	~Palette();
	/** @return A list of KDE palettes */
	static QStringList kdePalettes();
	/** sets palette name */
	void setName(const QString& name);
	/** @return palette name */
	const QString& name() const;
	/** inserts a color at a given position */
	void insert(const int index, Color* const color);
	/** appends a color */
	void append(Color* const color);
	/** removes a color at index */
	void remove(const int index);
	/** @return the number of colors */
	int length() const;
	/** @return color at index */
	Color* color(const int index);
	/** @return a copy of palette at index, of length length */
	Palette copy(const int index, const int length);
	/** cuts a palette at index, of length length
	 *  @return a palette that has been cut out
	 */
	Palette cut(const int index, const int length);
	/** pastes a palette at index */
	void paste(const int index, Palette& palette);
	/** Loads a palette from a text stream
	 *  @return whether the load was succesfull
	 */
	bool load(QTextStream& stream, bool loadName = true);
	/** Loads a palette from a file.
	 *  If loadName is true, palette name is expected.
	 *  @return whether the load was succesfull
	 */
	bool load(const QString& fileName);
	/** Saves a palette into a text stream.
	 *  If file is given, it is checked for IO errors.
	 *  If saveName is true, palette name is saved.
	 *  @return whether save was succesfull
	 */
	bool save(QTextStream& stream, const QFile* file = 0, bool saveName = true);
	/** Saves a palette to a file
	 *  @return whether save was succesfull
	 */
	bool save(const QString& fileName);
	/** Deletes contents of the palette */
	void deleteContents();
	/** @return A possible error description from the last unsuccessfull
	 *  IO operation
	 */
	const QString& errorString() const;

private:
	/** The palette name */
	QString m_name;

private:
	/** Initialization method called by constructors */
	void init();

protected:
	/** A list of palette colors */
	QPtrList<Color> colors;
	/** An IO error description */
	QString m_errorString;
};

#endif
