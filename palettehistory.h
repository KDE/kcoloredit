/***************************************************************************
                          palettehistory.h  -  description
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

#ifndef __PALETTE_HISTORY_H__
#define __PALETTE_HISTORY_H__

#include "editablestreamhistory.h"

/** Definition of PaletteHistory type */
typedef EditableStreamHistory<Palette> PaletteHistory;

#endif /* !defined( __PALETTE_HISTORY_H__ ) */
