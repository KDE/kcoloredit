/***************************************************************************
                          main.cpp  -  description
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

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>

#include "kcoloredit.h"

static const char *description =
	I18N_NOOP("KColorEdit");
// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE
	
	
static KCmdLineOptions options[] =
{
  { "+[File]", I18N_NOOP("file to open"), 0 },
  { 0, 0, 0 }
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};

int main(int argc, char *argv[])
{

	KAboutData aboutData( "kcoloredit", I18N_NOOP("KColorEdit"),
		VERSION, description, KAboutData::License_GPL,
		"(c) 2000, Artur Rataj");
	aboutData.addAuthor("Artur Rataj",0, "art@zeus.polsl.gliwice.pl");
	KCmdLineArgs::init( argc, argv, &aboutData );
	KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

  KApplication app;
 
  if (app.isRestored())
  {
    RESTORE(KColorEditApp);
  }
  else 
  {
    KColorEditApp *kcoloredit = new KColorEditApp();
    kcoloredit->show();

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
		
		if (args->count())
		{
        kcoloredit->openDocumentFile(args->arg(0));
		}
		else
		{
		  kcoloredit->openDocumentFile();
		}
		args->clear();
  }

  return app.exec();
}  
