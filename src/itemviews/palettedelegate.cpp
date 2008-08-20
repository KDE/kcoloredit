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

#include <KColorScheme>

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
    editor->setGeometry(option.rect);
}

void PaletteDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QVariantMap vmap = index.data().toMap();

    float luminance;

    if (vmap.value("type").toString() == QString("color"))
    {
        QColor color = index.model()->data(index).toMap().value("color").value<QColor>();

        // get the luminance source wikipedia
        // Y = 0.2126 R + 0.7152 G + 0.0722 B
        // thanks to agave developer
        luminance = 0.2126*color.red() + 0.7152*color.green() + 0.0722*color.blue();

        if (luminance > (255 / 2.0))
            painter->setPen(Qt::black);
        else
            painter->setPen(Qt::white);

        painter->fillRect(option.rect, color);
        painter->drawText(option.rect, Qt::AlignCenter, index.model()->data(index).toMap().value("name").toString());
    }

    if (vmap.value("type").toString() == QString("comment"))
    {
        KColorScheme systemColorScheme(QPalette::Active);

        QColor baseWndColor = systemColorScheme.background(KColorScheme::NormalBackground).color();

        luminance = 0.2126*baseWndColor.red() + 0.7152*baseWndColor.green() + 0.0722*baseWndColor.blue();

        QBrush brush;
        brush.setStyle(Qt::Dense1Pattern);

        if (luminance > (255.0 / 2.0))
        {
            painter->setPen(Qt::white);
            brush.setColor(Qt::black);
        }
        else
        {
            painter->setPen(Qt::black);
            brush.setColor(Qt::white);
        }

        painter->fillRect(option.rect, brush);
        painter->drawText(option.rect, Qt::AlignCenter, index.model()->data(index).toMap().value("comment").toString());
    }
}

#include "palettedelegate.moc"
