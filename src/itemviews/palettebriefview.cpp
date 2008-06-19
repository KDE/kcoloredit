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

#include <QtGui/QMouseEvent>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSlider>
#include <QtGui/QCheckBox>

#include <KLocalizedString>
#include <KAction>
#include <KMenu>
#include <KColorScheme>
#include <KPushButton>
#include <KColorCells>

#include "palettemodel.h"

//BEGIN class ColorCellsAdapter

ColorCellsAdapter::ColorCellsAdapter(QWidget * parent, int row, int column)
    : KColorCells(parent, row, column)
{
    setMouseTracking(true);
    setMinimumWidth(256);
    setAcceptDrops(false);
    setSelectionMode(QAbstractItemView::NoSelection);

    m_cutAction = new KAction(KIcon(QString("edit-cut")), i18n("Cut"), this);
    m_copyAction = new KAction(KIcon(QString("edit-copy")), i18n("Copy"), this);
    m_pasteAction = new KAction(KIcon(QString("edit-paste")), i18n("Paste"), this);

    m_menu = new KMenu(this);
    m_menu->addAction(m_cutAction);
    m_menu->addAction(m_copyAction);
    m_menu->addAction(m_pasteAction);
}

KAction * ColorCellsAdapter::cutAction() const
{
    return m_cutAction;
}

KAction * ColorCellsAdapter::copyAction() const
{
    return m_copyAction;
}

KAction * ColorCellsAdapter::pasteAction() const
{
    return m_pasteAction;
}

void ColorCellsAdapter::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::RightButton)
        if (!selectedItems().empty())
            m_menu->popup(mapToGlobal(event->pos()));

    if (event->button() == Qt::LeftButton)
        KColorCells::mousePressEvent(event);
}

//END class ColorCellsAdapter

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
    m_showCommentsCheckBox->setStatusTip(i18n("If is checked, selection enabled and you can cut,copy and paste items"));

    m_setColumnSlider = new QSlider(Qt::Horizontal, this);
    m_setColumnSlider->setRange(1, 20);
    m_setColumnSlider->setSingleStep(1);
    m_setColumnSlider->setPageStep(1);
    m_setColumnSlider->setTickInterval(1);
    m_setColumnSlider->setValue(1); // NOTE check this default value

    m_zoomOutButton = new KPushButton(KIcon(QString("zoom-out")), QString(), this);

    m_zoomInButton = new KPushButton(KIcon(QString("zoom-in")), QString(), this);

    m_colorCellsAdapter = new ColorCellsAdapter(this, 0, 1);

    QHBoxLayout * layoutHeader = new QHBoxLayout();
    layoutHeader->addWidget(m_quickNavigationCheckBox);
    layoutHeader->addWidget(m_showCommentsCheckBox);

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addLayout(layoutHeader);
    layout->addWidget(m_colorCellsAdapter);

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

    connect(m_colorCellsAdapter, SIGNAL( cellEntered(int, int) ), this, SLOT( trackColor(int, int) ));
    connect(m_colorCellsAdapter, SIGNAL( cellPressed(int, int) ), this, SLOT( updateIndex(int, int) ));

    connect(m_showCommentsCheckBox, SIGNAL( toggled(bool) ), this, SLOT( showComments(bool) ));

// TODO see avobe ColorCellsAdapter class
//     connect(m_cutAction,   SIGNAL( triggered() ), this, SLOT( cut()   ));
//     connect(m_copyAction,  SIGNAL( triggered() ), this, SLOT( copy()  ));
//     connect(m_pasteAction, SIGNAL( triggered() ), this, SLOT( paste() ));
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
        m_colorCellsAdapter->setColumnCount(factor);

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

void PaletteBriefView::updateWhenInsertItem(const QModelIndex & topLeft, const QModelIndex & bottomRight)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);

    loadDataFromModel();
}

void PaletteBriefView::updateWhenRemoveItem(const QModelIndex & parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);

    loadDataFromModel();
}

void PaletteBriefView::updateIndex(int row, int column)
{
    m_quickNavigationCheckBox->setChecked(false);

    if (m_showCommentsCheckBox->isChecked())
    {
        int index = row * m_colorCellsAdapter->columnCount() + column;

        emit selectedItem(index);
    }
}

void PaletteBriefView::trackColor(int row, int column)
{
    if (m_quickNavigationCheckBox->isChecked())
    {
        int i = row * m_colorCellsAdapter->columnCount() + column;

        // WARNING should use tableitemwidget?

        emit trackedColor(m_colorCellsAdapter->color(i));

        if (m_showCommentsCheckBox->isChecked())
            emit trackedItem(i);
    }
}

void PaletteBriefView::showComments(bool show)
{
    if (m_model->rowCount() > 0)
    {
        loadDataFromModel();

        if (show)
        {
            m_colorCellsAdapter->setSelectionMode(QAbstractItemView::ExtendedSelection);
            m_colorCellsAdapter->setAcceptDrags(false);
        }
        else
        {
            m_colorCellsAdapter->setSelectionMode(QAbstractItemView::NoSelection);
            m_colorCellsAdapter->setAcceptDrags(true);
        }
    }
}

void PaletteBriefView::cut()
{
//     m_buffer = m_colorCellsAdapter->selectedItems();
}

void PaletteBriefView::copy()
{
}

void PaletteBriefView::paste()
{
/*    int row = m_buffer[0]->row();
    int column = m_buffer[0]->column();
    int index = row * m_colorCellsAdapter->columnCount() + column;
*/
//     QVariantMap vmap = m_model->index(index, 0).data().toMap();
// 
//     if (vmap["type"] == QString("color"))



}


void PaletteBriefView::loadDataFromModel()
{
    m_colorCellsAdapter->clear();

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
            if (colors % m_colorCellsAdapter->columnCount() == 0)
                m_colorCellsAdapter->setRowCount(colors/m_colorCellsAdapter->columnCount());
            else
                m_colorCellsAdapter->setRowCount(1 + colors/m_colorCellsAdapter->columnCount());
        }
        else
            return ;
    }
    else
    {
        if (rows > 0)
        {
            if (rows % m_colorCellsAdapter->columnCount() == 0)
                m_colorCellsAdapter->setRowCount(rows/m_colorCellsAdapter->columnCount());
            else
                m_colorCellsAdapter->setRowCount(1 + rows/m_colorCellsAdapter->columnCount());
        }
        else
            return ;
    }

    int tableRow;
    int tableColumn;

    // NOTE same code in delegate ... utils.h ?
    KColorScheme systemColorScheme(QPalette::Active);
    QColor baseWndColor = systemColorScheme.background(KColorScheme::NormalBackground).color();

    if (!m_showCommentsCheckBox->isChecked())
    {
        int colorCount = 0;

        for (int i = 0; i < rows; i++)
        {
            vmap = m_model->index(i, 0).data().toMap();

            if (vmap.value("type").toString() == QString("color"))
            {
                tableRow = colorCount / m_colorCellsAdapter->columnCount();
                tableColumn = colorCount % m_colorCellsAdapter->columnCount();

                m_colorCellsAdapter->setColor(colorCount, vmap.value("color").value<QColor>());

                QTableWidgetItem * colorItem = m_colorCellsAdapter->item(tableRow, tableColumn);

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
            tableRow = i / m_colorCellsAdapter->columnCount();
            tableColumn = i % m_colorCellsAdapter->columnCount();

            vmap = m_model->index(i, 0).data().toMap();

            if (vmap.value("type").toString() == QString("color"))
            {
                m_colorCellsAdapter->setColor(i, vmap.value("color").value<QColor>());

                QTableWidgetItem * colorItem = m_colorCellsAdapter->item(tableRow, tableColumn);

                if ((!vmap.value("name").toString().isEmpty()) && (colorItem))
                    colorItem->setToolTip(i18n("Name : ") + vmap.value("name").toString());
            }

            if (vmap.value("type").toString() == QString("comment"))
            {
                QTableWidgetItem * commentItem = new QTableWidgetItem();

                m_colorCellsAdapter->setItem(tableRow, tableColumn, commentItem);

                int luminance = 0.2126*baseWndColor.red() + 0.7152*baseWndColor.green() + 0.0722*baseWndColor.blue();

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

    int hFactor = m_colorCellsAdapter->width() / m_colorCellsAdapter->columnCount();
    int vFactor = m_colorCellsAdapter->height() / m_colorCellsAdapter->rowCount();
    int tableColumns = m_colorCellsAdapter->columnCount();
    int tableRows = m_colorCellsAdapter->rowCount();

    for (int i = 0 ; i < tableColumns ; i++)
        m_colorCellsAdapter->horizontalHeader()->resizeSection(i, hFactor);

    for (int i = 0 ; i < tableRows ; i++)
        m_colorCellsAdapter->verticalHeader()->resizeSection(i, vFactor);
}

#include "palettebriefview.moc"
