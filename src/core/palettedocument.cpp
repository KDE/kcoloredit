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

PaletteDocument::PaletteDocument(QObject * parent)
    : QObject(parent)
    , m_model(new PaletteModel(this))
    , m_fullPathFile(QString())
{
    connect(m_model, SIGNAL( dataChanged(QModelIndex, QModelIndex) ), this, SLOT( updateDocStateWhenInsertItem(QModelIndex, QModelIndex) ));
    connect(m_model, SIGNAL( rowsRemoved(QModelIndex, int, int) ), this, SLOT( updateDocStateWhenRemoveItem(QModelIndex, int, int) ));
}

PaletteDocument::~PaletteDocument()
{
}

QString PaletteDocument::fullPathFileName() const
{
    return m_fullPathFile;
}

QString PaletteDocument::fileName() const
{
    return m_file;
}

PaletteModel * PaletteDocument::model()
{
    return m_model;
}

#include <kdebug.h>

#include <QTime>

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

    // NOT DEPRECATED i solved the issue with deletion see palettemodel removeRows
    // NOTE this 4 lines are very important
    // always work with 1 only single model X file
    // is to slowyyyyyy proccess the load files if you wrok with one model X app
    if (m_model)
        delete m_model;

    m_model = new PaletteModel(this);

    connect(m_model, SIGNAL( dataChanged(QModelIndex, QModelIndex) ), this, SLOT( updateDocStateWhenInsertItem(QModelIndex, QModelIndex) ));
    connect(m_model, SIGNAL( rowsRemoved(QModelIndex, int, int) ), this, SLOT( updateDocStateWhenRemoveItem(QModelIndex, int, int) ));

    // TOO SLOWWWW load time no wayy
    // NOTE now we simply remove items not delete the model
    //if (m_model->rowCount() > 0)
    //   m_model->removeRows(0, m_model->rowCount());

    line = QString::fromLocal8Bit(paletteFile.readLine());

    QStringList strLst = line.split(": ");

    if (strLst[0] == "Name")
    {
        QString palName = strLst[1];
        palName.remove('\n');

        m_model->setPaletteName(palName);
    }

    int rows = 0;
    int r, g, b;
    int pos = 0;

    QVariantMap vmap;

    while( !paletteFile.atEnd() )
    {
        line = QString::fromLocal8Bit(paletteFile.readLine());

        if (line[0] != '#')
        {
            // This is a color line, hopefully
            line = line.trimmed();

            if (line.isEmpty())
                continue;

            if (sscanf(line.toAscii(), "%d %d %d%n", &r, &g, &b, &pos) >= 3)
            {
                r = qBound(0, r, 255);
                g = qBound(0, g, 255);
                b = qBound(0, b, 255);

                m_model->insertColorRows(rows, 1);

                vmap.insert("type", QString("color"));  // NOTE
                vmap.insert("color", QColor(r, g, b));
                vmap.insert("name", line.mid(pos).trimmed());

                m_model->setData(m_model->index(rows - 1, 0), vmap);
            }
        }
        else
        {
            // This is a comment line
            line = line.mid(1); // Strip '#'
            line = line.trimmed(); // Strip remaining white space..

            if (!line.isEmpty())
            {
                m_model->insertCommentRows(rows, 1);

                vmap.insert("type", QString("comment"));  // NOTE
                vmap.insert("comment", line);

                m_model->setData(m_model->index(rows - 1, 0), vmap);
            }
        }

        rows++;
    }

    m_fullPathFile = fileName;
    m_file = m_fullPathFile.split("/")[m_fullPathFile.split("/").count() - 1];

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

    bool finalize = sf.finalize();

    if (!finalize)
    {
        m_lastErrorString = sf.errorString();

        return false;
    }

    m_fullPathFile = fileName;
    m_file = m_fullPathFile.split("/")[m_fullPathFile.split("/").count() - 1];

    return true;
}

QString PaletteDocument::lastErrorString() const
{
    return m_lastErrorString;
}

void PaletteDocument::updateDocStateWhenInsertItem(const QModelIndex & /* topLeft */, const QModelIndex & /* bottomRight */)
{
    emit modified();
}

void PaletteDocument::updateDocStateWhenRemoveItem(const QModelIndex & /* parent */, int /* start */, int /* end */)
{
    emit modified();
}

#include "palettedocument.moc"
