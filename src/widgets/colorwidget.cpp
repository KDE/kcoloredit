#include "colorwidget.h"

#include <QtGui/QVBoxLayout>

#include <KColorPatch>

ColorWidget::ColorWidget(QWidget * parent)
    : QWidget(parent)
{
    m_colorPatch = new KColorPatch(this);
    setMinimumHeight(64);
    m_colorPatch->setColor(Qt::red); // default color

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(m_colorPatch);

    connect(m_colorPatch, SIGNAL( colorChanged(QColor) ), this, SLOT( updateColor(QColor) ));
}

QColor ColorWidget::color() const
{
    return m_color;
}

void ColorWidget::setColor(const QColor & color)
{
    m_color = color;

    m_colorPatch->setColor(m_color);

    emit colorChanged(m_color);
}

void ColorWidget::updateColor(const QColor & color)
{
    m_color = color;

    emit colorChanged(m_color);
}

#include "colorwidget.moc"
