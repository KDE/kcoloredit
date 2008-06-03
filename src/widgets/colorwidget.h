#ifndef COLOR_WIDGET_H
#define COLOR_WIDGET_H

#include <QtGui/QWidget>

class KColorPatch;

// NOTE dispatcher class

class ColorWidget : public QWidget
{
    Q_OBJECT

    public:
        ColorWidget(QWidget * parent = 0);

        QColor color() const;

    public slots:
        void setColor(const QColor & color);

    private slots:
        void updateColor(const QColor & color);

    signals:
        void colorChanged(const QColor & color);

    private:
        QColor m_color;

        KColorPatch * m_colorPatch;
};

#endif // COLOR_WIDGET_H
