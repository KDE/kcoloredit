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

#include "colorinfovisual.h"
#include "colorinfotext.h"

KColorEditWidget::KColorEditWidget(QWidget * parent) : QWidget(parent), m_color(Qt::red) // default color
{
    // TODO i18n and l10n

    m_selectors = new MultiPageWidget(this);

    KdeColorSelector * kdeColorSelector = new KdeColorSelector(m_selectors);

    m_selectors->addPage(kdeColorSelector, KIcon("kde"), QString("KDE Style"));

    m_infoStyles = new MultiPageWidget(this);
    m_infoStyles->setMaximumHeight(128); // NOTE default value here;

    m_colorInfoVisualSingle = new ColorInfoVisualSingle(m_infoStyles);

    m_colorInfoVisualComplement = new ColorInfoVisualComplement(m_infoStyles);

    m_infoStyles->addPage(m_colorInfoVisualSingle, KIcon(), QString("Single"));
    m_infoStyles->addPage(m_colorInfoVisualComplement, KIcon(), QString("Complement"));

    m_infoTextModels = new MultiPageWidget(this);
    m_infoTextModels->setMaximumHeight(100); // NOTE default value here;

    ColorInfoTextRGB * infoTextRGB = new ColorInfoTextRGB(m_infoTextModels);
    infoTextRGB->setColor(Qt::red); // default color

    ColorInfoTextHSV * infoTextHSV = new ColorInfoTextHSV(m_infoTextModels);
    infoTextHSV->setColor(Qt::red); // default color

    ColorInfoTextCMY * infoTextCMY = new ColorInfoTextCMY(m_infoTextModels);
    infoTextCMY->setColor(Qt::red); // default color

    ColorInfoTextHTML * infoTextHTML = new ColorInfoTextHTML(m_infoTextModels);
    infoTextHTML->setColor(Qt::red); // default color

    m_infoTextModels->addPage(infoTextRGB, KIcon(), QString("RGB Model"));
    m_infoTextModels->addPage(infoTextHSV, KIcon(), QString("HSV Model"));
    m_infoTextModels->addPage(infoTextCMY, KIcon(), QString("CMY Model"));
    m_infoTextModels->addPage(infoTextHTML, KIcon(), QString("CMY Model"));

    /// layouts

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_selectors);
    mainLayout->addWidget(m_infoStyles);
    mainLayout->addWidget(m_infoTextModels);


    // think ...
    //connect(m_kdeColorSelector, SIGNAL( colorSelected(QColor) ), m_colorInfoVisualSingle, SLOT( slotSetColor(QColor) ));
    //connect(m_kdeColorSelector, SIGNAL( colorSelected(QColor) ), m_colorInfoVisualComplement, SLOT( slotSetColor(QColor) ));

    // workflow ?
    // si o si
    connect(kdeColorSelector, SIGNAL( colorSelected(QColor) ), infoTextRGB, SLOT( setColor(QColor) ));
    connect(kdeColorSelector, SIGNAL( colorSelected(QColor) ), infoTextHSV, SLOT( setColor(QColor) ));
    connect(kdeColorSelector, SIGNAL( colorSelected(QColor) ), infoTextCMY, SLOT( setColor(QColor) ));
    connect(kdeColorSelector, SIGNAL( colorSelected(QColor) ), infoTextHTML, SLOT( setColor(QColor) ));

    connect(kdeColorSelector, SIGNAL( colorSelected(QColor) ), m_colorInfoVisualSingle, SLOT( setColor(QColor) ));
    connect(kdeColorSelector, SIGNAL( colorSelected(QColor) ), m_colorInfoVisualComplement, SLOT( setColor(QColor) ));

    // especifico para esta clase lo de arriba es para q le de
    //a los styles el color osea cambian el main color de los diferentes styles
    connect(kdeColorSelector, SIGNAL( colorSelected(QColor) ), this, SLOT( getColorFromColorSelector(QColor) ));
}

QColor KColorEditWidget::selectedColor() const
{
    return m_color;
}

void KColorEditWidget::setColor(const QColor & color)
{
    m_color = color;

    m_colorInfoVisualSingle->setColor(m_color);
    m_colorInfoVisualComplement->setColor(m_color);
}

void KColorEditWidget::getColorFromColorSelector(const QColor & color)
{
    m_color = color;
}

#include "kcoloreditwidget.moc"
