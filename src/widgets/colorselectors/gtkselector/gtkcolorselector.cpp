#include "gtkcolorselector.h"

#include <QPaintEvent>
#include <QPainter>

#include <math.h>

#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

#include "trianglecolorwidget.h"

GtkColorSelector::GtkColorSelector(QWidget* parent) : ColorSelector(parent)
{
    m_triangleColorWidget = new TriangleColorWidget(this);
    m_triangleColorWidget->setMinimumSize(QSize(200, 200));

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_triangleColorWidget);

    connect(m_triangleColorWidget, SIGNAL(colorChanged(QColor)), this, SLOT( updateColor(QColor) ));
}

void GtkColorSelector::updateColor(const QColor & color)
{
    emit colorSelected(color);
}

#include "gtkcolorselector.moc"
