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

#include <QtGui/QLayout>

#include <KLocalizedString>
#include <KIcon>

#include "palettemodel.h"
#include "multipagewidget.h"
#include "kdecolorselector.h"
#include "gtkcolorselector.h"
#include "blendercolorselector.h"
#include "colortoolwidget.h"
#include "colorwidget.h"
#include "colorinfovisual.h"
#include "colorinfotext.h"

KColorEditWidget::KColorEditWidget(QWidget * parent)
    : QWidget(parent)
{

    MultiPageWidget * colorTools = new MultiPageWidget(this, i18n("Color tool:"));

    m_kdeColorSelector = new KdeColorSelector(colorTools);
    m_kdeColorSelector->setWindowTitle(i18n("KDE selector"));
    m_kdeColorSelector->setWindowIcon(KIcon("kde"));

    m_gtkColorSelector = new GtkColorSelector(colorTools);
    m_gtkColorSelector->setWindowTitle(i18n("GTK selector"));
    m_gtkColorSelector->setWindowIcon(KIcon("fill-color"));

    m_blenderColorSelector = new BlenderColorSelector(colorTools);
    m_blenderColorSelector->setWindowTitle(i18n("Gradient selector"));
    m_blenderColorSelector->setWindowIcon(KIcon("fill-color"));

    m_colorToolWidget = new ColorToolWidget(colorTools);
    m_colorToolWidget->setWindowTitle(i18nc("Set of extra tools apart of color selectors", "Extras"));
    m_colorToolWidget->setWindowIcon(KIcon("fill-color"));

    colorTools->addPage(m_kdeColorSelector);
    colorTools->addPage(m_gtkColorSelector);
    colorTools->addPage(m_blenderColorSelector);
    colorTools->addPage(m_colorToolWidget);

    m_colorDispatcher = new ColorWidget(this, ColorWidget::Simple);

    MultiPageWidget * colorInfoTexts = new MultiPageWidget(this, i18n("Color information:"));
    colorInfoTexts->setMaximumHeight(110);

    ColorInfoTextRGB * infoTextRGB = new ColorInfoTextRGB(colorInfoTexts);
    ColorInfoTextHSV * infoTextHSV = new ColorInfoTextHSV(colorInfoTexts);
    ColorInfoTextCMYK * infoTextCMYK = new ColorInfoTextCMYK(colorInfoTexts);
    ColorInfoTextHTML * infoTextHTML = new ColorInfoTextHTML(colorInfoTexts);

    colorInfoTexts->addPage(infoTextRGB);
    colorInfoTexts->addPage(infoTextHSV);
    colorInfoTexts->addPage(infoTextCMYK);
    colorInfoTexts->addPage(infoTextHTML);

    MultiPageWidget * colorInfoVisuals = new MultiPageWidget(this, i18n("Color scheme:"));
    colorInfoVisuals->setMaximumHeight(128);

    m_colorInfoVisualComplement = new ColorInfoVisualComplement(colorInfoVisuals);
    m_colorInfoVisualTriadic = new ColorInfoVisualTriadic(colorInfoVisuals);
    m_colorInfoVisualTetradic = new ColorInfoVisualTetradic(colorInfoVisuals);
    m_colorInfoVisualAnalogous = new ColorInfoVisualAnalogous(colorInfoVisuals);

    colorInfoVisuals->addPage(m_colorInfoVisualComplement);
    colorInfoVisuals->addPage(m_colorInfoVisualTriadic);
    colorInfoVisuals->addPage(m_colorInfoVisualTetradic);
    colorInfoVisuals->addPage(m_colorInfoVisualAnalogous);

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(colorTools);
    mainLayout->addWidget(m_colorDispatcher);
    mainLayout->addWidget(colorInfoTexts);
    mainLayout->addWidget(colorInfoVisuals);

    connect(m_blenderColorSelector, SIGNAL( colorsAdded(QVector<QColor>) ), SLOT( appendColorsFromGradientSelector(QVector<QColor>) ));

    // All color selector can change the color of the dispatcher and extratools
    for (int i = 0; i < colorTools->count() - 1; i++)
    {
        connect(colorTools->widget(i), SIGNAL( colorSelected(QColor) ), m_colorDispatcher, SLOT( setColor(QColor) ));
        connect(colorTools->widget(i), SIGNAL( colorSelected(QColor) ), colorTools->widget(colorTools->count() - 1), SLOT( setColor(QColor) ));
    }

    // Extratools can change the color of the dispatcher
    connect(colorTools->widget(colorTools->count() - 1), SIGNAL( colorSelected(QColor) ), m_colorDispatcher, SLOT( setColor(QColor) ));

    for (int j = 0; j < colorInfoTexts->count(); j++)
        connect(m_colorDispatcher, SIGNAL( colorChanged(QColor) ), colorInfoTexts->widget(j), SLOT( setColor(QColor) ));

    for (int l = 0; l < colorInfoVisuals->count(); l++)
        connect(m_colorDispatcher, SIGNAL( colorChanged(QColor) ), colorInfoVisuals->widget(l), SLOT( setColor(QColor) ));

    connect(m_colorInfoVisualComplement, SIGNAL( colorAdded(QColor) ), SLOT( appendColorFromSchemes(QColor) ));
    connect(m_colorInfoVisualTriadic,    SIGNAL( colorAdded(QColor) ), SLOT( appendColorFromSchemes(QColor) ));
    connect(m_colorInfoVisualTetradic,   SIGNAL( colorAdded(QColor) ), SLOT( appendColorFromSchemes(QColor) ));
    connect(m_colorInfoVisualAnalogous,  SIGNAL( colorAdded(QColor) ), SLOT( appendColorFromSchemes(QColor) ));
}

void KColorEditWidget::setModel(PaletteModel * model)
{
    m_model = model;
}

QColor KColorEditWidget::selectedColor() const
{
    return m_colorDispatcher->color();
}

void KColorEditWidget::setColor(const QColor & color)
{
    // NOTE this is necesary couse the comments make invalid color when track/hover inside briefview
    if (color.isValid())
    {
        m_kdeColorSelector->setColor(color);
        m_gtkColorSelector->setColor(color);
        m_colorToolWidget->setColor(color);
    }
}

void KColorEditWidget::appendColorsFromGradientSelector(const QVector<QColor> & colors)
{
    for (int i = 0; i < colors.count(); i++)
        m_model->appendColorItem(colors[i]);
}

void KColorEditWidget::appendColorFromSchemes(const QColor & color)
{
    m_model->appendColorItem(color);
}

#include "kcoloreditwidget.moc"
