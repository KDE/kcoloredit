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

#include "palettedialog.h"

#include <QtCore/QStringList>
#include <QtGui/QMouseEvent>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>

#include <KLocalizedString>
#include <KStandardDirs>
#include <KColorCollection>
#include <KCursor>
#include <KTabWidget>
#include <kfilewidget.h> // no sale en el include de KDE

//BEGIN public class CollectionGrid

CollectionGrid::CollectionGrid(const QString & collection, QWidget * parent)
    : KColorCells(parent, 0, 0)
    , m_collection(collection)
    , m_isEmpty(true)
{
    setCursor(KCursor("hand2"));

    KColorCollection colorCollection(collection);

    int columns = 4; // default value
    int rows = colorCollection.count() / columns;

    // NOTE
    // becouse some times custom and recent colors start with only
    // 1,2,3 or 4 colors and if the row of the colorcell is 0
    // then custon and recent colors will not show it
    if (rows == 0)
        rows = 1;

    if (colorCollection.count() > 0)
        m_isEmpty = false;

    setRowCount(rows);
    setColumnCount(columns);

    for (int i = 0; i < count(); i++)
        setColor(i, colorCollection.color(i));

    setMaximumWidth(96); // 96*7=672 so the minimum resolution is more than 672 px (width)
}

bool CollectionGrid::isEmpty() const
{
    return m_isEmpty;
}

void CollectionGrid::mousePressEvent(QMouseEvent * event)
{
    if ( event->button() == Qt::LeftButton)
        emit collectionClicked(m_collection);
}

//END public class CollectionGrid

//BEGIN public class CollectionsWidget

// this construct need a lot of explanation .. hehehe i think :P
CollectionsWidget::CollectionsWidget(QWidget * parent) : QWidget(parent)
{
    QString emptyStr = '\n' + i18n("[Empty]");

    QHBoxLayout * layout = new QHBoxLayout(this);

    QStringList collections = KColorCollection::installedCollections();

    for (int i = 0; i < collections.count(); i++)
    {
        m_headers.append(new QLabel(collections[i], this));
        m_collections.append(new CollectionGrid(collections[i], this));

        if (m_collections[i]->isEmpty())
            m_headers[i]->setText(m_headers[i]->text() + emptyStr);

        QVBoxLayout * collectionLayout = new QVBoxLayout();
        collectionLayout->addWidget(m_headers[i]);
        collectionLayout->addWidget(m_collections[i]);

        layout->addLayout(collectionLayout);

        connect(m_collections[i], SIGNAL( collectionClicked(QString) ), SLOT( chooseCollection(QString) ));
    }
}

CollectionsWidget::~CollectionsWidget()
{
    while (!m_headers.isEmpty())
    {
        QLabel * tmpLabel = m_headers.takeFirst();

        if (tmpLabel)
            delete tmpLabel;

        tmpLabel = 0;
    }

    while (!m_collections.isEmpty())
    {
        CollectionGrid * tmpCollectionGrid = m_collections.takeFirst();

        if (tmpCollectionGrid)
            delete tmpCollectionGrid;

        tmpCollectionGrid = 0;
    }
}

void CollectionsWidget::chooseCollection(const QString & collection)
{
    selectedCollection(collection);
}

//END public class CollectionsWidget

//BEGIN public class PaletteDialog

QString PaletteDialog::filter()
{
    return QString("*.colors *.gpl|") + i18n("All Supported Files") +
        QString("\n*.colors|") + i18n("KDE Palette") + QString(" (*.colors)") +
        QString("\n*.gpl|") + i18n("GIMP Palette") + QString(" (*.gpl)");
}

KUrl PaletteDialog::getOpenUrl(QWidget * parent)
{
    PaletteDialog paletteDialog(parent);

    KUrl tmpUrl;

    if (paletteDialog.exec() == QDialog::Accepted)
    {
        tmpUrl = paletteDialog.paletteUrl();

        if (paletteDialog.isKdePalette())
            tmpUrl = KUrl(KStandardDirs::locate("config", "colors/"+ tmpUrl.path()));
    }

    return tmpUrl;
}

PaletteDialog::PaletteDialog(QWidget * parent, Qt::WFlags flags) : KDialog(parent, flags), m_isKdePalette(false)
{
    setCaption(i18n("Load Palette"));
    setButtons(Cancel);
    setDefaultButton(Cancel);
    setModal( true );
    resize(700, 400);

    m_mainWidget = new KTabWidget(this);

    m_collectionsWidget = new CollectionsWidget(m_mainWidget);

    m_fileWidget = new KFileWidget(KUrl(QDir::homePath()), m_mainWidget);
    m_fileWidget->setMode(KFile::ExistingOnly | KFile::LocalOnly);
    m_fileWidget->setOperationMode(KFileWidget::Opening);

    QString allSupportedStr = i18n("All Supported Files");
    QString kdePaletteStr = i18n("KDE Palette");
    QString gimpPaletteStr = i18n("GIMP Palette");

    m_fileWidget->setFilter(PaletteDialog::filter());

    m_mainWidget->addTab(m_collectionsWidget, KIcon("kde"), i18n("KDE Palettes"));
    m_mainWidget->addTab(m_fileWidget, KIcon("edit-find"), i18n("Search Palettes"));

    setMainWidget(m_mainWidget);

    connect(m_mainWidget, SIGNAL( currentChanged(int)), SLOT( updateDialogBtns(int) ));
    connect(m_collectionsWidget, SIGNAL( selectedCollection(QString) ), SLOT( selectKdePalette(QString) ));
    connect(m_fileWidget, SIGNAL( accepted() ), SLOT( selectUrl() ));
    connect(this, SIGNAL( okClicked() ), m_fileWidget, SLOT( slotOk() ));
    connect(this, SIGNAL( okClicked() ), SLOT( selectUrl() ));
}

PaletteDialog::~PaletteDialog()
{
}

KUrl PaletteDialog::paletteUrl() const
{
    return m_paletteUrl;
}

bool PaletteDialog::isKdePalette() const
{
    return m_isKdePalette;
}

//BEGIN public slots

void PaletteDialog::updateDialogBtns(int tabIndex)
{
    // KDE palettes
    if (tabIndex == 0)
    {
        setButtons(Cancel);
        m_isKdePalette = true;
    }

    // local palettes
    if (tabIndex == 1)
    {
        setButtons(Ok | Cancel);
        m_isKdePalette = false;
    }
}

void PaletteDialog::selectKdePalette(const QString & kdePalette)
{
    m_isKdePalette = true;

    m_paletteUrl = KUrl(kdePalette);

    // Return to acepted and close the dialog
    accept();
}

void PaletteDialog::selectUrl()
{
    m_isKdePalette = false;

    // Fill the filename field of KFileWidget
    m_fileWidget->accept();

    m_paletteUrl = m_fileWidget->selectedUrl();

    accept();
}

//END public slots

//END public class PaletteDialog

#include "palettedialog.moc"
