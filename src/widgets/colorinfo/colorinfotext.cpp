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

#include <QtGui/QClipboard>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>

#include <KApplication>
#include <KIntSpinBox>
#include <KLineEdit>

//BEGIN ColorInfoText

ColorInfoText::ColorInfoText(QWidget * parent)
    : ColorInfo(parent)
{
    m_componentName1 = new QLabel(this);
    m_componentName2 = new QLabel(this);
    m_componentName3 = new QLabel(this);

    m_componentValue1 = new KIntSpinBox(this);
    m_componentValue2 = new KIntSpinBox(this);
    m_componentValue3 = new KIntSpinBox(this);

    m_componentValue1->setRange(0, 255);
    m_componentValue2->setRange(0, 255);
    m_componentValue3->setRange(0, 255);

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

void ColorInfoText::toClipboard()
{
    QClipboard * clipboard = KApplication::clipboard();
    clipboard->setText(textToClipboard(), QClipboard::Clipboard);
    clipboard->setText(textToClipboard(), QClipboard::Selection);
}

QString ColorInfoText::textToClipboard() const
{
    return m_componentValue1->text() + ", " + m_componentValue2->text() + ", " + m_componentValue3->text();
}

void ColorInfoText::setComponentNames(const QString & nameComponent1, const QString & nameComponent2, const QString & nameComponent3)
{
    m_componentName1->setText(nameComponent1);
    m_componentName2->setText(nameComponent2);
    m_componentName3->setText(nameComponent3);
}

void ColorInfoText::setComponentValues(int valueComponent1, int valueComponent2, int valueComponent3)
{
    m_componentValue1->setValue(valueComponent1);
    m_componentValue2->setValue(valueComponent2);
    m_componentValue3->setValue(valueComponent3);

    //m_toClipboardAction->setText(textToClipboard());
}

//END ColorInfoText

//BEGIN ColorInfoTextRGB

ColorInfoTextRGB::ColorInfoTextRGB(QWidget * parent)
    : ColorInfoText(parent)
{
    setWindowTitle(i18n("RGB Model"));
    setWindowIcon(KIcon("asd"));
    setComponentNames(i18n("Red"), i18n("Green"), i18n("Blue"));
}

ColorInfoTextRGB::~ColorInfoTextRGB()
{
}

void ColorInfoTextRGB::setColor(const QColor & color)
{
    setComponentValues(color.red(), color.green(), color.blue());
}

//END ColorInfoTextRGB

//BEGIN ColorInfoTextHSV

ColorInfoTextHSV::ColorInfoTextHSV(QWidget * parent) : ColorInfoText(parent)
{
    setWindowTitle(i18n("HSV Model"));
    setWindowIcon(KIcon("asd"));
    setComponentNames(i18n("Hue"), i18n("Saturation"), i18n("Value"));
    m_componentValue1->setMaximum(359);
}

ColorInfoTextHSV::~ColorInfoTextHSV()
{
}

void ColorInfoTextHSV::setColor(const QColor & color)
{
    setComponentValues(color.hue(), color.saturation(), color.value());
}

//END ColorInfoTextHSV

//BEGIN ColorInfoTextCMYK

ColorInfoTextCMYK::ColorInfoTextCMYK(QWidget * parent)
    : ColorInfoText(parent)
{
    setWindowTitle(i18n("CMYK Model"));
    setWindowIcon(KIcon("asd"));
    setComponentNames(i18n("Cyan"), i18n("Magenta"), i18n("Yellow"));
}

ColorInfoTextCMYK::~ColorInfoTextCMYK()
{
}

void ColorInfoTextCMYK::setColor(const QColor & color)
{
    setComponentValues(color.cyan(), color.magenta(), color.yellow());
}

//END ColorInfoTextCMYK

//END ColorInfoTextHTML

ColorInfoTextHTML::ColorInfoTextHTML(QWidget * parent)
    : ColorInfo(parent)
{
    setWindowTitle(i18n("Textual"));
    setWindowIcon(KIcon("asd"));

    m_componentName1 = new QLabel(this);
    m_componentName2 = new QLabel(this);

    m_componentValue1 = new KLineEdit(this);
    m_componentValue2 = new KLineEdit(this);

    m_componentName1->setText(i18n("HTML"));
    m_componentName2->setText("Hexadecimal");

    QGridLayout * layout = new QGridLayout(this);
    layout->addWidget(m_componentName1, 0, 0, Qt::AlignLeft);
    layout->addWidget(m_componentName2, 0, 1, Qt::AlignLeft);
    layout->addWidget(m_componentValue1, 1, 0, Qt::AlignLeft);
    layout->addWidget(m_componentValue2, 1, 1, Qt::AlignLeft);
}

ColorInfoTextHTML::~ColorInfoTextHTML()
{
}

void ColorInfoTextHTML::setColor(const QColor & color)
{
//    setComponentValues(color.name(), color.name().remove(0, 1), "");

    //m_toClipboardAction->setText(m_componentValue1->text() + ", " + m_componentValue2->text());
}

void ColorInfoTextHTML::toClipboard()
{
    QClipboard * clipboard = KApplication::clipboard();

    clipboard->setText(m_componentValue1->text() + ", " + m_componentValue2->text(), QClipboard::Clipboard);
    clipboard->setText(m_componentValue1->text() + ", " + m_componentValue2->text(), QClipboard::Selection);
}

//END ColorInfoTextHTML

#include "colorinfotext.moc"
