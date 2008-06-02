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

#include "colorinfotext.h"

#include <QtGui/QGridLayout>
#include <QtGui/QLabel>

#include <KLocalizedString>

#include "clipboardlineedit.h"

//BEGIN ColorInfoText

ColorInfoText::ColorInfoText(QWidget * parent) : ColorInfo(parent)
{
    m_componentName1 = new QLabel(this);

    m_componentName2 = new QLabel(this);

    m_componentName3 = new QLabel(this);

    m_componentValue1 = new ClipboardLineEdit(this);

    m_componentValue2 = new ClipboardLineEdit(this);

    m_componentValue3 = new ClipboardLineEdit(this);

    QGridLayout * layout = new QGridLayout(this);
    layout->addWidget(m_componentName1, 0, 0, Qt::AlignLeft);
    layout->addWidget(m_componentName2, 0, 1, Qt::AlignLeft);
    layout->addWidget(m_componentName3, 0, 2, Qt::AlignLeft);
    layout->addWidget(m_componentValue1, 1, 0, Qt::AlignLeft);
    layout->addWidget(m_componentValue2, 1, 1, Qt::AlignLeft);
    layout->addWidget(m_componentValue3, 1, 2, Qt::AlignLeft);
}

ColorInfoText::~ColorInfoText()
{
}

void ColorInfoText::setComponentNames(const QString & nameComponent1, const QString & nameComponent2, const QString & nameComponent3)
{
    m_componentName1->setText(nameComponent1);
    m_componentName2->setText(nameComponent2);
    m_componentName3->setText(nameComponent3);
}

void ColorInfoText::setComponentValues(const QString & valueComponent1, const QString & valueComponent2, const QString & valueComponent3)
{
    m_componentValue1->setText(valueComponent1);
    m_componentValue2->setText(valueComponent2);
    m_componentValue3->setText(valueComponent3);
}

//END ColorInfoText

//BEGIN ColorInfoTextRGB

ColorInfoTextRGB::ColorInfoTextRGB(QWidget * parent) : ColorInfoText(parent)
{
    setComponentNames(i18n("Red"), i18n("Green"), i18n("Blue"));
}

ColorInfoTextRGB::~ColorInfoTextRGB()
{
}

void ColorInfoTextRGB::setColor(const QColor & color)
{
    setComponentValues(QString::number(color.red()), QString::number(color.green()), QString::number(color.blue()));
}

//END ColorInfoTextRGB

//BEGIN ColorInfoTextHSV

ColorInfoTextHSV::ColorInfoTextHSV(QWidget * parent) : ColorInfoText(parent)
{
    setComponentNames(i18n("Hue"), i18n("Saturation"), i18n("Value"));
}

ColorInfoTextHSV::~ColorInfoTextHSV()
{
}

void ColorInfoTextHSV::setColor(const QColor & color)
{
    setComponentValues(QString::number(color.hue()), QString::number(color.saturation()), QString::number(color.value()));
}

//END ColorInfoTextHSV

//BEGIN ColorInfoTextCMY

ColorInfoTextCMY::ColorInfoTextCMY(QWidget * parent) : ColorInfoText(parent)
{
    setComponentNames(i18n("Cyan"), i18n("Magenta"), i18n("Yellow"));
}

ColorInfoTextCMY::~ColorInfoTextCMY()
{
}

void ColorInfoTextCMY::setColor(const QColor & color)
{
    setComponentValues(QString::number(color.cyan()), QString::number(color.magenta()), QString::number(color.yellow()));
}

//END ColorInfoTextCMY

//END ColorInfoTextHTML

ColorInfoTextHTML::ColorInfoTextHTML(QWidget * parent) : ColorInfoText(parent)
{
    setComponentNames(i18n("HTML"), "", "");

    m_componentName2->setVisible(false);
    m_componentName3->setVisible(false);

    m_componentValue2->setVisible(false);
    m_componentValue3->setVisible(false);
}

ColorInfoTextHTML::~ColorInfoTextHTML()
{
}

void ColorInfoTextHTML::setColor(const QColor & color)
{
    setComponentValues(color.name(), "", "");
}

//END ColorInfoTextHTML

#include "colorinfotext.moc"
