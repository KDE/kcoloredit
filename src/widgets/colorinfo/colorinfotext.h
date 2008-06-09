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

#ifndef COLOR_INFO_TEXT_H
#define COLOR_INFO_TEXT_H

#include "colorinfo.h"

class QLabel;

class ClipboardLineEdit;

class ColorInfoText : public ColorInfo
{
    Q_OBJECT

    public:
        ColorInfoText(QWidget * parent = 0);
        virtual ~ColorInfoText();

    protected slots:
        virtual void toClipboard();

    protected:
        QString textToClipboard() const;

        void setComponentNames(const QString & nameComponent1, const QString & nameComponent2, const QString & nameComponent3);
        void setComponentValues(const QString & valueComponent1, const QString & valueComponent2, const QString & valueComponent3);

    protected:
        KAction * m_toClipboardAction;

        QLabel * m_componentName1;
        QLabel * m_componentName2;
        QLabel * m_componentName3;

        ClipboardLineEdit * m_componentValue1;
        ClipboardLineEdit * m_componentValue2;
        ClipboardLineEdit * m_componentValue3;
};

class ColorInfoTextRGB : public ColorInfoText
{
    Q_OBJECT

    public:
        ColorInfoTextRGB(QWidget * parent = 0);
        ~ColorInfoTextRGB();

    public slots:
        void setColor(const QColor & color);
};

class ColorInfoTextHSV : public ColorInfoText
{
    Q_OBJECT

    public:
        ColorInfoTextHSV(QWidget * parent = 0);
        ~ColorInfoTextHSV();

    public slots:
        void setColor(const QColor & color);
};

class ColorInfoTextCMY : public ColorInfoText
{
    Q_OBJECT

    public:
        ColorInfoTextCMY(QWidget * parent = 0);
        ~ColorInfoTextCMY();

    public slots:
        void setColor(const QColor & color);
};

class ColorInfoTextHTML : public ColorInfoText
{
    Q_OBJECT

    public:
        ColorInfoTextHTML(QWidget * parent = 0);
        ~ColorInfoTextHTML();

    public slots:
        void setColor(const QColor & color);

    private slots:
        void toClipboard();
};

#endif // COLOR_INFO_TEXT_H
