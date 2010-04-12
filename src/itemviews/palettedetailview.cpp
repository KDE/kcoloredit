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

#include "palettedetailview.h"

#include <QtGui/QLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QTableView>
#include <QtGui/QToolButton>

#include <KLocalizedString>
#include <KLineEdit>
#include <KPushButton>
#include <KUrlLabel>

#include "colorutil.h"
#include "palettemodel.h"
#include "palettedelegate.h"
#include "palettedescriptionwidget.h"

//BEGIN Public class ViewControls

PaletteDetailViewControls::PaletteDetailViewControls(PaletteDetailView *paletteDetailView)
    : QWidget(paletteDetailView)
    , m_paletteDetailView(paletteDetailView)
{
    QToolButton *moveItemToFirstPositionButton = new QToolButton(this);
    moveItemToFirstPositionButton->setIcon(KIcon("go-top"));
    moveItemToFirstPositionButton->setToolTip(i18n("First position"));

    QToolButton *moveItemToPrevPositionButton = new QToolButton(this);
    moveItemToPrevPositionButton->setIcon(KIcon("go-up"));
    moveItemToPrevPositionButton->setToolTip(i18n("Prev position"));
    moveItemToPrevPositionButton->setAutoRepeat(true);

    QToolButton *insertColorItemButton = new QToolButton(this);
    insertColorItemButton->setIcon(KIcon("insert-horizontal-rule"));
    insertColorItemButton->setToolTip(i18n("Insert Color"));

    QToolButton *insertCommentItemButton = new QToolButton(this);
    insertCommentItemButton->setIcon(KIcon("list-add-font"));
    insertCommentItemButton->setToolTip(i18n("Insert Comment"));

    QToolButton *removeItemButton = new QToolButton(this);
    removeItemButton->setIcon(KIcon("list-remove"));
    removeItemButton->setToolTip(i18n("Remove Item"));

    QToolButton *moveItemToNextPositionButton = new QToolButton(this);
    moveItemToNextPositionButton->setIcon(KIcon("go-down"));
    moveItemToNextPositionButton->setToolTip(i18n("Next position"));
    moveItemToNextPositionButton->setAutoRepeat(true);

    QToolButton *moveItemToLastPositionButton = new QToolButton(this);
    moveItemToLastPositionButton->setIcon(KIcon("go-bottom"));
    moveItemToLastPositionButton->setToolTip(i18n("Last position"));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch(1);
    mainLayout->addWidget(moveItemToFirstPositionButton);
    mainLayout->addWidget(moveItemToLastPositionButton);
    mainLayout->addStretch(1);
    mainLayout->addWidget(moveItemToPrevPositionButton);
    mainLayout->addWidget(moveItemToNextPositionButton);
    mainLayout->addStretch(1); 
    mainLayout->addWidget(insertColorItemButton);
    mainLayout->addWidget(insertCommentItemButton);
    mainLayout->addWidget(removeItemButton);
    mainLayout->addStretch(1);

    connect(moveItemToFirstPositionButton, SIGNAL(pressed()), this, SLOT(moveItemToFirstPosition()));
    connect(moveItemToPrevPositionButton, SIGNAL(pressed()), this, SLOT(moveItemToPrevPosition()));
    connect(insertColorItemButton, SIGNAL(pressed()), this, SLOT(insertColorItem()));
    connect(insertCommentItemButton, SIGNAL(pressed()), this, SLOT(insertCommentItem()));
    connect(removeItemButton, SIGNAL(pressed()), this, SLOT(removeItem()));
    connect(moveItemToNextPositionButton, SIGNAL(pressed()), this, SLOT(moveItemToNextPosition()));
    connect(moveItemToLastPositionButton, SIGNAL(pressed()), this, SLOT(moveItemToLastPosition()));
}

void PaletteDetailViewControls::insertColorItem()
{
    m_paletteDetailView->insertColorItem(m_paletteDetailView->selectedIndex(), m_paletteDetailView->currentKColorEditColor());
}

void PaletteDetailViewControls::insertCommentItem()
{
    m_paletteDetailView->insertCommentItem(m_paletteDetailView->selectedIndex());
}

void PaletteDetailViewControls::removeItem()
{
    m_paletteDetailView->removeItem(m_paletteDetailView->selectedIndex());
}

void PaletteDetailViewControls::moveItemToNextPosition()
{
    m_paletteDetailView->moveItem(m_paletteDetailView->selectedIndex(), Palette::MoveToPrev);
}

void PaletteDetailViewControls::moveItemToPrevPosition()
{
    m_paletteDetailView->moveItem(m_paletteDetailView->selectedIndex(), Palette::MoveToNext);
}

void PaletteDetailViewControls::moveItemToFirstPosition()
{
    m_paletteDetailView->moveItem(m_paletteDetailView->selectedIndex(), Palette::MoveToStart);
}

void PaletteDetailViewControls::moveItemToLastPosition()
{
    m_paletteDetailView->moveItem(m_paletteDetailView->selectedIndex(), Palette::MoveToEnd);
}


//END Public class ViewControls

//BEGIN Public class PaletteDetailView

PaletteDetailView::PaletteDetailView(PaletteModel * model, QWidget * parent)
    : QWidget(parent)
    , m_currentKColorEditColor(ColorUtil::DEFAULT_COLOR)
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

    PaletteDetailViewControls *viewControls = new PaletteDetailViewControls(this);

    QHBoxLayout *viewLayout = new QHBoxLayout();
    viewLayout->addWidget(m_view);
    viewLayout->addWidget(viewControls);

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
    mainLayout->addLayout(viewLayout);

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
            if (index == (m_model->rowCount() - 1))
                break ;

            m_view->setCurrentIndex(m_view->model()->index(index + 1, 0));
            break;

        case Palette::MoveToNext:
            if (index == 0)
                break ;

            m_view->setCurrentIndex(m_view->model()->index(index - 1, 0));
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

QColor PaletteDetailView::currentKColorEditColor() const
{
    return m_currentKColorEditColor;
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

void PaletteDetailView::setCurrentKColorEditColor(const QColor color)
{
    m_currentKColorEditColor = color;
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

//END Public class PaletteDetailView

#include "palettedetailview.moc"
