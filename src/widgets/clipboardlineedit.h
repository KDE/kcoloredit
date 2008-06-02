#ifndef CLIPBOARD_LINEEDIT_H
#define CLIPBOARD_LINEEDIT_H

#include <QtGui/QWidget>

class KLineEdit;
class KPushButton;

class ClipboardLineEdit : public QWidget
{
    Q_OBJECT

    public:
        ClipboardLineEdit(QWidget * parent = 0);

        QString text() const;
        void setText(const QString & text);

    private slots:
        void copyTextToClipboard();

    private:
        KLineEdit * m_lineEdit;
        KPushButton * m_clipboardButton;
};

#endif // CLIPBOARD_LINEEDIT_H
