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

#ifndef MULTIPAGE_WIDGET_H
#define MULTIPAGE_WIDGET_H

#include <QtCore/QVector>
#include <QtCore/QPair>
#include <QtGui/QFrame>

#include <KIcon>

class QStackedLayout;

class KPushButton;

class KColorEditPage;

class MultiPageWidget : public QFrame
{
    Q_OBJECT

    public:
        MultiPageWidget(QWidget * parent = 0);

        int count() const;

        QWidget * page(int index) const;

        void addPage(KColorEditPage * page);

        void setPrevToolTip(const QString & prevToolTip);
        void setNextToolTip(const QString & nextToolTip);

    public slots:
        void switchToNextWidget();
        void switchToPreviousWidget();

    private:
        void switchToPage(int index);

    private:
        // TODO use QStackedLayout but make a bug when resize
        //QStackedLayout * m_stackedHeaderLayout;
        QVector< KPushButton * > m_headers;
        KPushButton * m_currentHeader;

        QStackedLayout * m_stackedBodyLayout;

        KPushButton * m_nextPushButton;
        KPushButton * m_previousPushButton;
};

#endif // MULTIPAGE_WIDGET_H
