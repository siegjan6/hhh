/**************************************************************************
 *
 *	project				 : VisiScript
 *
 *	copyright            : (C) 2004 by Michael Neuroth
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

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>

#include "qextscintillalexerlisp.h"

#ifdef _WITH_QSCINTILLA

// The list of lisp keywords that can be used by other friendly lexers.
const char *QsciLexerLisp::keywordClass =
	"define defun cond car cdr cons lambda setq setf let list nth last append length "
	"eq equal not and or write read print "
	"apply mapcar";


// The ctor.
QsciLexerLisp::QsciLexerLisp(QObject *parent)
    : QsciLexer(parent)
{
}

// The dtor.
QsciLexerLisp::~QsciLexerLisp()
{
}

// Returns the language name.
const char *QsciLexerLisp::language() const
{
	return "Lisp";
}

// Returns the lexer name.
const char *QsciLexerLisp::lexer() const
{
	return "lisp";
}

// Return the list of characters that can start a block.
const char *QsciLexerLisp::blockStart(int *style) const
{
	if (style)
		*style = Operator;

	return "(";
}

// Return the list of characters that can end a block.
const char *QsciLexerLisp::blockEnd(int *style) const
{
	if (style)
		*style = Operator;

	return ")";
}

// Return the style used for braces.
int QsciLexerLisp::braceStyle() const
{
	return Operator;
}

// Returns the foreground colour of the text for a style.
QColor QsciLexerLisp::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80,0x80,0x80);

    case CommentBlock:
    case Comment:
        return QColor(0x00,0x7f,0x00);

    case Number:
        return QColor(0x00,0x7f,0x7f);

    case Keyword:
    case Keyword_KW:
        return QColor(0x00,0x00,0x7f);

    case StringEol:
    case String:
        return QColor(0x7f,0x00,0x7f);

    case Operator:
    case Identifier:
        break;

    case Symbol:
    case Special:
        return QColor(0xff,0x00,0x00);
    }

    return QsciLexer::defaultColor(style);
}

// Returns the font of the text for a style.
QFont QsciLexerLisp::defaultFont(int style) const
{
	QFont f;

	switch (style)
	{
	case Comment:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
		break;

    case String:
#if defined(Q_OS_WIN)
        f = QFont("Courier New",10);
#elif defined(Q_OS_MAC)
        f = QFont("Courier", 12);
#else
        f = QFont("Bitstream Vera Sans Mono",9);
#endif
		break;

    case Keyword_KW:
	case Keyword:
	case Operator:
        f = QsciLexer::font(style);
        f.setBold(true);
		break;

	default:
        f = QsciLexer::font(style);
	}

	return f;
}

// Returns the set of keywords.
const char *QsciLexerLisp::keywords(int set) const
{
    if (set != 1)
		return 0;

	return keywordClass;
}

// Returns the user name of a style.
QString QsciLexerLisp::description(int style) const
{
	switch (style)
	{
	case Default:
		return tr("Default");

	case Comment:
		return tr("Comment");

	case Number:
		return tr("Number");

    case Symbol:
        return tr("Symbol");

    case Special:
        return tr("Special");

	case Keyword:
		return tr("Keyword");

    case Keyword_KW:
        return tr("Keyworkd_KW");

    case String:
        return tr("String");

    case StringEol:
        return tr("String Eol");

	case Operator:
		return tr("Operator");

	case Identifier:
		return tr("Identifier");

	case CommentBlock:
		return tr("Comment block");
	}

	return QString::null;
}

#endif
