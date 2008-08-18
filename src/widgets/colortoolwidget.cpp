#include "colortoolwidget.h"

#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>

#include <KLocalizedString>
#include <KPushButton>

#include <QMouseEvent>
#include <KApplication>
#include <QDesktopWidget>
#include <QImage>

#if defined(Q_WS_X11)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <QX11Info>

class KCDPickerFilter: public QWidget
{
public:
    KCDPickerFilter(QWidget* parent): QWidget(parent) {}

    virtual bool x11Event(XEvent* event) {
        if (event->type == ButtonRelease) {
            QMouseEvent e(QEvent::MouseButtonRelease, QPoint(),
                          QPoint(event->xmotion.x_root, event->xmotion.y_root) , Qt::NoButton, Qt::NoButton, Qt::NoModifier);
            QApplication::sendEvent(parentWidget(), &e);
            return true;
        } else return false;
    }
};

#endif

ColorToolWidget::ColorToolWidget(QWidget * parent)
    : QWidget(parent)
{
    setWindowTitle(i18n("Lighting tol"));

    m_decreaseBrightnessButton = new KPushButton(this);
    m_increaseBrightnessButton = new KPushButton(this);

    m_decreaseSaturationButton = new KPushButton(this);
    m_increaseSaturationButton = new KPushButton(this);

    m_generateRandomColorButton = new KPushButton(this);

    KPushButton * pickColorButton = new KPushButton(this);

    QHBoxLayout * grabColorLayout = new QHBoxLayout();
    grabColorLayout->addWidget(new QLabel(i18n("pick a color fron desktop"), this));
    grabColorLayout->addWidget(pickColorButton);
    grabColorLayout->addWidget(new QCheckBox(this));

    QHBoxLayout * randomColorLayout = new QHBoxLayout();
    randomColorLayout->addWidget(new QLabel(i18n("random colorss"), this));
    randomColorLayout->addWidget(m_generateRandomColorButton);

    QHBoxLayout * brilay = new QHBoxLayout();
    brilay->addWidget(new QLabel("Brighness:", this));
    brilay->addWidget(m_decreaseBrightnessButton);
    brilay->addWidget(m_increaseBrightnessButton);
    brilay->addWidget(new QLabel("45%", this));

    QHBoxLayout * satlay = new QHBoxLayout();
    satlay->addWidget(new QLabel("Saturation:", this));
    satlay->addWidget(m_decreaseSaturationButton);
    satlay->addWidget(m_increaseSaturationButton);
    satlay->addWidget(new QLabel("15%", this));

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addLayout(brilay);
    layout->addLayout(satlay);
    layout->addLayout(grabColorLayout);
    layout->addLayout(randomColorLayout);

    connect(m_generateRandomColorButton, SIGNAL( pressed () ), this, SLOT( generateRandomColor() ));
    connect(pickColorButton, SIGNAL( pressed () ), this, SLOT( pickColorFromDesktop() ));
}

void ColorToolWidget::generateRandomColor()
{
    QColor randColor(qrand() % 255, qrand()  % 255, qrand() % 255);

    emit colorSelected(randColor);
}

#include <QEvent>

bool ColorToolWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);

    QColor tempColor;

    QPoint point = QCursor::pos();

#if defined(Q_WS_X11)
/*
  It seems the Qt4 stuff returns a null grabbed pixmap when the Display
  has ARGB visuals.
  Then, access directly to the screen pixels using the X API.
*/
    Window root = RootWindow(QX11Info::display(), QX11Info::appScreen());
    XImage *ximg = XGetImage(QX11Info::display(), root, point.x(), point.y(), 1, 1, -1, ZPixmap);
    unsigned long xpixel = XGetPixel(ximg, 0, 0);
    XDestroyImage(ximg);
    XColor xcol;
    xcol.pixel = xpixel;
    xcol.flags = DoRed | DoGreen | DoBlue;
    XQueryColor(QX11Info::display(), DefaultColormap(QX11Info::display(), QX11Info::appScreen()), &xcol);
    tempColor =  QColor::fromRgbF(xcol.red / 65535.0, xcol.green / 65535.0, xcol.blue / 65535.0);



#else
    QDesktopWidget *desktop = KApplication::desktop();
    QPixmap pix = QPixmap::grabWindow(desktop->winId(), point.x(), point.y(), 1, 1);
    QImage img = pix.toImage();
    tempColor = QColor(img.pixel(0, 0));
#endif

    grabMouse(Qt::CrossCursor);
    grabKeyboard();


    }
    return QWidget::eventFilter(watched, event);
}

void ColorToolWidget::pickColorFromDesktop()
{
//     QColor tempColor;
// 
//     QPoint point = QCursor::pos();
// 
// #if defined(Q_WS_X11)
// /*
//   It seems the Qt4 stuff returns a null grabbed pixmap when the Display
//   has ARGB visuals.
//   Then, access directly to the screen pixels using the X API.
// */
//     Window root = RootWindow(QX11Info::display(), QX11Info::appScreen());
//     XImage *ximg = XGetImage(QX11Info::display(), root, point.x(), point.y(), 1, 1, -1, ZPixmap);
//     unsigned long xpixel = XGetPixel(ximg, 0, 0);
//     XDestroyImage(ximg);
//     XColor xcol;
//     xcol.pixel = xpixel;
//     xcol.flags = DoRed | DoGreen | DoBlue;
//     XQueryColor(QX11Info::display(), DefaultColormap(QX11Info::display(), QX11Info::appScreen()), &xcol);
//     tempColor =  QColor::fromRgbF(xcol.red / 65535.0, xcol.green / 65535.0, xcol.blue / 65535.0);
// 
// 
//     KCDPickerFilter * filter = new KCDPickerFilter(this);
//     kapp->installX11EventFilter(filter);
// #else
//     QDesktopWidget *desktop = KApplication::desktop();
//     QPixmap pix = QPixmap::grabWindow(desktop->winId(), point.x(), point.y(), 1, 1);
//     QImage img = pix.toImage();
//     tempColor = QColor(img.pixel(0, 0));
// #endif
// 
//     grabMouse(Qt::CrossCursor);
//     grabKeyboard();
}

#include "colortoolwidget.moc"
