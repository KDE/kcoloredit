/***************************************************************************
                          main.h  -  description
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

#ifndef __MAIN_H__
#define __MAIN_H__

#include <qstring.h>
#include <qdir.h>

/** The maximum red, green or blue component value in RGB scheme
 */
const int RGB_MAX_COMPONENT_VALUE = 255;

/** The maximum hue in HSV scheme
 */
const int HSV_MAX_H_VALUE = 359;

/** The maximum saturation in HSV scheme
 */
const int HSV_MAX_S_VALUE = 255;

/** The maximum value in HSV scheme
 */
const int HSV_MAX_V_VALUE = 255;

/** name of KDE config directories containing palette files
 */
static const QString palettesDir("colors");

/** last open file dialog path
 */
static QString lastOpenPaletteFileDir = QDir::homeDirPath();

/** last save file as dialog path
 */
static QString lastSavePaletteAsFileDir = QDir::homeDirPath();

#endif /* !defined( __MAIN_H__ ) */
