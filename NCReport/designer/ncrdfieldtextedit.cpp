/****************************************************************************
*
*  Copyright (C) 2002-2010 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: nszabo@helta.hu, info@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  nszabo@helta.hu if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/
#include "ncrdfieldtextedit.h"

#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QtDebug>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>

NCRDFieldTextEdit::NCRDFieldTextEdit( QWidget* parent ) : QTextEdit(parent), m_completer(0)
{
    setAcceptRichText(false);
    setTabChangesFocus(true);
}

NCRDFieldTextEdit::~NCRDFieldTextEdit()
{
}

void NCRDFieldTextEdit::setCompleter(QCompleter *completer)
{
    m_completer = completer;
    if (!m_completer)
        return;

    m_completer->setWidget(this);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    //m_completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    //m_completer->setCompletionPrefix("?");
    //QString cp = m_completer->completionPrefix();
    QObject::connect(m_completer, SIGNAL(activated(const QString&)), this, SLOT(insertCompletion(const QString&)));
}

QCompleter *NCRDFieldTextEdit::completer() const
{
    return m_completer;
}

void NCRDFieldTextEdit::insertCompletion(const QString& completion)
{
    if (m_completer->widget() != this)
        return;
    QTextCursor tc = textCursor();
    if (false) {
        int extra = completion.length() - m_completer->completionPrefix().length();
        //tc.movePosition(QTextCursor::Left);
        tc.movePosition(QTextCursor::EndOfWord);
        tc.insertText(completion.right(extra));
    } else {
        tc.movePosition(QTextCursor::EndOfWord);
        int length = m_completer->completionPrefix().length();
        for (int i=0; i<length; ++i)
            tc.deletePreviousChar();
        tc.insertText(completion);
    }
    setTextCursor(tc);
}

QString NCRDFieldTextEdit::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void NCRDFieldTextEdit::focusInEvent(QFocusEvent *e)
{
    if (m_completer)
        m_completer->setWidget(this);
    QTextEdit::focusInEvent(e);
}

void NCRDFieldTextEdit::keyPressEvent(QKeyEvent *e)
{
    if (m_completer && m_completer->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
        switch (e->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Escape:
            case Qt::Key_Tab:
            case Qt::Key_Backtab:
                e->ignore();
                return; // let the completer do default behavior
            default:
                break;
        }
    }
    if (e->key()==Qt::Key_Return && e->modifiers() == Qt::ControlModifier ) {
        emit ctrlEnter();
        return;
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space); // CTRL+Space
    if (!m_completer || !isShortcut) // dont process the shortcut when we have a completer
        QTextEdit::keyPressEvent(e);

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!m_completer || (ctrlOrShift && e->text().isEmpty()))
        return;

    //static QString eow("~!@#%^&*()_+}|:\"<>?,./;'[]\\-="); // end of word
    static QString eow("()+<>-="); // end of word
    QString completionPrefix = textUnderCursor();
    /*
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 2
                        || eow.contains(e->text().right(1)))) {
        m_completer->popup()->hide();
        return;
    }
    */
    if (!isShortcut && (e->text().isEmpty() || completionPrefix.length() < 1 || eow.contains(e->text().right(1)) ) ) {
        m_completer->popup()->hide();
        return;
    }

    if (completionPrefix != m_completer->completionPrefix()) {
        m_completer->setCompletionPrefix(completionPrefix);
        m_completer->popup()->setCurrentIndex(m_completer->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(m_completer->popup()->sizeHintForColumn(0)
                + m_completer->popup()->verticalScrollBar()->sizeHint().width());
    m_completer->complete(cr); // popup it up!
}
