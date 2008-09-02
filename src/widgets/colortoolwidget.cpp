/*********************************************************************************
*  Copyright (C) 2008 by Percy Camilo Triveño Aucahuasi <orgyforever@gmail.com>  *
*                                                                                *
*  Filters:                                                                      *
*  Is based on kdeui/colors/kcolordialog                                         *
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

#include "colortoolwidget.h"

#include <QtGui/QMouseEvent>
#include <QtGui/QLayout>
#include <QtGui/QLabel>

#include <QtGui/QGroupBox>
#include <QtGui/QDesktopWidget>

#include <KIcon>
#include <KApplication>
#include <KLocalizedString>
#include <KPushButton>
#include <KColorPatch>

#ifdef Q_WS_X11

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <QX11Info>
#include <fixx11h.h>

#include <QtGui/QCheckBox>

#include <KWindowSystem>

class KCDPickerFilter: public QWidget
{
    public:
        KCDPickerFilter(QWidget* parent): QWidget(parent) {}

        virtual bool x11Event(XEvent* event)
        {
            if (event->type == ButtonRelease)
            {
                QMouseEvent e(QEvent::MouseButtonRelease, QPoint(),
                    QPoint(event->xmotion.x_root, event->xmotion.y_root) , Qt::NoButton, Qt::NoButton, Qt::NoModifier);
                KApplication::sendEvent(parentWidget(), &e);

                return true;
            }
            else
                return false;
    }
};

#endif

ColorToolWidget::ColorToolWidget(QWidget * parent)
    : QWidget(parent)
    , m_colorPicking(false)
{
    setMouseTracking(true);

    //BEGIN Brightness settings

    QGroupBox * brightnessBox = new QGroupBox(i18n("Brightness"), this);

    KPushButton * decreaseBrightnessButton = new KPushButton(KIcon("arrow-down"), i18n("Decrease"), brightnessBox);
    KPushButton * increaseBrightnessButton = new KPushButton(KIcon("arrow-up"), i18n("Increase"), brightnessBox);

    m_brightnessPercentage = new QLabel(brightnessBox);

    QHBoxLayout * brightnessLayout = new QHBoxLayout(brightnessBox);
    brightnessLayout->addWidget(decreaseBrightnessButton);
    brightnessLayout->addWidget(increaseBrightnessButton);
    brightnessLayout->addWidget(m_brightnessPercentage);

    connect(decreaseBrightnessButton, SIGNAL( pressed () ), SLOT( decreaseBrightness() ));
    connect(increaseBrightnessButton, SIGNAL( pressed () ), SLOT( increaseBrightness() ));

    //END Brightness settings

    //BEGIN Saturation settings

    QGroupBox * saturationBox = new QGroupBox(i18n("Saturation"), this);

    KPushButton * decreaseSaturationButton = new KPushButton(KIcon("arrow-down"), i18n("Decrease"), saturationBox);
    KPushButton * increaseSaturationButton = new KPushButton(KIcon("arrow-up"), i18n("Increase"), saturationBox);

    m_saturationPercentage = new QLabel(saturationBox);

    QHBoxLayout * saturationLayout = new QHBoxLayout(saturationBox);
    saturationLayout->addWidget(decreaseSaturationButton);
    saturationLayout->addWidget(increaseSaturationButton);
    saturationLayout->addWidget(m_saturationPercentage);

    connect(decreaseSaturationButton, SIGNAL( pressed () ), SLOT( decreaseSaturation() ));
    connect(increaseSaturationButton, SIGNAL( pressed () ), SLOT( increaseSaturation() ));

    //END Saturation settings

    //BEGIN Extra color selectors settings

    QGroupBox * extraSelectorsBox = new QGroupBox(i18n("Color selectors"), this);

    KPushButton * pickColorButton = new KPushButton(KIcon("color-picker"), i18n("Pick a color"), extraSelectorsBox);

    QHBoxLayout * pickColorLayout = new QHBoxLayout();
    pickColorLayout->addWidget(pickColorButton);

#ifdef Q_WS_X11

    if (KWindowSystem::compositingActive())
    {
        m_checkBoxHideWindow = new QCheckBox(i18n("Hide window"), extraSelectorsBox);

        pickColorLayout->addWidget(m_checkBoxHideWindow);
    }

#endif

    QVBoxLayout * extraSelectorsLayout = new QVBoxLayout(extraSelectorsBox);
    extraSelectorsLayout->addLayout(pickColorLayout);
    //extraSelectorsLayout->addWidget(Choose from image);

    connect(pickColorButton, SIGNAL( pressed () ), this, SLOT( pickColorFromDesktop() ));

    //END Extra color selectors settings

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(brightnessBox);
    mainLayout->addWidget(saturationBox);
    mainLayout->addWidget(extraSelectorsBox);
}

ColorToolWidget::~ColorToolWidget()
{
#ifdef Q_WS_X11

    if (m_colorPicking && kapp)
        kapp->removeX11EventFilter(m_filter);

#endif
}

void ColorToolWidget::setColor(const QColor & color)
{
    m_color = color;

    m_brightnessPercentage->setText(QString::number(static_cast<int>(m_color.value()*100/255)) + "%");
    m_saturationPercentage->setText(QString::number(static_cast<int>(m_color.saturation()*100/255)) + "%");
}

void ColorToolWidget::keyPressEvent(QKeyEvent * event)
{
    if (m_colorPicking)
    {
        if (event->key() == Qt::Key_Escape)
            releasePicking();

        event->accept();

        return ;
    }

    QWidget::keyPressEvent(event);
}

void ColorToolWidget::mouseMoveEvent(QMouseEvent * event)
{
    if (m_colorPicking)
    {
        m_color = grabColor(event->globalPos());

#ifdef Q_WS_X11

    if (KWindowSystem::compositingActive())
        if (m_checkBoxHideWindow->isChecked())
            m_colorView->setColor(m_color);

#endif

        setColor(m_color);

        emit colorSelected(m_color);
        return ;
    }

    QWidget::mouseMoveEvent(event);
}

void ColorToolWidget::mousePressEvent(QMouseEvent * event)
{
    if (m_colorPicking)
    {
        releasePicking();

        m_color = grabColor(event->globalPos());

        setColor(m_color);

        emit colorSelected(m_color);

        return ;
    }

    QWidget::mousePressEvent(event);
}


void ColorToolWidget::pickColorFromDesktop()
{
    grabPicking();
}

void ColorToolWidget::decreaseBrightness()
{
    int tmpValue = qBound(0, m_color.value() - 5, 255);

    m_color.setHsv(m_color.hue(), m_color.saturation(), tmpValue);

    m_brightnessPercentage->setText(QString::number(static_cast<int>(m_color.value()*100/255)) + "%");

    emit colorSelected(m_color);
}

void ColorToolWidget::increaseBrightness()
{
    int tmpValue = qBound(0, m_color.value() + 5, 255);

    m_color.setHsv(m_color.hue(), m_color.saturation(), tmpValue);

    m_brightnessPercentage->setText(QString::number(static_cast<int>(m_color.value()*100/255)) + "%");

    emit colorSelected(m_color);
}

void ColorToolWidget::decreaseSaturation()
{
    int tmpSaturation = qBound(0, m_color.saturation() - 5, 255);

    m_color.setHsv(m_color.hue(), tmpSaturation, m_color.value());

    m_saturationPercentage->setText(QString::number(static_cast<int>(m_color.saturation()*100/255)) + "%");

    emit colorSelected(m_color);
}

void ColorToolWidget::increaseSaturation()
{
    int tmpSaturation = qBound(0, m_color.saturation() + 5, 255);

    m_color.setHsv(m_color.hue(), tmpSaturation, m_color.value());

    m_saturationPercentage->setText(QString::number(static_cast<int>(m_color.saturation()*100/255)) + "%");

    emit colorSelected(m_color);
}

void ColorToolWidget::grabPicking()
{
    m_colorPicking = true;

#ifdef Q_WS_X11

    m_filter = new KCDPickerFilter(this);
    kapp->installX11EventFilter(m_filter);

    // NOTE
    // Hide the MainWindow (KColorEdit) (make it transparent)

    if (KWindowSystem::compositingActive())
        if (m_checkBoxHideWindow->isChecked())
        {
            parentWidget()->parentWidget()->parentWidget()->parentWidget()->setWindowOpacity(0.0);

            m_checkBoxHideWindow->setEnabled(false);

            m_colorView = new KColorPatch(0);
            m_colorView->setMaximumSize(48, 48);
            m_colorView->setWindowFlags(Qt::ToolTip);
            m_colorView->show();
        }

#endif

    grabMouse(Qt::CrossCursor);
    grabKeyboard();
}

void ColorToolWidget::releasePicking()
{
    m_colorPicking = false;

#ifdef Q_WS_X11

    kapp->removeX11EventFilter(m_filter);
    delete m_filter;
    m_filter = 0;

    // NOTE
    // Restore the opacity of the MainWindow (KColorEdit)

    if (KWindowSystem::compositingActive())
        if (m_checkBoxHideWindow->isChecked())
        {
            parentWidget()->parentWidget()->parentWidget()->parentWidget()->setWindowOpacity(1.0);

            m_checkBoxHideWindow->setEnabled(true);

            delete m_colorView;
            m_colorView = 0;
        }

#endif

    releaseMouse();
    releaseKeyboard();
}

QColor ColorToolWidget::grabColor(const QPoint & p)
{
#ifdef Q_WS_X11

    // we use the X11 API directly in this case as we are not getting back a valid
    // return from QPixmap::grabWindow in the case where the application is using
    // an argb visual
    if( !KApplication::desktop()->geometry().contains( p ))
        return QColor();
    Window root = RootWindow(QX11Info::display(), QX11Info::appScreen());
    XImage *ximg = XGetImage(QX11Info::display(), root, p.x(), p.y(), 1, 1, -1, ZPixmap);
    unsigned long xpixel = XGetPixel(ximg, 0, 0);
    XDestroyImage(ximg);
    XColor xcol;
    xcol.pixel = xpixel;
    xcol.flags = DoRed | DoGreen | DoBlue;
    XQueryColor(QX11Info::display(),
                DefaultColormap(QX11Info::display(), QX11Info::appScreen()),
                &xcol);
    return QColor::fromRgbF(xcol.red / 65535.0, xcol.green / 65535.0, xcol.blue / 65535.0);

#else

    QWidget *desktop = KApplication::desktop();
    QPixmap pm = QPixmap::grabWindow(desktop->winId(), p.x(), p.y(), 1, 1);
    QImage i = pm.toImage();
    return i.pixel(0, 0);

#endif
}

#include "colortoolwidget.moc"
