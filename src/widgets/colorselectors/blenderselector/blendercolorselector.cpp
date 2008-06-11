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

#include "blendercolorselector.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QSlider>
#include <QtGui/QLabel>

#include <KColorUtils>
#include <KPushButton>
#include <KColorButton>

BlenderColorSelector::BlenderColorSelector(QWidget * parent)
    : ColorSelector(parent)
{
    m_header->setText(i18n("Mix Colors"));
    m_header->setIcon(KIcon("fill-color"));

    m_addAllColorRangeAction = new KAction(KIcon("list-add"), i18n("Add All Color Range"), header()->menu());
    m_addLowestColorRangeAction = new KAction(KIcon("list-add"), i18n("Add Lowest Color Range"), header()->menu());
    m_addHighestColorRangeAction = new KAction(KIcon("list-add"), i18n("Add Highest Color Range"), header()->menu());

    header()->menu()->addAction(m_addAllColorRangeAction);
    header()->menu()->addAction(m_addLowestColorRangeAction);
    header()->menu()->addAction(m_addHighestColorRangeAction);

    m_baseColor = new KColorButton(this);
    m_baseColor->setMinimumHeight(64);

    m_linearMixer = new QSlider(Qt::Horizontal, this);
    m_linearMixer->setRange(1, 100);
    m_linearMixer->setTickInterval(1);

    m_overlayedColor = new KColorButton(this);
    m_overlayedColor->setMinimumHeight(64);

    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(m_baseColor);
    layout->addWidget(m_linearMixer);
    layout->addWidget(m_overlayedColor);

    connect(m_linearMixer, SIGNAL( valueChanged(int) ), this, SLOT( updateMixWhenChangeBias(int) ));

    connect(m_baseColor, SIGNAL( changed(QColor) ), this, SLOT( updateMixWhenChangeColor(QColor) ));
    connect(m_overlayedColor, SIGNAL( changed(QColor) ), this, SLOT( updateMixWhenChangeColor(QColor) ));
}

KAction * BlenderColorSelector::addAllColorRangeAction() const
{
    return m_addAllColorRangeAction;
}

KAction * BlenderColorSelector::addLowestColorRangeAction() const
{
    return m_addLowestColorRangeAction;
}

KAction * BlenderColorSelector::addHighestColorRangeAction() const
{
    return m_addHighestColorRangeAction;
}

QVector<QColor> BlenderColorSelector::allColorRange() const
{
    QVector<QColor> tmpColorRange;

    for (int i = 1; i <= 100; i++)
        tmpColorRange.append(KColorUtils::mix(m_baseColor->color(), m_overlayedColor->color(), static_cast<float>(i)/100.0f));

    return tmpColorRange;
}

QVector<QColor> BlenderColorSelector::lowestColorRange() const
{
    QVector<QColor> tmpColorRange;

    for (int i = 1; i <= m_linearMixer->value(); i++)
        tmpColorRange.append(KColorUtils::mix(m_baseColor->color(), m_overlayedColor->color(), static_cast<float>(i)/100.0f));

    return tmpColorRange;
}

QVector<QColor> BlenderColorSelector::highestColorRange() const
{
    QVector<QColor> tmpColorRange;

    for (int i = m_linearMixer->value(); i <= 100; i++)
        tmpColorRange.append(KColorUtils::mix(m_baseColor->color(), m_overlayedColor->color(), static_cast<float>(i)/100.0f));

    return tmpColorRange;
}

void BlenderColorSelector::setColor(const QColor & color)
{
    m_baseColor->setColor(color);
    m_overlayedColor->setColor(QColor(255 - color.red(), 255 - color.green(), 255 - color.blue()));
}

void BlenderColorSelector::updateMixWhenChangeBias(int factor)
{
    m_bias = static_cast< float >(factor)/100.0f;

    performMix();
}

void BlenderColorSelector::updateMixWhenChangeColor(const QColor & color)
{
    Q_UNUSED(color);

    performMix();
}

void BlenderColorSelector::performMix()
{
    emit colorSelected(KColorUtils::mix(m_baseColor->color(), m_overlayedColor->color(), m_bias));
}

#include "blendercolorselector.moc"
