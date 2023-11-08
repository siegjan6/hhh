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

#ifndef QSCISCINTILLAMOCK_H
#define QSCISCINTILLAMOCK_H

#include "configure.h"

#include <QPlainTextEdit>

#ifndef _WITH_QSCINTILLA

#include <QSyntaxHighlighter>

// ************************************************************************

// simple syntax highlighter for QScript
class QScriptSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    QScriptSyntaxHighlighter(QTextDocument * parent);

protected:
    virtual void highlightBlock(const QString &text);

private:
    void appendRules(const QStringList & lstSymbolPatterns, const QTextCharFormat & aFormat);
    void checkForComments(int iStart, const QString &sText, bool bRecursive);

    struct HighlightingRule
    {
        QRegExp                 pattern;
        QTextCharFormat         format;
    };
    QVector<HighlightingRule>   m_aHighlightingRules;

// numbers, strings, operators, klammern, constants, variables, functions, commands

    QTextCharFormat             m_aKeywordFormat;
    QTextCharFormat             m_aCommentFormat;
    QTextCharFormat             m_aNumbersFormat;
    QTextCharFormat             m_aStringsFormat;
    QTextCharFormat             m_aConstantsFormat;
    QTextCharFormat             m_aVariablesFormat;
    //QTextCharFormat             m_aCommandsFormat;
    QTextCharFormat             m_aFunctionsFormat;
    QTextCharFormat             m_aNormalFormat;
};

// ************************************************************************

class _QsciScintilla : public QPlainTextEdit
{
    Q_OBJECT

public:
    // enum definitions copied from qscintilla...

    enum AutoCompletionSource {
        //! No sources are used, ie. automatic auto-completion is disabled.
        AcsNone,

        //! The source is all available sources.
        AcsAll,

        //! The source is the current document.
        AcsDocument,

        //! The source is any installed APIs.
        AcsAPIs
    };

    enum
    {
        SC_EOL_CRLF = 0,
        SC_EOL_CR = 1,
        SC_EOL_LF = 2
    };

    enum EolMode {
        //! A carriage return/line feed as used on Windows systems.
        EolWindows = SC_EOL_CRLF,

        //! A line feed as used on Unix systems, including OS/X.
        EolUnix = SC_EOL_LF,

        //! A carriage return as used on Mac systems prior to OS/X.
        EolMac = SC_EOL_CR
    };

    enum
    {
        SCWS_INVISIBLE = 0,
        SCWS_VISIBLEALWAYS = 1,
        SCWS_VISIBLEAFTERINDENT = 2
    };

    enum WhitespaceVisibility {
        //! Whitespace is invisible.
        WsInvisible = SCWS_INVISIBLE,

        //! Whitespace is always visible.
        WsVisible = SCWS_VISIBLEALWAYS,

        //! Whitespace is visible after the whitespace used for indentation.
        WsVisibleAfterIndent = SCWS_VISIBLEAFTERINDENT
    };

    enum FoldStyle {
        //! Folding is disabled.
        NoFoldStyle,

        //! Plain folding style using plus and minus symbols.
        PlainFoldStyle,

        //! Circled folding style using circled plus and minus symbols.
        CircledFoldStyle,

        //! Boxed folding style using boxed plus and minus symbols.
        BoxedFoldStyle,

        //! Circled tree style using a flattened tree with circled plus and
        //! minus symbols and rounded corners.
        CircledTreeFoldStyle,

        //! Boxed tree style using a flattened tree with boxed plus and minus
        //! symbols and right-angled corners.
        BoxedTreeFoldStyle
    };

    enum BraceMatch {
        //! Brace matching is disabled.
        NoBraceMatch,

        //! Brace matching is enabled for a brace immediately before the
        //! current position.
        StrictBraceMatch,

        //! Brace matching is enabled for a brace immediately before or after
        //! the current position.
        SloppyBraceMatch
    };

    _QsciScintilla( QWidget * parent);
    ~_QsciScintilla();

    void setQScriptHighlighter( bool bEnable );
    void setText( const QString & text );

    // qscintilla interface:
    bool isUtf8() const;
    void setUtf8( bool value );
    bool isModified() const;
    void setModified( bool value );
    void setTabWidth( int iWidth );
    void setCursorPosition( int iLine, int iIndex );
    void ensureLineVisible( int iLine );
    QString text() const;
    QString text( int iLineNo ) const;
    void foldAll();
    void setMarginsFont( const QFont & aFont );
    void setAutoCompletionSource( int value );
    void setAutoCompletionThreshold( int value );
    void setIndentationsUseTabs( bool value );
    void setWhitespaceVisibility( bool value );
    void setEolVisibility( bool value );
    void setMarginWidth( int margin, int width );
    void setMarginWidth( int margin, const QString & sText );
    void setAutoIndent( bool value );
    void setFolding( int style, int value );
    void setBraceMatching( int value ); //*** enum
    void setMatchedBraceForegroundColor( const QColor & color );
    void setUnmatchedBraceForegroundColor( const QColor & color );
    void setEolMode( int value );   //*** enum
    void convertEols( int value );  //*** enum
    bool hasSelectedText() const;
    void setSelection( int lineFrom, int indexFrom, int lineTo, int indexTo );
    void getSelection( int * lineFrom, int * indexFrom, int * lineTo, int * indexTo ) const;
    void insertAt( const QString & text, int iLine, int iIndex );
    void beginUndoAction();
    void endUndoAction();
    void removeSelectedText();
    void selectAll( bool value );
    QString selectedText() const;
    void getCursorPosition( int * pLine, int * pCol );
    bool findFirst( const QString & sFindText,
                    bool bRegularExpr,
                    bool bCaseSensitive,
                    bool bOnlyWholeWords,
                    bool bWrap,
                    bool bSearchForward,
                    int iLine,
                    int iIndex );
    bool findNext();
    void replace( const QString & value );
    int lines() const;
    void setMarginLineNumbers( int i, bool value );
    void setSelectionBackgroundColor( const QColor & color );
    void setSelectionForegroundColor( const QColor & color );

    // for linenumbers
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

signals:
    void cursorPositionChanged(int iLine, int iPos);

public slots:
    void sltCursorPositionChanged();

    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &, int);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QScriptSyntaxHighlighter *  m_pSyntaxHighlighter;
    QWidget *                   m_pLineNumberArea;
    QString                     m_sSearchText;
    int                         m_iSearchFlags;
    bool                        m_bUtf8;
    bool                        m_bShowLineNumbers;
};

// workaround to resolve duplicate linked symbols when linkin libqscintilla in QPlainTextEdit modus
#define QsciScintilla _QsciScintilla

// ************************************************************************

// helper class to show line numbers for QPlainTextEdit
// see codeeditor example of Qt
class LineNumberArea : public QWidget
{
public:
    LineNumberArea(_QsciScintilla *editor)
        : QWidget(editor)
    {
        m_pCodeEditor = editor;
    }

    QSize sizeHint() const
    {
        return QSize(m_pCodeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event)
    {
        m_pCodeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    _QsciScintilla *    m_pCodeEditor;
};

#else

// ************************************************************************

class _QsciScintilla : public QPlainTextEdit
{
    Q_OBJECT
};

#endif

#endif // QSCISCINTILLAMOCK_H
