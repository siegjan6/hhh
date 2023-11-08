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

#ifndef _CODEEDIT_H
#define _CODEEDIT_H

#include "configure.h"

#include <QString>
#include <QFont>
#include <QEvent>
#include <QKeyEvent>
#include <QStack>
#include <QList>
#include <QPair>

#ifdef _WITH_QSCINTILLA
#include <qsciscintilla.h>
#include <qscimacro.h>
#else
#include "qscintillamock.h"
#endif

// ************************************************************************

class QsciLexer;
class QsciAPIs;

class QDropEvent;
class QKeyEvent;
class QTabWidget;
class QWidget;
class QFileSystemWatcher;
class QTimer;

// ************************************************************************

bool WriteTextFile( const QString & sFullFileName, const QString & sText, bool bAppend = false, bool bUtf8 = false );

// ************************************************************************

struct TextAreaStruct
{
    int iLineFrom;
    int iIndexFrom;
    int iLineTo;
    int iIndexTo;
};

// ************************************************************************

struct NavigationPos
{
    NavigationPos(int iLine=0, int iPos=0)
        : m_iLine(iLine),
          m_iPos(iPos)
    {}

    bool operator==(const NavigationPos & aOther)
    {
        return m_iLine==aOther.m_iLine && m_iPos==aOther.m_iPos;
    }

    int m_iLine;
    int m_iPos;
};

// ************************************************************************

class NavigationStack : public QStack<NavigationPos>
{
public:
    NavigationStack();
    void push_pos(int iLine, int iPos);
    NavigationPos getActPos();
    NavigationPos forward();
    NavigationPos backward();

private:
    int m_iActPos;
};

// ************************************************************************

class CheckForReloadEvent : public QEvent
{
public:
    CheckForReloadEvent()
    : QEvent( (Type)(_USER_EVENT_CHECK_RELOAD) )
    {
    }
};

// ************************************************************************
// ************************************************************************
// ************************************************************************

class CodeEdit :  public QsciScintilla
{
	Q_OBJECT

public:
    CodeEdit( QWidget * parent=0, const char * name=0, QTabWidget * pTabWidget = 0,
              QWidget * pTab = 0, QWidget * pMainWin = 0,
              QFileSystemWatcher * pFileSystemWatcher = 0, bool bUtf8 = true );
    ~CodeEdit();

    void PrintContent();

    void updateLanguage( int iActLanguageID );
    void setLanguage( QsciLexer * pLexer, int iLanguageID );
	void updateFontForLexer( const QFont & aFont );
    int  languageID();

    void setAPIFile( const QString & sFileName );
    void setHorizScrollbar( bool bValue );
    void setWordWrap( bool bValue );
	void setOverwriteMode( bool bValue );
	bool overwriteMode();
	void setEdited( bool bValue );
	bool edited();

	void setFileName( const QString & sFileName );
    QString onlyFileName() const;
	QString fileName() const;
    QString extension() const;
    QString language() const;

	bool isLastKeyReturn() const;

	bool isMainFile() const;
	void setMainFile(bool bValue);

    bool isWriting() const;
    void setIsWriting(bool bValue);

    void setMarkAllOccurencesOfSelection( bool bValue );
    bool isMarkAllOccurencesOfSelection() const;

    void setUserSymbol( const QString & sSymbol );
    QString userSymbol() const;

    void doRepaint();

    // navigation of cursor
    NavigationStack & GetNavigationStack();
    void forward();
    void backward();

    QString getMacro() const;
    void setMacro( const QString & sMarco );

public slots:
    void sltClose();
    void sltUpdate();
    void sltUpdateTabName();
    void sltFileChanged(const QString & path, bool & bYesToAll);
    void sltSelectionChanged();
    void sltCheckForMultitouch();
    void sltMarginClicked(int margin, int line, Qt::KeyboardModifiers state);
    void sltToggleRecordMacro( bool bStartRecord );
    void sltPlayMacro();

	virtual void show();

signals:
	void isShowing();
	void sigTabWithSelectedTextPressed();
	void sigShiftTabWithSelectedTextPressed();
    void sigCloseEditTab(CodeEdit * pMe);

protected:
    //! Re-implemented to handle the key presses for the widget.
    virtual void keyPressEvent(QKeyEvent *ke);
    virtual void focusInEvent(QFocusEvent *fe);
#if defined(Q_OS_ANDROID)
    virtual void mousePressEvent(QMouseEvent *e);
#endif

private:
    void setLanguageID( int iLanguageID );
    QString GetSelectedString() const;
    void ClearAllTextAreas();
    void MarkAllTextAreas(const QString & sMarkText);

    QWidget *               m_pMainWin;
    QsciAPIs *              m_pAPIs;
	QsciLexer *	            m_pLexer;
    QTimer *                m_pTimer;
    QFileSystemWatcher *	m_pFileSystemWatcher;	// not an owner !
    QTabWidget *            m_pTabWidget;           // not an owner !
    QWidget *               m_pTab;                 // not an owner !
    QString					m_sFileName;
    QString                 m_sSymbol;              // user symbol to show in tab header
	bool					m_bIsMainFile;
	bool					m_bWasLastKeyReturn;
    bool                    m_bIsWriting;   // signals that the file change is caused by a save of the file by this application
    bool                    m_bMarkAllOccurencesOfSelection;
    int                     m_iMouseClickCount;
    int						m_iLanguageID;
    int                     m_iIndicator;
    NavigationStack         m_aNavigationStack;
    QList<TextAreaStruct>   m_lstTextArea;
    QsciMacro               m_aCurrentMacro;
};

#endif //_CODEEDIT_H
