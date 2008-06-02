#include "clipboardlineedit.h"

#include <QtGui/QClipboard>
#include <QtGui/QHBoxLayout>

#include <KLocalizedString>
#include <KApplication>
#include <KLineEdit>
#include <KPushButton>

ClipboardLineEdit::ClipboardLineEdit(QWidget * parent)
    : QWidget(parent)
{
    m_lineEdit = new KLineEdit(this);
    m_lineEdit->setReadOnly(true);

    m_clipboardButton = new KPushButton(KIcon(QString("edit-copy")), QString(), this);
    m_clipboardButton->setMaximumWidth(32);
    m_clipboardButton->setToolTip(i18n("To Clipboard"));

    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(m_lineEdit);
    layout->addWidget(m_clipboardButton);

    connect(m_clipboardButton, SIGNAL( pressed () ), this, SLOT( copyTextToClipboard() ));
}

QString ClipboardLineEdit::text() const
{
    return m_lineEdit->text();
}

void ClipboardLineEdit::setText(const QString & text)
{
    m_lineEdit->setText(text);
}

void ClipboardLineEdit::copyTextToClipboard()
{
    QClipboard * clipboard = KApplication::clipboard();
    clipboard->setText(m_lineEdit->text(), QClipboard::Clipboard);
    clipboard->setText(m_lineEdit->text(), QClipboard::Selection);
}

#include "clipboardlineedit.moc"
