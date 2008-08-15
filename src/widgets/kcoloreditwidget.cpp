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

#include <QtGui/QVBoxLayout>

#include "palettemodel.h"
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

    MultiPageWidget * colorSelectors = new MultiPageWidget(this, i18n("Color tool:"));

    m_kdeColorSelector = new KdeColorSelector(colorSelectors);

    m_gtkColorSelector = new GtkColorSelector(colorSelectors);

    m_blenderColorSelector = new BlenderColorSelector(colorSelectors);

    colorSelectors->addPage(m_kdeColorSelector);
    colorSelectors->addPage(m_gtkColorSelector);
    colorSelectors->addPage(m_blenderColorSelector);

    m_colorDispatcher = new ColorWidget(this, ColorWidget::Simple);

    MultiPageWidget * colorInfoTexts = new MultiPageWidget(this, i18n("Color information:"));
    colorInfoTexts->setMaximumHeight(110); // NOTE default value here;

    ColorInfoTextRGB * infoTextRGB = new ColorInfoTextRGB(colorInfoTexts);
    ColorInfoTextHSV * infoTextHSV = new ColorInfoTextHSV(colorInfoTexts);
    ColorInfoTextCMYK * infoTextCMYK = new ColorInfoTextCMYK(colorInfoTexts);
    ColorInfoTextHTML * infoTextHTML = new ColorInfoTextHTML(colorInfoTexts);

    colorInfoTexts->addPage(infoTextRGB);
    colorInfoTexts->addPage(infoTextHSV);
    colorInfoTexts->addPage(infoTextCMYK);
    colorInfoTexts->addPage(infoTextHTML);

    MultiPageWidget * colorInfoVisuals = new MultiPageWidget(this, i18n("Color scheme:"));
    colorInfoVisuals->setMaximumHeight(128); // NOTE default value here;

    m_colorInfoVisualComplement = new ColorInfoVisualComplement(colorInfoVisuals);
    m_colorInfoVisualTriadic = new ColorInfoVisualTriadic(colorInfoVisuals);
    m_colorInfoVisualTetradic = new ColorInfoVisualTetradic(colorInfoVisuals);
    m_colorInfoVisualAnalogous = new ColorInfoVisualAnalogous(colorInfoVisuals);

    colorInfoVisuals->addPage(m_colorInfoVisualComplement);
    colorInfoVisuals->addPage(m_colorInfoVisualTriadic);
    colorInfoVisuals->addPage(m_colorInfoVisualTetradic);
    colorInfoVisuals->addPage(m_colorInfoVisualAnalogous);

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(colorSelectors);
    mainLayout->addWidget(m_colorDispatcher);
    mainLayout->addWidget(colorInfoTexts);
    mainLayout->addWidget(colorInfoVisuals);

//     connect(m_blenderColorSelector->addAllColorRangeAction(), SIGNAL( triggered(bool) ), this, SLOT( addAllColorRange() ));
//     connect(m_blenderColorSelector->addLowestColorRangeAction(), SIGNAL( triggered(bool) ), this, SLOT( addLowestColorRange() ));
//     connect(m_blenderColorSelector->addHighestColorRangeAction(), SIGNAL( triggered(bool) ), this, SLOT( addHighestColorRange() ));

    for (int i = 0; i < colorSelectors->count(); i++)
        connect(colorSelectors->widget(i), SIGNAL( colorSelected(QColor) ), m_colorDispatcher, SLOT( setColor(QColor) ));

    for (int j = 0; j < colorInfoTexts->count(); j++)
        connect(m_colorDispatcher, SIGNAL( colorChanged(QColor) ), colorInfoTexts->widget(j), SLOT( setColor(QColor) ));

    for (int l = 0; l < colorInfoVisuals->count(); l++)
        connect(m_colorDispatcher, SIGNAL( colorChanged(QColor) ), colorInfoVisuals->widget(l), SLOT( setColor(QColor) ));

    connect(m_colorInfoVisualComplement, SIGNAL( colorAdded(QColor) ), this, SLOT( addColorFromSchemes(QColor) ));
    connect(m_colorInfoVisualTriadic,    SIGNAL( colorAdded(QColor) ), this, SLOT( addColorFromSchemes(QColor) ));
    connect(m_colorInfoVisualTetradic,   SIGNAL( colorAdded(QColor) ), this, SLOT( addColorFromSchemes(QColor) ));
    connect(m_colorInfoVisualAnalogous,  SIGNAL( colorAdded(QColor) ), this, SLOT( addColorFromSchemes(QColor) ));
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
    // NOTE this is necesary couse the comments make invalid color when track arroud brief view
    if (color.isValid())
    {
        m_kdeColorSelector->setColor(color);
        m_gtkColorSelector->setColor(color);
        //m_blenderColorSelector->setColor(color); // DEPRECATED
    }
}

// NOTE duplicated code

void KColorEditWidget::addAllColorRange()
{
    for (int i = 0;  i < m_blenderColorSelector->allColorRange().count(); i++)
    {
        m_model->insertColorRows(m_model->rowCount(), 1);

        QVariantMap vmap;

        vmap.insert("type", QString("color"));
        vmap.insert("color", m_blenderColorSelector->allColorRange()[i]);

        m_model->setData(m_model->index(m_model->rowCount() - 1, 0), vmap);
    }
}

void KColorEditWidget::addLowestColorRange()
{
    for (int i = 0;  i < m_blenderColorSelector->lowestColorRange().count(); i++)
    {
        m_model->insertColorRows(m_model->rowCount(), 1);

        QVariantMap vmap;

        vmap.insert("type", QString("color"));
        vmap.insert("color", m_blenderColorSelector->lowestColorRange()[i]);

        m_model->setData(m_model->index(m_model->rowCount() - 1, 0), vmap);
    }
}

void KColorEditWidget::addHighestColorRange()
{
    for (int i = 0;  i < m_blenderColorSelector->highestColorRange().count(); i++)
    {
        m_model->insertColorRows(m_model->rowCount(), 1);

        QVariantMap vmap;

        vmap.insert("type", QString("color"));
        vmap.insert("color", m_blenderColorSelector->highestColorRange()[i]);

        m_model->setData(m_model->index(m_model->rowCount() - 1, 0), vmap);
    }
}

void KColorEditWidget::addColorFromSchemes(const QColor color)
{

    m_model->insertColorRows(m_model->rowCount(), 1);
    QVariantMap vmap;
    vmap.insert("type", QString("color"));
    vmap.insert("color", color);
    m_model->setData(m_model->index(m_model->rowCount() - 1, 0), vmap);

}

#include "kcoloreditwidget.moc"
