#ifndef GTK_COLOR_SELECTOR
#define GTK_COLOR_SELECTOR

#include "colorselector.h"

class TriangleColorWidget;

class GtkColorSelector : public ColorSelector
{
    Q_OBJECT

    public:
        GtkColorSelector(QWidget* parent);

    public slots:
        void setColor(const QColor & color);

    private slots:
        void updateColor(const QColor & color);

    private:
        TriangleColorWidget * m_triangleColorWidget;
};

#endif // GTK_COLOR_SELECTOR
