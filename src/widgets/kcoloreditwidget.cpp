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

#include "kcoloreditwidget.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>

#include "multipagewidget.h"
#include "kdecolorselector.h"
#include "gtkcolorselector.h"
#include "blendercolorselector.h"
#include "colorwidget.h"
#include "colorinfovisual.h"
#include "colorinfotext.h"

KColorEditWidget::KColorEditWidget(QWidget * parent)
    : QWidget(parent)
{

    MultiPageWidget * colorSelectors = new MultiPageWidget(this);
    colorSelectors->setPrevToolTip(i18n("Prev selector"));
    colorSelectors->setNextToolTip(i18n("Next selector"));

    m_kdeColorSelector = new KdeColorSelector(colorSelectors);

    m_gtkColorSelector = new GtkColorSelector(colorSelectors);

    m_blenderColorSelector = new BlenderColorSelector(colorSelectors);

    colorSelectors->addPage(m_kdeColorSelector);
    colorSelectors->addPage(m_gtkColorSelector);
    colorSelectors->addPage(m_blenderColorSelector);


    m_colorDispatcher = new ColorWidget(this);

    MultiPageWidget * colorInfoTexts = new MultiPageWidget(this);
    colorInfoTexts->setMaximumHeight(100); // NOTE default value here;
    colorInfoTexts->setPrevToolTip(i18n("Prev text style"));
    colorInfoTexts->setNextToolTip(i18n("Next text style"));

    ColorInfoTextRGB * infoTextRGB = new ColorInfoTextRGB(colorInfoTexts);

    ColorInfoTextHSV * infoTextHSV = new ColorInfoTextHSV(colorInfoTexts);

    ColorInfoTextCMY * infoTextCMY = new ColorInfoTextCMY(colorInfoTexts);

    ColorInfoTextHTML * infoTextHTML = new ColorInfoTextHTML(colorInfoTexts);

    colorInfoTexts->addPage(infoTextRGB);
    colorInfoTexts->addPage(infoTextHSV);
    colorInfoTexts->addPage(infoTextCMY);
    colorInfoTexts->addPage(infoTextHTML);

    MultiPageWidget* colorInfoVisuals = new MultiPageWidget(this);
    colorInfoVisuals->setMaximumHeight(128); // NOTE default value here;
    colorInfoVisuals->setPrevToolTip(i18n("Prev visual style"));
    colorInfoVisuals->setNextToolTip(i18n("Next visual style"));

    ColorInfoVisualComplement * colorInfoVisualComplement = new ColorInfoVisualComplement(colorInfoVisuals);

    ColorInfoVisualTriadic * colorInfoVisualTriadic = new ColorInfoVisualTriadic(colorInfoVisuals);

    ColorInfoVisualTetradic * colorInfoVisualTetradic = new ColorInfoVisualTetradic(colorInfoVisuals);

    ColorInfoVisualAnalogous * colorInfoVisualAnalogous = new ColorInfoVisualAnalogous(colorInfoVisuals);

    colorInfoVisuals->addPage(colorInfoVisualComplement);
    colorInfoVisuals->addPage(colorInfoVisualTriadic);
    colorInfoVisuals->addPage(colorInfoVisualTetradic);
    colorInfoVisuals->addPage(colorInfoVisualAnalogous);


    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(colorSelectors);
    mainLayout->addWidget(m_colorDispatcher);
    mainLayout->addWidget(colorInfoTexts);
    mainLayout->addWidget(colorInfoVisuals);

    for (int i = 0; i < colorSelectors->count(); i++)
        connect(colorSelectors->page(i), SIGNAL( colorSelected(QColor) ), m_colorDispatcher, SLOT( setColor(QColor) ));

    for (int k = 0; k < colorInfoTexts->count(); k++)
        connect(m_colorDispatcher, SIGNAL( colorChanged(QColor) ), colorInfoTexts->page(k), SLOT( setColor(QColor) ));

    for (int j = 0; j < colorInfoVisuals->count(); j++)
        connect(m_colorDispatcher, SIGNAL( colorChanged(QColor) ), colorInfoVisuals->page(j), SLOT( setColor(QColor) ));
}

QColor KColorEditWidget::selectedColor() const
{
    return m_colorDispatcher->color();
}

void KColorEditWidget::setColor(const QColor & color)
{
    m_kdeColorSelector->setColor(color);
    m_gtkColorSelector->setColor(color);
    m_blenderColorSelector->setColor(color);
}

#include "kcoloreditwidget.moc"
