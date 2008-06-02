#include <palettedelegateeditors.h>

#include <QtGui/QHBoxLayout>

#include <KColorButton>
#include <KLineEdit>

//BEGIN public class ColorItemEditor

ColorItemEditor::ColorItemEditor(QWidget * parent)
    : QWidget(parent)
{
    m_color = new KColorButton(this);
    m_color->setMinimumWidth(92);

    m_colorName = new KLineEdit(this);
    m_colorName->setMinimumWidth(128);
    m_colorName->setClearButtonShown(true);

    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(m_color);
    layout->addWidget(m_colorName);

    setMinimumHeight(40);
}

QColor ColorItemEditor::color() const
{
    return m_color->color();
}

void ColorItemEditor::setColor(const QColor & color)
{
    m_color->setColor(color);
}

QString ColorItemEditor::colorName() const
{
    return m_colorName->text();
}

void ColorItemEditor::setColorName(const QString & colorName)
{
    m_colorName->setText(colorName);
}

//END public class ColorItemEditor

//BEGIN public class CommentItemEditor

CommentItemEditor::CommentItemEditor(QWidget * parent)
    : QWidget(parent)
{
    m_comment = new KLineEdit(this);
    m_comment->setMinimumWidth(128);
    m_comment->setClearButtonShown(true);

    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(m_comment);

    setMinimumHeight(40);
}

QString CommentItemEditor::comment() const
{
    return m_comment->text();
}

void CommentItemEditor::setComment(const QString & comment)
{
    m_comment->setText(comment);
}

//END public class CommentItemEditor
