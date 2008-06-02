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

#include "clipboardlineedit.h"

#include <QtGui/QClipboard>
#include <QtGui/QHBoxLayout>

#include <KLocalizedString>
#include <KApplication>
#include <KLineEdit>
#include <KPushButton>

ClipboardLineEdit::ClipboardLineEdit(QWidget * parent)
    : QWidget(parent)
{
    m_lineEdit = new KLineEdit(this);
    m_lineEdit->setReadOnly(true);

    m_clipboardButton = new KPushButton(KIcon(QString("edit-copy")), QString(), this);
    m_clipboardButton->setMaximumWidth(32);
    m_clipboardButton->setToolTip(i18n("To Clipboard"));

    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(m_lineEdit);
    layout->addWidget(m_clipboardButton);

    connect(m_clipboardButton, SIGNAL( pressed () ), this, SLOT( copyTextToClipboard() ));
}

QString ClipboardLineEdit::text() const
{
    return m_lineEdit->text();
}

void ClipboardLineEdit::setText(const QString & text)
{
    m_lineEdit->setText(text);
}

void ClipboardLineEdit::copyTextToClipboard()
{
    QClipboard * clipboard = KApplication::clipboard();
    clipboard->setText(m_lineEdit->text(), QClipboard::Clipboard);
    clipboard->setText(m_lineEdit->text(), QClipboard::Selection);
}

#include "clipboardlineedit.moc"
