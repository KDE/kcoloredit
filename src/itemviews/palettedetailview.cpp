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

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSplitter>
#include <QtGui/QTableView>

#include <KLocalizedString>
#include <KLineEdit>
#include <KTextEdit>
#include <KComboBox>

#include "palettemodel.h"
#include "palettedelegate.h"

PaletteDetailView::PaletteDetailView(PaletteModel * model, QWidget * parent)
    : QWidget(parent)
{
    m_model = model;

    m_view = new QTableView(this);
    m_view->setModel(m_model);
    m_view->setItemDelegate(new PaletteDelegate(this));
    //m_view->setMinimumWidth(292); // NOTE 128 + 40 = 168 from kcoloreditwidget
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    m_view->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    m_view->setCornerButtonEnabled(false);
    m_view->setMouseTracking(true);
    m_view->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    m_view->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    // NOTE update headers when start the app
    updateHeaders(QModelIndex(), QModelIndex());

    QLabel * m_paletteNameLabel = new QLabel(i18n("Palette Name"), this);

    m_paletteNameLineEdit = new KLineEdit(this);
    m_paletteNameLineEdit->setClearButtonShown(true);
    m_paletteNameLineEdit->setText(m_model->paletteName());

    QLabel * m_paletteDescriptionLabel = new QLabel(i18n("Palette Description"), this);
    m_paletteDescriptionLabel->setAlignment(Qt::AlignCenter);

    KComboBox * descriptionModeComboBox = new KComboBox(this);
    descriptionModeComboBox->addItem(i18n("Brief Description"));
    descriptionModeComboBox->addItem(i18n("Full Description"));

    m_briefDescriptionDocument = new QTextDocument(this);

    m_fullDescriptionDocument = new QTextDocument(this);

    if (model->rowCount() > 0)
        updateDescriptions(QModelIndex(), QModelIndex());

    m_paletteDescriptionTextEdit = new KTextEdit(this);
    m_paletteDescriptionTextEdit->setMaximumHeight(100);
    m_paletteDescriptionTextEdit->setReadOnly(true);
    m_paletteDescriptionTextEdit->setDocument(m_briefDescriptionDocument);

    QSplitter * descriptionAndViewSplitter = new QSplitter(Qt::Vertical, this);
    descriptionAndViewSplitter->addWidget(m_paletteDescriptionTextEdit);
    descriptionAndViewSplitter->addWidget(m_view);
    descriptionAndViewSplitter->setCollapsible(0, false);
    descriptionAndViewSplitter->setCollapsible(1, false);

    setMinimumHeight(290);

    QHBoxLayout * nameLayout = new QHBoxLayout();
    nameLayout->addWidget(m_paletteNameLabel);
    nameLayout->addWidget(m_paletteNameLineEdit);

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(nameLayout);
    mainLayout->addWidget(m_paletteDescriptionLabel);
    mainLayout->addWidget(descriptionModeComboBox);
    mainLayout->addWidget(descriptionAndViewSplitter);


    connect(descriptionModeComboBox, SIGNAL( currentIndexChanged(int) ), this, SLOT( setDescriptionMode(int) ));
    connect(m_paletteNameLineEdit, SIGNAL( textEdited(QString) ), this, SLOT( updatePaletteName(QString) ));
    connect(m_model, SIGNAL( dataChanged(QModelIndex, QModelIndex) ), this, SLOT( updateHeaders(QModelIndex, QModelIndex) ));
    connect(m_model, SIGNAL( dataChanged(QModelIndex, QModelIndex) ), this, SLOT( updateDescriptions(QModelIndex, QModelIndex) ));

/*
    PaletteModel * model = dynamic_cast<PaletteModel *>(m_model);

    connect(model, SIGNAL( paletteChanged(Palette) ), this, SLOT( slotUpdatePaletteChanged(Palette) ));

    connect(m_view, SIGNAL( entered(QModelIndex) ), this, SLOT( slotGetSelectedColor(QModelIndex) ));
    connect(m_view, SIGNAL( pressed(QModelIndex) ), this, SLOT( slotGetSelectedColor(QModelIndex) ));
*/
}

void PaletteDetailView::setModel(PaletteModel * model)
{
    m_model = model;

    m_view->setModel(m_model);

    connect(m_model, SIGNAL( dataChanged(QModelIndex, QModelIndex) ), this, SLOT( updateHeaders(QModelIndex, QModelIndex) ));
    connect(m_model, SIGNAL( dataChanged(QModelIndex, QModelIndex) ), this, SLOT( updateDescriptions(QModelIndex, QModelIndex) ));

    m_paletteNameLineEdit->clear();
    m_paletteNameLineEdit->setText(m_model->paletteName());
}

int PaletteDetailView::selectedIndex() const
{
    if (m_view->selectionModel()->isSelected(m_view->currentIndex()))
        return m_view->currentIndex().row();

    // WARNING take care with this return value could be a bug
    return 0;
}

void PaletteDetailView::addColorItem(const QColor & color, const QString & colorName)
{
    m_model->insertColorRows(m_model->rowCount(), 1);

    QVariantMap vmap;

    vmap.insert("type", QString("color"));
    vmap.insert("color", color);
    vmap.insert("name", colorName);

    m_model->setData(m_model->index(m_model->rowCount() - 1, 0), vmap);
}

void PaletteDetailView::insertColorItem(int index, const QColor & color, const QString & colorName)
{
    m_model->insertColorRows(index, 1);

    QVariantMap vmap;

    vmap.insert("type", QString("color"));
    vmap.insert("color", color);
    vmap.insert("name", colorName);

    m_model->setData(m_model->index(index , 0), vmap);
}

void PaletteDetailView::addCommentItem(const QString & comment)
{
    m_model->insertCommentRows(m_model->rowCount(), 1);

    QVariantMap vmap;

    vmap.insert("type", QString("comment"));
    vmap.insert("comment", comment);

    m_model->setData(m_model->index(m_model->rowCount() - 1, 0), vmap);
}

void PaletteDetailView::insertCommentItem(int index, const QString & comment)
{
    m_model->insertCommentRows(index, 1);

    QVariantMap vmap;

    vmap.insert("type", QString("comment"));
    vmap.insert("comment", comment);

    m_model->setData(m_model->index(index, 0), vmap);
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
    // WARNING check this
    if (m_model->rowCount())
        m_view->setCurrentIndex(m_model->index(index, 0));
}

void PaletteDetailView::slotScrollToItem(int index)
{
    // WARNING check this
    if (m_model->rowCount())
        m_view->scrollTo(m_model->index(index, 0), QAbstractItemView::PositionAtTop);
}

void PaletteDetailView::updateHeaders(const QModelIndex & topLeft, const QModelIndex & bottomRight)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);

    m_view->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);

    for (int i = 0; i < m_view->model()->rowCount(); i++)
        m_view->verticalHeader()->resizeSection(i, 44);
}

void PaletteDetailView::updatePaletteName(const QString & text)
{
    m_model->setPaletteName(text);
}

void PaletteDetailView::setDescriptionMode(int index)
{
    if (index == 0)
        m_paletteDescriptionTextEdit->setDocument(m_briefDescriptionDocument);

    if (index == 1)
        m_paletteDescriptionTextEdit->setDocument(m_fullDescriptionDocument);
}

void PaletteDetailView::updateDescriptions(const QModelIndex & topLeft, const QModelIndex & bottomRight)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);

    m_briefDescriptionDocument->setPlainText(m_model->briefDescription());
    m_fullDescriptionDocument->setPlainText(m_model->fullDescription());
}

#include "palettedetailview.moc"
