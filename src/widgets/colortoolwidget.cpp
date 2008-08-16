#include "colortoolwidget.h"

#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>

#include <KLocalizedString>
#include <KPushButton>

ColorToolWidget::ColorToolWidget(QWidget * parent)
    : QWidget(parent)
{
    setWindowTitle(i18n("Lighting tol"));

    m_decreaseBrightnessButton = new KPushButton(this);
    m_increaseBrightnessButton = new KPushButton(this);

    m_decreaseSaturationButton = new KPushButton(this);
    m_increaseSaturationButton = new KPushButton(this);

    m_generateRandomColorButton = new KPushButton(this);

    QHBoxLayout * grabColorLayout = new QHBoxLayout();
    grabColorLayout->addWidget(new QLabel(i18n("pick a color fron desktop"), this));
    grabColorLayout->addWidget(new KPushButton(this));
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
}

void ColorToolWidget::generateRandomColor()
{
    QColor randColor(qrand() % 255, qrand()  % 255, qrand() % 255);

    //m_triangleColorWidget->setColor(randColor);
    emit colorSelected(randColor);
}

#include "colortoolwidget.moc"
