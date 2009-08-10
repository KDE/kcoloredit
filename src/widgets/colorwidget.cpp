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

#include <QtCore/QMimeData>
#include <QtGui/QClipboard>
#include <QtGui/QVBoxLayout>
#include <QtGui/QToolButton>

#include <KLocalizedString>
#include <KApplication>
#include <KColorPatch>

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
        m_buttonAdd = new QToolButton(m_colorPatch);
        m_buttonAdd->setIcon(KIcon("list-add"));
        //m_buttonAdd->setAutoRaise(true);
        m_buttonAdd->setToolTip(i18n("Append color"));

        m_buttonToClipboard = new QToolButton(m_colorPatch);
        //m_buttonToClipboard->setAutoRaise(true);
        m_buttonToClipboard->setIcon(KIcon("edit-copy"));
        m_buttonToClipboard->setToolTip(i18n("Copy color to clipboard"));

        QVBoxLayout * vlayout = new QVBoxLayout(m_colorPatch);
        vlayout->addWidget(m_buttonAdd, Qt::AlignRight);
        vlayout->addWidget(m_buttonToClipboard, Qt::AlignRight);

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
    QMimeData *mime = new QMimeData();
    mime->setColorData(m_color);
    mime->setText(m_color.name());

    QApplication::clipboard()->setMimeData(mime, QClipboard::Clipboard);
}

#include "colorwidget.moc"
