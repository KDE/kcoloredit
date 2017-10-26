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

#ifndef COLOR_INFO_TEXT_H
#define COLOR_INFO_TEXT_H

#include <QWidget>

class QLabel;

class KLineEdit;
class QPushButton;

class ColorInfoText : public QWidget
{
    Q_OBJECT

    public:
        ColorInfoText(QWidget * parent = 0);
        virtual ~ColorInfoText();

    protected slots:
        virtual void setColor(const QColor & color);
        void copyColorValuesToClipboard();

    protected:
        void setComponentNames(const QString & nameComponent1, const QString & nameComponent2, const QString & nameComponent3);
        void setComponentValues(int valueComponent1, int valueComponent2, int valueComponent3);

    protected:
        QLabel * m_componentName1;
        QLabel * m_componentName2;
        QLabel * m_componentName3;

        KLineEdit * m_componentValue1;
        KLineEdit * m_componentValue2;
        KLineEdit * m_componentValue3;

        QPushButton * m_buttonToClipboard;
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

class ColorInfoTextCMYK : public ColorInfoText
{
    Q_OBJECT

    public:
        ColorInfoTextCMYK(QWidget * parent = 0);
        ~ColorInfoTextCMYK();

    public slots:
        void setColor(const QColor & color);
};

class ColorInfoTextHTML : public QWidget
{
    Q_OBJECT

    public:
        ColorInfoTextHTML(QWidget * parent = 0);
        ~ColorInfoTextHTML();

    public slots:
        void setColor(const QColor & color);

    private slots:
        void copyColorValue1ToClipboard();
        void copyColorValue2ToClipboard();

    private:
        QLabel * m_componentName1;
        QLabel * m_componentName2;

        KLineEdit * m_componentValue1;
        KLineEdit * m_componentValue2;

        QPushButton * m_buttonValue1ToClipboard;
        QPushButton * m_buttonValue2ToClipboard;
};

#endif // COLOR_INFO_TEXT_H
