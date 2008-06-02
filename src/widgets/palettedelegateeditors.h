#ifndef PALETTE_DELEGATE_EDITORS_H
#define PALETTE_DELEGATE_EDITORS_H

#include <QtGui/QWidget>

class KColorButton;
class KLineEdit;

class ColorItemEditor : public QWidget
{
    public:
        ColorItemEditor(QWidget * parent = 0);

        QColor color() const;
        void setColor(const QColor & color);

        QString colorName() const;
        void setColorName(const QString & colorName);

    private:
        KColorButton * m_color;
        KLineEdit * m_colorName;
};

class CommentItemEditor : public QWidget
{
    public:
        CommentItemEditor(QWidget * parent = 0);

        QString comment() const;
        void setComment(const QString & comment);

    private:
        KLineEdit * m_comment;
};

#endif // PALETTE_DELEGATE_EDITORS_H
