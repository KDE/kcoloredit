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

#include "buildercolorselector.h"

#include <QMouseEvent>
#include <QLayout>
#include <QRadioButton>
#include <QStackedWidget>
#include <QTabWidget>

#include <KColorUtils>
#include <KColorButton>

#include "colorutil.h"

#include "kcoloredit_settings.h"

BuilderColorSelector::BuilderColorSelector(QWidget * parent)
    : ColorSelector(parent)
    , m_currentSource(0)
{
    QWidget *builderWidget = new QWidget(this);

    m_uiBuilderForm.setupUi(builderWidget);

    setInputType(Settings::textInput());

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(builderWidget);

    connect(m_uiBuilderForm.buildColorPushButton, SIGNAL(pressed()), SLOT(buildColor()));
    connect(m_uiBuilderForm.controlsInputTabWidget, SIGNAL(currentChanged(int)), SLOT(setCurrentSource(int)));

    connect(m_uiBuilderForm.controlsInputTabWidget, SIGNAL(currentChanged(int)), SLOT(setCurrentSource(int)));
//     valueChanged ( int i )
}

void BuilderColorSelector::setInputType(bool textInput)
{
    if (textInput)
    {
        m_uiBuilderForm.controlsInputTabWidget->setVisible(false);
        m_uiBuilderForm.textInputWidget->setVisible(true);
    }
    else
    {
        m_uiBuilderForm.controlsInputTabWidget->setVisible(true);
        m_uiBuilderForm.textInputWidget->setVisible(false);
    }
}

void BuilderColorSelector::setColor(const QColor & color)
{
    Q_UNUSED(color);
}

void BuilderColorSelector::buildColor()
{
    //QMessageBox::warning(this, tr("My Application"), QString::number(m_currentSource), QMessageBox::Ok);

    emit colorSelected(Qt::yellow);
}

void BuilderColorSelector::setCurrentSource(int tabIndex)
{
    m_currentSource = tabIndex;
}

//
