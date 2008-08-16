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

#include "palettemodel.h"

#include <QtCore/QString>
#include <QtGui/QColor>

#include <KLocalizedString>

//BEGIN constructor

PaletteModel::PaletteModel(QObject * parent)
    : QAbstractTableModel(parent)
{
}

//END constructor

//BEGIN public methods

Qt::ItemFlags PaletteModel::flags(const QModelIndex & index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QVariant PaletteModel::data(const QModelIndex & index, int role) const
{
    Q_UNUSED(role);

    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_palette.count())
        return QVariant();

    if (m_palette.itemType(index.row()) == PaletteItem::ColorType)
    {
        QVariantMap vmap;
        vmap.insert("type", QString("color"));
        vmap.insert("color", m_palette.colorItem(index.row())->color());
        vmap.insert("name", m_palette.colorItem(index.row())->colorName());

        return vmap;
    }

    if (m_palette.itemType(index.row()) == PaletteItem::CommentType)
    {
        QVariantMap vmap;
        vmap.insert("type", QString("comment"));
        vmap.insert("comment", m_palette.commentItem(index.row())->comment());

        return vmap;
    }

    return QVariant();
}

bool PaletteModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        QVariantMap vmap = value.toMap();

        if (m_palette.itemType(index.row()) == PaletteItem::ColorType)
        {
            data(index, Qt::BackgroundRole).toMap().insert("type", QString("color"));  // NOTE

            PaletteColorItem * colorItem = m_palette.colorItem(index.row());

            if (colorItem)
            {
                colorItem->setColor(vmap.value("color").value<QColor>());
                colorItem->setColorName(vmap.value("name").toString());
            }

            emit dataChanged(index, index);

            return true;
        }

        if (m_palette.itemType(index.row()) == PaletteItem::CommentType)
        {
            data(index, Qt::BackgroundRole).toMap().insert("type", QString("comment"));  // NOTE

            PaletteCommentItem * commentItem = m_palette.commentItem(index.row());

            if (commentItem)
                commentItem->setComment(vmap.value("comment").toString());

            emit dataChanged(index, index);

            return true;
        }
    }

    return false;
}

QVariant PaletteModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        if (section == 0)
            return i18n("Items");

     if (orientation == Qt::Vertical)
        return QString("%1").arg(section + 1);

    return QVariant();
}

int PaletteModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);

    return m_palette.count();
}

bool PaletteModel::insertColorRows(int row, int count, const QModelIndex & parent)
{
    Q_UNUSED(parent);

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; i++)
        m_palette.insertColorItem(row, new PaletteColorItem());

    endInsertRows();

    return true;
}

bool PaletteModel::insertCommentRows(int row, int count, const QModelIndex & parent)
{
    Q_UNUSED(parent);

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; i++)
        m_palette.insertCommentItem(row, new PaletteCommentItem());

    endInsertRows();

    return true;
}

bool PaletteModel::removeRows(int row, int count, const QModelIndex & parent)
{
    Q_UNUSED(parent);

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; i++)
        m_palette.removeItem(row);

    endRemoveRows();

    return true;
}

int PaletteModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);

    return 1;
}

void PaletteModel::moveItem(const QModelIndex & itemIndex, Palette::MoveOperation operation)
{
    m_palette.moveItem(itemIndex.row(), operation);

    emit dataChanged(QModelIndex(), QModelIndex());
}

QString PaletteModel::paletteName() const
{
    return m_palette.name();
}

void PaletteModel::setPaletteName(const QString & paletteName)
{
    m_palette.setName(paletteName);
}

QString PaletteModel::description() const
{
    return m_palette.description();
}

QString PaletteModel::comments() const
{
    return m_palette.comments();
}

void PaletteModel::generateColorNames()
{
    for (int i = 0; i < m_palette.count(); i++)
        if (m_palette.itemType(i) == PaletteItem::ColorType)
            m_palette.colorItem(i)->setColorName(m_palette.colorItem(i)->color().name());

    emit dataChanged(QModelIndex(), QModelIndex());
}

void PaletteModel::completeColorNames()
{
    for (int i = 0; i < m_palette.count(); i++)
        if (m_palette.itemType(i) == PaletteItem::ColorType)
            if (m_palette.colorItem(i)->colorName().isEmpty())
                m_palette.colorItem(i)->setColorName(m_palette.colorItem(i)->color().name());

    emit dataChanged(QModelIndex(), QModelIndex());
}

//END public methods

#include "palettemodel.moc"
