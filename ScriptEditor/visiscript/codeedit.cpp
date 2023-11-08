/**************************************************************************
 *
 *	project				 : VisiScript
 *
 *	copyright            : (C) 2012 by Michael Neuroth
 *
 ***************************************************************************/
/*********************************************************************************
 *																				 *
 * This file is part of the VisiScript package (a frontend for script languages) *
 *																				 *
 * Copyright (C) 2012 by Michael Neuroth.										 *
 *                                                                               *
 * This program is free software; you can redistribute it and/or modify			 *
 * it under the terms of the GNU General Public License as published by    		 *
 * the Free Software Foundation; either version 2 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 ********************************************************************************/

#include "codeedit.h"
#include "inline_code.h"

#include <QApplication>
#include <QMessageBox>
#include <QTabWidget>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QKeyEvent>
#include <QStringList>
#include <QTimer>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#if !defined(Q_OS_IOS)
#include <QPrintDialog>
#include <QPrinter>
#endif

// ************************************************************************

#ifdef _WITH_QSCINTILLA

#include <qscilexer.h>
#include <qsciapis.h>
#include <qsciprinter.h>
#include <qscilexercpp.h>
#include <qscilexercsharp.h>
#include <qscilexerpython.h>
#include <qscilexerjava.h>
#include <qscilexerjavascript.h>
#include <qscilexerd.h>
#include <qscilexerperl.h>
#include <qscilexeridl.h>
#include <qscilexersql.h>
#include <qscilexerhtml.h>
#include <qscilexerxml.h>
#include <qscilexerlua.h>
#include <qscilexerpascal.h>
#include <qscilexerfortran.h>
#include <qscilexerruby.h>
#include <qscilexerdiff.h>
#include <qsciapis.h>
#include <qscilexervhdl.h>
#include <qscilexermatlab.h>
#include <qscilexeroctave.h>
#include <qscilexermakefile.h>
#include <qscilexerbash.h>
#include <qscilexerbatch.h>
#include <qscilexerpo.h>
#include <qscilexerproperties.h>
#include <qscilexerspice.h>
#include <qscilexeryaml.h>
#include <qscilexercmake.h>
#include <qscilexerpostscript.h>
#include <qscilexercoffeescript.h>
#include <qscilexercss.h>
#include <qscilexertex.h>
#include <qscilexertcl.h>
#include <qextscintillalexerlisp.h>
#include <qscilexerhaskell.h>

#endif

// ************************************************************************

#define MAX_NAVIGATION_STACK_SIZE   1000

// ************************************************************************

static QString GetTabHeaderLabel( const QString & sLabel )
{
#if defined(Q_OS_ANDROID)
    const int c_iMax = 15;
#else
    const int c_iMax = 25;
#endif
    const int c_iExt = 6;
    const int c_iMiddle = 2;

    if( sLabel.length()>c_iMax )
    {
        return sLabel.mid(0,c_iMax-c_iExt-c_iMiddle)+".."+sLabel.mid(sLabel.length()-c_iExt,c_iExt);
    }
    else
    {
        return sLabel;
    }
}

// count the number of lines where iPos is found
static QPair<int,int> GetLineNumberAndIndexForPos(const QStringList & lstLines, int iPos, int iStartLineCount = 0, int iStartCount = 0)
{
    int iLineCount = iStartLineCount;
    int iCount = iStartCount;
    while( iCount<=iPos )
    {
		iCount += lstLines[iLineCount++].length()+1;  // +1 for new line
    }
    return QPair<int,int>(iLineCount-1,/*posLineStart*/iCount-lstLines[iLineCount-1].length()-1);
}

/*
static QString GetLineNo( const QString & sText, int iLineNo )
{
    QString s = sText;
    int iFound = 0;
    int i = 0;
    while( iFound>=0 )
    {
        iFound = s.indexOf(QRegExp("[\r\n]"));
        if( i == iLineNo )
        {
            return s.mid(0,iFound);
        }
        i++;
    }
}

bool PeekForUtf8Signature( const QString & sText )
{
    int iNewLinePos = sText.indexOf("\n");
    if( iNewLinePos>=0 )
    {
        for( int i=0; i<3; i++ )
        {
            QString s = GetLineNo(sText,i).toLower();
            if( ContainsEncoding(s,"utf-8","utf8") )
            {
                return true;
            }
        }
    }
    return false;
}
*/

bool WriteTextFile( const QString & sFullFileName, const QString & sText, bool bAppend, bool bUtf8 )
{
    QFile aFile( sFullFileName );

    if( !aFile.open( (bAppend ? QIODevice::Append : QIODevice::WriteOnly) | QIODevice::Text) )
    {
        return false;
    }
    else
    {
        QTextStream aOutStream( &aFile );
        if( bUtf8 )
        {
            aOutStream.setCodec(QTextCodec::codecForName("UTF-8"));
        }
        else
        {
            aOutStream.setCodec(QTextCodec::codecForName("ISO-8859-1"));
        }

        aOutStream << sText;

        return aOutStream.status()==QTextStream::Ok;
    }
}

// ************************************************************************

NavigationStack::NavigationStack()
: m_iActPos(0)
{
}

void NavigationStack::push_pos(int iLine, int iPos)
{
    // falls value(actpos)==existing value --> forward() Navigation, daten bleiben erhalten
    // else --> alle nachfolgenden daten von actpos an loeschen, neuen wert einfuegen
    if( m_iActPos>0 && m_iActPos<size() )
    {
        // navigation modus
        NavigationPos aNewPos(iLine,iPos);
        NavigationPos aActPos = getActPos();
        if( aActPos==aNewPos )
        {
            replace(m_iActPos-1,aNewPos);
        }
        else
        {
            resize(m_iActPos);
            push_back(aNewPos);
            m_iActPos++;
        }
    }
    else
    {
        push_back(NavigationPos(iLine,iPos));
        m_iActPos++;
    }
    if( size()>=MAX_NAVIGATION_STACK_SIZE )
    {
        pop_front();
        m_iActPos--;
    }
}

NavigationPos NavigationStack::getActPos()
{
    return value(m_iActPos-1);
}

NavigationPos NavigationStack::forward()
{
    m_iActPos++;
    if( m_iActPos>size() )
    {
        m_iActPos = size();
    }
    return getActPos();
}

NavigationPos NavigationStack::backward()
{
    m_iActPos--;
    if( m_iActPos<0 )
    {
        m_iActPos = 0;
    }
    return getActPos();
}

// ************************************************************************

CodeEdit::CodeEdit( QWidget * parent, const char * name, QTabWidget * pTabWidget,
                    QWidget * pTab, QWidget * pMainWin, QFileSystemWatcher * pFileSystemWatcher, bool bUtf8 )
: QsciScintilla( parent ),
  m_pMainWin( pMainWin ),
  m_pAPIs( 0 ),
  m_pLexer( 0 ),
  m_pTimer( 0 ),
  m_pFileSystemWatcher( pFileSystemWatcher ),
  m_pTabWidget( pTabWidget ),
  m_pTab( pTab ),
  m_bIsMainFile( false ),
  m_bWasLastKeyReturn( false ),
  m_bIsWriting( false ),
  m_bMarkAllOccurencesOfSelection( true ),
  m_iMouseClickCount( 0 ),
  m_iLanguageID( LANG_JAVASCRIPT ),
  m_aCurrentMacro( this )
{
    setObjectName(name);    
    //setStyle(QStyle::SH_RequestSoftwareInputPanel);

    connect( this, SIGNAL( textChanged() ), this, SLOT( sltUpdateTabName() ) );
	connect( this, SIGNAL( isShowing() ), pMainWin, SLOT( sltUpdateChilds() ) );
	connect( this, SIGNAL( sigTabWithSelectedTextPressed() ), pMainWin, SLOT( sltTabWithSelectedTextPressed() ) );
	connect( this, SIGNAL( sigShiftTabWithSelectedTextPressed() ), pMainWin, SLOT( sltShiftTabWithSelectedTextPressed() ) );
    connect( this, SIGNAL( sigCloseEditTab(CodeEdit *) ), pMainWin, SLOT( sltCloseTab(CodeEdit *) ) );
    connect( this, SIGNAL( selectionChanged() ), this, SLOT( sltSelectionChanged() ) );
    connect( this, SIGNAL( marginClicked(int, int, Qt::KeyboardModifiers)), this, SLOT( sltMarginClicked(int,int,Qt::KeyboardModifiers) ) );

    setMarginSensitivity(0,true);

#ifdef _WITH_QSCINTILLA
    // add indicator (red box) around selected text
    m_iIndicator = indicatorDefine(BoxIndicator /*StraightBoxIndicator*/ /*DotBoxIndicator*/);
    setIndicatorForegroundColor("red",m_iIndicator);
#else
    connect( this, SIGNAL( cursorPositionChanged() ), this, SLOT( sltCursorPositionChanged() ) );
    setViewportMargins(lineNumberAreaWidth()+5, 0, 0, 0);
    setUndoRedoEnabled(true);
#endif

    // set encoding (latin1 is default of QScintilla)
    setUtf8(bUtf8);

    QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setSizePolicy(sizePolicy);
}

CodeEdit::~CodeEdit()
{
#ifdef _WITH_QSCINTILLA
    delete m_pAPIs;
    if( m_pLexer )          // bugfix 10.1.2010: handle case if document has no lexer
    {
        m_pLexer->setAPIs(0);
        delete m_pLexer;
    }
#endif
    delete m_pTimer;
}

void CodeEdit::updateLanguage( int iActLanguageID )
{
#ifdef _WITH_QSCINTILLA
    QsciLexer * pLexer = 0;

    switch( iActLanguageID )
    {
        case LANG_NONE:
            pLexer = 0;
            break;
        case LANG_MIMDCRIPT:
        case LANG_CPP:
            pLexer = new QsciLexerCPP( this );
            break;
        case LANG_CSHARP:
            pLexer = new QsciLexerCSharp( this );
            break;
        case LANG_PYTHON:
            pLexer = new QsciLexerPython( this );
            break;
        case LANG_JAVA:
            pLexer = new QsciLexerJava( this );
            break;
        case LANG_JAVASCRIPT:
            pLexer = new QsciLexerJavaScript( this );
            break;
        case LANG_D:
            pLexer = new QsciLexerD( this );
            break;
        case LANG_RUBY:
            pLexer = new QsciLexerRuby( this );
            break;
        case LANG_PHP:
            // php lexer is embedded in html lexer !
            // see: http://www.mail-archive.com/pyqt@riverbankcomputing.com/msg15574.html
            pLexer = new QsciLexerHTML( this );
            break;
        case LANG_PERL:
            pLexer = new QsciLexerPerl( this );
            break;
        case LANG_IDL:
            pLexer = new QsciLexerIDL( this );
            break;
        case LANG_HTML:
            pLexer = new QsciLexerHTML( this );
            break;
        case LANG_CSS:
            pLexer = new QsciLexerCSS( this );
            break;
        case LANG_XML:
            pLexer = new QsciLexerXML( this );
            break;
        case LANG_SQL:
            pLexer = new QsciLexerSQL( this );
            break;
        case LANG_LUA:
            pLexer = new QsciLexerLua( this );
            break;
        case LANG_PASCAL:
            pLexer = new QsciLexerPascal( this );
            break;
        case LANG_FORTRAN:
            pLexer = new QsciLexerFortran( this );
            break;
        case LANG_SCHEME:
            pLexer = new QsciLexerLisp( this );
            break;
        case LANG_LISP:
            pLexer = new QsciLexerLisp( this );
            break;
        case LANG_HASKELL:
            pLexer = new QsciLexerHaskell( this );
            break;
        case LANG_TCL:
            pLexer = new QsciLexerTCL( this );
            break;
        case LANG_VHDL:
            pLexer = new QsciLexerVHDL( this );
            break;
        case LANG_MATLAB:
            pLexer = new QsciLexerMatlab( this );
            break;
        case LANG_OCTAVE:
            pLexer = new QsciLexerOctave( this );
            break;
        case LANG_MAKEFILE:
            pLexer = new QsciLexerMakefile( this );
            break;
        case LANG_BATCH:
            pLexer = new QsciLexerBatch( this );
            break;
        case LANG_BASH:
            pLexer = new QsciLexerBash( this );
            break;
        case LANG_TEX:
            pLexer = new QsciLexerTeX( this );
            break;
        case LANG_DIFF:
            pLexer = new QsciLexerDiff( this );
            break;
        case LANG_YAML:
            pLexer = new QsciLexerYAML( this );
            break;
        case LANG_PO:
            pLexer = new QsciLexerPO( this );
            break;
        case LANG_PROPERTIES:
            pLexer = new QsciLexerProperties( this );
            break;
        case LANG_SPICE:
            pLexer = new QsciLexerSpice( this );
            break;
        case LANG_CMAKE:
            pLexer = new QsciLexerCMake( this );
            break;
        case LANG_POSTSCRIPT:
            pLexer = new QsciLexerPostScript( this );
            break;
        case LANG_COFFEESCRIPT:
            pLexer = new QsciLexerCoffeeScript( this );
            break;
        default:
            pLexer = 0;
    }

    setLanguage( pLexer, iActLanguageID );

    if( pLexer )
    {
        pLexer->refreshProperties();
    }
#else
    setLanguage( 0, iActLanguageID );
    setQScriptHighlighter( iActLanguageID!=LANG_NONE );
#endif
    // generate a repaint() so that the lexer colorizes the source code again
    if( !text().isEmpty() )
    {
        setText( text() );
        // by just repainting the ui control nothing changed at the data !
        setEdited( false );
    }

    update();
    repaint();
}

NavigationStack & CodeEdit::GetNavigationStack()
{
    return m_aNavigationStack;
}

void CodeEdit::forward()
{
    int iLine,iIndex;
    NavigationPos aPos = m_aNavigationStack.forward();
    iLine = aPos.m_iLine;
    iIndex = aPos.m_iPos;
    setCursorPosition(iLine,iIndex);
}

void CodeEdit::backward()
{
    int iLine,iIndex;
    NavigationPos aPos = m_aNavigationStack.backward();
    iLine = aPos.m_iLine;
    iIndex = aPos.m_iPos;
    setCursorPosition(iLine,iIndex);
}

void CodeEdit::setWordWrap( bool bValue )
{
#ifdef _WITH_QSCINTILLA
    if( bValue )
	{
		SendScintilla(QsciScintillaBase::SCI_SETWRAPMODE, QsciScintillaBase::SC_WRAP_WORD );
	}
	else
	{
		SendScintilla(QsciScintillaBase::SCI_SETWRAPMODE, (unsigned long)QsciScintillaBase::SC_WRAP_NONE );
	}
#else
    Q_UNUSED(bValue);
#endif
}

void CodeEdit::setOverwriteMode( bool bValue )
{
#ifdef _WITH_QSCINTILLA
    SendScintilla(QsciScintillaBase::SCI_SETOVERTYPE, (unsigned long)bValue );
#else
    Q_UNUSED(bValue);
#endif
}

bool CodeEdit::overwriteMode()
{
#ifdef _WITH_QSCINTILLA
    return (bool)SendScintilla(QsciScintillaBase::SCI_GETOVERTYPE );
#else
    return false;
#endif
}

void CodeEdit::setHorizScrollbar( bool bValue )
{
#ifdef _WITH_QSCINTILLA
    SendScintilla(QsciScintillaBase::SCI_SETHSCROLLBAR, (unsigned long)bValue );
#else
    Q_UNUSED(bValue);
#endif
}

void CodeEdit::setEdited( bool bValue )
{
    setModified( bValue );
    sltUpdateTabName();
}

void CodeEdit::ClearAllTextAreas()
{
#ifdef _WITH_QSCINTILLA
    for( int i=0; i<m_lstTextArea.size(); i++ )
    {
        const TextAreaStruct & aArea = m_lstTextArea[i];
        clearIndicatorRange(aArea.iLineFrom,aArea.iIndexFrom,aArea.iLineTo,aArea.iIndexTo,m_iIndicator);
    }
#endif
    m_lstTextArea.clear();
}

void CodeEdit::MarkAllTextAreas(const QString & sMarkText)
{    
    // support disabling of mark all text areas,
    // because this makes find and replace very slow !
    if( !isMarkAllOccurencesOfSelection() )
    {
        return;
    }

    QString sText = text();
    QStringList lstText = sText.split("\n");

    QStringList lstMarkedText = sMarkText.split("\n");
    int iLineCountInMarkText = lstMarkedText.size()-1;
    int iPos = 0;
	int iLastPos = 0;
	int iLastLineNo = 0;
    iPos = sText.indexOf(sMarkText,iPos);
	int iMarkTextLength = sMarkText.length();
    while( iMarkTextLength>0 && iPos>=0 )
    {
        TextAreaStruct aArea;
        QPair<int,int> aStart = GetLineNumberAndIndexForPos(lstText,iPos,iLastLineNo,iLastPos);	
        iLastPos = aStart.second;       // performance optimization --> do not loop from 0 again !
		iLastLineNo = aStart.first;
        QPair<int,int> aStop = GetLineNumberAndIndexForPos(lstText,iPos+iMarkTextLength,iLastLineNo,iLastPos);
		iLastPos = aStop.second;
		iLastLineNo = aStop.first;
        aArea.iLineFrom = aStart.first;
        aArea.iIndexFrom = iPos-aStart.second;
        aArea.iLineTo = aStart.first+iLineCountInMarkText;
        aArea.iIndexTo = iPos+iMarkTextLength-aStop.second;
        m_lstTextArea.push_back(aArea);
        iPos = sText.indexOf(sMarkText,iPos+iMarkTextLength);
    }

#ifdef _WITH_QSCINTILLA
    for( int i=0; i<m_lstTextArea.size(); i++ )
    {
        const TextAreaStruct & aArea = m_lstTextArea[i];
        fillIndicatorRange(aArea.iLineFrom,aArea.iIndexFrom,aArea.iLineTo,aArea.iIndexTo,m_iIndicator);
    }
#endif
}

QString CodeEdit::GetSelectedString() const
{
    QString sMarkText;
    int lineFrom,indexFrom,lineTo,indexTo;
    getSelection(&lineFrom,&indexFrom,&lineTo,&indexTo);
    if(lineFrom>=0)
    {
        sMarkText += text(lineFrom).mid(indexFrom,lineFrom==lineTo ? indexTo-indexFrom : -1);
        for( int i=lineFrom+1; i<lineTo; i++ )
        {
            sMarkText += text(i);
        }
        if( lineFrom<lineTo )
        {
            sMarkText += text(lineTo).left(indexTo);
        }
    }
    return sMarkText;
}

void CodeEdit::sltSelectionChanged()
{
    ClearAllTextAreas();
    MarkAllTextAreas(GetSelectedString());
}

bool CodeEdit::edited()
{
    return isModified();
}

void CodeEdit::setLanguageID( int iLanguageID )
{
	m_iLanguageID = iLanguageID;
}

int CodeEdit::languageID()
{
	return m_iLanguageID;
}

void CodeEdit::PrintContent()
{
#if !defined(Q_OS_IOS)
#ifdef _WITH_QSCINTILLA
    QsciPrinter aPrinter;
#else
    QPrinter aPrinter;
#endif
    aPrinter.setFullPage( true );
    QPrintDialog aDlg(&aPrinter, this);
    if( aDlg.exec() )
    {
#ifdef _WITH_QSCINTILLA
        aPrinter.printRange( this );
#else
        document()->print(&aPrinter);
#endif
    }
#endif
}

void CodeEdit::setLanguage( QsciLexer * pLexer, int iLanguageID )
{
#ifdef _WITH_QSCINTILLA
    delete m_pLexer;
#endif

    m_pLexer = pLexer;

	setLanguageID( iLanguageID );

#ifdef _WITH_QSCINTILLA
    QsciScintilla::setLexer( m_pLexer );
#endif
}

void CodeEdit::updateFontForLexer( const QFont & aFont )
{
#ifdef _WITH_QSCINTILLA
    if( m_pLexer )
    {
        m_pLexer->setFont( aFont );
		m_pLexer->setDefaultFont( aFont );
		m_pLexer->refreshProperties();
        QsciScintilla::setLexer( m_pLexer );
    }
#else
    Q_UNUSED(aFont);
#endif
}

void CodeEdit::setAPIFile( const QString & sFileName )
{
    if( sFileName!=QString::null )
    {
#ifdef _WITH_QSCINTILLA
        if( m_pAPIs )
        {
            delete m_pAPIs;
            m_pAPIs = 0;
        }
        m_pAPIs = new QsciAPIs(m_pLexer);
        m_pAPIs->clear();
        m_pAPIs->load( sFileName );
        m_pAPIs->prepare();     // WARNING: very time consuming !!!
        m_pLexer->setAPIs( m_pAPIs );
        
//        setAutoCompletionThreshold(2);
//        setCallTipsStyle(/*_QsciScintilla::*/_QsciScintilla::CallTipsNoContext );
//        setCallTipsStyle(/*_QsciScintilla::*/_QsciScintilla::CallTipsNoAutoCompletionContext );
//        setCallTipsStyle(/*_QsciScintilla::*/_QsciScintilla::CallTipsContext );
#endif
    }
}

void CodeEdit::setFileName( const QString & sFileName )
{
	if( m_pFileSystemWatcher )
	{
		if( !m_sFileName.isEmpty() )
		{
            m_pFileSystemWatcher->removePath(m_sFileName);
		}
		m_pFileSystemWatcher->addPath(sFileName);
	}
	m_sFileName = sFileName;

#ifdef WITH_MARKERS
// TODO working on marker tests
//
      markerDeleteAll();
//      setMarginMarkerMask(0,1+2+4+8+16+32+64+128);
      setMarginMarkerMask(0, ~SC_MASK_FOLDERS);
      setMarginMarkerMask(1, 0x0);    // blende symbole fuer markierungen aus spalte mit zeilennummern...
      setMarginMarkerMask(2, 0/*SC_MASK_FOLDERS*/);
      setMarginMarkerMask(3, 0);
      setMarginLineNumbers(0,false);
//      setMarginOptions(
////
setMarginWidth(0,15);
//      setMarginWidth(1,100);
//      setMarginWidth(2,70);
//      setMarginWidth(3,80);
//      setMarginWidth(4,60);

//      int markerId = markerDefine(/*MarkerSymbol::*/Circle);
//      int markerId2 = markerDefine(/*MarkerSymbol::*/RightArrow);
//      qDebug() << "MARKER " << markerId << " " << markerId2 << endl;
//      int markerxxx = markerAdd(0,markerId);
//      int markeryyy = markerAdd(3,markerId2);
//      qDebug() << "MARKER inst " << markerxxx << " " << markeryyy << endl;
//      qDebug() << "mask " << (unsigned long)marginMarkerMask(0) << " " << (unsigned long)marginMarkerMask(1) << " " << (unsigned long)marginMarkerMask(2) << " "<< (unsigned long)marginMarkerMask(3) << " " << (unsigned long)marginMarkerMask(4) << " " << endl;

  //    markerAdd()
#endif

    sltUpdateTabName();
}

QString CodeEdit::onlyFileName() const
{
    return QFileInfo(m_sFileName).fileName();
}

QString CodeEdit::fileName() const
{
	return m_sFileName;
}

QString CodeEdit::extension() const
{
    return QFileInfo(m_sFileName).suffix();
}

QString CodeEdit::language() const
{
#ifdef _WITH_QSCINTILLA
    return lexer() ? lexer()->language() : tr("unknown_language");
#else
    return tr("unknown language");
#endif
}

void CodeEdit::sltClose()
{
    emit sigCloseEditTab(this);
}

void CodeEdit::sltUpdate()
{
    sltUpdateTabName();
}

void CodeEdit::sltFileChanged(const QString & path, bool & bYesToAll)
{
	if( path==m_sFileName )
	{
        if( isWriting() )
        {
            setIsWriting(false);
        }
        else
        {
            QMessageBox::StandardButton result;
    		QFileInfo aFileInfo(path);
    		if( !aFileInfo.exists() )
    		{
                if( !bYesToAll )
                {
                    result = QMessageBox::question( this, tr( "File deleted externally" ),
                                                        tr( "Close tab for file \"")+path+tr("\" ?" ),
                                                        QMessageBox::YesToAll|QMessageBox::Yes|QMessageBox::No );
                }
                if( bYesToAll || result == QMessageBox::Yes || result == QMessageBox::YesToAll )
    			{
                    bYesToAll = bYesToAll || (result == QMessageBox::YesToAll);
    				emit sigCloseEditTab(this);
    			}
    		}
    		else
    		{
                if( !bYesToAll )
                {
                    result = QMessageBox::question( this, tr( "File changed externally" ),
                                                         tr( "Reload changed file \"")+path+tr("\" from disk?" ),
                                                         QMessageBox::YesToAll|QMessageBox::Yes|QMessageBox::No );
                }
                if( bYesToAll || result == QMessageBox::Yes || result == QMessageBox::YesToAll )
    			{
                    bYesToAll = bYesToAll || (result == QMessageBox::YesToAll);
    				QString sText;
                    QPair<bool,bool> bPeekIsUtf8;
                    if( ReadTextFile( path, sText, isUtf8(), &bPeekIsUtf8 ) )
    				{
                        int iLine,iIndex;
                        getCursorPosition(&iLine,&iIndex);
                        setUtf8(bPeekIsUtf8.first ? bPeekIsUtf8.second : isUtf8());
                        setText(sText);
    					setEdited( false );		// after reloading the file from disk nothing hapend !
                        setCursorPosition(iLine,iIndex);
    				}
    			}
    		}
        }
	}
}

void CodeEdit::show()
{
#ifdef _WITH_QSCINTILLA
    QsciScintilla::show();
#else
    QPlainTextEdit::show();
#endif

	emit isShowing();
}

void CodeEdit::sltUpdateTabName()
{
	if( m_pTabWidget )
	{
		QString sAddTemp;
		QString sFrontAddTemp;

        if( edited() )
		{
			sAddTemp = " *";
		}
		if( m_bIsMainFile )
		{
			sFrontAddTemp = "# ";
		}

        m_pTabWidget->setTabText( m_pTabWidget->indexOf(m_pTab), sFrontAddTemp+m_sSymbol+GetTabHeaderLabel(QFileInfo( m_sFileName ).fileName())+sAddTemp );
        m_pTabWidget->setTabToolTip( m_pTabWidget->indexOf(m_pTab), m_sFileName );
    }
}

void CodeEdit::keyPressEvent(QKeyEvent *ke)
{
	m_bWasLastKeyReturn = false;

	switch( ke->key() )
	{
        case Qt::Key_Return:
			m_bWasLastKeyReturn = true;
			break;
        case Qt::Key_Backtab:
            if( hasSelectedText() )
			{
				emit sigShiftTabWithSelectedTextPressed();
				return;
			}
			break;
        case Qt::Key_Tab:
            if( hasSelectedText() )
			{
				emit sigTabWithSelectedTextPressed();
				return;
			}
			break;
		default:
			m_bWasLastKeyReturn = false;
	}

#ifdef _WITH_QSCINTILLA
    QsciScintilla::keyPressEvent( ke );
#else
    QPlainTextEdit::keyPressEvent( ke );
#endif

    // WORKAROUND: update after each keypress, because textChanged() is emmited before text is changed ?
    sltUpdateTabName();
}

void CodeEdit::focusInEvent(QFocusEvent *fe)
{
#ifdef _WITH_QSCINTILLA
    QsciScintilla::focusInEvent( fe );
#else
    QPlainTextEdit::focusInEvent( fe );
#endif
    QEvent * pUserEvent = new CheckForReloadEvent();
    QApplication::postEvent(m_pMainWin,pUserEvent);
}

void CodeEdit::sltCheckForMultitouch()
{
    if( m_iMouseClickCount==1 )
    {
        QEvent * pEvent = new QEvent(QEvent::RequestSoftwareInputPanel);
        QCoreApplication::postEvent(this,pEvent);
        // pEvent will be deleted from event handler ! see Qt documentation
    }
    delete m_pTimer;
    m_pTimer = 0;
}

void CodeEdit::sltMarginClicked(int margin, int line, Qt::KeyboardModifiers state)
{
#ifdef WITH_MARKERS
///    qDebug() << "*** MARGIN CLICKED " << margin << " " << line << " " << state << endl;

    int markerId = markerDefine(/*MarkerSymbol::*/Circle);

    unsigned mask = markersAtLine(line);
///    qDebug() << "mask for line " << mask << " " << line << endl;
    if( mask>0 )
    {
        markerDelete(line);
    }
    else
    {
        int markerxxx = markerAdd(line,markerId);
///        qDebug() << "set MARKER " << markerId << endl;
    }

    for(int i = 0; i<10; i++ )
    {
///        qDebug() << "check: " << i << " " << markerLine(i) << endl;
    }
#endif
}

#if defined(Q_OS_ANDROID)

// 2.12.2011: workaround for activating softkeyboard in QScintilla widget
// --> could be removed in following Qt for Android versions ?
void CodeEdit::mousePressEvent(QMouseEvent *e)
{
    // do not trigger keyboard if a multitouch event is found ...
    // --> count click events in a special time intervall
    m_iMouseClickCount++;
    if( m_pTimer==0 )
    {
        m_iMouseClickCount = 1;
        m_pTimer = new QTimer(this);
        connect(m_pTimer, SIGNAL(timeout()), this, SLOT(sltCheckForMultitouch()));
        m_pTimer->setSingleShot(true);
        m_pTimer->start(250);
    }

    QsciScintilla::mousePressEvent(e);
}

#endif

bool CodeEdit::isLastKeyReturn() const
{
	return m_bWasLastKeyReturn;
}

bool CodeEdit::isMainFile() const
{
	return m_bIsMainFile;
}

bool CodeEdit::isWriting() const
{
    return m_bIsWriting;
}

void CodeEdit::setIsWriting(bool bValue)
{
    m_bIsWriting = bValue;
}

void CodeEdit::setMarkAllOccurencesOfSelection( bool bValue )
{
    m_bMarkAllOccurencesOfSelection = bValue;
}

bool CodeEdit::isMarkAllOccurencesOfSelection() const
{
    return m_bMarkAllOccurencesOfSelection;
}

void CodeEdit::setUserSymbol( const QString & sSymbol )
{
    m_sSymbol = sSymbol;
}

QString CodeEdit::userSymbol() const
{
    return m_sSymbol;
}

void CodeEdit::doRepaint()
{
    repaint();
}

void CodeEdit::setMainFile(bool bValue)
{
	m_bIsMainFile = bValue;
}

void CodeEdit::sltToggleRecordMacro( bool bStartRecord )
{
    if( bStartRecord )
    {
        m_aCurrentMacro.startRecording();
    }
    else
    {
        m_aCurrentMacro.endRecording();
    }
}

void CodeEdit::sltPlayMacro()
{
    m_aCurrentMacro.play();
}

QString CodeEdit::getMacro() const
{
    return m_aCurrentMacro.save();
}

void CodeEdit::setMacro( const QString & sMarco )
{
    m_aCurrentMacro.load( sMarco );
}
