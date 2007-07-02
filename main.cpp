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
#include <kdeversion.h>
#include <klocale.h>
#include <kdebug.h>

#include "kcoloredit.h"

static const char description[] =
	I18N_NOOP("KColorEdit");
// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE


int main(int argc, char *argv[])
{

	KAboutData aboutData( "kcoloredit", 0, ki18n("KColorEdit"),
		KDE_VERSION_STRING, ki18n(description), KAboutData::License_GPL,
		ki18n("(c) 2000, Artur Rataj"));
	aboutData.addAuthor(ki18n("Artur Rataj"),KLocalizedString(), "art@zeus.polsl.gliwice.pl");
    aboutData.addCredit( ki18n("Nadeem Hasan"), ki18n( "Rewrote UI code "
        "to be KDE standards compliant" ), "nhasan@kde.org" );
	KCmdLineArgs::init( argc, argv, &aboutData );

	KCmdLineOptions options;
	options.add("+[File]", ki18n("File to open"));
	KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

  KApplication app;

  if (app.isSessionRestored())
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
        kcoloredit->openDocumentFile(args->arg(0).toLocal8Bit().data());
		}
		else
		{
		  kcoloredit->openDocumentFile();
		}
		args->clear();
  }

  return app.exec();
}
