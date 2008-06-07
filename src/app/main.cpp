/*********************************************************************************
*  Copyright (C) 2008 by Percy Camilo Triveño Aucahuasi <orgyforever@gmail.com>  *
*                                                                                *
*  This program is free software; you can redistribute it and/or modify          *
*  it under the terms of the GNU General Public License as published by          *
*  the Free Software Foundation; either version 2 of the License, or             *
*  (at your option) any later version.                                           *
*                                                                                *
*  This program is distributed in the hope that it will be useful,               *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of                *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 *
*  GNU General Public License for more details.                                  *
*                                                                                *
*  You should have received a copy of the GNU General Public License             *
*  along with this program; if not, write to the                                 *
*  Free Software Foundation, Inc.,                                               *
*  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.                 *
*********************************************************************************/

#include "kcoloredit.h"

#include <KApplication>
#include <KCmdLineArgs>
#include <KAboutData>

#include <KIcon>

inline KAboutData kcoloreditAboutData( const char* name, const char* iname )
{
    KAboutData about(name, "KColorEdit", ki18n(iname), "1.60.95 (KColorEdit 1.7 RC2)",
        ki18n(I18N_NOOP("KColorEdit, great tool for editing color palettes.")),
        KAboutData::License_GPL, ki18n("(C) 2008, Percy Camilo Triveño Aucahuasi\n" "(C) 2000, Artur Rataj"));
    about.addAuthor(ki18n("Percy Camilo Triveño Aucahuasi"), ki18n("Current maintainer - Upgraded KColorEdit to KDE 4"), "orgyforever@gmail.com");
    about.addAuthor(ki18n("Artur Rataj"), ki18n("Former maintainer - Created KColorEdit"), "art@zeus.polsl.gliwice.pl");

    return about;
}

int main(int argc, char * argv[])
{
    KAboutData about = kcoloreditAboutData("kcoloredit", I18N_NOOP("KColorEdit"));

    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("+[File]", ki18n("File to open"));
    KCmdLineArgs::addCmdLineOptions(options);

    KApplication app;
    QApplication::setWindowIcon(KIcon("kcoloredit"));

    // check if we are starting with session management
    if (app.isSessionRestored())
    {
        RESTORE(KColorEditMainWnd);
    }
    else
    {
        KColorEditMainWnd * kceWindow = new KColorEditMainWnd();
        kceWindow->show();

        KCmdLineArgs * args = KCmdLineArgs::parsedArgs();

        if (args->count())
        {
        //kcoloredit->openDocumentFile(args->arg(0).toLocal8Bit().data());
        }
        else
        {
          //kcoloredit->openDocumentFile();
        }

        args->clear();
    }

    return app.exec();
}
