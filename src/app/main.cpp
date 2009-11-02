/*********************************************************************************
*  Copyright (C) 2009 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>   *
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

#include <KUrl>
#include <KApplication>
#include <KCmdLineArgs>
#include <KAboutData>
#include <KIcon>

int main(int argc, char * argv[])
{
    KAboutData about("kcoloredit", 0, ki18n(I18N_NOOP("KColorEdit")), "2.09.99 (KColorEdit 2.1 RC 2)",
        ki18n(I18N_NOOP("KColorEdit, a tool for editing color palettes.")),
        KAboutData::License_GPL, ki18n("(C) 2009, Percy Camilo Triveño Aucahuasi\n" "(C) 2000, Artur Rataj"));
    about.addAuthor(ki18n("Percy Camilo Triveño Aucahuasi"), ki18n("Current maintainer - Upgraded and developed the new version."), "percy.camilo.ta@gmail.com");
    about.addAuthor(ki18n("Artur Rataj"), ki18n("Former maintainer - Created KColorEdit"), "art@zeus.polsl.gliwice.pl");

    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("+[URL]", ki18n("File to open (Text file with colors and comments items)"));
    KCmdLineArgs::addCmdLineOptions(options);

    KApplication app;
    QApplication::setWindowIcon(KIcon("kcoloredit"));

    KCmdLineArgs * args = KCmdLineArgs::parsedArgs();

    KColorEditMainWnd * kceMainWnd = new KColorEditMainWnd;

    if (args->count())
        kceMainWnd->openFile(args->url(0));

    args->clear();

    kceMainWnd->show();

    return app.exec();
}
