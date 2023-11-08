/**************************************************************************
 *
 *	project				 : VisiScript
 *
 *	copyright            : (C) 2002-20012 by Michael Neuroth
 *
 ***************************************************************************/
/*********************************************************************************
 *																			     *
 * This file is part of the VisiScript package (a frontend for script languages) *
 *																			     *
 * Copyright (C) 2002-2012 by Michael Neuroth.								     *
 *                                                                               *
 * This program is free software; you can redistribute it and/or modify		     *
 * it under the terms of the GNU General Public License as published by          *
 * the Free Software Foundation; either version 2 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 ********************************************************************************/

#ifndef VISISCRIPT_H
#define VISISCRIPT_H

#include <QList>
#include <QVector>
#include <QStringList>
#include <QMap>
#include <QPair>
#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QCloseEvent>
#include <QMainWindow>
#include <QReadWriteLock>
#include <QProcess>

#include "configure.h"
#include "extensions.h"

#include "codeedit.h"
#include "configuredlgimpl.h"
#include "BaseForm.h"//zj add

class QObject;
class QToolBar;
class QMenuBar;
class QMenu;
class QAction;
class QTabWidget;
class QLabel;
class QComboBox;
class QWidget;
class QDialog;
class QFileSystemWatcher;
class QScriptEngine;
    
class OutputAreaMultiLineEdit;
class FindDlgImpl;
class ReplaceDlgImpl;
class QScriptProxy;

// ************************************************************************

class TabContextMenu : public QMenu
{
    Q_OBJECT

public:
    TabContextMenu( CodeEdit * pCodeEdit );

public slots:
    void sltCopyFullPath();
    void sltOpenContainingFolder();

private:
    CodeEdit *          m_pCodeEdit;            // not an owner
};

// ************************************************************************

// for support of Qt 4.8
class TabWidget : public QTabWidget
{
public:
    TabWidget( QWidget * pParent )
        : QTabWidget( pParent )
    {
    }

    QTabBar * GetTabBar() const
    {
        return tabBar();
    }
};

// ************************************************************************

class VisiScript : public QMainWindow, public IVisiScript, public IAdvancedVisiScriptDebugger
{
    Q_OBJECT

    friend class QScriptProxy;

public:
    VisiScript( const QList<IVisiScriptExtension *> aExtensions,const QString & sFileName = "", QWidget* parent = 0, Qt::WindowFlags fl = 0 );
    VisiScript(const QString& sText,QWidget* parent, Qt::WindowFlags fl );
    QString m_sText; //脚本内容
    virtual ~VisiScript();

    bool DoImportFile( const QString & sFileName, int x = 0, int y = 0, int iLastEncodingUtf8 = 1 );
    bool ImportText(const QString& sTitleName,const QString& sText);
    bool HandleClose( bool bAskForSave );

    bool WasSaveAlreadyAsked() const;

    bool AddExtension( IVisiScriptExtension * pExtension );
    bool RemoveExtension( IVisiScriptExtension * pExtension );

    void UnregisterExtensions( const QList<IVisiScriptExtension *> aExtensions );

    bool CallProcessStdOutLineCallback( const QString & sLine );
    bool CallProcessStdErrLineCallback( const QString & sLine );


    // implement IAdvancedVisiScriptDebugger interface
    virtual bool Register( IDebugger * pExtension );
    virtual bool Unregister( IDebugger * pExtension );

    virtual int StartDebugging();

    virtual int GetLanguageIdOfCurrentScript() const;

    // implement IVisiScript interface
    virtual IAdvancedVisiScript * GetAdvancedInterface( int id );
    virtual QMainWindow * GetWindow();
    virtual QMenuBar * GetMenuBar();
    virtual OutputAreaMultiLineEdit * GetAndLockCurrentOutput();
    virtual void UnlockOutput( OutputAreaMultiLineEdit * pOutput );
    virtual CodeEdit * GetCurrentScript();
    virtual QProcess & GetCurrentScriptProcess();
    virtual QScriptEngine * GetCurrentScriptEngine();
    virtual void ScriptEnginePushLineToInputQueue( const QString & sValue );
    virtual void ScriptEngineClearInputQueue();
    virtual bool Register( IExtendQScript * pExtension );
    virtual bool Unregister( IExtendQScript * pExtension );
    virtual bool Register( IExecuteScript * pExtension );
    virtual bool Unregister( IExecuteScript * pExtension );
    virtual bool AddScriptInterpreterPath( const QString & sInterpreter );
    virtual QString SelectExistingDirectory( const QString & sCurrentDir );
    virtual QString GetFileTag() const;
    virtual QString GetLineTag() const;
    virtual void SetCurrentCursorPosition( int iLine, int iIndex );
    virtual bool IsCurrentScriptModified() const;
    virtual void SetCurrentScriptModified( bool value );
    virtual QString GetCurrentScriptText() const;
    virtual QString GetCurrentSearchText() const;
    virtual QString GetSelectedText() const;
    virtual void AddToCurrentOutput( const QString & sText );
    virtual void AddToOutput( OutputAreaMultiLineEdit * pOutput, const QString & sText );
    virtual void PushToLastSearchStringList( const QString & sText );
    virtual void PushToLastSearchFilesList( const QString & sText );
    virtual void PushToLastSearchDirList( const QString & sText );
    virtual QStringList & Property_LastSearchStringList();
    virtual QStringList & Property_LastSearchFilesList();
    virtual QStringList & Property_LastSearchDirList();
    virtual QString & Property_LastSearchText();
    virtual bool & Property_RegularExpr();
    virtual bool & Property_Wildcard();
    virtual bool & Property_CaseSensitive();
    virtual bool & Property_OnlyWholeWords();
    virtual bool & Property_SearchSubDirectories();
    virtual bool OnSave();//ZJ ADD
public slots:
    void sltFileNew();
    void sltFileOpen();
    void sltFileSave();
    void sltFileSaveAs();
    void sltFileOpenProject();
    void sltFileSaveProject();
    void sltFileSaveOutputAs();
	void sltFilePrint();
    void sltOutputPrint();
    void sltEditUndo();
    void sltEditRedo();
    void sltEditForward();
    void sltEditBackward();
    void sltEditCut();
    void sltEditCopy();
    void sltEditPaste();
	void sltEditSelectAll();
    void sltEditFindText();
    void sltEditFindNext();
    void sltEditReplaceText();
	void sltEditGotoLine();
    void sltToggleRecordMacro();
    void sltPlayMacro();
	void sltClearOutput();
    void sltKillOrClearOutput();
	void sltCommentSelection();
	void sltTabWithSelectedTextPressed();
	void sltShiftTabWithSelectedTextPressed();
    void sltCloseTab(CodeEdit * pEdit);
    void sltTabCloseRequested(int index);
	void sltUnCommentSelection();
    void sltAddToOutput( const QString & sText );
    void sltAddLineToOutput( const QString & sLine );
    void sltClearScript();
    void sltRun();
	void sltActRun();
    void sltShowInterpreterVersion();
    void sltKill();
    void sltSelectFont();
	void sltZoomIn();
	void sltZoomOut();
	void sltMaximizeOutput();
	void sltMaximizeScript();
    void sltSplitScriptOutput();
    void sltSwitchToTab();
    void sltSwitchToLastTab();
	void sltToggleWordWrap();
	void sltToggleOverwrite();
	void sltCodeFolding();
    void sltShowAbout();
    void sltShowExtensions();
    void sltShowInterpreterHelp();
    void sltShowVisiscriptHelp();
    void sltUpdateLastFilesMenu();
    void sltLastFilesMenuTriggered(QAction * pAction);
	void sltViewMarkActAsMain();
	void sltUpdateChilds();
	void sltOutputDoubleClicked( int iPara, int iPos ); 
	void sltPositionChanged( int iLine, int iPos );
    void sltFileChanged(const QString & path);
    void sltConfiguration();
    void sltShowScript();
    void sltFileAboutToShow();
    void sltEditAboutToShow();
    void sltRunAboutToShow();
    void sltViewAboutToShow();
    void sltLanguagesAboutToShow();
    void sltLanguagesTriggered( QAction * pAction );
    void sltEncodingsAboutToShow();
    void sltEncodingsTriggered( QAction * pAction );
	void sltCloseAllDocuments();
	void sltCloseActDocument();
    void sltClearActDocument();
    void sltCopyFullPath();
    void sltOpenContainingFolder();
    void sltCurrentTabPageChanged(int iIndex);
	void sltCheckReloadFiles();
    void sltOutputSplitterMoved( int pos, int index );

    void sltErrorScript(QProcess::ProcessError error);
    void sltFinishedScript(int exitValue, QProcess::ExitStatus status);
    void sltReadyReadStandardErrorScript();
    void sltReadyReadStandardOutputScript();
    void sltReadyRead();

    void sltCustomContextMenuRequested( const QPoint & pos );

protected:
	virtual void dragEnterEvent( QDragEnterEvent * pEvent );
	virtual void dropEvent( QDropEvent * pEvent );
	virtual void customEvent( QEvent * pEvent );
    virtual void closeEvent( QCloseEvent * pCloseEvent );
    virtual void keyPressEvent( QKeyEvent * pKeyEvent );
    virtual void keyReleaseEvent( QKeyEvent * pKeyEvent );
#if !defined(Q_OS_ANDROID)
    virtual void focusInEvent( QFocusEvent * pEvent );
#endif

    void UpdateCaption( const QString & sName=QString::null );
    void UpdateOutputArea();
    void CheckTempFile();
    void LoadInit();
    void SaveInit();
    bool CheckData( CodeEdit * pScript );
    bool CheckAllData( bool bAskForSave=true );
    QFont GetCurrentFont();
    void UpdateFont( bool bForAllDocuments );
    void UpdateFont( CodeEdit * pScript, OutputAreaMultiLineEdit * pOutput );
    void UpdateFont( CodeEdit * pScript, OutputAreaMultiLineEdit * pOutput, QFont & aFont );
    void UpdateFlags();
    void UpdateLanguageForCurrent( int iActLanguageID );
    void InsertInSelection( bool bDoInsert, const QString & sToken, const QString & sTokenStop );
    int GetHeightForOutput(double dPercent );
	void PushLastFileName( const QString & sName );
	bool ExistsLastFileName( const QString & sName );
    bool DoExportFile( CodeEdit * pScript, bool bShowError = false );
    void SetOutputSize(OutputAreaMultiLineEdit * pOutput, double dPercentSize );
    void SetOutputSizeForAll( double dPercentSize );
    int DoRun( bool bUseMainScript, bool bShowOnlyVersion = false, bool bDebug = false );
    CodeEdit * GetActMainScript( int * pIndexFound=0 ) const;
    CodeEdit * GetActScript() const;
	OutputAreaMultiLineEdit * GetActOutput() const;
    int GetIndexForScript( CodeEdit * pScript ) const;
    CodeEdit * GetScriptFor( int iIndex ) const;
    CodeEdit * GetScriptForFileName(const QString & strFileName, int * pIndex = 0 , bool bCompareOnlyFilename=false) const;
	OutputAreaMultiLineEdit * GetOutputFor( int iIndex ) const;
	QWidget * GetActDocumentTabPage() const;
	int GetDocumentCount() const;
    void AddDocument( const QString & sFileName );
    bool DoFindFirst(CodeEdit *pActScript, int iLine = -1, int iIndex = -1 );
    QString GetCurrentWordUnderCursor() const;
	QString GetStringForTab() const;
    int GetLanguageIdForAction( QAction * pAction );
    void SetAutoCompletionSource( QsciScintilla::AutoCompletionSource aSrc, int iThresh, const QString & sAPIFileName );
    void UpdateExternalProgs( QComboBox * pCombo );
    void AddActionToToolbarOrMenubar(QAction * pNewAction );
    void WriteTempFileIfNeeded( const QString & sScript, bool bIsNotQScript, bool bUtf8 );
    bool isScriptRunning() const;
    void EnableRunCommand( bool bEnable );
    void ProcessScriptFinished();
    QString GetExecutionTimeStrg() const;
    void CallFinishedScriptCallback();
    void CallAboutToStartScriptCallback( const QString & sScriptFileName );
    void ProcessReadIntent();
    void ConnectScriptProcessSignals();
    void DisconnectScriptProcessSignals();
    void ToggleRecordMacro( bool bStart );

	template <class T1>
	void ForAllDocumentScripts( T1 pFcn );
	template <class T1, class T2>
	void ForAllDocumentScripts( T1 aValue, T2 pFcn );
	template <class T1, class T2, class T3>
	void ForAllDocumentScripts( T1 aValue1, T2 aValue2, T3 pFcn );
	template <class T1>
	void ForAllDocumentOutput( T1 pFcn );
	template <class T1, class T2>
	void ForAllDocumentOutput( T1 aValue, T2 pFcn );

private:
	// ** data **
    QLabel *			    m_pStatusBarMsg;
    QToolBar *	   		    m_pButtonBar;
	QMenuBar *			    m_pMenuBar;
    TabWidget *             m_pTabWidget;

    QMenu * 		        m_pFile;
    QMenu * 		        m_pEdit;
    QMenu *                 m_pRun;
    QMenu * 		        m_pView;
    QMenu *                 m_pHelp;
    QMenu *                 m_pTab;
    QMenu *                 m_pLanguages;
    QMenu *     	        m_pEncodings;
    QMenu * 		        m_pLastOpendFiles;

    QVector<QAction *>      m_aMenuLanguages;
    QVector<QAction *>      m_aMenuLastFiles;
    QAction *               m_pUTF8;
    QAction *               m_pLatin1;
	QAction *			    m_pMaximizeScript;
	QAction *			    m_pMaximizeOutput;
    QAction *			    m_pUserSizeOutput;
    QAction *			    m_pSwitchTab;
	QAction *			    m_pTabMoveLeft;
	QAction *		        m_pTabMoveRight;
	QAction *			    m_pToggleWordWrap;
	QAction *			    m_pToggleOverwrite;
	QAction *			    m_pCodeFolding;
	QAction *			    m_pCopyAction;
	QAction *			    m_pCutAction;
	QAction *			    m_pPasteAction;
	QAction *			    m_pSelectAllAction;
	QAction *			    m_pFindInFilesAction;
	QAction *			    m_pCommentAction;
	QAction *			    m_pUnCommentAction;
	QAction *			    m_pMarkAsMain;
    QAction *               m_pClearOutput;
    QAction *               m_pKillOrClearOutput;
    QAction *               m_pRunScript;
    QAction *               m_pRunActScript;
    QAction *               m_pKillScript;
    QAction *               m_pRecordMacro;

    // data for configuration state
	QString				    m_sScriptCmd;
	QString				    m_sScriptArguments;
	QString		    		m_sFontName;
	QString			    	m_sFontStyle;
    QString                 m_sTempFileName;    // temp
    QString                 m_sLastCurrentDir;  // temp
	int					    m_iFontSize;
	int					    m_iActPageIndex;
	int					    m_iLastPageIndex;
	int					    m_iWordWrap;
	int					    m_iOverwrite;		// temp
	int					    m_iSpacesForTabs;
    int                     m_iShowCurrentLine;
	int					    m_iShowWhitespaces;
	int					    m_iShowLineNumbers;
	int					    m_iAutoIndent;
	int					    m_iCodeFolding;
	int					    m_iTabSpace;
	int					    m_iEolMode;
    int					    m_iShowStartupFileDialog;
    int                     m_iIconSize;
    double				    m_dOutputRatio;         // for the ratio of Script and output
    QString				    m_sLastDirectory;
    QString				    m_sLastFindText;		// temps for find/replace dialog
	QString				    m_sReplaceText;
    QString				    m_sAutoCompletionFileName;
    bool				    m_bAutoFileLoad;
    bool				    m_bIsReplace;
    bool				    m_bRegularExpr;
    bool				    m_bWildcard;
    bool				    m_bCaseSensitive;
    bool				    m_bOnlyWholeWords;
    bool				    m_bSearchBackward;
    bool                    m_bShowAllOccurrences;
    bool				    m_bSearchSubDirectories;
    bool				    m_bReplaceAll;
    bool				    m_bInteractiveModus;    // not used any more 30.1.2014
    bool				    m_bChangeActDir;
    bool                    m_bShowOutputArea;
    bool				    m_bAutoSave;
    bool				    m_bBraceMatching;
    bool				    m_bAutoClearOutput;
    bool				    m_bIsDefaultUtf8;
    bool                    m_bUseNewIcons;
    bool                    m_bUseSvgIcons;
    bool                    m_bUseTempFile;
    bool                    m_bSaveAlreadyAsked;
    bool                    m_bFirstLaunch;
    bool                    m_bCloseButtonInTabPage;//Tab页上的XX关闭按钮
    bool                    m_bMeasureExecutionTime;
    bool                    m_bPipeScriptToInterpreter;
    bool                    m_bUseToolbar;
    bool                    m_bUseUserInterpreter;
    bool                    m_bUseRootForFileDialog;
    bool                    m_bIsMoveAllKeyPressed;
    bool                    m_bIsDebugging;
    bool                    m_bIsMacroRecording;
    bool                    m_bUseFile;//ZJ ADD

    QString                 m_sCurrentMacro;

    int                     m_iCountLastOpendFiles;     // temp
	int					    m_iAutoCompletionThresh;

	QFileSystemWatcher *    m_pFileSystemWatcher;

	int					    m_iLastMainScriptIndex;
	int					    m_iLastActFileIndex;
	QList< QPair<int,int> >	m_aLastCursorPos;
    QList< QList<int> >	    m_aLastSpliterPos;
    QList<int>              m_aLastEncodingUtf8;

    QStringList             m_aActvatedTabList;
    QStringList             m_aLastFilesList;
    QStringList             m_aLastOpendFilesList;
    QStringList             m_aExternalProgsList;
    QStringList             m_aLastSearchStringList;
    QStringList             m_aLastReplaceStringList;
    QStringList             m_aLastSearchFilesList;			// for find in files
    QStringList             m_aLastSearchDirList;			// for find in files
    QStringList 		    m_aLastScriptArgumentsList;
	QStringList			    m_aModifiedFiles;				// list to relaod files after external change of contents

	QMap<QString,int>	    m_aExtensionToLanguageMap;

    QList<IVisiScriptExtension *>  m_aExtensions;
    UserScriptInterpreters         m_aUserInterpreters;

    QReadWriteLock          m_aModifiedFilesLock;
    int                     m_iCheckModifiedFilesCounter;
    int					    m_iLastPosX;
    int					    m_iLastPosY;
    int					    m_iLastPosDX;
    int					    m_iLastPosDY;
    qint64                  m_iElapsedTime;

	const QString	        c_strLineTag;
	const QString	        c_strFileTag;
	const QString	        c_strLineTagOrg;
	const QString		    c_strFileTagOrg;
    const QString           c_strUnknownFileName;

    // pointer to dialogs
    FindDlgImpl *		    m_pFindDlg;
    ReplaceDlgImpl *	    m_pReplaceDlg;

    QProcess                m_aScriptProcess;

    QScriptProxy *          m_pQScriptProxy;
    QScriptEngine *         m_pEngine;

    QList<IExtendQScript *> m_aExtendQScript;
    QList<IExecuteScript *> m_aExecuteScript;
    QList<IDebugger *>      m_aDebugger;

};

// ************************************************************************

class QScriptProxy : public QObject
{
    Q_OBJECT

public:
    QScriptProxy(VisiScript * pApp);
    ~QScriptProxy();

public slots:
    VisiScript * GetApp() const;
    QString GetActScriptText();
    QString GetActOutputText();    
    void AddToOutput( const QString & sText );

private:
    VisiScript * m_pApp;        // not an owner !
};

#endif // VISISCRIPT
