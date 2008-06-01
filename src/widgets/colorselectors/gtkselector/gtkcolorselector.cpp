#include "gtkcolorselector.h"

#include <QtGui/QVBoxLayout>

#include "trianglecolorwidget.h"

GtkColorSelector::GtkColorSelector(QWidget* parent) : ColorSelector(parent)
{
    m_triangleColorWidget = new TriangleColorWidget(this);
    m_triangleColorWidget->setColor(Qt::red);

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_triangleColorWidget);

    connect(m_triangleColorWidget, SIGNAL(colorChanged(QColor)), this, SLOT( updateColor(QColor) ));
}

void GtkColorSelector::setColor(const QColor & color)
{
    m_triangleColorWidget->setColor(color);
}

void GtkColorSelector::updateColor(const QColor & color)
{
    emit colorSelected(color);
}

#include "gtkcolorselector.moc"
