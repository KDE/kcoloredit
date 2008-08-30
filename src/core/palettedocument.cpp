/*********************************************************************************
*  Copyright (C) 2008 by Percy Camilo Triveño Aucahuasi <orgyforever@gmail.com>  *
*                                                                                *
*  openPaletteFile(const QString & fileName):                                    *
*  Is based on kdeui/colors/kcolordialog                                         *
*  Copyright (C) 1999 Waldo Bastian (bastian@kde.org)                            *
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

#include <QtCore/QTextStream>

#include <KSaveFile>
#include <KLocalizedString>

#include "palettemodel.h"

PaletteDocument::PaletteDocument(QObject * parent)
    : QObject(parent)
    , m_model(new PaletteModel(this))
    , m_fullPathFile(QString())
    , m_documentType(PaletteDocument::KDEType)
{
    connect(m_model, SIGNAL( dataChanged(QModelIndex, QModelIndex) ), SLOT( updatePaletteDocument() ));
    connect(m_model, SIGNAL( rowsRemoved(QModelIndex, int, int)    ), SLOT( updatePaletteDocument() ));
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

bool PaletteDocument::openPaletteFile(const QString & fileName)
{
    QFile file(fileName);

    if (!file.exists())
    {
        m_lastErrorString = i18n("File not found");

        return false;
    }

    if (!file.open(QIODevice::ReadOnly))
    {
        m_lastErrorString = i18n("Could not open the file");

        return false;
    }

    QString line;

    // Read first line
    // Expected "GIMP Palette" or "KDE RGB Palette"
    line = QString::fromLocal8Bit(file.readLine());

    if (line.indexOf(" Palette") == -1)
    {
        m_lastErrorString = i18n("Is not a palette file");

        return false; 
    }

    if (line.indexOf("KDE") == -1)
        m_documentType = PaletteDocument::KDEType;

    if (line.indexOf("GIMP") == -1)
        m_documentType = PaletteDocument::GIMPType;

    // NOT DEPRECATED i solved the issue with deletion see palettemodel removeRows
    // NOTE this 4 lines are very important
    // always work with 1 only single model X file
    // is to slowyyyyyy proccess the load files if you wrok with one model X app
    if (m_model)
        delete m_model;

    m_model = new PaletteModel(this);

    connect(m_model, SIGNAL( dataChanged(QModelIndex, QModelIndex) ), SLOT( updatePaletteDocument() ));
    connect(m_model, SIGNAL( rowsRemoved(QModelIndex, int, int)    ), SLOT( updatePaletteDocument() ));

    // TOO SLOWWWW load time no wayy
    // NOTE now we simply remove items not delete the model
    //if (m_model->rowCount() > 0)
    //   m_model->removeRows(0, m_model->rowCount());

    int r, g, b;
    int pos = 0;

    // TODO last improve here

    while( !file.atEnd() )
    {
        line = QString::fromLocal8Bit(file.readLine());

        // NOTE looking for the palette name
        if (line[0] == 'N')
        {
            QStringList strLst = line.split(": ");

            if (strLst[0] == "Name")
            {
                QString palName = strLst[1];
                palName.remove('\n');

                m_model->setPaletteName(palName);
            }
        }

                // NOTE looking for columns
        if (line[0] == 'C')
        {
            QStringList strLst = line.split(": ");

            if (strLst[0] == "Columns")
            {
                QString palName = strLst[1];
                palName.remove('\n');

                m_model->setPreferredPaletteColumns(palName.toInt());
            }
        }

        //WARNING inefficient

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

                m_model->appendColorItem(QColor(r, g, b), line.mid(pos).trimmed());
            }
        }
        else
        {
            // This is a comment line
            line = line.mid(1); // Strip '#'
            line = line.trimmed(); // Strip remaining white space..

            //if (!line.isEmpty())
            m_model->appendCommentItem(line);
        }
    }

    extractDescriptionFromModel();

    m_fullPathFile = fileName;
    m_file = m_fullPathFile.split('/')[m_fullPathFile.split('/').count() - 1];

    file.close();

    return true;
}

bool PaletteDocument::saveFileAs(const QString & fileName)
{
    KSaveFile saveFile(fileName);

    if (!saveFile.open())
    {
        m_lastErrorString = saveFile.errorString();

        return false;
    }

    QTextStream textStream(&saveFile);

    switch (m_documentType)
    {
        case PaletteDocument::KDEType:
            textStream << "KDE RGB Palette" << endl;
            break;
        case PaletteDocument::GIMPType:
            textStream << "GIMP Palette" << endl;
            break;
    }

    textStream << "Name: " << m_model->paletteName() << endl;

    // NOTE write the description (first comments before the first color ocurrence)
    QStringList tmpDescription = m_model->paletteDescription().split('\n');

    for(int i = 0; i < tmpDescription.count(); i++)
        textStream << "#" << tmpDescription[i] << endl;

    // NOTE write the data (color and comment items)
    for(int i = 0; i < m_model->rowCount(); i++)
    {
        PaletteItem::ItemType itemType = m_model->itemType(i);

        if (itemType == PaletteItem::ColorType)
        {
            QColor color = m_model->colorItem(i).color();

            textStream << color.red() << " " << color.green() << " " << color.blue() << " " << m_model->colorItem(i).colorName() << endl;
        }

        if (itemType == PaletteItem::CommentType)
            textStream << "#" << m_model->commentItem(i).comment() << endl;
    }

    saveFile.flush();

    bool finalize = saveFile.finalize();

    if (!finalize)
    {
        m_lastErrorString = saveFile.errorString();

        return false;
    }

    m_fullPathFile = fileName;
    m_file = m_fullPathFile.split('/')[m_fullPathFile.split('/').count() - 1];

    return true;
}

PaletteDocument::DocumentType PaletteDocument::type() const
{
    return m_documentType;
}

QString PaletteDocument::lastErrorString() const
{
    return m_lastErrorString;
}

void PaletteDocument::updatePaletteDocument()
{
    emit modified();
}

void PaletteDocument::extractDescriptionFromModel()
{
    QStringList comments;

    int j = 0;

    for (int i = 0; i < m_model->rowCount(); i++)
    {
        if (m_model->itemType(i) == PaletteItem::CommentType)
        {
            j++;

            // NOTE the last comment not belong to description
            if (i != (j - 1))
                break;

            comments.append(m_model->commentItem(i).comment());
        }
    }

    QString tmpDescription;

    if (comments.count() > 0)
    {
        for (int i = 0; i < comments.count() - 1; i++)
            tmpDescription += comments[i] + '\n';

        tmpDescription += comments[comments.count() - 1];
    }

    m_model->setPaletteDescription(tmpDescription);

    // NOTE Remove the comment items that belongs to the description
    // commentsToRemove equals to first comments before the first color item ocurrence
    int commentsToRemove;

    for (int i = 0; i < m_model->rowCount(); i++)
    {
        if (m_model->itemType(i) != PaletteItem::CommentType)
        {
            commentsToRemove = i;
            break ;
        }
    }

    if (commentsToRemove != 0)
        m_model->removeRows(0, commentsToRemove);
}

#include "palettedocument.moc"
