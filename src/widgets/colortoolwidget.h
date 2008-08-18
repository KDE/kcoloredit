#ifndef COLORTOOL_WIDGET_H
#define COLORTOOL_WIDGET_H

#include <QtGui/QWidget>

class QEvent;

class KPushButton;

class ColorToolWidget : public QWidget
{
    Q_OBJECT

    public:
        ColorToolWidget(QWidget * parent = 0);

    signals:
        void colorSelected(const QColor & color);

    protected:
        virtual bool eventFilter(QObject *watched, QEvent *event);

    private slots:
        void generateRandomColor();
        void pickColorFromDesktop();

    private:
        KPushButton * m_decreaseBrightnessButton;
        KPushButton * m_increaseBrightnessButton;

        KPushButton * m_decreaseSaturationButton;
        KPushButton * m_increaseSaturationButton;

        KPushButton * m_generateRandomColorButton;
};

#endif // COLORTOOL_WIDGET_H
