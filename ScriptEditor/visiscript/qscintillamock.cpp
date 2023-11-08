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

#include "qscintillamock.h"

#ifndef _WITH_QSCINTILLA

#include <QTextBlock>
#include <QSyntaxHighlighter>
#include <QPainter>
#include <QTextDocument>

// ************************************************************************

_QsciScintilla::_QsciScintilla(QWidget * parent)
: QPlainTextEdit(parent),
  m_pSyntaxHighlighter(0),
  m_bUtf8(false),
  m_bShowLineNumbers(false)
{
    m_pLineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));

    updateLineNumberAreaWidth(0);

    QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setSizePolicy(sizePolicy);
}

_QsciScintilla::~_QsciScintilla()
{
    delete m_pLineNumberArea;
}

void _QsciScintilla::setQScriptHighlighter( bool bEnable )
{
    if( bEnable )
    {
        if( m_pSyntaxHighlighter==0 )
        {
            m_pSyntaxHighlighter = new QScriptSyntaxHighlighter(document());
        }
    }
    else
    {
        delete m_pSyntaxHighlighter;
        m_pSyntaxHighlighter = 0;
    }
}

void _QsciScintilla::setText( const QString & text )
{
    setPlainText(text);
}

void _QsciScintilla::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    m_pLineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

int _QsciScintilla::lineNumberAreaWidth()
{
    if( !m_bShowLineNumbers )
    {
        return 0;
    }

    int digits = 1;
    int max = qMax(1, document()->blockCount());
    while( max >= 10 )
    {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void _QsciScintilla::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void _QsciScintilla::updateLineNumberArea(const QRect &rect, int dy)
{
    if( dy )
    {
        m_pLineNumberArea->scroll(0, dy);
    }
    else
    {
        m_pLineNumberArea->update(0, rect.y(), m_pLineNumberArea->width(), rect.height());
    }

    if( rect.contains(viewport()->rect()) )
    {
        updateLineNumberAreaWidth(0);
    }
}

void _QsciScintilla::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_pLineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, m_pLineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void _QsciScintilla::sltCursorPositionChanged()
{
    int iLine = textCursor().block().blockNumber();
    int iPos = textCursor().positionInBlock();
    emit cursorPositionChanged( iLine, iPos );
}

void _QsciScintilla::setCursorPosition( int iLine, int iIndex )
{
    QTextCursor cursor = textCursor();
    QTextBlock start = document()->findBlockByLineNumber(iLine);
    cursor.setPosition(start.position()+iIndex);
    setTextCursor(cursor);
}

QString _QsciScintilla::text() const
{
    return document()->toPlainText();
}

QString _QsciScintilla::text( int iLine ) const
{
    return document()->findBlockByLineNumber(iLine).document()->toPlainText();
}

bool _QsciScintilla::isUtf8() const
{
    return m_bUtf8;
}

void _QsciScintilla::setUtf8( bool value )
{
    m_bUtf8 = value;
}

bool _QsciScintilla::isModified() const
{
    return document()->isModified();
}

void _QsciScintilla::setModified( bool value )
{
    document()->setModified(value);
}

void _QsciScintilla::setTabWidth( int iWidth )
{
    int pixels = font().pixelSize();
    int points = font().pointSize();
    setTabStopWidth(iWidth*(points>0 ? points : pixels));
}

void _QsciScintilla::ensureLineVisible( int iLine )
{
    int iCurLine, iCurPos;
    getCursorPosition(&iCurLine,&iCurPos);
    setCursorPosition(iLine,0);
    ensureCursorVisible();
    setCursorPosition(iCurLine,iCurPos);
}

void _QsciScintilla::foldAll()
{

}

void _QsciScintilla::setMarginsFont( const QFont & aFont )
{
    Q_UNUSED(aFont);
    // --> use always the same font as for the text content
}

void _QsciScintilla::setAutoCompletionSource( int value )
{
    Q_UNUSED(value);
}

void _QsciScintilla::setAutoCompletionThreshold( int value )
{
    Q_UNUSED(value);
}

void _QsciScintilla::setIndentationsUseTabs( bool value )
{
    Q_UNUSED(value);
}

void _QsciScintilla::setWhitespaceVisibility( bool value )
{
    Q_UNUSED(value);
}

void _QsciScintilla::setEolVisibility( bool value )
{
    Q_UNUSED(value);
}

void _QsciScintilla::setMarginWidth( int margin, int width )
{
    Q_UNUSED(margin);
    m_bShowLineNumbers = (width!=0);
    updateLineNumberAreaWidth(0);
}

void _QsciScintilla::setMarginWidth( int margin, const QString & sText )
{
    Q_UNUSED(margin);
    m_bShowLineNumbers = (sText.length()>0);
    updateLineNumberAreaWidth(0);
}

void _QsciScintilla::setAutoIndent( bool value )
{
    Q_UNUSED(value);
}

void _QsciScintilla::setFolding( int style, int value )
{
    Q_UNUSED(value);
    Q_UNUSED(style);
}

void _QsciScintilla::setBraceMatching( int value ) //***
{
    Q_UNUSED(value);
}

void _QsciScintilla::setMatchedBraceForegroundColor( const QColor & color )
{
    Q_UNUSED(color);
}

void _QsciScintilla::setUnmatchedBraceForegroundColor( const QColor & color )
{
    Q_UNUSED(color);
}

void _QsciScintilla::setEolMode( int value )   //***
{
    Q_UNUSED(value);
}

void _QsciScintilla::convertEols( int value )  //***
{
    Q_UNUSED(value);
}

bool _QsciScintilla::hasSelectedText() const
{
    return textCursor().hasSelection();
}

void _QsciScintilla::setSelection(int lineFrom, int indexFrom, int lineTo, int indexTo )
{
    QTextBlock start = document()->findBlockByLineNumber(lineFrom);
    QTextBlock stop = document()->findBlockByLineNumber(lineTo);
    QTextCursor c = textCursor();
    c.setPosition(start.position()+indexFrom);
    c.setPosition(stop.position()+indexTo, QTextCursor::KeepAnchor);
    setTextCursor(c);
}

void _QsciScintilla::getSelection(int * lineFrom, int * indexFrom, int * lineTo, int * indexTo ) const
{
    int iStart = textCursor().selectionStart();
    int iStop = textCursor().selectionEnd();
    QTextBlock start = document()->findBlock(iStart);
    QTextBlock stop = document()->findBlock(iStop);
    if( lineFrom )
    {
        *lineFrom = start.blockNumber();
    }
    if( indexFrom )
    {
        *indexFrom = iStart-start.position();
    }
    if( lineTo )
    {
        *lineTo = stop.blockNumber();
    }
    if( indexTo )
    {
        *indexTo = iStop-stop.position();
    }
}

void _QsciScintilla::insertAt(const QString & text, int iLine, int iIndex )
{
    textCursor().clearSelection();
    setCursorPosition(iLine,iIndex);
    textCursor().insertText(text);
}

void _QsciScintilla::beginUndoAction()
{
    // not available for QPlainTextEdit !
}

void _QsciScintilla::endUndoAction()
{
    // not available for QPlainTextEdit !
}

void _QsciScintilla::removeSelectedText()
{
    textCursor().removeSelectedText();
}

void _QsciScintilla::selectAll( bool value )
{
    if( value )
    {
        textCursor().select(QTextCursor::Document);
    }
    else
    {
        textCursor().removeSelectedText();
    }
}

QString _QsciScintilla::selectedText() const
{
    return textCursor().selectedText();
}

void _QsciScintilla::getCursorPosition( int * pLine, int * pCol )
{
    int iLine = textCursor().block().blockNumber();
    int iCol = textCursor().positionInBlock();
    if( pLine )
    {
        *pLine = iLine;
    }
    if( pCol )
    {
        *pCol = iCol;
    }
}

bool _QsciScintilla::findFirst(const QString & sFindText,
                bool bRegularExpr,
                bool bCaseSensitive,
                bool bOnlyWholeWords,
                bool bWrap,
                bool bSearchForward,
                int iLine,
                int iIndex )
{
    Q_UNUSED(bRegularExpr);
    Q_UNUSED(bWrap);
    if( iLine>=0 && iIndex>=0 )
    {
        setCursorPosition(iLine,iIndex);
    }
    int flags = 0;
    if( !bSearchForward )
    {
        flags |= QTextDocument::FindBackward;
    }
    if( bCaseSensitive )
    {
        flags |= QTextDocument::FindCaseSensitively;
    }
    if( bOnlyWholeWords )
    {
        flags |= QTextDocument::FindWholeWords;
    }
    m_sSearchText = sFindText;
    m_iSearchFlags = flags;
    return find(sFindText,(QTextDocument::FindFlags)flags);
}

bool _QsciScintilla::findNext()
{
    return find(m_sSearchText,(QTextDocument::FindFlags)m_iSearchFlags);
}

void _QsciScintilla::replace( const QString & value )
{
    textCursor().insertText(value);
}

int _QsciScintilla::lines() const
{
    return document()->lineCount();
}

void _QsciScintilla::setMarginLineNumbers( int i, bool value )
{
    Q_UNUSED(value);
    Q_UNUSED(i);
}

void _QsciScintilla::setSelectionBackgroundColor( const QColor & color )
{
    Q_UNUSED(color);
// http://www.qtcentre.org/threads/32517-QPlainTextEdit-selection-color

//QPalette p = textEdit->palette();
//p.setColor(QPalette::Highlight, QColor(/*what ever you want*/);
//p.setColor(QPalette::HighlightedText, QColor(/*what ever you want*/);
//textEdit->setPalette(p);
}

void _QsciScintilla::setSelectionForegroundColor( const QColor & color )
{
    Q_UNUSED(color);
}

// ************************************************************************

QScriptSyntaxHighlighter::QScriptSyntaxHighlighter(QTextDocument * parent)
    : QSyntaxHighlighter(parent)
{
    m_aKeywordFormat.setForeground(Qt::darkGreen);
    m_aKeywordFormat.setFontWeight(QFont::Bold);
    m_aFunctionsFormat.setForeground(Qt::blue);
    m_aFunctionsFormat.setFontWeight(QFont::Bold);
    m_aVariablesFormat.setForeground(Qt::darkMagenta);
    m_aVariablesFormat.setFontWeight(QFont::Bold);
    m_aConstantsFormat.setForeground(Qt::darkRed);
    m_aConstantsFormat.setFontWeight(QFont::Bold);
    m_aStringsFormat.setForeground(Qt::darkMagenta);
    m_aStringsFormat.setFontWeight(QFont::Bold);
    m_aNormalFormat.setForeground((Qt::black));
    m_aCommentFormat.setForeground(Qt::gray);

    QStringList lstSymbolPatterns;

    lstSymbolPatterns
            << "\\bif\\b" << "\\bfor\\b" << "\\bdo\\b"
            << "\\while\\b" << "\\bswitch\\b" << "\\bcase\\b"
            << "\\breturn\\b" << "\\bbreak\\b" << "\\bcatch\\b"
            << "\\bcontinue\\b" << "\\bdebugger\\b" << "\\bdefault\\b"
            << "\\bdelete\\b" << "\\belse\\b" << "\\bfinally\\b"
            << "\\bfunction\\b" << "\\bin\\b" << "\\binstanceof\\b"
            << "\\bnew\\b" << "\\bthis\\b" << "\\bthrow\\b"
            << "\\btry\\b" << "\\btypeof\\b" << "\\bvar\\b"
            << "\\bvoid\\b" << "\\bwith\\b" << "\\bsuper\\b"
            << "\\babstract\\b" << "\\bboolean\\b" << "\\bbyte\\b"
            << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
            << "\\bdouble\\b" << "\\benum\\b" << "\\bexport\\b"
            << "\\bextends\\b" << "\\bfinal\\b" << "\\bfloat\\b"
            << "\\binterface\\b" << "\\blong\\b" << "\\bnative\\b"
            << "\\bpackage\\b" << "\\bprivate\\b" << "\\bprotected\\b"
            << "\\bpublic\\b" << "\\bshort\\b" << "\\bstatic\\b"
            << "\\bsynchronized\\b" << "\\bthrows\\b" << "\\btransient\\b"
            << "\\bgoto\\b" << "\\bimplements\\b" << "\\bimport\\b"
            << "\\bvolatile\\b" << "\\bimplements\\b" << "\\bint\\b";
    appendRules(lstSymbolPatterns,m_aKeywordFormat);

    lstSymbolPatterns.clear();
    lstSymbolPatterns
            << "\\babs\\b" << "\\bacos\\b" << "\\bacosh\\b"
            << "\\barg\\b" << "\\basin\\b" << "\\basinh\\b"
            << "\\batan\\b" << "\\batan2\\b" << "\\batanh\\b"
            << "\\bbesj0\\b" << "\\bbesj1\\b" << "\\bbesy0\\b"
            << "\\bbesy1\\b" << "\\bceil\\b" << "\\bcos\\b"
            << "\\bcosh\\b" << "\\berf\\b" << "\\berfc\\b"
            << "\\bexp\\b" << "\\bfloor\\b" << "\\bgamma\\b"
            << "\\bibeta\\b" << "\\binverf\\b" << "\\bigamma\\b"
            << "\\bimag\\b" << "\\binvnorm\\b" << "\\bint\\b"
            << "\\blambertw\\b" << "\\blgamma\\b" << "\\blog\\b"
            << "\\blog10\\b" << "\\bnorm\\b" << "\\brand\\b"
            << "\\breal\\b" << "\\bsgn\\b" << "\\bsin\\b"
            << "\\bsinh\\b" << "\\bsqrt\\b" << "\\btan\\b"
            << "\\btanh\\b" << "\\bcolumn\\b" << "\\bdefined\\b"
            << "\\btm_hour\\b" << "\\btm_mday\\b" << "\\btm_min\\b"
            << "\\btm_mon\\b" << "\\btm_sec\\b" << "\\btm_wday\\b"
            << "\\btm_yday\\b" << "\\btm_year\\b" << "\\bvalid\\b"
            << "\\bexistsgprintf\\b" << "\\bsprintf\\b" << "\\bstringcolumn\\b"
            << "\\bstrlen\\b" << "\\bstrstrt\\b" << "\\bsubstr\\b"
            << "\\bsystem\\b" << "\\bword\\b" << "\\bwords\\b"
            << "\\bpi\\b";
    appendRules(lstSymbolPatterns,m_aFunctionsFormat);

    lstSymbolPatterns.clear();
    appendRules(lstSymbolPatterns,m_aVariablesFormat);

    lstSymbolPatterns.clear();
    appendRules(lstSymbolPatterns,m_aConstantsFormat);

    lstSymbolPatterns.clear();
    lstSymbolPatterns
            << "\"([^\\\"]|\\.)*\""
            << "\\("
            << "\\)";
    appendRules(lstSymbolPatterns,m_aStringsFormat);
}

void QScriptSyntaxHighlighter::appendRules(const QStringList & lstSymbolPatterns, const QTextCharFormat & aFormat)
{
    HighlightingRule aRule;
    for(int i = 0 ;i < lstSymbolPatterns.count();i++)
    {
        aRule.pattern = QRegExp(lstSymbolPatterns[i]);
        aRule.format = aFormat;
        m_aHighlightingRules.append(aRule);
    }
}

void QScriptSyntaxHighlighter::checkForComments(int iStart, const QString & sText, bool bRecursive)
{
    if( sText.length()>0 )
    {
        QString text = sText;
        int iFound = iStart;
        bool bStartParse = bRecursive ? true : previousBlockState()<1;
        // handle c comment /**/
        if( bStartParse )
        {
            // parse all /**/ areas in one line/text block
            bool bParse = true;
            while( bParse )
            {
                iFound = text.indexOf("/*", iFound);
                if( iFound>=0 )
                {
                    int iEnd = text.indexOf("*/",iFound+2);
                    setCurrentBlockState( iEnd<0 ? 1 : 0);
                    setFormat(iFound, iEnd>=0 ? iEnd+2-iFound : sText.length(), m_aCommentFormat);
                    bParse = iEnd>=0;
                    if( iEnd>=0 )
                    {
                        iFound = iEnd+2;
                    }
                }
                else
                {
                    bParse = false;
                }
            }
        }
        else
        {
            if( iFound<text.length() )
            {
                int iEnd = text.indexOf("*/",iFound);
                setCurrentBlockState( iEnd<0 ? 1 : 0);
                setFormat(iFound, iEnd>=0 ? iEnd+2-iFound : sText.length(), m_aCommentFormat);
                // check for further comment blocks
                if( iEnd>=0 )
                {
                    checkForComments(iEnd+2,sText,true);
                }
            }
        }
    }
}

void QScriptSyntaxHighlighter::highlightBlock(const QString &sText)
{
    setCurrentBlockState( 0 );

    QString text = sText;
    setFormat(0,text.length(),m_aNormalFormat);

    // remove characters of c++ comments from highlighting text...
    // c++ comment: all chars after a // belongs to a comment
    int iFound = text.indexOf("//");
    if( iFound>=0 )
    {
        // update color for comment...
        text = text.mid(0,iFound);
        setFormat(iFound, sText.length()-iFound, m_aCommentFormat);
    }

    checkForComments(0,text,false);

    for(int i =0 ;i < m_aHighlightingRules.count();i++)  {
        const HighlightingRule &rule = m_aHighlightingRules[i];
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            if( format(index)!=m_aCommentFormat )
            {
                setFormat(index, length, rule.format);
            }
            index = expression.indexIn(text, index + length);
        }
    }

/*
    QTextCharFormat myClassFormat;
    myClassFormat.setFontWeight(QFont::Bold);
    myClassFormat.setForeground(Qt::darkMagenta);
*/

// http://astro.berkeley.edu/~mkmcc/software/src/gnuplot.html
// https://groups.google.com/forum/?fromgroups#!topic/comp.graphics.apps.gnuplot/UufLJwOntCE
// http://www.vim.org/scripts/script.php?script_id=1737
// http://lists.kde.org/?l=kwrite-devel&m=108299163904146

// numbers, strings, operators, klammern, constants, variables, functions, commands
/*
    QString pattern = "\\bMy[A-Za-z]+\\b";
    //QString pattern = "\\b[0-9\.,eE+-]+\\b";
    //QString pattern = "\\b[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)\\b";
    QRegExp expression(pattern);
    int index = text.indexOf(expression);
    while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, myClassFormat);
        index = text.indexOf(expression, index + length);
    }
*/
}

#endif
