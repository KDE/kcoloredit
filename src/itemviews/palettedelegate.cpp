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

#include "palettedelegate.h"

#include <QtGui/QPainter>

#include "colorutil.h"
#include "palettemodel.h"
#include "palettedelegateeditors.h"

PaletteDelegate::PaletteDelegate(QObject * parent)
    : QItemDelegate(parent)
{
}

QWidget * PaletteDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_UNUSED(option);

    QVariantMap vmap = index.data().toMap();

    if (vmap.value("type").toString() == QString("color"))   return new ColorItemEditor(parent);
    if (vmap.value("type").toString() == QString("comment")) return new CommentItemEditor(parent);

    return new QWidget(parent);
}

void PaletteDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    QVariantMap vmap = index.data().toMap();

    if (vmap.value("type").toString() == QString("color"))
    {
        ColorItemEditor * colorEditor = dynamic_cast<ColorItemEditor *>(editor);

        if (colorEditor)
        {
            colorEditor->setColor(vmap.value("color").value<QColor>());
            colorEditor->setColorName(vmap.value("name").toString());
        }
    }

    if (vmap.value("type").toString() == QString("comment"))
    {
        CommentItemEditor * commentEditor = dynamic_cast<CommentItemEditor *>(editor);

        if (commentEditor)
            commentEditor->setComment(vmap.value("comment").toString());
    }
}

void PaletteDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    QVariantMap vmap = index.data().toMap();

    if (vmap.value("type").toString() == QString("color"))
    {
        ColorItemEditor * colorEditor = dynamic_cast<ColorItemEditor *>(editor);

        if (colorEditor)
            dynamic_cast<PaletteModel *>(model)->setColorItem(index.row(), colorEditor->color(), colorEditor->colorName());
    }

    if (vmap.value("type").toString() == QString("comment"))
    {
        CommentItemEditor * commentEditor = dynamic_cast<CommentItemEditor *>(editor);

        if (commentEditor)
            dynamic_cast<PaletteModel *>(model)->setCommentItem(index.row(), commentEditor->comment());
    }
}

void PaletteDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & /* index */) const
{
    // NOTE correct the Y pos instead to update the height of each cell item in detailview
    QRect rect = option.rect;
    rect.setY(rect.y() - 5);

    editor->setGeometry(rect);
}

void PaletteDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QVariantMap vmap = index.data().toMap();

    if (vmap.value("type").toString() == QString("color"))
    {
        QColor color = index.model()->data(index).toMap().value("color").value<QColor>();

        painter->setPen(ColorUtil::contrastColor(color));
        painter->fillRect(option.rect, color);
        painter->drawText(option.rect, Qt::AlignCenter, index.model()->data(index).toMap().value("name").toString());
    }

    if (vmap.value("type").toString() == QString("comment"))
    {
        QBrush brush;

        if (ColorUtil::luminance(ColorUtil::backgroundColorOfWindow()) < (255 / 2.0))
        {
            brush.setColor(Qt::white);
            brush.setStyle(Qt::Dense1Pattern);
            painter->setPen(ColorUtil::contrastColor(brush.color()));
        }
        else
        {
            brush.setStyle(Qt::Dense7Pattern);
            painter->setPen(ColorUtil::contrastColor(ColorUtil::backgroundColorOfWindow()));
            brush.setColor(Qt::black);
        }

        painter->fillRect(option.rect, brush);
        painter->drawText(option.rect, Qt::AlignCenter, index.model()->data(index).toMap().value("comment").toString());
    }
}

#include "palettedelegate.moc"
