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

#include "colorinfotext.h"

#include <QClipboard>
#include <QLayout>
#include <QLabel>
#include <QPushButton>

#include <KLocalizedString>
#include <KApplication>
#include <KLineEdit>

//BEGIN ColorInfoText

ColorInfoText::ColorInfoText(QWidget * parent)
    : QWidget(parent)
{
    m_componentName1 = new QLabel(this);
    m_componentName2 = new QLabel(this);
    m_componentName3 = new QLabel(this);

    m_componentValue1 = new KLineEdit(this);
    m_componentValue1->setReadOnly(true);

    m_componentValue2 = new KLineEdit(this);
    m_componentValue2->setReadOnly(true);

    m_componentValue3 = new KLineEdit(this);
    m_componentValue3->setReadOnly(true);

    m_buttonToClipboard = new QPushButton(this);
    m_buttonToClipboard->setMaximumWidth(36);
    m_buttonToClipboard->setIcon(QIcon::fromTheme("edit-copy"));
    m_buttonToClipboard->setToolTip(i18n("Copy color values to clipboard"));

    QGridLayout * layout = new QGridLayout(this);
    layout->addWidget(m_componentName1, 0, 0, Qt::AlignLeft);
    layout->addWidget(m_componentName2, 0, 1, Qt::AlignLeft);
    layout->addWidget(m_componentName3, 0, 2, Qt::AlignLeft);
    layout->addWidget(m_componentValue1, 1, 0, Qt::AlignLeft);
    layout->addWidget(m_componentValue2, 1, 1, Qt::AlignLeft);
    layout->addWidget(m_componentValue3, 1, 2, Qt::AlignLeft);
    layout->addWidget(m_buttonToClipboard, 1, 3, Qt::AlignRight);

    connect(m_buttonToClipboard, SIGNAL( pressed () ), this, SLOT( copyColorValuesToClipboard() ));
}

ColorInfoText::~ColorInfoText()
{
}

void ColorInfoText::setColor(const QColor & color)
{
    Q_UNUSED(color);
}

void ColorInfoText::copyColorValuesToClipboard()
{
    QString tmpString = m_componentValue1->text() + ", " + m_componentValue2->text() + ", " + m_componentValue3->text();

    QClipboard * clipboard = KApplication::clipboard();
    clipboard->setText(tmpString, QClipboard::Clipboard);
    clipboard->setText(tmpString, QClipboard::Selection);
}

void ColorInfoText::setComponentNames(const QString & nameComponent1, const QString & nameComponent2, const QString & nameComponent3)
{
    m_componentName1->setText(nameComponent1);
    m_componentName2->setText(nameComponent2);
    m_componentName3->setText(nameComponent3);
}

void ColorInfoText::setComponentValues(int valueComponent1, int valueComponent2, int valueComponent3)
{
    m_componentValue1->setText(QString::number(valueComponent1));
    m_componentValue2->setText(QString::number(valueComponent2));
    m_componentValue3->setText(QString::number(valueComponent3));
}

//END ColorInfoText

//BEGIN ColorInfoTextRGB

ColorInfoTextRGB::ColorInfoTextRGB(QWidget * parent)
    : ColorInfoText(parent)
{
    setWindowTitle(i18n("RGB"));
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
    setWindowTitle(i18n("HSV"));
    setComponentNames(i18n("Hue"), i18n("Saturation"), i18n("Value"));
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
    setWindowTitle(i18n("CMY"));
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
    : QWidget(parent)
{
    setWindowTitle(i18n("Textual"));
    setWindowIcon(QIcon::fromTheme("format-text-color"));

    m_componentName1 = new QLabel(this);
    m_componentName2 = new QLabel(this);

    m_componentValue1 = new KLineEdit(this);
    m_componentValue1->setReadOnly(true);

    m_componentValue2 = new KLineEdit(this);
    m_componentValue2->setReadOnly(true);

    m_componentName1->setText(i18n("HTML"));
    m_componentName2->setText(i18n("Hexadecimal"));

    m_buttonValue1ToClipboard = new QPushButton(this);
    m_buttonValue1ToClipboard->setMaximumWidth(36);
    m_buttonValue1ToClipboard->setIcon(QIcon::fromTheme("edit-copy"));
    m_buttonValue1ToClipboard->setToolTip(i18n("Copy HTML value to clipboard"));

    m_buttonValue2ToClipboard  = new QPushButton(this);
    m_buttonValue2ToClipboard->setMaximumWidth(36);
    m_buttonValue2ToClipboard->setIcon(QIcon::fromTheme("edit-copy"));
    m_buttonValue2ToClipboard->setToolTip(i18n("Copy color hexadecimal value to clipboard"));

    QGridLayout * layout = new QGridLayout(this);
    layout->addWidget(m_componentName1, 0, 0, 1, 2, Qt::AlignLeft);
    layout->addWidget(m_componentName2, 0, 1, 1, 2, Qt::AlignLeft);
    layout->addWidget(m_componentValue1, 1, 0, Qt::AlignJustify);
    layout->addWidget(m_buttonValue1ToClipboard, 1, 1, Qt::AlignLeft);
    layout->addWidget(m_componentValue2, 1, 2, Qt::AlignJustify);
    layout->addWidget(m_buttonValue2ToClipboard, 1, 3, Qt::AlignLeft);

    connect(m_buttonValue1ToClipboard, SIGNAL( pressed () ), this, SLOT( copyColorValue1ToClipboard() ));
    connect(m_buttonValue2ToClipboard, SIGNAL( pressed () ), this, SLOT( copyColorValue2ToClipboard() ));
}

ColorInfoTextHTML::~ColorInfoTextHTML()
{
}

void ColorInfoTextHTML::setColor(const QColor & color)
{
    m_componentValue1->setText(color.name());
    m_componentValue2->setText(color.name().remove(0, 1));
}

void ColorInfoTextHTML::copyColorValue1ToClipboard()
{
    QClipboard * clipboard = KApplication::clipboard();

    clipboard->setText(m_componentValue1->text(), QClipboard::Clipboard);
    clipboard->setText(m_componentValue1->text(), QClipboard::Selection);
}

void ColorInfoTextHTML::copyColorValue2ToClipboard()
{
    QClipboard * clipboard = KApplication::clipboard();

    clipboard->setText(m_componentValue2->text(), QClipboard::Clipboard);
    clipboard->setText(m_componentValue2->text(), QClipboard::Selection);
}

//END ColorInfoTextHTML
