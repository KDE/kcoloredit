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

#include <QApplication>
#include <QCommandLineParser>
#include <QUrl>

#include <KAboutData>

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("kcoloredit");

    KAboutData about(QLatin1Literal("kcoloredit"),
                     i18n("KColorEdit"),
                     "2.09.99 (KColorEdit 2.1 RC 2)",
                     i18n("KColorEdit, a tool for editing color palettes."),
                     KAboutLicense::GPL_V2,
                     i18n("(C) 2009, Percy Camilo Triveño Aucahuasi\n" "(C) 2000, Artur Rataj"),
                     QLatin1String(""),
                     QLatin1String("https://userbase.kde.org/KColorEdit")
                     );

    about.addAuthor(i18n("Percy Camilo Triveño Aucahuasi"), i18n("Current maintainer - Upgraded and developed the new version."), "percy.camilo.ta@gmail.com");
    about.addAuthor(i18n("Artur Rataj"), i18n("Former maintainer - Created KColorEdit"), "art@zeus.polsl.gliwice.pl");

    QCommandLineParser parser;

    KAboutData::setApplicationData(about);
    about.setupCommandLine(&parser);

    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("+[URL]"), i18n("File to open (Text file with colors and comments items)")));

    parser.process(app);
    about.processCommandLine(&parser);

    QApplication::setWindowIcon(QIcon::fromTheme("kcoloredit"));

    KColorEditMainWnd * kceMainWnd = new KColorEditMainWnd;

    const QStringList urls = parser.positionalArguments();
    if (urls.count())
        kceMainWnd->openFile(QUrl::fromUserInput(urls.at(0)));

    kceMainWnd->show();

    return app.exec();
}
