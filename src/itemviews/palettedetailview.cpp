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

#include "palettedetailview.h"

#include <QtGui/QLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTableView>

#include <KLocalizedString>
#include <KLineEdit>
#include <KUrlLabel>

#include "palettemodel.h"
#include "palettedelegate.h"
#include "palettedescriptionwidget.h"

PaletteDetailView::PaletteDetailView(PaletteModel * model, QWidget * parent)
    : QWidget(parent)
{
    m_model = model;

    m_view = new QTableView(this);
    m_view->setModel(m_model);
    m_view->setItemDelegate(new PaletteDelegate(this));
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_view->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    m_view->setCornerButtonEnabled(false);
    m_view->setMouseTracking(true);
    m_view->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    m_view->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    updatePaletteDetails();

    m_paletteNameLineEdit = new KLineEdit(this);
    m_paletteNameLineEdit->setClearButtonShown(true);
    m_paletteNameLineEdit->setText(m_model->paletteName());

    m_paletteDescriptionLink = new KUrlLabel(QString(), i18n("Add description"), this);

    updateDescriptionLink();

    QHBoxLayout * nameLayout = new QHBoxLayout();
    nameLayout->addWidget(new QLabel(i18n("Palette Name:"), this));
    nameLayout->addWidget(m_paletteNameLineEdit);

    QHBoxLayout * descriptionLayout = new QHBoxLayout();
    descriptionLayout->addWidget(m_paletteDescriptionLink);
    descriptionLayout->addWidget(new QLabel(i18n(" for this palette"), this), Qt::AlignLeft);

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(nameLayout);
    mainLayout->addLayout(descriptionLayout);
    mainLayout->addWidget(m_view);

    connect(m_paletteNameLineEdit   , SIGNAL( textEdited(QString) ), SLOT( updatePaletteName(QString)     ));
    connect(m_paletteDescriptionLink, SIGNAL( leftClickedUrl()    ), SLOT( showPaletteDescriptionWidget() ));

    connect(m_model, SIGNAL( dataChanged(QModelIndex, QModelIndex) ), SLOT( updatePaletteDetails() ));
    connect(m_model, SIGNAL( rowsRemoved(QModelIndex, int, int)    ), SLOT( updatePaletteDetails() ));
}

void PaletteDetailView::setModel(PaletteModel * model)
{
    m_model = model;

    m_view->setModel(m_model);

    connect(m_model, SIGNAL( dataChanged(QModelIndex, QModelIndex) ), SLOT( updatePaletteDetails() ));
    connect(m_model, SIGNAL( rowsRemoved(QModelIndex, int, int)    ), SLOT( updatePaletteDetails() ));

    m_paletteNameLineEdit->clear();
    m_paletteNameLineEdit->setText(m_model->paletteName());

    updateDescriptionLink();
}

int PaletteDetailView::selectedIndex() const
{
    if (m_view->selectionModel()->isSelected(m_view->currentIndex()))
        return m_view->currentIndex().row();

    // WARNING take care with this return value ... could be cause a bug
    return 0;
}

void PaletteDetailView::appendColorItem(const QColor & color, const QString & colorName)
{
    m_model->appendColorItem(color, colorName);
}

void PaletteDetailView::insertColorItem(int index, const QColor & color, const QString & colorName)
{
    m_model->insertColorItem(index, color, colorName);
}

void PaletteDetailView::appendCommentItem(const QString & comment)
{
    m_model->appendCommentItem(comment);
}

void PaletteDetailView::insertCommentItem(int index, const QString & comment)
{
    m_model->insertCommentItem(index, comment);
}

void PaletteDetailView::removeItem(int index)
{
    if (m_model->rowCount() == 0)
        return ;

    m_model->removeRows(index, 1);
}

void PaletteDetailView::moveItem(int index, Palette::MoveOperation operation)
{
    if (m_model->rowCount() == 0)
        return ;

    m_model->moveItem(m_model->index(index, 0), operation);

    switch (operation)
    {
        case Palette::MoveToPrev:
            if (index == 0)
                break ;

            m_view->setCurrentIndex(m_view->model()->index(index - 1, 0));
            break;

        case Palette::MoveToNext:
            if (index == (m_model->rowCount() - 1))
                break ;

            m_view->setCurrentIndex(m_view->model()->index(index + 1, 0));
            break;

        case Palette::MoveToStart:
            if (index == 0)
                break ;

            m_view->setCurrentIndex(m_view->model()->index(0, 0));
            break;

        case Palette::MoveToEnd:
            if (index == (m_model->rowCount() - 1))
                break ;

            m_view->setCurrentIndex(m_view->model()->index(m_model->rowCount() - 1, 0));
            break;
    }
}

void PaletteDetailView::setSelectedItem(int index)
{
    if (m_model->rowCount() > 0)
        m_view->setCurrentIndex(m_model->index(index, 0));
}

void PaletteDetailView::scrollToItem(int index)
{
    if (m_model->rowCount() > 0)
        m_view->scrollTo(m_model->index(index, 0), QAbstractItemView::PositionAtTop);
}

void PaletteDetailView::updatePaletteDetails()
{
    m_view->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
}

void PaletteDetailView::updatePaletteName(const QString & text)
{
    m_model->setPaletteName(text);
}

void PaletteDetailView::updateDescriptionLink()
{
    if (m_model->hasDescription())
        m_paletteDescriptionLink->setText(i18n("Edit description"));
    else
        m_paletteDescriptionLink->setText(i18n("Add description"));
}

void PaletteDetailView::showPaletteDescriptionWidget()
{
    PaletteDescriptionWidget paletteDescriptionWidget;

    if (m_model->hasDescription())
        paletteDescriptionWidget.setDescription(m_model->paletteDescription());

    if (paletteDescriptionWidget.exec(QCursor::pos()))
    {
        m_model->setPaletteDescription(paletteDescriptionWidget.description());

        updateDescriptionLink();

        update();
    }
}

#include "palettedetailview.moc"
