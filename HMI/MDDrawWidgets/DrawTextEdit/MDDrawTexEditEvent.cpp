#include <QApplication>
#include <QTextCursor>
#include <QLineEdit>
#include "MDDrawTexEditEvent.h"
#include "MDDrawTextEdit.h"
#include "DrawTextEditData.h"

CMDDrawTexEditEvent::CMDDrawTexEditEvent()
{
    m_lineEdit = new QLineEdit();
    m_lineEdit->setEchoMode(QLineEdit::EchoMode::Password);
}

CMDDrawTexEditEvent::~CMDDrawTexEditEvent()
{
    if(m_lineEdit != NULL)
    {
        delete m_lineEdit;
        m_lineEdit = NULL;
    }
}

/*!
\section  绑定文本变化事件
*/
void CMDDrawTexEditEvent::connectEvent()
{
    CMDDrawBasicWidgetEvent::connectEvent();

    if(parent()->parent() == NULL)
        return;
    if(parent()->parent()->framework()->isStudioMode())
        return;

    QTextEdit* sender = static_cast<QTextEdit*>(parent()->widget());
    connect(sender, &QTextEdit::textChanged, this, &CMDDrawTexEditEvent::textChanged);
}

void CMDDrawTexEditEvent::textChanged()
{
    //script
    QList<QVariant> paras;
    w = dynamic_cast<CMDDrawTextEdit*>(parent());
    if(w != NULL)
    {
        paras.append(w->text());
    }
    parent()->parent()->run()->fireEvent(parent()->name(), "onTextChanged",paras);
}

/*!
\section  绑定写变量事件
*/
bool CMDDrawTexEditEvent::eventFilter(QObject *obj, QEvent *event)
{
    w = dynamic_cast<CMDDrawTextEdit*>(parent());

    if(parent()->parent() == NULL)
        return false;

    if(parent()->isStudioMode())
        return false;

    if(w == NULL)
    {
        return false;
    }
    else
    {
        if(w->writeVarEnable())
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (obj == parent()->widget())
            {
                switch (event->type())
                {
                case QEvent::KeyPress:
                    if(w->hasPassword())
                    {
                        QTextCursor cursor = w->widget()->textCursor();
                        m_lineEdit->setCursorPosition(cursor.position());
                        if(cursor.hasSelection())
                        {
                            int start = cursor.selectionStart();
                            int len = cursor.selectionEnd() - start;
                            int pos = cursor.position();

                            if(start == pos)    //! 光标在选择项开头
                                m_lineEdit->setSelection(start+len, -len);
                            else                //! 光标在选择项末尾
                                m_lineEdit->setSelection(start, len);
                        }
                        m_lineEdit->event(event);

                        w->widget()->setText(m_lineEdit->displayText());
                        cursor.setPosition(m_lineEdit->cursorPosition());
                        if(m_lineEdit->hasSelectedText())
                        {
                            int start = m_lineEdit->selectionStart();
                            int len = m_lineEdit->selectedText().length();
                            int pos = m_lineEdit->cursorPosition();
                            QTextCursor::MoveOperation op = QTextCursor::PreviousCharacter;
                            if(start == pos)    //！ 光标在选择项开头
                            {
                                op = QTextCursor::PreviousCharacter;
                                start = start + len;
                            }
                            else                //！ 光标在选择项末尾
                            {
                                op = QTextCursor::NextCharacter;
                                start = start;
                            }

                            cursor.setPosition(start, QTextCursor::MoveAnchor);
                            cursor.movePosition(op, QTextCursor::KeepAnchor, len);
                        }
                        w->widget()->setTextCursor(cursor);
                        w->data()->setText(m_lineEdit->text());

                        if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
                        {
                            if(w->enterSetVarEnable())
                            {
                                w->writeVariable();
                            }
                        }
                        return true;
                    }
                    else
                    {
                        if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
                        {
                            if(w->enterSetVarEnable())
                            {
                                w->handleEventWriteVarNoPassword();
                                return true;
                            }
                        }
                    }
                    break;
                case QEvent::FocusOut:
                    w->writeVariable();
                    break;
                default:
                    break;
                }
            }
        }
        return CMDDrawBasicWidgetEvent::eventFilter(obj, event);
    }
}
