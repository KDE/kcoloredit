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

#include "colorwidget.h"

#include <QtGui/QClipboard>
#include <QtGui/QVBoxLayout>

#include <KLocalizedString>
#include <KApplication>
#include <KColorPatch>
#include <KPushButton>

ColorWidget::ColorWidget(QWidget * parent, ColorWidget::Mode mode)
    : QWidget(parent)
{
    m_colorPatch = new KColorPatch(this);
    m_colorPatch->setColor(Qt::red); // default color
    m_colorPatch->setAcceptDrops(false);

    setMinimumSize(64, 64);

    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(m_colorPatch, Qt::AlignJustify);

    if (mode == ColorWidget::WithActions)
    {
        m_buttonAdd = new KPushButton(this);
        m_buttonAdd->setIcon(KIcon("list-add"));
        m_buttonAdd->setMaximumWidth(36);
        m_buttonAdd->setToolTip(i18n("Append color"));

        m_buttonToClipboard = new KPushButton(this);
        m_buttonToClipboard->setMaximumWidth(36);
        m_buttonToClipboard->setIcon(KIcon("edit-copy"));
        m_buttonToClipboard->setToolTip(i18n("Copy color name to clipboard"));

        QVBoxLayout * vlayout = new QVBoxLayout();
        vlayout->addWidget(m_buttonAdd);
        vlayout->addWidget(m_buttonToClipboard);

        layout->addLayout(vlayout);

        connect(m_buttonAdd, SIGNAL( pressed () ), this, SLOT( addColor() ));
        connect(m_buttonToClipboard, SIGNAL( pressed () ), this, SLOT( copyColorNameToClipboard() ));
    }

    connect(m_colorPatch, SIGNAL( colorChanged(QColor) ), this, SLOT( updateColor(QColor) ));
}

QColor ColorWidget::color() const
{
    return m_color;
}

void ColorWidget::setColor(const QColor & color)
{
    m_color = color;

    m_colorPatch->setColor(m_color);

    emit colorChanged(m_color);
}

void ColorWidget::updateColor(const QColor & color)
{
    m_color = color;

    emit colorChanged(m_color);
}

void ColorWidget::addColor()
{
    emit colorAdded(m_color);
}

void ColorWidget::copyColorNameToClipboard()
{
    QClipboard * clipboard = KApplication::clipboard();
    clipboard->setText(m_color.name(), QClipboard::Clipboard);
    clipboard->setText(m_color.name(), QClipboard::Selection);
}

#include "colorwidget.moc"
