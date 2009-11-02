/*********************************************************************************
*  Copyright (C) 2009 by Percy Camilo T. Aucahuasi <percy.camilo.ta@gmail.com>   *
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
#include "buildercolorselector.h"
#include "colorwidget.h"
#include "colortoolwidget.h"
#include "colorinfovisual.h"
#include "colorinfotext.h"

#include "kcoloredit_settings.h"

KColorEditWidget::KColorEditWidget(QWidget * parent)
    : QWidget(parent)
{
    MultiPageWidget * colorTools = new MultiPageWidget(this, i18n("Tool:"));

    m_kdeColorSelector = new KdeColorSelector(colorTools);
    m_kdeColorSelector->setWindowTitle(i18n("KDE selector"));
    m_kdeColorSelector->setWindowIcon(KIcon("kde"));

    m_gtkColorSelector = new GtkColorSelector(colorTools);
    m_gtkColorSelector->setWindowTitle(i18n("GTK selector"));
    m_gtkColorSelector->setWindowIcon(KIcon("fill-color"));

    m_blenderColorSelector = new BlenderColorSelector(colorTools);
    m_blenderColorSelector->setWindowTitle(i18n("Gradient selector"));
    m_blenderColorSelector->setWindowIcon(KIcon("fill-color"));

    m_builderColorSelector = new BuilderColorSelector(colorTools);
    m_builderColorSelector->setWindowTitle(i18n("Color builder"));
    m_builderColorSelector->setWindowIcon(KIcon("format-stroke-color"));

    m_colorToolWidget = new ColorToolWidget(colorTools);
    m_colorToolWidget->setWindowTitle(i18nc("Set of extra tools apart of color selectors", "Extras"));
    m_colorToolWidget->setWindowIcon(KIcon("fill-color"));

    colorTools->addPage(m_kdeColorSelector);
    colorTools->addPage(m_gtkColorSelector);
    colorTools->addPage(m_blenderColorSelector);
    colorTools->addPage(m_builderColorSelector);
    colorTools->addPage(m_colorToolWidget);

    MultiPageWidget * colorInfoVisuals = new MultiPageWidget(this, i18nc("Refer to color scheme ... just scheme", "Scheme:"));
    colorInfoVisuals->setMaximumHeight(128);

    m_colorDispatcher = new ColorInfoVisualNormal(colorInfoVisuals);

    m_colorInfoVisualComplement = new ColorInfoVisualComplement(colorInfoVisuals);
    m_colorInfoVisualTriadic = new ColorInfoVisualTriadic(colorInfoVisuals);
    m_colorInfoVisualTetradic = new ColorInfoVisualTetradic(colorInfoVisuals);
    m_colorInfoVisualAnalogous = new ColorInfoVisualAnalogous(colorInfoVisuals);

    colorInfoVisuals->addPage(m_colorDispatcher);

    colorInfoVisuals->addPage(m_colorInfoVisualComplement);
    colorInfoVisuals->addPage(m_colorInfoVisualTriadic);
    colorInfoVisuals->addPage(m_colorInfoVisualTetradic);
    colorInfoVisuals->addPage(m_colorInfoVisualAnalogous);

    MultiPageWidget * colorInfoTexts = new MultiPageWidget(this, i18nc("Color models: for instance RGB", "Model:"));
    colorInfoTexts->setMaximumHeight(110);

    ColorInfoTextRGB * infoTextRGB = new ColorInfoTextRGB(colorInfoTexts);
    ColorInfoTextHSV * infoTextHSV = new ColorInfoTextHSV(colorInfoTexts);
    ColorInfoTextCMYK * infoTextCMYK = new ColorInfoTextCMYK(colorInfoTexts);
    ColorInfoTextHTML * infoTextHTML = new ColorInfoTextHTML(colorInfoTexts);

    colorInfoTexts->addPage(infoTextRGB);
    colorInfoTexts->addPage(infoTextHSV);
    colorInfoTexts->addPage(infoTextCMYK);
    colorInfoTexts->addPage(infoTextHTML);

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(colorTools);
    mainLayout->addWidget(colorInfoVisuals);
    mainLayout->addWidget(colorInfoTexts);

    connect(m_blenderColorSelector, SIGNAL( colorsAdded(QVector<QColor>) ), SLOT( appendColorsFromGradientSelector(QVector<QColor>) ));

    // All color selector can change the color of the dispatcher and extratools
    for (int i = 0; i < colorTools->count() - 1; i++)
    {
        connect(colorTools->widget(i), SIGNAL( colorSelected(QColor) ), m_colorDispatcher->colorWidget(), SLOT( setColor(QColor) ));
        connect(colorTools->widget(i), SIGNAL( colorSelected(QColor) ), colorTools->widget(colorTools->count() - 1), SLOT( setColor(QColor) ));
    }

    // Extratools can change the color of the dispatcher
    connect(colorTools->widget(colorTools->count() - 1), SIGNAL( colorSelected(QColor) ), m_colorDispatcher->colorWidget(), SLOT( setColor(QColor) ));

    for (int j = 0; j < colorInfoTexts->count(); j++)
        connect(m_colorDispatcher->colorWidget(), SIGNAL( colorChanged(QColor) ), colorInfoTexts->widget(j), SLOT( setColor(QColor) ));

    // All exept the dispatcher m_colorDispatcher (ColorInfoVisualNormal)
    for (int l = 1; l < colorInfoVisuals->count(); l++)
        connect(m_colorDispatcher->colorWidget(), SIGNAL( colorChanged(QColor) ), colorInfoVisuals->widget(l), SLOT( setColor(QColor) ));

    connect(m_colorDispatcher, SIGNAL( colorAdded(QColor) ), SLOT( appendColorFromSchemes(QColor) ));

    connect(m_colorInfoVisualComplement, SIGNAL( colorAdded(QColor) ), SLOT( appendColorFromSchemes(QColor) ));
    connect(m_colorInfoVisualTriadic,    SIGNAL( colorAdded(QColor) ), SLOT( appendColorFromSchemes(QColor) ));
    connect(m_colorInfoVisualTetradic,   SIGNAL( colorAdded(QColor) ), SLOT( appendColorFromSchemes(QColor) ));
    connect(m_colorInfoVisualAnalogous,  SIGNAL( colorAdded(QColor) ), SLOT( appendColorFromSchemes(QColor) ));

    // Export the color to PaletteDetailView to implement insertColorItem
    connect(m_colorDispatcher->colorWidget(), SIGNAL( colorChanged(QColor) ), SIGNAL( colorSelected(QColor) ));
}

void KColorEditWidget::setModel(PaletteModel * model)
{
    m_model = model;
}

QColor KColorEditWidget::selectedColor() const
{
    return m_colorDispatcher->colorWidget()->color();
}

void KColorEditWidget::setColor(const QColor & color)
{
    // NOTE this is necesary becouse the comments make invalid color when track/hover inside briefview
    if (color.isValid())
    {
        m_kdeColorSelector->setColor(color);
        m_gtkColorSelector->setColor(color);
        m_colorToolWidget->setColor(color);
    }
}

void KColorEditWidget::updateSettings(QString page)
{
    Q_UNUSED(page);

    m_builderColorSelector->setInputType(Settings::textInput());
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
