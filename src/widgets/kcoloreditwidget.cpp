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

#include <KLocalizedString>

#include "multipagewidget.h"
#include "kdecolorselector.h"
#include "gtkcolorselector.h"
#include "colorinfovisual.h"
#include "colorinfotext.h"

KColorEditWidget::KColorEditWidget(QWidget * parent)
    : QWidget(parent)
    , m_color(Qt::red) // default color
{
    MultiPageWidget * colorSelectors = new MultiPageWidget(this);
    colorSelectors->setPrevToolTip(i18n("Prev selector"));
    colorSelectors->setNextToolTip(i18n("Next selector"));

    m_kdeColorSelector = new KdeColorSelector(colorSelectors);

    m_gtkColorSelector = new GtkColorSelector(colorSelectors);

    colorSelectors->addPage(m_kdeColorSelector, KIcon("kde"), i18n("KDE Style"));
    colorSelectors->addPage(m_gtkColorSelector, KIcon("fill-color"), i18n("GTK Style"));

    MultiPageWidget* colorInfoVisuals = new MultiPageWidget(this);
    colorInfoVisuals->setMaximumHeight(128); // NOTE default value here;
    colorInfoVisuals->setPrevToolTip(i18n("Prev visual style"));
    colorInfoVisuals->setNextToolTip(i18n("Next visual style"));


    ColorInfoVisualSingle * colorInfoVisualSingle = new ColorInfoVisualSingle(colorInfoVisuals);

    ColorInfoVisualComplement * colorInfoVisualComplement = new ColorInfoVisualComplement(colorInfoVisuals);

    colorInfoVisuals->addPage(colorInfoVisualSingle, KIcon(), i18n("Single color"));
    colorInfoVisuals->addPage(colorInfoVisualComplement, KIcon(), i18n("Complementary color"));

    MultiPageWidget * colorInfoTexts = new MultiPageWidget(this);
    colorInfoTexts->setMaximumHeight(100); // NOTE default value here;
    colorInfoTexts->setPrevToolTip(i18n("Prev text style"));
    colorInfoTexts->setNextToolTip(i18n("Next text style"));

    ColorInfoTextRGB * infoTextRGB = new ColorInfoTextRGB(colorInfoTexts);
    infoTextRGB->setColor(Qt::red); // default color

    ColorInfoTextHSV * infoTextHSV = new ColorInfoTextHSV(colorInfoTexts);
    infoTextHSV->setColor(Qt::red); // default color

    ColorInfoTextCMY * infoTextCMY = new ColorInfoTextCMY(colorInfoTexts);
    infoTextCMY->setColor(Qt::red); // default color

    ColorInfoTextHTML * infoTextHTML = new ColorInfoTextHTML(colorInfoTexts);
    infoTextHTML->setColor(Qt::red); // default color

    colorInfoTexts->addPage(infoTextRGB, KIcon(), i18n("RGB Model"));
    colorInfoTexts->addPage(infoTextHSV, KIcon(), i18n("HSV Model"));
    colorInfoTexts->addPage(infoTextCMY, KIcon(), i18n("CMY Model"));
    colorInfoTexts->addPage(infoTextHTML, KIcon(), i18n("Other"));

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(colorSelectors);
    mainLayout->addWidget(colorInfoVisuals);
    mainLayout->addWidget(colorInfoTexts);

    for (int i = 0; i < colorSelectors->count(); i++)
    {
        connect(colorSelectors->page(i), SIGNAL( colorSelected(QColor) ), infoTextRGB, SLOT( setColor(QColor) ));
        connect(colorSelectors->page(i), SIGNAL( colorSelected(QColor) ), infoTextHSV, SLOT( setColor(QColor) ));
        connect(colorSelectors->page(i), SIGNAL( colorSelected(QColor) ), infoTextCMY, SLOT( setColor(QColor) ));
        connect(colorSelectors->page(i), SIGNAL( colorSelected(QColor) ), infoTextHTML, SLOT( setColor(QColor) ));

        connect(colorSelectors->page(i), SIGNAL( colorSelected(QColor) ), colorInfoVisualSingle, SLOT( setColor(QColor) ));
        connect(colorSelectors->page(i), SIGNAL( colorSelected(QColor) ), colorInfoVisualComplement, SLOT( setColor(QColor) ));

        connect(colorSelectors->page(i), SIGNAL( colorSelected(QColor) ), this, SLOT( getColorFromColorSelector(QColor) ));
    }
}

QColor KColorEditWidget::selectedColor() const
{
    return m_color;
}

void KColorEditWidget::setColor(const QColor & color)
{
    m_kdeColorSelector->setColor(color);
    m_gtkColorSelector->setColor(color);
}

void KColorEditWidget::getColorFromColorSelector(const QColor & color)
{
    m_color = color;
}

#include "kcoloreditwidget.moc"
