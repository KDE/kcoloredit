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

#include <QtCore/QModelIndex>
#include <QtGui/QPainter>

#include <QtGui/QHBoxLayout>

#include <KColorScheme>
#include <KColorButton>
#include <KLineEdit>

class ColorItemEditor : public QWidget
{
    public:
        ColorItemEditor(QWidget * parent = 0)
            : QWidget(parent)
        {
            m_color = new KColorButton(this);
            m_color->setMinimumWidth(92);

            m_colorName = new KLineEdit(this);
            m_colorName->setMinimumWidth(128);
            m_colorName->setClearButtonShown(true);

            QHBoxLayout * layout = new QHBoxLayout(this);
            layout->addWidget(m_color);
            layout->addWidget(m_colorName);

            setMinimumHeight(40);
        }

        QColor color() const { return m_color->color(); }
        void setColor(const QColor & color) { m_color->setColor(color); }

        QString colorName() const { return m_colorName->text(); }
        void setColorName(const QString & colorName) {     m_colorName->setText(colorName); }

    private:
        KColorButton * m_color;
        KLineEdit * m_colorName;
};

class CommentItemEditor : public QWidget
{
    public:
        CommentItemEditor(QWidget * parent = 0)
            : QWidget(parent)
        {
            m_comment = new KLineEdit(this);
            m_comment->setMinimumWidth(128);
            m_comment->setClearButtonShown(true);

            QHBoxLayout * layout = new QHBoxLayout(this);
            layout->addWidget(m_comment);

            setMinimumHeight(40);
        }

        QString comment() const { return m_comment->text(); }
        void setComment(const QString & comment) { m_comment->setText(comment); }

    private:
        KLineEdit * m_comment;
};

PaletteDelegate::PaletteDelegate(QObject * parent)
    : QItemDelegate(parent)
{
}

QWidget * PaletteDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & /* option */, const QModelIndex & index) const
{
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

        //NOTE necesary to perform the move of items
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

        vmap.insert("type", QString("color"));

        if (colorEditor)
        {
            vmap.insert("color", colorEditor->color());
            vmap.insert("name", colorEditor->colorName());
        }

        model->setData(index, vmap, Qt::EditRole);
    }

    if (vmap.value("type").toString() == QString("comment"))
    {
        CommentItemEditor * commentEditor = dynamic_cast<CommentItemEditor *>(editor);

        vmap.insert("type", QString("comment"));

        if (commentEditor)
            vmap.insert("comment", commentEditor->comment());

        model->setData(index, vmap, Qt::EditRole);
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
        brush.setStyle(Qt::Dense7Pattern);

        if (luminance > (255 / 2.0))
        {
            painter->setPen(Qt::black);
            brush.setColor(Qt::black);
        }
        else
        {
            painter->setPen(Qt::white);
            brush.setColor(Qt::white);
        }

        painter->fillRect(option.rect, brush);
        painter->drawText(option.rect, Qt::AlignCenter, index.model()->data(index).toMap().value("comment").toString());
    }
}

#include "palettedelegate.moc"
