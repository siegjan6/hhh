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

#ifndef QEXTSCINTILLALEXERLISP_H
#define QEXTSCINTILLALEXERLISP_H

#include <configure.h>

#ifdef _WITH_QSCINTILLA

#include <QObject>

#include "qscilexer.h"

//! \brief The QextScintillaLexerLisp class encapsulates the Scintilla Lisp
//! lexer.
class QsciLexerLisp : public QsciLexer
{
	Q_OBJECT

public:
	//! This enum defines the meanings of the different styles used by the
    //! Lisp lexer.
	enum {
		//! The default.
		Default = 0,

		//! \obsolete White space.
		WhiteSpace = Default,

		//! A comment.
		Comment = 1,

		//! A number.
		Number = 2,

		//! A keyword.
        Keyword = 3,

        //! A keyworkd kw
        Keyword_KW = 4,

        //! A symbol
        Symbol = 5,

        //! A string.
        String = 6,

        //! A string Eol
        StringEol = 8,

		//! An identifier
        Identifier = 9,

        //! An operator.
        Operator = 10,

        //! A special token
        Special = 11,

        //! A comment block.
        CommentBlock = 12
    };

	//! This enum defines the different conditions that can cause
	//! indentations to be displayed as being bad.
	//! Construct a QextScintillaLexerLisp with parent \a parent and name
	//! \a name.  \a parent is typically the QextScintilla instance.
    QsciLexerLisp(QObject *parent = 0);

	//! Destroys the QextScintillaLexerLisp instance.
    virtual ~QsciLexerLisp();

	//! Returns the name of the language.
	const char *language() const;

	//! Returns the name of the lexer.  Some lexers support a number of
	//! languages.
	const char *lexer() const;

	//! \internal Returns a space separated list of words or characters in
	//! a particular style that define the end of a block for
	//! auto-indentation.  The styles is returned via \a style.
	const char *blockEnd(int *style = 0) const;

	//! \internal Returns a space separated list of words or characters in
	//! a particular style that define the start of a block for
	//! auto-indentation.  The styles is returned via \a style.
	const char *blockStart(int *style = 0) const;

	//! \internal Returns the style used for braces for brace matching.
	int braceStyle() const;

	//! Returns the foreground colour of the text for style number
	//! \a style.
	//!
	//! \sa paper()
    QColor defaultColor(int style) const;

	//! Returns the font for style number \a style.
    QFont defaultFont(int style) const;

	//! Returns the set of keywords for the keyword set \set recognised by
	//! the lexer as a space separated string.
	const char *keywords(int set) const;

	//! Returns the descriptive name for style number \a style.  If the
	//! style is invalid for this language then QString::null is returned.
	//! This is intended to be used in user preference dialogs.
	QString description(int style) const;

private:
	static const char *keywordClass;

    QsciLexerLisp(const QsciLexerLisp &);
    QsciLexerLisp &operator=(const QsciLexerLisp &);
};

#endif

#endif
