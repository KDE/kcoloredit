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

#include "blendercolorselector.h"

#include <QtGui/QMouseEvent>
#include <QtGui/QLayout>
#include <QtGui/QToolButton>

#include <KAction>
#include <KColorUtils>
#include <KColorButton>

#include "colorutil.h"

HighlighterGradientSelector::HighlighterGradientSelector(QWidget * parent)
    : KGradientSelector(parent)
    , m_region(HighlighterGradientSelector::NoneRegion)
    , m_leftButtonPressed(false)
{
    setMouseTracking(true);
    setArrowDirection(Qt::UpArrow);
    setRange(1, 100);
    setValue(50);
}

void HighlighterGradientSelector::setRegion(HighlighterGradientSelector::Region region)
{
    if (m_region == region)
        return ;

    m_region = region;

    repaint();
}

void HighlighterGradientSelector::mouseMoveEvent(QMouseEvent * event)
{
    if (m_leftButtonPressed)
    {
        KGradientSelector::mouseMoveEvent(event);

        return ;
    }

    m_region = HighlighterGradientSelector::NoneRegion;

    update();
}

void HighlighterGradientSelector::mousePressEvent(QMouseEvent * event)
{
    KGradientSelector::mousePressEvent(event);

    if (event->button() == Qt::LeftButton)
        m_leftButtonPressed = true;
    else
        m_leftButtonPressed = false;
}

void HighlighterGradientSelector::mouseReleaseEvent(QMouseEvent * event)
{
    KGradientSelector::mouseReleaseEvent(event);

    m_leftButtonPressed = false;
}

void HighlighterGradientSelector::paintEvent(QPaintEvent * event)
{
    KGradientSelector::paintEvent(event);

    if (m_region == HighlighterGradientSelector::NoneRegion)
        return ;

    QPainter painter;
    QRect rect;

    int x = contentsRect().x();
    int y = contentsRect().y();
    int width = contentsRect().width();
    int height = contentsRect().height();

    switch (m_region)
    {
        case HighlighterGradientSelector::FirstQuarterRegion:
            rect = QRect(x, y, static_cast<int>(width/4), height);
            break;
        case HighlighterGradientSelector::SecondQuarterRegion:
            rect = QRect(x + static_cast<int>(width/4), y, static_cast<int>(width/4), height);
            break;
        case HighlighterGradientSelector::ThirdQuarterRegion:
            rect = QRect(x + static_cast<int>(width/2), y, static_cast<int>(width/4), height);
            break;
        case HighlighterGradientSelector::FourthQuarterRegion:
            rect = QRect(x + static_cast<int>(width*3/4), y, static_cast<int>(width/4), height);
            break;
        case HighlighterGradientSelector::FirstRegion:
            rect = QRect(x, y, static_cast<int>(width*value()/100), height);
            break;
        case HighlighterGradientSelector::SecondRegion:
            rect = QRect(x + static_cast<int>(width*value()/100), y, width - static_cast<int>(width*value()/100), height);
            break;
        case HighlighterGradientSelector::EntireRegion:
            rect = contentsRect();
            break;
        case HighlighterGradientSelector::NoneRegion:
            break;
    }

    painter.begin(this);
        QPen pen(highlightedColor());
        pen.setWidth(4);
        painter.setPen(pen);
        painter.drawRect(rect);
    painter.end();
}

QColor HighlighterGradientSelector::highlightedColor() const
{
    QColor tmpColor = ColorUtil::triadicColors(firstColor())[0];

    if (tmpColor.saturation() < 64)
        tmpColor = ColorUtil::triadicColors(secondColor())[1];

    if (tmpColor.saturation() < 64)
        tmpColor = Qt::darkBlue;

    if (tmpColor.value() < 64)
        tmpColor = Qt::yellow;

    return tmpColor;
}

BlenderColorSelector::BlenderColorSelector(QWidget * parent)
    : ColorSelector(parent)
{
    m_highlighterGradientSelector = new HighlighterGradientSelector(this);

    m_highlighterGradientSelector->setColors(ColorUtil::DEFAULT_COLOR, ColorUtil::complementColor(ColorUtil::DEFAULT_COLOR));

    QToolButton * buttonAppendFirstQuarterRegion = new QToolButton(m_highlighterGradientSelector);
    QToolButton * buttonAppendSecondQuarterRegion = new QToolButton(m_highlighterGradientSelector);
    QToolButton * buttonAppendThirdQuarterRegion = new QToolButton(m_highlighterGradientSelector);
    QToolButton * buttonAppendFourthQuarterRegion = new QToolButton(m_highlighterGradientSelector);
    QToolButton * buttonAppendFirstRegion = new QToolButton(m_highlighterGradientSelector);
    QToolButton * buttonAppendSecondRegion = new QToolButton(m_highlighterGradientSelector);
    QToolButton * buttonAppendEntireRegion = new QToolButton(m_highlighterGradientSelector);

//     buttonAppendFirstQuarterRegion->setAutoRaise(true);
//     buttonAppendSecondQuarterRegion->setAutoRaise(true);
//     buttonAppendThirdQuarterRegion->setAutoRaise(true);
//     buttonAppendFourthQuarterRegion->setAutoRaise(true);
//     buttonAppendFirstRegion->setAutoRaise(true);
//     buttonAppendSecondRegion->setAutoRaise(true);
//     buttonAppendEntireRegion->setAutoRaise(true);

    buttonAppendFirstQuarterRegion->setDefaultAction(new KAction(KIcon("list-add"), QString(), this));
    buttonAppendSecondQuarterRegion->setDefaultAction(new KAction(KIcon("list-add"), QString(), this));
    buttonAppendThirdQuarterRegion->setDefaultAction(new KAction(KIcon("list-add"), QString(), this));
    buttonAppendFourthQuarterRegion->setDefaultAction(new KAction(KIcon("list-add"), QString(), this));
    buttonAppendFirstRegion->setDefaultAction(new KAction(KIcon("list-add"), QString(), this));
    buttonAppendSecondRegion->setDefaultAction(new KAction(KIcon("list-add"), QString(), this));
    buttonAppendEntireRegion->setDefaultAction(new KAction(KIcon("list-add"), QString(), this));

    QHBoxLayout * quartersLayout = new QHBoxLayout();
    quartersLayout->addWidget(buttonAppendFirstQuarterRegion);
    quartersLayout->addWidget(buttonAppendSecondQuarterRegion);
    quartersLayout->addWidget(buttonAppendThirdQuarterRegion);
    quartersLayout->addWidget(buttonAppendFourthQuarterRegion);

    QHBoxLayout * halfsLayout = new QHBoxLayout();
    halfsLayout->addWidget(buttonAppendFirstRegion);
    halfsLayout->addWidget(buttonAppendSecondRegion);

    QHBoxLayout * entireRegionLayout = new QHBoxLayout();
    entireRegionLayout->addWidget(buttonAppendEntireRegion);

    QVBoxLayout * mixerLayout = new QVBoxLayout(m_highlighterGradientSelector);
    mixerLayout->addLayout(quartersLayout);
    mixerLayout->addLayout(halfsLayout);
    mixerLayout->addLayout(entireRegionLayout);

    m_firstColor = new KColorButton(this);
    m_firstColor->setMinimumHeight(200);
    m_firstColor->setMaximumWidth(45);
    m_firstColor->setColor(ColorUtil::DEFAULT_COLOR);

    m_secondColor = new KColorButton(this);
    m_secondColor->setMinimumHeight(200);
    m_secondColor->setMaximumWidth(45);
    m_secondColor->setColor(ColorUtil::complementColor(ColorUtil::DEFAULT_COLOR));

    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(m_firstColor);
    layout->addWidget(m_highlighterGradientSelector);
    layout->addWidget(m_secondColor);

    connect(m_highlighterGradientSelector, SIGNAL( valueChanged(int) ), this, SLOT( updateMixWhenChangeBias(int) ));

    connect(m_firstColor , SIGNAL( changed(QColor) ), this, SLOT( updateMixWhenChangeColor(QColor) ));
    connect(m_secondColor, SIGNAL( changed(QColor) ), this, SLOT( updateMixWhenChangeColor(QColor) ));

    connect(buttonAppendFirstQuarterRegion->defaultAction() , SIGNAL( hovered() ), SLOT( highlightFirstQuarterRegion()  ));
    connect(buttonAppendSecondQuarterRegion->defaultAction(), SIGNAL( hovered() ), SLOT( highlightSecondQuarterRegion() ));
    connect(buttonAppendThirdQuarterRegion->defaultAction() , SIGNAL( hovered() ), SLOT( highlightThirdQuarterRegion()  ));
    connect(buttonAppendFourthQuarterRegion->defaultAction(), SIGNAL( hovered() ), SLOT( highlightFourthQuarterRegion() ));
    connect(buttonAppendFirstRegion->defaultAction()        , SIGNAL( hovered() ), SLOT( highlightFirstRegion()     ));
    connect(buttonAppendSecondRegion->defaultAction()       , SIGNAL( hovered() ), SLOT( highlightSecondRegion()    ));
    connect(buttonAppendEntireRegion->defaultAction()       , SIGNAL( hovered() ), SLOT( highlightEntireRegion()        ));

    connect(buttonAppendFirstQuarterRegion->defaultAction() , SIGNAL( triggered(bool) ), SLOT( appendFirstQuarterRegion()  ));
    connect(buttonAppendSecondQuarterRegion->defaultAction(), SIGNAL( triggered(bool) ), SLOT( appendSecondQuarterRegion() ));
    connect(buttonAppendThirdQuarterRegion->defaultAction() , SIGNAL( triggered(bool) ), SLOT( appendThirdQuarterRegion()  ));
    connect(buttonAppendFourthQuarterRegion->defaultAction(), SIGNAL( triggered(bool) ), SLOT( appendFourthQuarterRegion() ));
    connect(buttonAppendFirstRegion->defaultAction()        , SIGNAL( triggered(bool) ), SLOT( appendFirstRegion()     ));
    connect(buttonAppendSecondRegion->defaultAction()       , SIGNAL( triggered(bool) ), SLOT( appendSecondRegion()    ));
    connect(buttonAppendEntireRegion->defaultAction()       , SIGNAL( triggered(bool) ), SLOT( appendEntireRegion()        ));
}

void BlenderColorSelector::setColor(const QColor & color)
{
    Q_UNUSED(color);
}

void BlenderColorSelector::updateMixWhenChangeBias(int factor)
{
    m_bias = static_cast< float >(factor)/100.0f;

    performMix();
}

void BlenderColorSelector::updateMixWhenChangeColor(const QColor & color)
{
    Q_UNUSED(color);

    m_highlighterGradientSelector->setColors(m_firstColor->color(), m_secondColor->color());

    performMix();
}

void BlenderColorSelector::highlightFirstQuarterRegion()
{
    m_highlighterGradientSelector->setRegion(HighlighterGradientSelector::FirstQuarterRegion);
}

void BlenderColorSelector::highlightSecondQuarterRegion()
{
    m_highlighterGradientSelector->setRegion(HighlighterGradientSelector::SecondQuarterRegion);
}

void BlenderColorSelector::highlightThirdQuarterRegion()
{
    m_highlighterGradientSelector->setRegion(HighlighterGradientSelector::ThirdQuarterRegion);
}

void BlenderColorSelector::highlightFourthQuarterRegion()
{
    m_highlighterGradientSelector->setRegion(HighlighterGradientSelector::FourthQuarterRegion);
}

void BlenderColorSelector::highlightFirstRegion()
{
    m_highlighterGradientSelector->setRegion(HighlighterGradientSelector::FirstRegion);
}

void BlenderColorSelector::highlightSecondRegion()
{
    m_highlighterGradientSelector->setRegion(HighlighterGradientSelector::SecondRegion);
}

void BlenderColorSelector::highlightEntireRegion()
{
    m_highlighterGradientSelector->setRegion(HighlighterGradientSelector::EntireRegion);
}

void BlenderColorSelector::appendFirstQuarterRegion()
{
    appendColorsOfRegion(HighlighterGradientSelector::FirstQuarterRegion);
}

void BlenderColorSelector::appendSecondQuarterRegion()
{
    appendColorsOfRegion(HighlighterGradientSelector::SecondQuarterRegion);
}

void BlenderColorSelector::appendThirdQuarterRegion()
{
    appendColorsOfRegion(HighlighterGradientSelector::ThirdQuarterRegion);
}

void BlenderColorSelector::appendFourthQuarterRegion()
{
    appendColorsOfRegion(HighlighterGradientSelector::FourthQuarterRegion);
}

void BlenderColorSelector::appendFirstRegion()
{
    appendColorsOfRegion(HighlighterGradientSelector::FirstRegion);
}

void BlenderColorSelector::appendSecondRegion()
{
    appendColorsOfRegion(HighlighterGradientSelector::SecondRegion);
}

void BlenderColorSelector::appendEntireRegion()
{
    appendColorsOfRegion(HighlighterGradientSelector::EntireRegion);
}

void BlenderColorSelector::performMix()
{
    emit colorSelected(KColorUtils::mix(m_firstColor->color(), m_secondColor->color(), m_bias));
}

void BlenderColorSelector::appendColorsOfRegion(HighlighterGradientSelector::Region region)
{
    QVector<QColor> tmpColorRange;
    int regionLimitMin;
    int regionLimitMax;

    switch (region)
    {
        case HighlighterGradientSelector::FirstQuarterRegion:
            regionLimitMin = 1;
            regionLimitMax = 25;
            break;
        case HighlighterGradientSelector::SecondQuarterRegion:
            regionLimitMin = 26;
            regionLimitMax = 50;
            break;
        case HighlighterGradientSelector::ThirdQuarterRegion:
            regionLimitMin = 51;
            regionLimitMax = 75;
            break;
        case HighlighterGradientSelector::FourthQuarterRegion:
            regionLimitMin = 76;
            regionLimitMax = 100;
            break;
        case HighlighterGradientSelector::FirstRegion:
            regionLimitMin = 1;
            regionLimitMax = m_highlighterGradientSelector->value();
            break;
        case HighlighterGradientSelector::SecondRegion:
            regionLimitMin = m_highlighterGradientSelector->value() + 1;
            regionLimitMax = 100;
            break;
        case HighlighterGradientSelector::EntireRegion:
            regionLimitMin = 1;
            regionLimitMax = 100;
            break;

        case HighlighterGradientSelector::NoneRegion:
            break;
    }

    for (int i = regionLimitMin; i <= regionLimitMax; i++)
        tmpColorRange.append(KColorUtils::mix(m_firstColor->color(), m_secondColor->color(), static_cast<float>(i)/100.0f));

    emit colorsAdded(tmpColorRange);
}

#include "blendercolorselector.moc"
