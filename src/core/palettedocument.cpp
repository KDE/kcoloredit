/*********************************************************************************
*  Copyright (C) 2008 by Percy Camilo Triveño Aucahuasi <orgyforever@gmail.com>  *
*  openPaletteFile(const QString & fileName):                                    *
*  Is based on kdeui: Copyright (C) 1999 Waldo Bastian (bastian@kde.org)         *
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

#include "palettedocument.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <KSaveFile>
#include <KLocalizedString>

#include "palettemodel.h"

PaletteDocument::PaletteDocument(QObject * parent) : QObject(parent), m_model(new PaletteModel(this)), m_file(QString())
{
}

PaletteDocument::~PaletteDocument()
{
}

QString PaletteDocument::fileName() const
{
    return m_file;
}

PaletteModel * PaletteDocument::model()
{
    return m_model;
}

void PaletteDocument::setModel(PaletteModel * model)
{
    m_model = model;
}

bool PaletteDocument::openPaletteFile(const QString & fileName)
{
    QFile paletteFile(fileName);
    paletteFile.open(QIODevice::ReadOnly);

    QString line;

    // Read first line
    // Expected "GIMP Palette"
    line = QString::fromLocal8Bit(paletteFile.readLine());

    if (line.indexOf(" Palette") == -1)
    {
        m_lastErrorString = i18n("Is not a palette file");

        return false; 
    }

    m_file = fileName;

    // NOTE this 4 lines are very important
    // always work with 1 only single model X file
    // is to slowyyyyyy proccess the load files if you wrok with one model X app
    if (m_model)
        delete m_model;

    m_model = new PaletteModel(this);

    line = QString::fromLocal8Bit(paletteFile.readLine());

    QStringList strLst = line.split(": ");

    if (strLst[0] == "Name")
    {
        QString palName = strLst[1];
        palName.remove('\n');

        m_model->setPaletteName(palName);
    }

    while( !paletteFile.atEnd() )
    {
        line = QString::fromLocal8Bit(paletteFile.readLine());

        if (line[0] != '#')
        {
            // This is a color line, hopefully
            line = line.trimmed();

            if (line.isEmpty())
                continue;

            int r, g, b;
            int pos = 0;

            if (sscanf(line.toAscii(), "%d %d %d%n", &r, &g, &b, &pos) >= 3)
            {
                r = qBound(0, r, 255);
                g = qBound(0, g, 255);
                b = qBound(0, b, 255);

                QColor color(r, g, b);
                QString colorName = line.mid(pos).trimmed();

                m_model->insertColorRows(m_model->rowCount(), 1);

                QVariantMap vmap;
                vmap.insert("type", QString("color"));  // NOTE
                vmap.insert("color", color);
                vmap.insert("name", colorName);

                m_model->setData(m_model->index(m_model->rowCount() - 1, 0), vmap);
            }
        }
        else
        {
            // This is a comment line
            line = line.mid(1); // Strip '#'
            line = line.trimmed(); // Strip remaining white space..
            if (!line.isEmpty())
            {
                m_model->insertCommentRows(m_model->rowCount(), 1);

                QVariantMap vmap;
                vmap.insert("type", QString("comment"));  // NOTE
                vmap.insert("comment", line);

                m_model->setData(m_model->index(m_model->rowCount() - 1, 0), vmap);
            }
        }
    }

    return true;
}

bool PaletteDocument::saveFileAs(const QString & fileName)
{
    KSaveFile sf(fileName);

    if (!sf.open())
    {
        m_lastErrorString = sf.errorString();

        return false;
    }

    QTextStream str(&sf);

    str << "KDE RGB Palette\n";
    str << "Name: " << m_model->paletteName() << "\n";

    QVariantMap vmap;

    for(int i = 0; i < m_model->rowCount(); i++)
    {
        vmap = m_model->index(i, 0).data().toMap();

        if (vmap.value("type").toString() == QString("color"))
        {
            QColor color = vmap.value("color").value<QColor>();
            QString colorName = vmap.value("name").toString();

            str << color.red() << " " << color.green() << " " << color.blue() << " " << colorName << "\n";
        }

        if (vmap.value("type").toString() == QString("comment"))
        {
            QString comment = vmap.value("comment").toString();

            str << "#" << comment << "\n";
        }
    }

    sf.flush();

    m_file = fileName;

    bool finalize = sf.finalize();

    if (!finalize)
        m_lastErrorString = sf.errorString();

    return finalize;
}

QString PaletteDocument::lastErrorString() const
{
    return m_lastErrorString;
}

#include "palettedocument.moc"
