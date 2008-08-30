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

#ifndef BLENDER_COLOR_SELECTOR_H
#define BLENDER_COLOR_SELECTOR_H

#include "colorselector.h"

#include <KGradientSelector>

class KColorButton;

class HighlighterGradientSelector : public KGradientSelector
{
    public:
        enum Region { FirstQuarterRegion = 0, SecondQuarterRegion, ThirdQuarterRegion, FourthQuarterRegion, FirstRegion, SecondRegion, EntireRegion, NoneRegion };

        HighlighterGradientSelector(QWidget * parent = 0);

        void setRegion(HighlighterGradientSelector::Region region);

    protected:
        virtual void mouseMoveEvent(QMouseEvent * event);
        virtual void mousePressEvent(QMouseEvent * event);
        virtual void mouseReleaseEvent(QMouseEvent * event);
        virtual void paintEvent(QPaintEvent * event);

    private:
        QColor highlightedColor() const;

    private:
        HighlighterGradientSelector::Region m_region;
        bool m_leftButtonPressed;
};

class BlenderColorSelector : public ColorSelector
{
    Q_OBJECT

    public:
        BlenderColorSelector(QWidget * parent = 0);

    public slots:
        void setColor(const QColor & color);

    signals:
        void colorsAdded(const QVector<QColor> & colors);

    private slots:
        void updateMixWhenChangeBias(int factor);
        void updateMixWhenChangeColor(const QColor & color);

        void highlightFirstQuarterRegion();
        void highlightSecondQuarterRegion();
        void highlightThirdQuarterRegion();
        void highlightFourthQuarterRegion();
        void highlightFirstRegion();
        void highlightSecondRegion();
        void highlightEntireRegion();

        void appendFirstQuarterRegion();
        void appendSecondQuarterRegion();
        void appendThirdQuarterRegion();
        void appendFourthQuarterRegion();
        void appendFirstRegion();
        void appendSecondRegion();
        void appendEntireRegion();

    private:
        void performMix();
        void appendColorsOfRegion(HighlighterGradientSelector::Region region);

    private:
        float m_bias;

        HighlighterGradientSelector * m_highlighterGradientSelector;

        KColorButton * m_firstColor;
        KColorButton * m_secondColor;
};

#endif // BLENDER_COLOR_SELECTOR_H
