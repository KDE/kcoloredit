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
#include <KPushButton>
#include <KColorCells>

#include "colorutil.h"
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
    m_setColumnSlider->setSingleStep(1);
    m_setColumnSlider->setPageStep(1);
    m_setColumnSlider->setTickInterval(1);

    m_zoomOutButton = new KPushButton(KIcon("zoom-out"), QString(), this);

    m_zoomInButton = new KPushButton(KIcon("zoom-in"), QString(), this);

    m_colorCells = new KColorCells(this, 0, 1);
    m_colorCells->setAcceptDrops(false);
    // WARNING Can't enabled drag support it disable mousetracking ...
    //m_colorCells->setAcceptDrags(true);
    m_colorCells->setMouseTracking(true);
    m_colorCells->setSelectionMode(QAbstractItemView::NoSelection);

    setupPreferredColumns();

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

    connect(m_model, SIGNAL( dataChanged(QModelIndex, QModelIndex) ), SLOT( updatePaletteView() ));
    connect(m_model, SIGNAL( rowsRemoved(QModelIndex, int, int)    ), SLOT( updatePaletteView() ));

    connect(m_setColumnSlider, SIGNAL( valueChanged(int) ), SLOT( setZoomFactor(int) ));
    connect(m_zoomOutButton, SIGNAL( pressed () ), SLOT( zoomOut() ));
    connect(m_zoomInButton, SIGNAL( pressed () ), SLOT( zoomIn() ));

    connect(m_colorCells, SIGNAL( cellEntered(int, int) ), SLOT( trackColor(int, int) ));
    connect(m_colorCells, SIGNAL( cellPressed(int, int) ), SLOT( updateIndex(int, int) ));

    connect(m_showCommentsCheckBox, SIGNAL( toggled(bool) ), SLOT( showComments(bool) ));
}

void PaletteBriefView::setModel(PaletteModel * model)
{
    m_model = model;

    setupPreferredColumns();

    connect(m_model, SIGNAL( dataChanged(QModelIndex, QModelIndex) ), SLOT( updatePaletteView() ));
    connect(m_model, SIGNAL( rowsRemoved(QModelIndex, int, int)    ), SLOT( updatePaletteView() ));
}

void PaletteBriefView::setZoomFactor(int factor)
{
    if (m_model->rowCount() > 0)
    {
        m_colorCells->setColumnCount(factor);

        updatePaletteView();
    }
    else
        m_setColumnSlider->setValue(Palette::MIN_PREFERRED_COLUMNS);
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
    m_colorCells->clear();

    // NOTE
    // First check if PaletteModel is not empty

    if (m_model->rowCount() == 0)
    {
        m_colorCells->setRowCount(0);
        m_colorCells->setColumnCount(Palette::MIN_PREFERRED_COLUMNS);

        m_setColumnSlider->setValue(Palette::MIN_PREFERRED_COLUMNS);

        return ;
    }

    // We change the columns in KColorCells acording to the value of QSlider
    m_colorCells->setColumnCount(m_setColumnSlider->value());

    // NOTE
    // Here we load data (color and comment items) from PaletteModel
    // to fill it inside KColorCells

    int rows = m_model->rowCount();
    int colors = 0;

    for (int i = 0; i < rows; i++)
        if (m_model->itemType(i) == PaletteItem::ColorType)
            colors++;

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

    // NOTE
    // Setup the brush to the background of the comment items

    QBrush brush;

    if (ColorUtil::luminance(ColorUtil::backgroundColorOfWindow()) < (255 / 2.0))
    {
        brush.setColor(Qt::white);
        brush.setStyle(Qt::Dense5Pattern);
    }
    else
    {
        brush.setColor(Qt::black);
        brush.setStyle(Qt::Dense7Pattern);
    }

    // NOTE
    // This variables represent the current row and column of the
    // item (color or comment) that are inside the ColorCells

    int tableRow;
    int tableColumn;

    // NOTE
    // If show comments is checked, then we filled KColorCells with all the
    // data (color and comment items) of PaletteModel, else
    // only filled KColorCells with color items

    if (m_showCommentsCheckBox->isChecked())
    {
        for (int i = 0; i < rows; i++)
        {
            tableRow = i / m_colorCells->columnCount();
            tableColumn = i % m_colorCells->columnCount();

            if (m_model->itemType(i) == PaletteItem::ColorType)
            {
                // NOTE
                // Here we use the standard KColorCells APi to set a color item

                m_colorCells->setColor(i, m_model->colorItem(i).color());

                QTableWidgetItem * colorItem = m_colorCells->item(tableRow, tableColumn);

                if ((!m_model->colorItem(i).colorName().isEmpty()) && (colorItem))
                    colorItem->setToolTip(m_model->colorItem(i).colorName());
            }

            if (m_model->itemType(i) == PaletteItem::CommentType)
            {
                // NOTE
                // Here we use QTableWidget (base class of KColorCells) APi to set a comment item

                QTableWidgetItem * commentItem = new QTableWidgetItem();

                m_colorCells->setItem(tableRow, tableColumn, commentItem);

                commentItem->setBackground(brush);

                if (!m_model->commentItem(i).comment().isEmpty())
                    commentItem->setToolTip(m_model->commentItem(i).comment());
            }
        }
    }
    else
    {
        colors = 0;

        for (int i = 0; i < rows; i++)
        {
            if (m_model->itemType(i) == PaletteItem::ColorType)
            {
                tableRow = colors / m_colorCells->columnCount();
                tableColumn = colors % m_colorCells->columnCount();

                m_colorCells->setColor(colors, m_model->colorItem(i).color());

                QTableWidgetItem * colorItem = m_colorCells->item(tableRow, tableColumn);

                if ((!m_model->colorItem(i).colorName().isEmpty()) && (colorItem))
                    colorItem->setToolTip(m_model->colorItem(i).colorName());

                colors++;
            }
        }
    }

    // NOTE
    // This last lines fix the size of each cell of KColorCells

    int hFactor = m_colorCells->width() / m_colorCells->columnCount();
    int vFactor = m_colorCells->height() / m_colorCells->rowCount();

    for (int i = 0 ; i < m_colorCells->columnCount(); i++)
        m_colorCells->horizontalHeader()->resizeSection(i, hFactor);

    for (int i = 0 ; i < m_colorCells->rowCount(); i++)
        m_colorCells->verticalHeader()->resizeSection(i, vFactor);
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
    Q_UNUSED(show);

    if ((m_model->rowCount() > 0))
        updatePaletteView();
}

void PaletteBriefView::setupPreferredColumns()
{
    int finalColumns = Palette::MAX_PREFERRED_COLUMNS;
    int finalValue = Palette::MIN_PREFERRED_COLUMNS;

    if (m_model->preferredPaletteColumns() > Palette::MIN_PREFERRED_COLUMNS)
    {
        if (m_model->preferredPaletteColumns() % 2 == 0)
            finalColumns = 2*m_model->preferredPaletteColumns();
        else
            finalColumns = 2*m_model->preferredPaletteColumns() - 1;

        finalValue = m_model->preferredPaletteColumns();
    }
    else
    {
        if (m_model->rowCount() % 2 == 0)
            finalColumns = 2*m_model->rowCount();
        else
            finalColumns = 2*m_model->rowCount() - 1;

        finalValue = static_cast<int>(finalColumns/2);
    }

    if (finalColumns > Palette::MAX_PREFERRED_COLUMNS)
    {
        finalColumns = Palette::MAX_PREFERRED_COLUMNS;
        finalValue = static_cast<int>(Palette::MAX_PREFERRED_COLUMNS/2);
    }

    if (m_model->rowCount() == 0)
    {
        finalColumns = Palette::MAX_PREFERRED_COLUMNS;
        finalValue = Palette::MIN_PREFERRED_COLUMNS;
    }

    m_colorCells->setColumnCount(finalColumns);

    m_setColumnSlider->setRange(Palette::MIN_PREFERRED_COLUMNS, finalColumns);
    m_setColumnSlider->setValue(finalValue);
}

#include "palettebriefview.moc"
