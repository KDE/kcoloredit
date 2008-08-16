#ifndef COLORTOOL_WIDGET_H
#define COLORTOOL_WIDGET_H

#include <QtGui/QWidget>

class KPushButton;

class ColorToolWidget : public QWidget
{
    Q_OBJECT

    public:
        ColorToolWidget(QWidget * parent = 0);

    signals:
        void colorSelected(const QColor & color);

    private slots:
        void generateRandomColor();

    private:
        KPushButton * m_decreaseBrightnessButton;
        KPushButton * m_increaseBrightnessButton;

        KPushButton * m_decreaseSaturationButton;
        KPushButton * m_increaseSaturationButton;

        KPushButton * m_generateRandomColorButton;
};

#endif // COLORTOOL_WIDGET_H
