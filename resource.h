/***************************************************************************
                          resource.h  -  description
                             -------------------
    begin                : Sat Jul  8 09:57:28 CEST 2000
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

#ifndef RESOURCE_H
#define RESOURCE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

///////////////////////////////////////////////////////////////////
// resource.h  -- contains macros used for commands


///////////////////////////////////////////////////////////////////
// COMMAND VALUES FOR MENUBAR AND TOOLBAR ENTRIES


///////////////////////////////////////////////////////////////////
// File-menu entries
#define ID_FILE_NEW_WINDOW          10010
#define ID_FILE_NEW                 10020
#define ID_FILE_OPEN                10030
#define ID_FILE_OPEN_RECENT         10040
#define ID_FILE_CLOSE               10050

#define ID_FILE_SAVE                10060
#define ID_FILE_SAVE_AS             10070

#define ID_FILE_PRINT               10080

#define ID_FILE_QUIT                10090

///////////////////////////////////////////////////////////////////
// Edit-menu entries
#define ID_EDIT_COPY                11010
#define ID_EDIT_CUT                 11020
#define ID_EDIT_PASTE               11030

///////////////////////////////////////////////////////////////////
// Color menu entries
#define ID_COLOR_FROM_PALETTE        11510
#define ID_COLOR_FROM_SCREEN         11520
#define ID_COLOR_COPY                11530
#define ID_COLOR_PASTE               11540

///////////////////////////////////////////////////////////////////
// View-menu entries                    
#define ID_VIEW_COLOR_NAMES         12010
#define ID_VIEW_TOOLBAR             12020
#define ID_VIEW_STATUSBAR           12030

///////////////////////////////////////////////////////////////////
// Help-menu entries
#define ID_HELP_CONTENTS            1002

///////////////////////////////////////////////////////////////////
// General application values
#define ID_STATUS_MSG               1001

#define IDS_STATUS_DEFAULT          "Ready."

#endif // RESOURCE_H
