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

#include <QtGui/QLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSlider>
#include <QtGui/QCheckBox>

#include <KLocalizedString>
#include <KColorScheme>
#include <KPushButton>
#include <KColorCells>

#include "palettemodel.h"

PaletteBriefView::PaletteBriefView(PaletteModel * model, QWidget * parent)
    : QWidget(parent)
    , m_model(model)
{
    m_quickNavigationCheckBox = new QCheckBox(this);
    m_quickNavigationCheckBox->setText(i18n("Quick navigation"));
    m_quickNavigationCheckBox->setChecked(false);
    m_quickNavigationCheckBox->setStatusTip(i18n("It will disable automatically if you clicked over a item"));

    m_showCommentsCheckBox = new QCheckBox(this);
    m_showCommentsCheckBox->setText(i18n("Show comments"));
    m_showCommentsCheckBox->setChecked(false);
    m_showCommentsCheckBox->setStatusTip(i18n("If is checked, views will be synchronized"));

    m_setColumnSlider = new QSlider(Qt::Horizontal, this);
    // HERE TODO
    m_setColumnSlider->setRange(1, 20);
    m_setColumnSlider->setSingleStep(1);
    m_setColumnSlider->setPageStep(1);
    m_setColumnSlider->setTickInterval(1);
    m_setColumnSlider->setValue(1); // TODO NOTE check this default value

    m_zoomOutButton = new KPushButton(KIcon(QString("zoom-out")), QString(), this);

    m_zoomInButton = new KPushButton(KIcon(QString("zoom-in")), QString(), this);

    m_colorCells = new KColorCells(this, 0, 1);
    m_colorCells->setMouseTracking(true);
    m_colorCells->setMinimumWidth(256);
    m_colorCells->setAcceptDrops(false);
    m_colorCells->setSelectionMode(QAbstractItemView::NoSelection);

    QHBoxLayout * layoutHeader = new QHBoxLayout();
    layoutHeader->addWidget(m_quickNavigationCheckBox);
    layoutHeader->addWidget(m_showCommentsCheckBox);

    QHBoxLayout * layoutZoom = new QHBoxLayout();
    layoutZoom->addWidget(m_zoomOutButton);
    layoutZoom->addWidget(m_setColumnSlider);
    layoutZoom->addWidget(m_zoomInButton);

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addLayout(layoutHeader);
    layout->addWidget(m_colorCells);
    layout->addLayout(layoutZoom);

    connect(m_model, SIGNAL( dataChanged(QModelIndex, QModelIndex) ), this, SLOT( updatePaletteView() ));
    connect(m_model, SIGNAL( rowsRemoved(QModelIndex, int, int) ), this, SLOT( updatePaletteView() ));

    connect(m_setColumnSlider, SIGNAL( valueChanged(int) ), this, SLOT( setZoomFactor(int) ));
    connect(m_zoomOutButton, SIGNAL( pressed () ), this, SLOT( zoomOut() ));
    connect(m_zoomInButton, SIGNAL( pressed () ), this, SLOT( zoomIn() ));

    connect(m_colorCells, SIGNAL( cellEntered(int, int) ), this, SLOT( trackColor(int, int) ));
    connect(m_colorCells, SIGNAL( cellPressed(int, int) ), this, SLOT( updateIndex(int, int) ));

    connect(m_showCommentsCheckBox, SIGNAL( toggled(bool) ), this, SLOT( showComments(bool) ));
}

void PaletteBriefView::setModel(PaletteModel * model)
{
    m_model = model;

    connect(m_model, SIGNAL( dataChanged(QModelIndex, QModelIndex) ), this, SLOT( updateWhenInsertItem(QModelIndex, QModelIndex) ));
    connect(m_model, SIGNAL( rowsRemoved(QModelIndex, int, int) ), this, SLOT( updateWhenRemoveItem(QModelIndex, int, int) ));
}

void PaletteBriefView::setZoomFactor(int factor)
{
    if (m_model->rowCount() > 0)
    {
        m_colorCells->setColumnCount(factor);

        loadDataFromModel();
    }
}

void PaletteBriefView::zoomOut()
{
    if (m_model->rowCount() > 0)
        m_setColumnSlider->setValue(m_setColumnSlider->value() - m_setColumnSlider->singleStep());
}

void PaletteBriefView::zoomIn()
{
    if (m_model->rowCount() > 0)
        m_setColumnSlider->setValue(m_setColumnSlider->value() + m_setColumnSlider->singleStep());
}

void PaletteBriefView::updatePaletteView()
{
    loadDataFromModel();
}

void PaletteBriefView::updateIndex(int row, int column)
{
    m_quickNavigationCheckBox->setChecked(false);

    int index = row * m_colorCells->columnCount() + column;

    if (m_showCommentsCheckBox->isChecked())
        emit itemSelected(index);

    emit colorSelected(m_colorCells->color(index));
}

void PaletteBriefView::trackColor(int row, int column)
{
    if (m_quickNavigationCheckBox->isChecked())
    {
        int i = row * m_colorCells->columnCount() + column;

        emit colorTracked(m_colorCells->color(i));

        if (m_showCommentsCheckBox->isChecked())
            emit itemTracked(i);
    }
}

void PaletteBriefView::showComments(bool show)
{
    if ((m_model->rowCount() > 0) && show)
        loadDataFromModel();
}

void PaletteBriefView::loadDataFromModel()
{
    m_colorCells->clear();

    int rows = m_model->rowCount();
    int colors = 0;

    QVariantMap vmap;

    for (int i = 0; i < rows; i++)
    {
        vmap = m_model->index(i, 0).data().toMap();

        if (vmap.value("type").toString() == QString("color"))
            colors++;
    }

    if (!m_showCommentsCheckBox->isChecked())
    {
        if (colors > 0)
        {
            if (colors % m_colorCells->columnCount() == 0)
                m_colorCells->setRowCount(colors/m_colorCells->columnCount());
            else
                m_colorCells->setRowCount(1 + colors/m_colorCells->columnCount());
        }
        else
            return ;
    }
    else
    {
        if (rows > 0)
        {
            if (rows % m_colorCells->columnCount() == 0)
                m_colorCells->setRowCount(rows/m_colorCells->columnCount());
            else
                m_colorCells->setRowCount(1 + rows/m_colorCells->columnCount());
        }
        else
            return ;
    }

    int tableRow;
    int tableColumn;

    // NOTE same code in delegate ... utils.h ?
    KColorScheme systemColorScheme(QPalette::Active);
    QColor baseWndColor = systemColorScheme.background(KColorScheme::NormalBackground).color();
    int luminance = 0.2126*baseWndColor.red() + 0.7152*baseWndColor.green() + 0.0722*baseWndColor.blue();

    if (!m_showCommentsCheckBox->isChecked())
    {
        int colorCount = 0;

        for (int i = 0; i < rows; i++)
        {
            vmap = m_model->index(i, 0).data().toMap();

            if (vmap.value("type").toString() == QString("color"))
            {
                tableRow = colorCount / m_colorCells->columnCount();
                tableColumn = colorCount % m_colorCells->columnCount();

                m_colorCells->setColor(colorCount, vmap.value("color").value<QColor>());

                QTableWidgetItem * colorItem = m_colorCells->item(tableRow, tableColumn);

                if ((!vmap.value("name").toString().isEmpty()) && (colorItem))
                    colorItem->setToolTip(i18n("Name : ") + vmap.value("name").toString());

                colorCount++;
            }
        }
    }
    else
    {
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

                QBrush brush;
                brush.setStyle(Qt::Dense1Pattern);

                if (luminance > (255 / 2.0))
                    brush.setColor(Qt::black);
                else
                    brush.setColor(Qt::white);

                commentItem->setBackground(brush);

                if (!vmap.value("comment").toString().isEmpty())
                    commentItem->setToolTip(i18n("Comment : ") + vmap.value("comment").toString());
            }
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
