/**************************************************************************
 *
 *	project				 : VisiScript
 *
 *	copyright            : (C) 2014 by Michael Neuroth
 *
 ***************************************************************************/
/*********************************************************************************
 *																				 *
 * This file is part of the VisiScript package (a frontend for script languages) *
 *																				 *
 * Copyright (C) 2014 by Michael Neuroth.										 *
 *                                                                               *
 * This program is free software; you can redistribute it and/or modify			 *
 * it under the terms of the GNU General Public License as published by    		 *
 * the Free Software Foundation; either version 2 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 ********************************************************************************/

#include "qscilexerhaskell.h"

#ifdef _WITH_QSCINTILLA

//*************************************************************************

// The list of lisp keywords that can be used by other friendly lexers.
const char *QsciLexerHaskell::keywordClass =
    "as case of class data family instance default deriving do forall foreign "
    "hiding if then else import infix infixl infixr let in mdo module newtype "
    "proc qualified rec type where";

//*************************************************************************

QsciLexerHaskell::QsciLexerHaskell( QObject *parent )
    : QsciLexer(parent)
{
}

QsciLexerHaskell::~QsciLexerHaskell()
{
}

const char * QsciLexerHaskell::language() const
{
    return "Haskell";
}

const char *QsciLexerHaskell::lexer() const
{
    return "haskell";
}

const char * QsciLexerHaskell::keywords(int set) const
{
    if (set != 1)
        return 0;

    return keywordClass;
}

QString QsciLexerHaskell::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Number:
        return tr("Number");

    case Module:
        return tr("Module");

    case Capital:
        return tr("Capital");

    case Data:
        return tr("Data");

    case Import:
        return tr("Import");

    case Instance:
        return tr("Instance");

    case String:
        return tr("String");

    case StringEol:
        return tr("String Eol");

    case Character:
        return tr("Character");

    case Keyword:
        return tr("Keyword");

    case ClassName:
        return tr("Class name");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case CommentLine:
        return tr("Comment line");

    case CommentBlock:
        return tr("Comment block");

    case CommentBlock2:
        return tr("Comment block 2");

    case CommentBlock3:
        return tr("Comment block 3");

    case Pragma:
        return tr("Pragma");

    case Preprocessor:
        return tr("Preprocessor");

    case ReservedOperator:
        return tr("ReservedOperator");

    case LiterateComment:
        return tr("LiterateComment");

    case LiterateCodeLim:
        return tr("LiterateCodeLim");
    }

    return QString();
}

QColor QsciLexerHaskell::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80,0x80,0x80);

    case Number:
        return QColor(0x00,0x7f,0x7f);

    case String:
    case Character:
        return QColor(0x7f,0x00,0x7f);

    case Keyword:
        return QColor(0x00,0x00,0x7f);

    case Import:
    case Capital:
    case Data:
    case Instance:
    case Module:
    case ClassName:
        return QColor(0x00,0x00,0xff);

    case Pragma:
    case Preprocessor:
        return QColor(0x00,0x00,0x7f);

    case Operator:
    case Identifier:
        break;

    case CommentLine:
    case CommentBlock:
    case CommentBlock2:
    case CommentBlock3:
        return QColor(0x00,0x7f,0x00);
    }

    return QsciLexer::defaultColor(style);
}

QFont QsciLexerHaskell::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Identifier:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    case String:
    case Character:
#if defined(Q_OS_WIN)
        f = QFont("Courier New",10);
#elif defined(Q_OS_MAC)
        f = QFont("Courier", 12);
#else
        f = QFont("Bitstream Vera Sans Mono",9);
#endif
        break;

    case Keyword:
    case ClassName:
    case Operator:
        f = QsciLexer::font(style);
        f.setBold(true);
        break;

    default:
        f = QsciLexer::font(style);
    }

    return f;
}

#endif
