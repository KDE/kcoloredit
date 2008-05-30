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

#include "palettebriefview.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSlider>
#include <QtGui/QLabel>

#include <KLocalizedString>
#include <KColorScheme>
#include <KPushButton>
#include <KColorCells>

#include "palettemodel.h"

PaletteGridView::PaletteGridView(PaletteModel * model, QWidget * parent) : QWidget(parent)
{
    m_model = model;

    m_paletteTitleLabel = new QLabel(i18n("Palette"), this);
    m_paletteTitleLabel->setAlignment(Qt::AlignCenter);

    m_setColumnSlider = new QSlider(Qt::Horizontal, this);
    m_setColumnSlider->setRange(1, 20);
    m_setColumnSlider->setSingleStep(1);
    m_setColumnSlider->setPageStep(1);
    m_setColumnSlider->setTickInterval(1);
    m_setColumnSlider->setValue(1); // NOTE check this default value

    m_zoomOutButton = new KPushButton(KIcon(QString("zoom-out")), QString(), this);

    m_zoomInButton = new KPushButton(KIcon(QString("zoom-in")), QString(), this);

    m_colorCells = new KColorCells(this, 0, 1);
    m_colorCells->setMouseTracking(true);
    m_colorCells->setMinimumWidth(256);
    m_colorCells->setAcceptDrags(false); // TODO
    m_colorCells->setAcceptDrops(false); // TODO
    m_colorCells->setSelectionMode(QAbstractItemView::ContiguousSelection);

    QHBoxLayout * layoutHeader = new QHBoxLayout();
    layoutHeader->addWidget(m_paletteTitleLabel);

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addLayout(layoutHeader);
    layout->addWidget(m_colorCells);

    QHBoxLayout * layoutZoom = new QHBoxLayout();
    layoutZoom->addWidget(m_zoomOutButton);
    layoutZoom->addWidget(m_setColumnSlider);
    layoutZoom->addWidget(m_zoomInButton);

    layout->addLayout(layoutZoom);

    connect(m_model, SIGNAL( dataChanged(QModelIndex, QModelIndex) ), this, SLOT( updateWhenInsertItem(QModelIndex, QModelIndex) ));
    connect(m_model, SIGNAL( rowsRemoved(QModelIndex, int, int) ), this, SLOT( updateWhenRemoveItem(QModelIndex, int, int) ));

    connect(m_setColumnSlider, SIGNAL( valueChanged(int) ), this, SLOT( setZoomFactor(int) ));
    connect(m_zoomOutButton, SIGNAL( pressed () ), this, SLOT( zoomOut() ));
    connect(m_zoomInButton, SIGNAL( pressed () ), this, SLOT( zoomIn() ));

    connect(m_colorCells, SIGNAL( cellEntered(int, int) ), this, SLOT( trackColor(int, int) ));
}

void PaletteGridView::setModel(PaletteModel * model)
{
    m_model = model;

    connect(m_model, SIGNAL( dataChanged(QModelIndex, QModelIndex) ), this, SLOT( updateWhenInsertItem(QModelIndex, QModelIndex) ));
    connect(m_model, SIGNAL( rowsRemoved(QModelIndex, int, int) ), this, SLOT( updateWhenRemoveItem(QModelIndex, int, int) ));
}

void PaletteGridView::setZoomFactor(int factor)
{
    if (m_model->rowCount() > 0)
    {
        m_colorCells->setColumnCount(factor);

        loadDataFromModel();
    }
}

void PaletteGridView::zoomOut()
{
    if (m_model->rowCount() > 0)
        m_setColumnSlider->setValue(m_setColumnSlider->value() - m_setColumnSlider->singleStep());
}

void PaletteGridView::zoomIn()
{
    if (m_model->rowCount() > 0)
        m_setColumnSlider->setValue(m_setColumnSlider->value() + m_setColumnSlider->singleStep());
}

void PaletteGridView::updateWhenInsertItem(const QModelIndex & /* topLeft */, const QModelIndex & /* bottomRight */)
{
    loadDataFromModel();
}

void PaletteGridView::updateWhenRemoveItem(const QModelIndex & /* parent */, int /* start */, int /* end */)
{
    loadDataFromModel();
}

void PaletteGridView::trackColor(int row, int column)
{
    int i = row * m_colorCells->columnCount() + column;

    // WARNING should use tableitemwidget?

    emit trackedColor(m_colorCells->color(i));
}

void PaletteGridView::loadDataFromModel()
{
    m_colorCells->clear();

    int rows = m_model->rowCount();

    if (rows != 0)
    {
        if (rows % m_colorCells->columnCount() == 0)
            m_colorCells->setRowCount(rows/m_colorCells->columnCount());
        else
            m_colorCells->setRowCount(1 + rows/m_colorCells->columnCount());
    }

    int tableRow;
    int tableColumn;

    // NOTE same code in delegate ... utils.h ?
    QVariantMap vmap;
    KColorScheme systemColorScheme(QPalette::Active);
    QColor baseWndColor = systemColorScheme.background(KColorScheme::NormalBackground).color();

    for (int i = 0; i < rows; i++)
    {
        tableRow = i / m_colorCells->columnCount();
        tableColumn = i % m_colorCells->columnCount();

        vmap = m_model->index(i, 0).data().toMap();

        if (vmap.value("type").toString() == QString("color"))
        {
            m_colorCells->setColor(i, vmap.value("color").value<QColor>());

            QTableWidgetItem * colorItem = m_colorCells->item(tableRow, tableColumn);

            if ((!vmap.value("name").toString().isEmpty()) && (colorItem))
                colorItem->setToolTip(i18n("Name : ") + vmap.value("name").toString());
        }

        if (vmap.value("type").toString() == QString("comment"))
        {
            QTableWidgetItem * commentItem = new QTableWidgetItem();

            m_colorCells->setItem(tableRow, tableColumn, commentItem);

            int luminance = 0.2126*baseWndColor.red() + 0.7152*baseWndColor.green() + 0.0722*baseWndColor.blue();

            QBrush brush;
            brush.setStyle(Qt::Dense7Pattern);

            if (luminance > (255 / 2.0))
                brush.setColor(Qt::black);
            else
                brush.setColor(Qt::white);

            commentItem->setBackground(brush);

            if (!vmap.value("comment").toString().isEmpty())
                commentItem->setToolTip(i18n("Comment : ") + vmap.value("comment").toString());
        }
    }

    int hFactor = m_colorCells->width() / m_colorCells->columnCount();
    int vFactor = m_colorCells->height() / m_colorCells->rowCount();
    int tableColumns = m_colorCells->columnCount();
    int tableRows = m_colorCells->rowCount();

    for (int i = 0 ; i < tableColumns ; i++)
        m_colorCells->horizontalHeader()->resizeSection(i, hFactor);

    for (int i = 0 ; i < tableRows ; i++)
        m_colorCells->verticalHeader()->resizeSection(i, vFactor);
}

#include "palettebriefview.moc"
