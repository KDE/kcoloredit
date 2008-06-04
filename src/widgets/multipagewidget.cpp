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

#include "multipagewidget.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QStackedLayout>

#include <KLocalizedString>
#include <KPushButton>
#include <KFadeWidgetEffect>

#include "kcoloreditpage.h"

MultiPageWidget::MultiPageWidget(QWidget * parent)
    : QFrame(parent)
{
    m_nextPushButton = new KPushButton(KIcon(QString("arrow-right")), QString(), this);
    m_nextPushButton->setEnabled(false);
    m_nextPushButton->setMaximumWidth(64);
    m_nextPushButton->setToolTip(i18n("Next page"));

    m_previousPushButton = new KPushButton(KIcon(QString("arrow-left")), QString(), this);
    m_previousPushButton->setEnabled(false);
    m_previousPushButton->setMaximumWidth(64);
    m_previousPushButton->setToolTip(i18n("Prev page"));

    m_stackedHeaderLayout = new QStackedLayout();

    m_stackedBodyLayout = new QStackedLayout();


    setLayout(new QGridLayout(this));
    dynamic_cast< QGridLayout* >(layout())->addWidget(m_previousPushButton, 0, 0, Qt::AlignTop);
    dynamic_cast< QGridLayout* >(layout())->addLayout(m_stackedHeaderLayout, 0, 1, Qt::AlignCenter);
    dynamic_cast< QGridLayout* >(layout())->addWidget(m_nextPushButton, 0, 2, Qt::AlignTop);
    dynamic_cast< QGridLayout* >(layout())->addLayout(m_stackedBodyLayout, 1, 0, 1, 3);

    connect(m_nextPushButton, SIGNAL( pressed() ), this, SLOT( switchToNextWidget() ));
    connect(m_previousPushButton, SIGNAL( pressed() ), this, SLOT( switchToPreviousWidget() ));
}

int MultiPageWidget::count() const
{
    return m_stackedBodyLayout->count();
}

QWidget * MultiPageWidget::page(int index) const
{
    return m_stackedBodyLayout->widget(index);
}

void MultiPageWidget::addPage(KColorEditPage * page)
{
    if (!m_stackedBodyLayout->isEmpty())
    {
        m_nextPushButton->setEnabled(true);
        m_previousPushButton->setEnabled(true);
    }

    m_stackedHeaderLayout->addWidget(page->header());
    m_stackedBodyLayout->addWidget(page);
}

void MultiPageWidget::setPrevToolTip(const QString & prevToolTip)
{
    m_previousPushButton->setToolTip(prevToolTip);
}

void MultiPageWidget::setNextToolTip(const QString & nextToolTip)
{
    m_nextPushButton->setToolTip(nextToolTip);
}

void MultiPageWidget::switchToNextWidget()
{
    if (!m_stackedBodyLayout->isEmpty())
    {
        int index = m_stackedBodyLayout->currentIndex() + 1;

        if (index >= m_stackedBodyLayout->count())
            index = 0;

        switchToPage(index);
    }
}

void MultiPageWidget::switchToPreviousWidget()
{
    if (!m_stackedBodyLayout->isEmpty())
    {
        int index = m_stackedBodyLayout->currentIndex() - 1;

        if (index < 0)
            index = m_stackedBodyLayout->count() - 1;

        switchToPage(index);
    }
}

void MultiPageWidget::switchToPage(int index)
{
    //KFadeWidgetEffect * headerEffect = new KFadeWidgetEffect(m_currentHeader);

    m_stackedHeaderLayout->setCurrentIndex(index);
    m_stackedBodyLayout->setCurrentIndex(index);

    //headerEffect->start(364);
}

#include "multipagewidget.moc"
