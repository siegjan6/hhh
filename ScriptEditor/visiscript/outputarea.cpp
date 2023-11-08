/**************************************************************************
 *
 *	project				 : VisiScript
 *
 *	copyright            : (C) 2011 by Michael Neuroth
 *
 ***************************************************************************/
/*********************************************************************************
 *																				 *
 * This file is part of the VisiScript package (a frontend for script languages) *
 *																				 *
 * Copyright (C) 2011 by Michael Neuroth.										 *
 *                                                                               *
 * This program is free software; you can redistribute it and/or modify			 *
 * it under the terms of the GNU General Public License as published by    		 *
 * the Free Software Foundation; either version 2 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 ********************************************************************************/

#include "outputarea.h"

#include <QTextEdit>
#include <QMouseEvent>
#include <QSizePolicy>
#if !defined(Q_OS_IOS)
#include <QPrintDialog>
#include <QPrinter>
#endif

OutputAreaMultiLineEdit::OutputAreaMultiLineEdit( QWidget * parent, const char * name )
    :  QTextEdit( parent ), //QPlainTextEdit( parent ),
       m_bLocked( false )
{
    setReadOnly(false);
    setObjectName(name);
    QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setSizePolicy(sizePolicy);
}

OutputAreaMultiLineEdit::~OutputAreaMultiLineEdit()
{
}
/*
void OutputAreaMultiLineEdit::setText( const QString & sText )
{
    setPlainText( sText );
}

void OutputAreaMultiLineEdit::append( const QString & sText )
{
    setPlainText( toPlainText()+QString("\n")+sText );
}
*/
bool OutputAreaMultiLineEdit::hasMarkedTxt()
{
    QList<QTextEdit::ExtraSelection> aSelection = extraSelections();
    return aSelection.length()>0;
}

QString OutputAreaMultiLineEdit::markedTxt()
{
    return extraSelections()[0].cursor.selectedText();
}

bool OutputAreaMultiLineEdit::isLocked() const
{
    return m_bLocked;
}

void OutputAreaMultiLineEdit::setLocked( bool value )
{
    m_bLocked = value;
}

void OutputAreaMultiLineEdit::PrintContent()
{
#if !defined(Q_OS_IOS)
    QPrinter aPrinter;
    aPrinter.setFullPage( true );
    QPrintDialog aDlg(&aPrinter, this);
    if( aDlg.exec() )
    {
        document()->print(&aPrinter);
    }
#endif
}

void OutputAreaMultiLineEdit::mouseDoubleClickEvent ( QMouseEvent * e )
{
    QTextCursor aCursor = cursorForPosition( e->pos() );
    emit doubleClicked(aCursor.blockNumber(),aCursor.position());
    /*QPlain*/QTextEdit::mouseDoubleClickEvent(e);
}
