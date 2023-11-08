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

#ifndef QSCILEXERHASKELL_H
#define QSCILEXERHASKELL_H

#include <configure.h>

#ifdef _WITH_QSCINTILLA

#include <QObject>

#include "qscilexer.h"

class QsciLexerHaskell : public QsciLexer
{
    Q_OBJECT

public:
    //! This enum defines the meanings of the different styles used by the
    //! Haskell lexer. --> use constants in SciLexer.h
    enum {
        Default = 0,

        WhiteSpace = Default,

        Identifier = 1,

        Keyword = 2,

        Number = 3,

        String = 4,

        Character = 5,

        ClassName = 6,

        Module = 7,

        Capital = 8,

        Data = 9,

        Import = 10,

        Operator = 11,

        Instance = 12,

        CommentLine = 13,

        CommentBlock = 14,

        CommentBlock2 = 15,

        CommentBlock3 = 16,

        Pragma = 17,

        Preprocessor = 18,

        StringEol = 19,

        ReservedOperator = 20,

        LiterateComment = 21,

        LiterateCodeLim = 22,
    };

    QsciLexerHaskell( QObject *parent = 0 );
    virtual ~QsciLexerHaskell();

    const char *lexer() const;
    const char *language() const;
    const char *keywords(int set) const;

    QString description(int style) const;

    QColor defaultColor(int style) const;
    QFont defaultFont(int style) const;

private:
    static const char *keywordClass;

    QsciLexerHaskell(const QsciLexerHaskell &);
    QsciLexerHaskell &operator=(const QsciLexerHaskell &);
};

#endif

#endif // QSCILEXERHASKELL_H
