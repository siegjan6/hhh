/**************************************************************************
 *
 *	project				 : VisiScript
 *
 *	copyright            : (C) 2002-2012 by Michael Neuroth
 *
 ***************************************************************************/
/*********************************************************************************
 *																				 *
 * This file is part of the VisiScript package (a frontend for script languages) *
 *																				 *
 * Copyright (C) 2002-2012 by Michael Neuroth.								     *
 *                                                                               *
 * This program is free software; you can redistribute it and/or modify			 *
 * it under the terms of the GNU General Public License as published by    		 *
 * the Free Software Foundation; either version 2 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 ********************************************************************************/

#include <QMimeData>
#include <QUrl>
#include <QPair>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QAction>
#include <QPixmap>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QApplication>
#include <QSettings>
#include <QFileDialog>
#include <QSplitter>
#include <QPushButton>
#include <QFontDialog>
#include <QFont>
#include <QFontDatabase>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QScrollBar>
#include <QLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QClipboard>
#include <QTabWidget>
#include <QObject>
#include <QDir>
#include <QTextStream>
#include <QLabel>
#include <QEvent>
#include <QMap>
#include <QKeySequence>
#include <QFileSystemWatcher>
#include <QScriptEngine>
#include <QScriptValue>
#include <QDesktopServices>
#include <QElapsedTimer>
#include <QThread>

// old icons:
// created with bin2c.cpp (http://homepage.ntlworld.com/rik.griffin/bin2c.html)
#include "images/editdelete_png.h"
#include "images/play_png.h"
#include "images/new_png.h"
#include "images/forward_png.h"
#include "images/back_png.h"
#include "images/next_png.h"
#include "images/filesave_png.h"
#include "images/find_png.h"
#include "images/fileopen_png.h"

#include "inline_code.h"
#include "visiscript.h"
#include "outputarea.h"
#include "codeedit.h"
#include "configuredlgimpl.h"
#include "aboutdlgimpl.h"
#include "extensionsdialog.h"
#include "finddlgimpl.h"
#include "replacedlgimpl.h"
#include "selectfromlistdlg.h"

#ifdef _WITH_MOBILE_FILEDIALOG
#include "mobilefileselection.h"
#endif

//*************************************************************************

#define VISISCRIPT_VERSION_STR  "0.5.1"

#if defined( Q_OS_ANDROID )
#define _TEMP_SCRIPT_FILE		"/data/data/de.mneuroth.visiscript/files/_tst.xyz"
#else
#define _TEMP_SCRIPT_FILE		"_tsf.xyz"
#endif
#define _TEMP_ERR_FILE			"_err.txt"
#define _TEMP_ERR_FILE_PIPE		"2>_err.txt"
#define _TEMP_OUT_FILE			"_out.txt"
#define _TEMP_OUT_FILE_PIPE		"1>_out.txt"
#define _VISISCRIPT_NAME		"visiscript"

#define _SPLITTER_NAME			"pSplitter"
#define _OUTPUT_NAME			"pOutput"
#define _SCRIPT_NAME			"pScript"

#define INPUT_QUEUE             "inputQueue"

#define MAX_BUFFER_LEN          512

#define MAX_LAST_FILES			10
#define MAX_EXTERNAL_PROGRAMS	10
#define MAX_LAST_SEARCH_STRINGS	20

#if defined( Q_OS_ANDROID )
const int c_iDefaultIconSize    = 32;
const int c_iSmall              = 12;
const int c_iMedium             = 16;
const int c_iLarge              = 20;
const int c_iMaxOutputRatio     = 100;
const int c_iMinOutputRatio     = 0;
#else
const int c_iDefaultIconSize    = 20;
const int c_iSmall              = 8;
const int c_iMedium             = 14;
const int c_iLarge              = 12;
const int c_iMaxOutputRatio     = 70;
const int c_iMinOutputRatio     = 15;
#endif

const int g_iStatusBarTimeoutInMs = 3000;

const char * g_sVisiScriptExtensionsPath = "/data/data/de.mneuroth.visiscriptextensions/files";
const char * g_sMetaReplace = "<extensions>";

const char * c_sScriptCmdKey 			= "ScriptCmd";
const char * c_sScriptArgumentsKey		= "ScriptArguments";
const char * c_sFontNameKey 			= "FontName";
const char * c_sFontStyleKey 			= "FontStyle";
const char * c_sFontSizeKey 			= "FontSize";
const char * c_sWordWrapKey				= "WordWrap";
const char * c_sTabSpaceKey				= "TabSpace";
const char * c_sOverwriteKey			= "Overwrite";
const char * c_sOutputRatio				= "OutputRatio";
const char * c_sLastDirectoryKey		= "LastDirectory";
const char * c_sInteractiveModusKey		= "InteractiveModus";
const char * c_sChangeActDirKey			= "ChangeActDir";
const char * c_sShowOutputArea          = "ShowOutputArea";
const char * c_sAutoSaveKey				= "AutoSave";
const char * c_sPipeScriptToInterpreter = "PipeScriptToInterpreter";
const char * c_sMeasureExecutionTimeKey = "MeasureExecutionTime";
const char * c_sShowStaruptFileDialog	= "ShowFileDialog";
const char * c_sBraceMatchingKey		= "BraceMatching";
const char * c_sAutoClearOutputKey		= "AutoClearOutput";
const char * c_sAutoCompletionFileKey	= "AutoCompletionFile";
const char * c_sAutoCompletionThresh	= "AutoCompletionThresh";
const char * c_sSpacesForTabs			= "SpacesForTabs";
const char * c_sShowCurrentLine			= "ShowCurrentLine";
const char * c_sShowWhitespaces			= "ShowWhitespaces";
const char * c_sShowLineNumbers			= "ShowLineNumbers";
const char * c_sAutoIndent				= "AutoIndent";
const char * c_sCodeFolding				= "CodeFolding";
const char * c_sEolMode					= "EolMode";
const char * c_sLastFileName			= "LastFileName";
const char * c_sLastOpendFileName		= "LastOpendFileName";
const char * c_sLastCursorPosX			= "LastCursorPosX";
const char * c_sLastActiveFileIndex		= "LastActFileIdx";
const char * c_sLastCursorPosY			= "LastCursorPosY";
const char * c_sLastEncodingUtf8        = "LastEncodingUtf8";
const char * c_sLastMainScriptIndex		= "LastMainScriptIdx";
const char * c_sLastSizerPos			= "LastSizerPos";
const char * c_sExternalProg			= "ExternalProg";
const char * c_sExtensionMap			= "ExtMap";
const char * c_sUserInterpreter         = "UserInterpreter";
const char * c_sPath                    = "Path";
const char * c_sArguments               = "Arguments";
const char * c_sExtension               = "Extension";
const char * c_sLastSearchString		= "LastSearchString";
const char * c_sLastReplaceString		= "LastReplaceString";
const char * c_sLastSearchFiles			= "LastSearchFiles";
const char * c_sLastSearchDirs			= "LastSearchDirs";
const char * c_sLastScriptArguments		= "LastScriptArguments";
const char * c_sSetUtf8					= "Utf8";
const char * c_sUseNewIcons             = "UseNewIcons";
const char * c_sUseToolbar              = "UseToolbar";
const char * c_sUseUserInterpreter      = "UseUserInterpreter";
const char * c_sCloseButtonInTabPage    = "CloseButtonInTabPage";
const char * c_sIconSize                = "IconSize";
const char * c_sRegExpr					= "RegExpr";
const char * c_sUseWildcard				= "UseWildcard";
const char * c_sCaseSensitive			= "CaseSensitive";
const char * c_sWholeWord				= "WholeWord";
const char * c_sSearchBackward			= "SearchBackward";
const char * c_sShowAllOccurrences      = "ShowAllOccurrences";
const char * c_sSearchSubDirs			= "SearchSubDirs";
const char * c_sReplaceAll				= "ReplaceAll";
const char * c_sAutoFileLoad			= "AutoFileLoad";
const char * c_sLastPosX				= "LastPosX";
const char * c_sLastPosY				= "LastPosY";
const char * c_sLastPosDX				= "LastPosDX";
const char * c_sLastPosDY				= "LastPosDY";
const char * c_sFirstLaunch				= "FirstLaunch";
const char * c_sUseRootForSDCard        = "UseRootForSDCard";

//*************************************************************************

static QElapsedTimer g_aScriptTimer;

//*************************************************************************

static QPixmap LoadFromMem( const QString & sName, const QString & sSvgName, uchar * pImgBuffer, uint iLen, bool bUseNewIcons, bool bUseSvgIcons )
{
    QPixmap aImg;
    if( bUseNewIcons || pImgBuffer==0 )
    {
        QPixmap aPixmap((bUseNewIcons && bUseSvgIcons) ? sSvgName : sName);
        aImg = aPixmap;
    }
    else
    {
        /*bool bOk =*/ aImg.loadFromData( pImgBuffer, iLen );
    }
    return aImg;
}

/*
static QPixmap LoadFromMem( const QString & sName, uchar * pImgBuffer, uint iLen, bool bUseNewIcons )
{
    return LoadFromMem( sName, "", pImgBuffer, iLen, bUseNewIcons, false );
}
*/

struct ScriptInfo
{
    QString     sCmd;
    QString     sVersion;
    QStringList lstArguments;
    bool        bNeedPipeWriting;
};

static ScriptInfo GetScriptCommandFromLanguageID( int iLanguageID, const QList<IVisiScriptExtension *> & lstExtensions )
{
    QString sCmd;
    QString sVersion = "-v";
    QStringList lstArguments;
    bool bNeedPipeWriting = false;

    foreach( IVisiScriptExtension * pExt, lstExtensions )
    {
        QList<int> lstIds = pExt->getSupportedLanguageIds();
        if( lstIds.contains( iLanguageID ) )
        {
            sCmd = pExt->getInterpreterPath();
            sVersion = pExt->getInterpreterVersionOption();
            lstArguments = pExt->getDefaultInterpreterArguments();
            bNeedPipeWriting = pExt->needsPipeWriting();
            break;
        }
    }

    // if no extension was found for this language id --> use default values
    if( sCmd.length()==0 )
    {
        switch( iLanguageID )
        {
            case LANG_JAVASCRIPT:
                sCmd = _QSCRIPT;
                break;
            case LANG_PYTHON:
                sCmd = "python";
                sVersion = "-V";
                break;
            case LANG_RUBY:
                sCmd = "ruby";
                break;
            case LANG_PERL:
                sCmd = "perl";
                break;
            case LANG_LUA:
                sCmd = "lua";
                break;
            case LANG_PASCAL:
                sCmd = "pascal";
                break;
            case LANG_FORTRAN:
                sCmd = "f77";
                break;
            case LANG_TCL:
                sCmd = "tclsh";
                break;
            case LANG_SCHEME:
                sCmd = "scheme";
                break;
            case LANG_LISP:
                sCmd = "lisp";
                break;
            case LANG_HASKELL:
                sCmd = "hugs";
                break;
            case LANG_MIMDCRIPT:
            case LANG_CPP:
                sCmd = "minscript";
                break;
            default:
                sCmd = "";
        }
    }

    ScriptInfo ret;
    ret.sCmd = sCmd;
    ret.sVersion = sVersion;
    ret.bNeedPipeWriting = bNeedPipeWriting;
    ret.lstArguments = lstArguments;
    return ret;
}

QStringList GetDefaultFileExtensions()
{
    QStringList aFileExtensions;
    aFileExtensions.append("c");
    aFileExtensions.append("cc");
    aFileExtensions.append("cxx");
    aFileExtensions.append("cpp");
    aFileExtensions.append("h");
    aFileExtensions.append("hpp");
    aFileExtensions.append("hxx");
    aFileExtensions.append("i");
    aFileExtensions.append("xmp");
    aFileExtensions.append("ic");
    aFileExtensions.append("min");
    aFileExtensions.append("d");
    aFileExtensions.append("java");
    aFileExtensions.append("js");
    aFileExtensions.append("qs");
    aFileExtensions.append("cs");
    aFileExtensions.append("py");
    aFileExtensions.append("pyw");
    aFileExtensions.append("rb");
    aFileExtensions.append("pl");
    aFileExtensions.append("php");
    aFileExtensions.append("lua");
    aFileExtensions.append("cs");
    aFileExtensions.append("el");
    aFileExtensions.append("lisp");
    aFileExtensions.append("scm");
    aFileExtensions.append("hs");
    aFileExtensions.append("tcl");
    aFileExtensions.append("pas");
    aFileExtensions.append("for");
    aFileExtensions.append("idl");
    aFileExtensions.append("sql");
    aFileExtensions.append("html");
    aFileExtensions.append("xml");
    aFileExtensions.append("css");
    aFileExtensions.append("bat");
    aFileExtensions.append("tex");
    aFileExtensions.append("txt");
    aFileExtensions.append("diff");
    aFileExtensions.append("po");
    aFileExtensions.append("yaml");
    aFileExtensions.append("properties");
    aFileExtensions.append("cmake");
    aFileExtensions.append("cir");
    aFileExtensions.append("ps");
    aFileExtensions.append("litcoffee");
    aFileExtensions.append("coffee");
    return aFileExtensions;
}

static int GetLanguageIDFromExtension( const QString & sExt, const QMap<QString,int> & aMap )
{
    return LANG_JAVASCRIPT;// ZJ ADD
    QString s = sExt.toLower();

    // Map of lexer to language id will only grow ! we do not need to resize,
    // because there exists only a limited number of extensions.
    // Maybe a future feature would be to delete/reset the map to be empty?
    QMap<QString,int>::ConstIterator aIter = aMap.find(s);
    if( aIter!=aMap.end() )
    {
        return aMap[s];
    }

    if( s=="cpp" || s=="cxx" || s=="c" || s=="cc" || s=="h" || s=="hpp" || s=="hxx" || s=="i" || s=="xpm" )
    {
        return LANG_CPP;
    }
    else if( s=="ic" || s=="min" )
    {
        return LANG_MIMDCRIPT;
    }
    else if( s=="d" )
    {
        return LANG_D;
    }
    else if( s=="java" )
    {
        return LANG_JAVA;
    }
    else if( s=="js" || s=="qs" )
    {
        return LANG_JAVASCRIPT;
    }
    else if( s=="cs" )
    {
        return LANG_CSHARP;
    }
    else if( s=="py" || s=="pyw" )
    {
        return LANG_PYTHON;
    }
    else if( s=="rb" )
    {
        return LANG_RUBY;
    }
    else if( s=="pl" )
    {
        return LANG_PERL;
    }
    else if( s=="php" )
    {
        return LANG_PHP;
    }
    else if( s=="lua" )
    {
        return LANG_LUA;
    }
    else if( s=="cl" || s=="el" || s=="lisp" )
    {
        return LANG_LISP;
    }
    else if( s=="scm" )
    {
        return LANG_SCHEME;
    }
    else if( s=="hs" )
    {
        return LANG_HASKELL;
    }
    else if( s=="tcl" )
    {
        return LANG_TCL;
    }
    else if( s=="pas" )
    {
        return LANG_PASCAL;
    }
    else if( s=="for" )
    {
        return LANG_FORTRAN;
    }
    else if( s=="idl" )
    {
        return LANG_IDL;
    }
    else if( s=="sql" )
    {
        return LANG_SQL;
    }
    else if( s=="html" || s=="htm" || s=="dtd" || s=="ui" || s=="properties" || s=="project" )
    {
        return LANG_HTML;
    }
    else if( s=="xml" || s=="xaml" || s=="sln" || s=="csproj" || s=="cxproj" )
    {
        return LANG_XML;
    }
    else if( s=="css" )
    {
        return LANG_CSS;
    }
    else if( s=="tex" )
    {
        return LANG_TEX;
    }
    else if( s=="bat" || s=="txt" || s=="log" )
    {
        return LANG_BATCH;
    }
    else if( s=="sh" )
    {
        return LANG_BASH;
    }
    else if( s=="pro" || s=="mak" )
    {
        return LANG_MAKEFILE;
    }
    else if( s=="vhd" || s=="vhdl" )
    {
        return LANG_VHDL;
    }
    else if( s=="m" )
    {
        return LANG_MATLAB;
    }
    else if( s=="m" )
    {
        return LANG_OCTAVE;
    }
    else if( s=="diff" )
    {
        return LANG_DIFF;
    }
    else if( s=="po" )
    {
        return LANG_PO;
    }
    else if( s=="yaml" )
    {
        return LANG_YAML;
    }
    else if( s=="properties" )
    {
        return LANG_PROPERTIES;
    }
    else if( s=="cmake" )
    {
        return LANG_CMAKE;
    }
    else if( s=="cir" )
    {
        return LANG_SPICE;
    }
    else if( s=="ps" )
    {
        return LANG_POSTSCRIPT;
    }
    else if( s=="litcoffee" || s=="coffee" )
    {
        return LANG_COFFEESCRIPT;
    }

    return LANG_NONE;
}

static QPair<QString,QString> GetCommentTokenForLine( int iLanguageID )
{
    QString sStartComment;
    QString sStopComment;

    switch( iLanguageID )
    {
        case LANG_NONE:
            break;
        case LANG_MIMDCRIPT:
        case LANG_CPP:
        case LANG_CSHARP:
        case LANG_JAVA:
        case LANG_JAVASCRIPT:
        case LANG_COFFEESCRIPT:
        case LANG_D:
        case LANG_PHP:
        case LANG_IDL:
        case LANG_PASCAL:       // valid for freepascal
            sStartComment = "//";
            break;
        case LANG_MAKEFILE:
        case LANG_CMAKE:
        case LANG_YAML:
        case LANG_PO:
        case LANG_PROPERTIES:
        case LANG_PERL:
        case LANG_PYTHON:
        case LANG_TCL:
        case LANG_BASH:
        case LANG_RUBY:
            sStartComment = "#";
            break;
        case LANG_SPICE:
            sStartComment = "*";
            break;
        case LANG_FORTRAN:
            sStartComment = "!";
            break;
        case LANG_BATCH:
            sStartComment = "REM ";
            break;
        case LANG_SCHEME:
        case LANG_LISP:
            sStartComment = ";;";
            break;
        case LANG_XML:
        case LANG_HTML:
            sStartComment = "<!--";
            sStopComment = "-->";
            break;
        case LANG_LUA:
        case LANG_HASKELL:
        case LANG_VHDL:
        case LANG_SQL:
            sStartComment = "--";
            break;
        case LANG_POSTSCRIPT:
        case LANG_TEX:
            sStartComment = "%";
            break;
        case LANG_CSS:
            sStartComment = "/*";
            sStopComment = "*/";
            break;
        default:
            sStartComment = "";
    }

    return QPair<QString,QString>(sStartComment,sStopComment);
}

static QString StripPath( const QString & sFileName )
{
    QString sRet = sFileName;
    sRet.replace( QString("\\"), QString("/") );
    sRet.replace( QString("\""), QString("") );
    return sRet;
}

static QString ToMetaPath( const QString & sPath )
{
    QString ret = sPath;
#if defined(Q_OS_ANDROID)
    if( ret.startsWith( g_sVisiScriptExtensionsPath ) )
    {
        ret.replace( g_sVisiScriptExtensionsPath, g_sMetaReplace );
    }
#endif
    return ret;
}

static QString FromMetaPath( const QString & sPath )
{
    QString ret = sPath;
#if defined(Q_OS_ANDROID)
    if( ret.startsWith( g_sMetaReplace ) )
    {
        ret.replace( g_sMetaReplace, g_sVisiScriptExtensionsPath );
    }
#endif
    return ret;
}

static bool ExistsInList( const QString & sName, QList<QString> & aList )
{
    for( int i=0; i<aList.count(); i++ )
    {
        if( aList.at( i ) == sName )
        {
            return true;
        }
    }

    return false;
}

static bool PushToList( const QString & sName, QList<QString> & aList, int MAX=0 )
{
    if( !ExistsInList( sName, aList ) )
    {
        aList.append( sName );

        if( MAX>0 && aList.count()>MAX )
        {
            aList.removeFirst();
        }

        return true;
    }
    return false;   // item was already in list
}

// make an object name from the label --> remove & for short key
static QString MakeObjectName( const QString & sLabel )
{
    QString ret = sLabel;
    ret.replace('&',"");
    return ret.toLower();
}

static QAction * CreateNewAction( const QString & sLabel, const QPixmap & aIcon, QKeySequence aKeySeq, QObject * pParent, const QString & sActionName = "" )
{
    QAction * pAction = new QAction( aIcon, sLabel, pParent );
    pAction->setShortcut(aKeySeq);
    pAction->setObjectName(sActionName.isEmpty() ? MakeObjectName(sLabel) : sActionName);
    pAction->setIconText("");       // makes the distance between the icons under linux small
    return pAction;
}

static QAction * CreateNewAction( const QString & sLabel, QKeySequence aKeySeq, QObject * pParent, const QString & sActionName = "" )
{
    QAction * pAction = new QAction( sLabel, pParent );
    pAction->setShortcut(aKeySeq);
    pAction->setObjectName(MakeObjectName(sActionName.isEmpty() ? MakeObjectName(sLabel) : sActionName));
    return pAction;
}

QAction * AddCheckableMenuItem( QMenu * pMenu, const QString & sLabel, const QString & sActionName = "" )
{
    QAction * pAction = pMenu->addAction( sLabel );
    pAction->setCheckable(true);
    pAction->setObjectName(MakeObjectName(sActionName.isEmpty() ? MakeObjectName(sLabel) : sActionName));
    return pAction;
}

#ifdef _WITH_MOBILE_FILEDIALOG

#if defined(Q_OS_ANDROID)
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtAndroidExtras/QAndroidJniObject>
#endif

static QString GetIntentData()
{
    // Warning: support for opening from intent data is not supported very good, works if visiscript is not running but not if visiscript is running !
    // <acitvity ... android:launchMode="singleTask"> see: http://developer.android.com/guide/topics/manifest/activity-element.html#lmode
    // see: http://qt-project.org/forums/viewthread/38703
    // and: https://groups.google.com/forum/#!searchin/android-qt/intent/android-qt/xxpKLnGftHc/5QBisi3c7iQJ
    QAndroidJniObject activity =  QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
    QAndroidJniObject intent = activity.callObjectMethod("getIntent","()Landroid/content/Intent;");
    QString pathToFile = intent.callObjectMethod("getDataString", "()Ljava/lang/String;").toString();
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck())
    {
        // Handle exception here.
        env->ExceptionClear();
        pathToFile = "Error: exception getting intent data !";
    }
    return pathToFile;
}

static QString GetSDCardPath( bool bUseRoot )
{
#if defined(Q_OS_ANDROID)
    if( bUseRoot )
    {
        return SDCARD_DIRECTORY;
    }
    else
    {
        // see: https://qt-project.org/forums/viewthread/35519
        QAndroidJniObject aMediaDir = QAndroidJniObject::callStaticObjectMethod("android/os/Environment", "getExternalStorageDirectory", "()Ljava/io/File;");
            // maybe better: getExternalFilesDir()
        QAndroidJniObject aMediaPath = aMediaDir.callObjectMethod( "getAbsolutePath", "()Ljava/lang/String;" );
        QString sSdCardAbsPath = aMediaPath.toString();
        QAndroidJniEnvironment env;
        if (env->ExceptionCheck())
        {
            // Handle exception here.
            env->ExceptionClear();
            sSdCardAbsPath = SDCARD_DIRECTORY;
        }
        // other option may be: getenv("EXTERNAL_STORAGE")
        // remark: files of visiscriptextensions app are visible in visiscript !
        //return "/data/data/de.mneuroth.visiscriptextensions/files";
        return sSdCardAbsPath;
    }
#else
    return SDCARD_DIRECTORY;
#endif
}

#endif

QString OpenFileName(QWidget * parent, const QString & sTitle, const QString & sDir, const QString & sFilter = "", bool bUseRoot = false)
{
    Q_UNUSED(sFilter);
    Q_UNUSED(bUseRoot);
#ifdef _WITH_MOBILE_FILEDIALOG
    Q_UNUSED(sTitle);
    MobileFileSelection aDlg(parent,sDir,FILES_DIR,GetSDCardPath(bUseRoot),false,false);
    aDlg.showMaximized();
    // update labels after show
    aDlg.update();
    if( aDlg.exec()==QDialog::Accepted )
    {
        return aDlg.getFullFileName();
    }
    return QString("");
#else
    return QFileDialog::getOpenFileName(parent,sTitle,sDir);
#endif
}

static QString SaveFileName(QWidget * parent, const QString & sTitle, const QString & sDir, const QString & sFilter = "", bool bUseRoot = false)
{
    Q_UNUSED(sFilter);
    Q_UNUSED(bUseRoot);
#ifdef _WITH_MOBILE_FILEDIALOG
    Q_UNUSED(sTitle);
    MobileFileSelection aDlg(parent,sDir,FILES_DIR,GetSDCardPath(bUseRoot),false,false);
    aDlg.prepareForFileSaveAs();
    aDlg.showMaximized();
    // update labels after show
    aDlg.update();
    if( aDlg.exec()==QDialog::Accepted )
    {
        return aDlg.getFullFileName();
    }
    return QString("");
#else
    return QFileDialog::getSaveFileName(parent,sTitle,sDir);
#endif
}

static bool IsValidSearchChar(const QChar & ch)
{
    return ch.isNumber() /*|| ch.isSymbol()*/ || ch.isLetter() || ch.toLatin1()=='_';
}

static QString GetWordForPosition(const QString & sLine, int iActPos)
{
    int iStart, iStop;

    for(iStart=iActPos; iStart>=0; iStart--)
    {
        if( !IsValidSearchChar( sLine[iStart] ) )
        {
            if( iStart<iActPos )
            {
                iStart += 1;	// we went one character to far !
            }
            break;
        }
    }
    for(iStop=iActPos; iStop<sLine.size(); iStop++)
    {
        if( !IsValidSearchChar( sLine[iStop] ) )
        {
            break;
        }
    }
    return sLine.mid(iStart,iStop-iStart);
}

static QString GetDirectoryFromFilePath( const QString & sFileName )
{
    QFileInfo aFileInfo(sFileName);
    return aFileInfo.dir().absolutePath();
}

static void SetComboBoxToText( QComboBox * pCombo, const QString & sText )
{
    for( int i=0; i<pCombo->count(); i++ )
    {
        if( pCombo->itemText( i ) == sText )
        {
            pCombo->setCurrentIndex( i );
            break;
        }
    }
}

static bool IsTextFileDrag( const QDropEvent * pEvent, QString & sFileNameOut )
{
    const QMimeData * pMimeData = pEvent->mimeData();

    if( pMimeData->hasUrls() )
    {
        QList<QUrl> aList = pMimeData->urls();
        if( aList.count() == 1 )
        {
            sFileNameOut = aList.at( 0 ).toLocalFile();
            return true;
        }
    }

    return false;
}

// ************************************************************************

static int ConvertToMax( int i )
{
    if( i<0 )
    {
        return (1 << 16)-1;
    }
    return i;
}

static int FindMin( int i1, int i2, int i3, int i4, int i5 )
{
    return std::min(ConvertToMax(i1),std::min(ConvertToMax(i2),std::min(ConvertToMax(i3),std::min(ConvertToMax(i4),ConvertToMax(i5)))));;
}

static int FindSeparatorChar( const QString & sLine, int iFound, bool bIgnorePathChars=false )
{
    // try some line-number stop characters
    int i1 = sLine.indexOf(",",iFound);
    int i2 = sLine.indexOf(";",iFound);
    int i3 = bIgnorePathChars ? -1 : sLine.indexOf(".",iFound);
    int i4 = bIgnorePathChars ? -1 : sLine.indexOf(":",iFound);
    int i5 = bIgnorePathChars ? -1 : sLine.indexOf(" ",iFound);
    int i = FindMin(i1,i2,i3,i4,i5);
    return i;
}

static void OpenContainingFolder( const QString & sFullFileName )
{
    QString sFileNamePath = QFileInfo( sFullFileName ).absolutePath();
    QString sCommand;
    QStringList args;
#ifdef Q_OS_MAC
    //sCommand = QString("open -R %1").arg(sFileNamePath);
    sCommand = "open";
    args << "-R";
    args << QDir::toNativeSeparators(sFileNamePath);
#endif
#ifdef Q_OS_WIN
    //sCommand = QString("cmd.exe /c \"explorer /root,%1\"").arg(sFileNamePath.replace('/','\\'));
    //sCommand = "cmd.exe";
    //args << "/c";
    //args << QString("\"explorer /root,%1\"").arg(sFileNamePath.replace('/','\\'));
    sCommand = "explorer";
    args << QDir::toNativeSeparators(sFileNamePath);    // AddStringKommasIfNeededAndConvertSeparators
#endif
#ifdef Q_OS_LINUX
    // TODO test for linux... see: https://qt.gitorious.org/qt-creator/alexander-anokhins-qt-creator/source/77761b37c3bc193b753583dbb8ec123e53b26ab3:src/libs/utils/unixutils.cpp
    // see: Utils::UnixUtils::fileBrowser
    // default file manager in ubuntu 12.04 --> nautilus oder gnome-open
    //sCommand = QString("start nautilus %1").arg(sFileNamePath);
    sCommand = "nautilus";      // xdg-open ?
    args << QDir::toNativeSeparators(sFileNamePath);
#endif

    // see also:
    // http://stackoverflow.com/questions/3569749/qt-open-default-file-explorer-on-nix
    // http://stackoverflow.com/questions/3490336/how-to-reveal-in-finder-or-show-in-explorer-with-qt

    //system(qPrintable(sCommand));

    QProcess::startDetached( sCommand, args );
}

// split a string into substrings but processing c-strings correctly:
// example:
//  input:  abc "text with spaces" option
//  output: ["abc","text with spaces","option"]
static QStringList SplitString( const QString & s )
{
    QStringList lstRet;
    QString temp;
    bool bItemFound = false;
    bool bFoundString = false;
    for( int i=0; i<s.length(); i++ )
    {
        const QChar & ch = s[i];
        if( ch=='"' )
        {
            bFoundString = !bFoundString;
        }
        else if( ch=='\\' )
        {
            if( i+1<s.length() && s[i+1]=='"' )
            {
                i++;
                temp += s[i];
            }
        }
        else if( ch.isSpace() )
        {
            if( bFoundString )
            {
                temp += ch;
            }
            else
            {
                if( bItemFound )
                {
                    lstRet.append(temp);
                    temp = "";
                }
                bItemFound = false;
            }
        }
        else if( !bItemFound )
        {
            bItemFound = true;
            temp += ch;
        }
        else
        {
            temp += ch;
        }
    }
    if( temp.length()>0 )
    {
       lstRet.append(temp);
    }
    return lstRet;
}

// ************************************************************************
// functions for QScript environment
// ************************************************************************

// set the default size and position of the visiscript application window
static QScriptValue qscript_setdefaultsize(QScriptContext * pContext, QScriptEngine * pEngine)
{
    Q_UNUSED(pContext);
    QScriptProxy * pScriptProxy = (QScriptProxy *)pEngine->globalObject().property("app").toQObject();

    // remove frame and title size from window dimensions
    VisiScript * pApp = pScriptProxy->GetApp();
    int deltaWidth = pApp->frameGeometry().width()-pApp->geometry().width();
    int deltaHeight = pApp->frameGeometry().height()-pApp->geometry().height();

    int geometry[4] = { deltaWidth, deltaHeight, 800-deltaWidth, 600-deltaHeight };
    for( int i=0; i<pContext->argumentCount(); i++ )
    {
        geometry[i] = pContext->argument(i).toInt32();
    }
    pApp->setGeometry(geometry[0],geometry[1],geometry[2],geometry[3]);
    return QScriptValue();
}

// for support of Qt 4.8
class HelperThreadClass : public QThread
{
public:
    static void _msleep( unsigned long msecs );
};

void HelperThreadClass::_msleep( unsigned long msecs )
{
    QThread::msleep( msecs );
}

// sleep a given time in ms
static QScriptValue qscript_sleep(QScriptContext * pContext, QScriptEngine * pEngine)
{
    Q_UNUSED(pEngine);
    if( pContext->argumentCount()>0 )
    {
        uint uiStart = g_aScriptTimer.elapsed();
        uint uiTimeInMs = pContext->argument(0).toUInt32();
        while( g_aScriptTimer.elapsed()-uiStart < uiTimeInMs )
        {
            QApplication::processEvents();
            HelperThreadClass::_msleep(1);
        }
    }
    QApplication::processEvents();
    return QScriptValue();
}

// return the time since start of the script
static QScriptValue qscript_clockms(QScriptContext * pContext, QScriptEngine * pEngine)
{
    Q_UNUSED(pContext);
    Q_UNUSED(pEngine);
    return QScriptValue( (uint)g_aScriptTimer.elapsed() );
}

// function to read a text(line) in the output area (needed for input() function in qscript)
static QScriptValue qscript_input(QScriptContext * pContext, QScriptEngine * pEngine)
{
    Q_UNUSED(pContext);
    QVariant aInputQueue = pEngine->property(INPUT_QUEUE);
    QStringList lstQScriptInputQueue = aInputQueue.toStringList();
    QString sRet;
    if( !lstQScriptInputQueue.isEmpty() )
    {
        sRet = lstQScriptInputQueue.first();
        lstQScriptInputQueue.pop_front();
        pEngine->setProperty( INPUT_QUEUE, QVariant( lstQScriptInputQueue ) );
    }
    QApplication::processEvents();
    return QScriptValue(sRet);
}

// function to show text in the output area (needed for print() function in qscript)
static QScriptValue qscript_print(QScriptContext * pContext, QScriptEngine * pEngine)
{
    QString sOutput;
    for( int i=0; i<pContext->argumentCount(); i++ )
    {
        if( sOutput.size()>0 )
        {
            sOutput += " ";
        }
        sOutput += pContext->argument(i).toString();
    }
    if( sOutput.size()>0 )
    {
        QScriptProxy * pScriptProxy = (QScriptProxy *)pEngine->globalObject().property("app").toQObject();

        if( !pScriptProxy->GetApp()->CallProcessStdOutLineCallback( sOutput ) )
        {
            pScriptProxy->AddToOutput( sOutput );
        }
    }
    QApplication::processEvents();
    return QScriptValue();
}

// show a messagebox from a qscript
static QScriptValue qscript_messagebox(QScriptContext * pContext, QScriptEngine * pEngine)
{
    Q_UNUSED(pEngine);
    bool ok = true;
    if( pContext->argumentCount()==2 )
    {
        QString sTitle = pContext->argument(0).toString();
        QString sMessage = pContext->argument(1).toString();

        QMessageBox::information(0,sTitle,sMessage);
    }
    else
    {
        ok = false;
    }
    QApplication::processEvents();
    return QScriptValue(ok);
}

// ************************************************************************

QScriptProxy::QScriptProxy(VisiScript * pApp)
{
    m_pApp = pApp;
}

QScriptProxy::~QScriptProxy()
{
}

VisiScript * QScriptProxy::GetApp() const
{
    return m_pApp;
}

QString QScriptProxy::GetActScriptText()
{
    return m_pApp->GetActScript()->text();
}

QString QScriptProxy::GetActOutputText()
{
    return m_pApp->GetActOutput()->toPlainText();
}

void QScriptProxy::AddToOutput( const QString & sText )
{
    m_pApp->sltAddLineToOutput( sText );
}

// ************************************************************************

TabContextMenu::TabContextMenu( CodeEdit * pCodeEdit )
    : m_pCodeEdit( pCodeEdit )
{
//    QAction * pAction = addAction(tr("Copy full path"));
//    connect(pAction,SIGNAL(triggered()),this,SLOT(sltCopyFullPath()));
//    pAction = addAction(tr("Open containing folder"));
//    connect(pAction,SIGNAL(triggered()),this,SLOT(sltOpenContainingFolder()));
//    addSeparator();
//    pAction = addAction(tr("Close tab"));
//    connect(pAction,SIGNAL(triggered()),m_pCodeEdit,SLOT(sltClose()));
}

void TabContextMenu::sltCopyFullPath()
{
    QString sFileName = m_pCodeEdit->fileName();
    sFileName = QDir::toNativeSeparators(sFileName);
    QClipboard * pClipboard = QApplication::clipboard();
    pClipboard->setText( sFileName, QClipboard::Clipboard );
}

void TabContextMenu::sltOpenContainingFolder()
{
    OpenContainingFolder( m_pCodeEdit->fileName() );
}

static int UpdateIconSize(int defaultSize)
{
    if( defaultSize==-1 )
    {
        // check once the resolution (dpi) and adjust
        // the default icon size for the resolution
        // (if needed)

        defaultSize = c_iDefaultIconSize;

        int dpix = qApp->desktop()->logicalDpiX();
        //int dpiy = qApp->desktop()->logicalDpiY();
        if(dpix<100)
        {
            return defaultSize;
        }
        if(dpix>=100 && dpix<200)
        {
            return 2*defaultSize;
        }
        if(dpix>=200 && dpix<320)
        {
            return 4*defaultSize;
        }
        if(dpix>=320)
        {
            return 8*defaultSize;
        }
    }
    return defaultSize;
}

// ************************************************************************
// ************************************************************************
// ************************************************************************
/*
 *  Constructs a VisiScript which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
VisiScript::VisiScript( const QList<IVisiScriptExtension *> aExtensions,const QString & sFileName , QWidget* parent, Qt::WindowFlags fl )
    : QMainWindow( parent,fl),
      m_pTabWidget( 0 ),
      m_sScriptCmd( _QSCRIPT ),
      m_sFontName( FONT_NAME ),
      m_sFontStyle( "normal" ),
      m_sTempFileName( _TEMP_SCRIPT_FILE ),
      m_iFontSize( c_iMedium ),
      m_iActPageIndex( -1 ),
      m_iLastPageIndex( -1 ),
      m_iWordWrap( 0 ),
      m_iOverwrite( 0 ),
      m_iSpacesForTabs( 0 ),
      m_iShowCurrentLine( 1 ),
      m_iShowWhitespaces( 0 ),
      m_iShowLineNumbers( _SHOW_LINENUMBERS ),
      m_iAutoIndent( 0 ),
      m_iCodeFolding( 1 ),
      m_iTabSpace( 4 ),
      m_iEolMode( EOL_MODE ),
      m_iShowStartupFileDialog( 0 ),
      m_iIconSize( -1 ),
      m_dOutputRatio( _INIT_OUTPUT_RATIO ),
      m_bAutoFileLoad( false ), //ZJ MODIFY
      m_bIsReplace( false ),
      m_bRegularExpr( false ),
      m_bWildcard( false ),
      m_bCaseSensitive( false ),
      m_bOnlyWholeWords( false ),
      m_bSearchBackward( false ),
      m_bShowAllOccurrences( false ),
      m_bSearchSubDirectories( true ),
      m_bReplaceAll( false ),
      m_bInteractiveModus( false ),
      m_bChangeActDir( true ),
      m_bShowOutputArea( true ),
      m_bAutoSave( false ),
      m_bBraceMatching( false ),
      m_bAutoClearOutput( false ),
      m_bIsDefaultUtf8( true ),
      m_bUseNewIcons( true ),
      m_bUseSvgIcons( true ),
      m_bUseTempFile( false ),
      m_bSaveAlreadyAsked( false ),
      m_bFirstLaunch( true ),
      m_bCloseButtonInTabPage( false ),
      m_bMeasureExecutionTime( false ),
      m_bPipeScriptToInterpreter( false ),
      m_bUseToolbar( true ),// override default value false for these platforms //ZJ 新的ToolBar
      m_bUseUserInterpreter( false ),
      m_bUseRootForFileDialog( false ),
      m_bIsMoveAllKeyPressed( false ),
      m_bIsDebugging( false ),
      m_bIsMacroRecording( false ),
      m_bUseFile(true),
      m_iCountLastOpendFiles( 0 ),
      m_iAutoCompletionThresh( 2 ),
      m_pFileSystemWatcher( 0 ),
      m_iLastMainScriptIndex( 0 ),
      m_iLastActFileIndex( 0 ),
      m_aModifiedFilesLock(QReadWriteLock::Recursive),
      m_iCheckModifiedFilesCounter(0),
      m_iLastPosX(5),
      m_iLastPosY(5),
      m_iLastPosDX(800),
      m_iLastPosDY(600),
      c_strLineTag( tr("line ") ),
      c_strFileTag( tr("file ") ),
      c_strLineTagOrg( "line " ),
      c_strFileTagOrg( "file " ),
      c_strUnknownFileName( tr("new") ),
      m_pFindDlg( 0 ),
      m_pReplaceDlg( 0 ),
      m_pQScriptProxy(0)
{
    //LoadInit();
    this->setWindowFlags(fl);
    setFocusPolicy(Qt::ClickFocus);

    m_pFileSystemWatcher = new QFileSystemWatcher();

    // *************************************************************
    // *************************************************************
    m_pEngine = NULL;
    m_pMenuBar = menuBar();
    if( m_bUseToolbar )
    {
        m_pButtonBar = addToolBar("main");
        m_iIconSize = UpdateIconSize(m_iIconSize);
        m_pButtonBar->setIconSize(QSize(m_iIconSize,m_iIconSize));
#if defined(Q_OS_ANDROID) //|| defined(Q_OS_IOS)
        m_pButtonBar->setMovable( false );
#endif
    }
    else
    {
        m_pButtonBar = 0;
    }

    QPixmap aIcon(":/icons/visiscript_512x512.png");
    setWindowIcon( aIcon );

    statusBar();
    m_pStatusBarMsg = new QLabel( statusBar() );
    statusBar()->addWidget( m_pStatusBarMsg );

    // TODO: prepared for additional informations in the statusbar
    //QLabel * pAdditionalInfos = new QLabel( statusBar() );
    //pAdditionalInfos->setText("interpreter:");
    //statusBar()->addPermanentWidget(pAdditionalInfos);

    setAcceptDrops( true );

    m_pFile = new QMenu( tr( "&File" ), m_pMenuBar );
    m_pFile->setObjectName(_MENU_FILE);
    m_pMenuBar->addAction(m_pFile->menuAction());
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    m_pTab = new QMenu( tr( "&Tabpage" ), m_pMenuBar );
    m_pTab->setObjectName(_MENU_TABPAGE);
    m_pMenuBar->addAction(m_pTab->menuAction());
#else
    m_pTab = m_pFile;
#endif
    m_pEdit = new QMenu( tr( "&Edit" ), m_pMenuBar );
    m_pEdit->setObjectName(_MENU_EDIT);
    m_pMenuBar->addAction(m_pEdit->menuAction());
    m_pRun = new QMenu( tr( "&Run" ), m_pMenuBar );
    m_pRun->setObjectName(_MENU_RUN);
    m_pMenuBar->addAction(m_pRun->menuAction());
    m_pView = new QMenu( tr( "&View" ), m_pMenuBar );
    m_pView->setObjectName(_MENU_VIEW);
    m_pMenuBar->addAction(m_pView->menuAction());

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    m_pLanguages = new QMenu( tr("Lan&guage"), m_pMenuBar );
    m_pMenuBar->addAction(m_pLanguages->menuAction());
    m_pEncodings = new QMenu( tr("&Encoding"), m_pTab );
    m_pTab->addAction(m_pEncodings->menuAction());
    m_pHelp = new QMenu( tr( "&Help" ), m_pMenuBar );
    m_pMenuBar->addAction(m_pHelp->menuAction());
    m_pRun->setTitle(tr("Search/Run"));
#else
    m_pHelp = new QMenu( tr( "&Help" ), m_pMenuBar );
    m_pHelp->setObjectName(_MENU_HELP);
    m_pMenuBar->addAction(m_pHelp->menuAction());
    m_pLanguages = new QMenu( tr("Lan&guage"), m_pView );
    m_pView->addAction(m_pLanguages->menuAction());
    m_pEncodings = new QMenu( tr("&Encoding"), m_pView );
    m_pView->addAction(m_pEncodings->menuAction());
#endif
    m_pEncodings->menuAction()->setObjectName(_MENU_ENCODING);
    m_pEncodings->setObjectName(_MENU_ENCODING);

    connect( m_pFile, SIGNAL( aboutToShow() ), this, SLOT( sltFileAboutToShow() ) );
    connect( m_pEdit, SIGNAL( aboutToShow() ), this, SLOT( sltEditAboutToShow() ) );
    connect( m_pRun, SIGNAL( aboutToShow() ), this, SLOT( sltRunAboutToShow() ) );
    connect( m_pView, SIGNAL( aboutToShow() ), this, SLOT( sltViewAboutToShow() ) );
    connect( m_pLanguages, SIGNAL( aboutToShow() ), this, SLOT( sltLanguagesAboutToShow() ) );
    connect( m_pLanguages, SIGNAL( triggered(QAction *) ), this, SLOT( sltLanguagesTriggered(QAction *) ) );
    connect( m_pEncodings, SIGNAL( aboutToShow() ), this, SLOT( sltEncodingsAboutToShow() ) );
    connect( m_pEncodings, SIGNAL( triggered(QAction *) ), this, SLOT( sltEncodingsTriggered(QAction *) ) );

    // ++++++++++++++++++++++++++++++++++++++++++

    QAction * pAction = 0;

//    pAction = CreateNewAction( tr( "&New" ), LoadFromMem(":/icons/DocumentTypeNone.png",":/svgs/1024px-Gnome-fs-regular.svg.png",new_png,sizeof(new_png),m_bUseNewIcons,m_bUseSvgIcons), Qt::CTRL+Qt::Key_N, this );
//    //ggf TODO: pAction->setStatusTip(tr("create a new script file"));
//    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltFileNew() ) );
//    m_pFile->addAction( pAction );

//    pAction = CreateNewAction( tr( "&Open..." ), LoadFromMem(":/icons/fileopen.png",":/svgs/1024px-Gnome-fs-directory.svg.png",fileopen_png,sizeof(fileopen_png),m_bUseNewIcons,m_bUseSvgIcons), Qt::CTRL+Qt::Key_O, this );
//    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltFileOpen() ) );
//    AddActionToToolbarOrMenubar( pAction );
//    m_pFile->addAction( pAction );

    pAction = CreateNewAction( tr( "&Save" ), LoadFromMem(":/icons/save.png",":/svgs/1024px-Gnome-dev-floppy.svg.png",filesave_png,sizeof(filesave_png),m_bUseNewIcons,m_bUseSvgIcons), Qt::CTRL+Qt::Key_S, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltFileSave() ) );
    AddActionToToolbarOrMenubar( pAction );
    m_pFile->addAction( pAction );

    pAction = CreateNewAction( tr( "Save &as..." ), 0, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltFileSaveAs() ) );
    m_pFile->addAction( pAction );

    pAction = CreateNewAction( tr( "&Export output..." ), 0, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltFileSaveOutputAs() ) );
    m_pFile->addAction( pAction );

    m_pLastOpendFiles = new QMenu( tr( "&Last files" ), m_pFile );
    connect( m_pLastOpendFiles, SIGNAL( aboutToShow() ), this, SLOT( sltUpdateLastFilesMenu() ) );
    connect( m_pLastOpendFiles, SIGNAL( triggered(QAction *) ), this, SLOT( sltLastFilesMenuTriggered(QAction *) ) );
    m_pFile->addMenu( m_pLastOpendFiles );

    m_pFile->addSeparator();

    pAction = CreateNewAction( tr( "Load &project..." ), 0, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltFileOpenProject() ) );
    m_pFile->addAction( pAction );

    pAction = CreateNewAction( tr( "Save p&roject..." ), 0, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltFileSaveProject() ) );
    m_pFile->addAction( pAction );

    m_pFile->addSeparator();

    pAction = CreateNewAction( tr( "&Clear tab" ), Qt::ALT+Qt::SHIFT+Qt::Key_C, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltClearActDocument() ) );
    m_pTab->addAction( pAction );

    pAction = CreateNewAction( tr( "Close &tab" ), /*Qt::ALT+*/Qt::Key_F4, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltCloseActDocument() ) );
    m_pTab->addAction( pAction );

    pAction = CreateNewAction( tr( "Close all tabs" ), 0, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltCloseAllDocuments() ) );
    m_pTab->addAction( pAction );

    pAction = CreateNewAction( tr( "Copy full path" ), Qt::CTRL+Qt::ALT+Qt::Key_C, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltCopyFullPath() ) );
    m_pTab->addAction( pAction );

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    pAction = CreateNewAction( tr( "Open containing folder" ), Qt::CTRL+Qt::ALT+Qt::Key_E, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltOpenContainingFolder() ) );
    m_pFile->addAction( pAction );

    m_pFile->addSeparator();

    pAction = CreateNewAction( tr( "&Print..." ), Qt::CTRL+Qt::Key_P, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltFilePrint() ) );
    m_pFile->addAction( pAction );
    pAction = CreateNewAction( tr( "Print &output..." ), 0, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltOutputPrint() ) );
    m_pFile->addAction( pAction );
#endif

    m_pFile->addSeparator();

    QAction * pConfigureAction = CreateNewAction( tr( "&Configuration..." ), 0, this );      // menu item shows up in Mac-Menu under Mac OS X !
    connect( pConfigureAction, SIGNAL( triggered() ), this, SLOT( sltConfiguration() ) );
#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    m_pFile->addAction( pConfigureAction );
#endif

    m_pFile->addSeparator();

    pAction = CreateNewAction( tr( "&Quit" ), Qt::CTRL+Qt::Key_Q, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT(close()));
    m_pFile->addAction( pAction );

    // ++++++++++++++++++++++++++++++++++++++++++

    pAction = CreateNewAction( tr( "&Undo" ), Qt::CTRL+Qt::Key_Z, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltEditUndo() ) );
    m_pEdit->addAction( pAction );

    pAction = CreateNewAction( tr( "Re&do" ), Qt::CTRL+Qt::Key_Y, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltEditRedo() ) );
    m_pEdit->addAction( pAction );

    pAction = CreateNewAction( tr( "Bac&kward" ), LoadFromMem(":/icons/back.png",":/svgs/1024px-Nuvola_apps_backward_arrow.svg.png",back_png,sizeof(back_png),m_bUseNewIcons,m_bUseSvgIcons), Qt::CTRL+Qt::Key_K, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltEditBackward() ) );
    AddActionToToolbarOrMenubar( pAction );
    m_pEdit->addAction( pAction );

    pAction = CreateNewAction( tr( "For&ward" ), LoadFromMem(":/icons/forward.png",":/svgs/1024px-Nuvola_apps_forward_arrow.svg.png",forward_png,sizeof(forward_png),m_bUseNewIcons,m_bUseSvgIcons), Qt::CTRL+Qt::Key_W, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltEditForward() ) );
    AddActionToToolbarOrMenubar( pAction );
    m_pEdit->addAction( pAction );

    m_pEdit->addSeparator();

    m_pCutAction = CreateNewAction( tr( "Cu&t" ), Qt::CTRL+Qt::Key_X, this );
    connect( m_pCutAction, SIGNAL( triggered() ), this, SLOT( sltEditCut() ) );
    m_pEdit->addAction( m_pCutAction );

    m_pCopyAction = CreateNewAction( tr( "&Copy" ), Qt::CTRL+Qt::Key_C, this );
    connect( m_pCopyAction, SIGNAL( triggered() ), this, SLOT( sltEditCopy() ) );
    m_pEdit->addAction( m_pCopyAction );

    m_pPasteAction = CreateNewAction( tr( "&Paste" ), Qt::CTRL+Qt::Key_V, this );
    connect( m_pPasteAction, SIGNAL( triggered() ), this, SLOT( sltEditPaste() ) );
    m_pEdit->addAction( m_pPasteAction );

    m_pEdit->addSeparator();

    m_pSelectAllAction = CreateNewAction( tr( "Select &all" ), Qt::CTRL+Qt::Key_A, this );
    connect( m_pSelectAllAction, SIGNAL( triggered() ), this, SLOT( sltEditSelectAll() ) );
    m_pEdit->addAction( m_pSelectAllAction );

    m_pEdit->addSeparator();

    QMenu * pFindMenu = 0;
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    pFindMenu = m_pRun;
#else
    pFindMenu = m_pEdit;
#endif
    pAction = CreateNewAction( tr( "&Find..." ), LoadFromMem(":/icons/mag.png",":/svgs/1024px-Searchtool.svg.png",find_png,sizeof(find_png),m_bUseNewIcons,m_bUseSvgIcons), Qt::CTRL+Qt::Key_F, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltEditFindText() ) );
    AddActionToToolbarOrMenubar( pAction );
    pFindMenu->addAction( pAction );

#if defined(Q_OS_MAC)
    QKeySequence aFindNextKeySeq = Qt::CTRL+Qt::Key_G;
    QKeySequence aReplaceKeySeq = Qt::CTRL+Qt::ALT+Qt::Key_F;
#else
    QKeySequence aFindNextKeySeq = Qt::Key_F3;
    QKeySequence aReplaceKeySeq = Qt::CTRL+Qt::Key_H;
#endif
    pAction = CreateNewAction( tr( "Find &next" ), LoadFromMem(":/icons/repeat.png",":/svgs/1024px-Gtk-redo-ltr.svg.png",next_png,sizeof(next_png),m_bUseNewIcons,m_bUseSvgIcons), aFindNextKeySeq, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltEditFindNext() ) );
    AddActionToToolbarOrMenubar( pAction );
    pFindMenu->addAction( pAction );

    pAction = CreateNewAction( tr( "R&eplace..." ), aReplaceKeySeq, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltEditReplaceText() ) );
    pFindMenu->addAction( pAction );

    pAction = pFindMenu->addSeparator();
    pAction->setObjectName(_MENU_SEPARATOR_SEARCH);

#if defined(Q_OS_MAC)
    QKeySequence aGotoKeySeq = Qt::Key_F9;
#else
    QKeySequence aGotoKeySeq = Qt::CTRL+Qt::Key_G;
#endif
    pAction = CreateNewAction( tr( "&Goto line..." ), aGotoKeySeq, this, _MENU_GOTO_LINE );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltEditGotoLine() ) );
    pFindMenu->addAction( pAction );

    // add menu item for "goto symbol under cursor" from visiscriptextensions module here

    pAction = pFindMenu->addSeparator();
    pAction->setObjectName(_MENU_SEPARATOR_GOTO);

    m_pRecordMacro = CreateNewAction( tr( "S&tart record macro" ), Qt::Key_F7, this );
    m_pRecordMacro->setCheckable(true);
    connect( m_pRecordMacro, SIGNAL( triggered() ), this, SLOT( sltToggleRecordMacro() ) );
    pFindMenu->addAction( m_pRecordMacro );

    pAction = CreateNewAction( tr( "Pla&y macro" ), Qt::Key_F8, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltPlayMacro() ) );
    pFindMenu->addAction( pAction );

    // ++++++++++++++++++++++++++++++++++++++++++

    m_pClearOutput = CreateNewAction( tr( "Clear &output" ), LoadFromMem(":/icons/editdelete.png",":/svgs/1024px-Nuvola_apps_error.svg.png",editdelete_png,sizeof(editdelete_png),m_bUseNewIcons,m_bUseSvgIcons), Qt::Key_F6, this );
    connect( m_pClearOutput, SIGNAL( triggered() ), this, SLOT( sltClearOutput() ) );

    m_pKillOrClearOutput = new QAction( LoadFromMem(":/icons/editdelete.png",":/svgs/1024px-Nuvola_apps_error.svg.png",editdelete_png,sizeof(editdelete_png),m_bUseNewIcons,m_bUseSvgIcons), "kill or clear", this);
    connect( m_pKillOrClearOutput, SIGNAL( triggered() ), this, SLOT( sltKillOrClearOutput() ) );

    m_pRunScript = CreateNewAction( tr( "&Run" ), LoadFromMem(":/icons/play.png",":/svgs/1024px-Gtk-media-play-ltr.svg.png",play_png,sizeof(play_png),m_bUseNewIcons,m_bUseSvgIcons), Qt::Key_F5, this );
    connect( m_pRunScript, SIGNAL( triggered() ), this, SLOT( sltRun() ) );
    m_pRun->addAction( m_pRunScript );

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    m_pRunActScript = CreateNewAction( tr( "Run act&." ), Qt::SHIFT+Qt::Key_F5, this );
    connect( m_pRunActScript, SIGNAL( triggered() ), this, SLOT( sltActRun() ) );
    m_pRun->addAction( m_pRunActScript );
#endif

    m_pKillScript = CreateNewAction( tr( "&Kill" ), LoadFromMem(":/icons/editdelete.png",":/svgs/1024px-Nuvola_apps_error.svg.png",editdelete_png,sizeof(editdelete_png),m_bUseNewIcons,m_bUseSvgIcons), 0, this );
    m_pKillScript->setObjectName( _MENU_STOP );
    connect( m_pKillScript, SIGNAL( triggered() ), this, SLOT( sltKill() ) );
    m_pRun->addAction( m_pKillScript );

    pAction = m_pRun->addSeparator();
    pAction->setObjectName( _MENU_SEPARATOR_STOP );

    pAction = CreateNewAction( tr( "Show interpreter &version" ), 0, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltShowInterpreterVersion() ) );
    m_pRun->addAction( pAction );

    pAction = m_pRun->addSeparator();
    pAction->setObjectName( _MENU_SEPARATOR_INTERPRETER_VERSION );

    m_pRun->addAction( m_pClearOutput );

    // ++++++++++++++++++++++++++++++++++++++++++

    pAction = CreateNewAction( tr( "&Font..." ), 0, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltSelectFont() ) );
    m_pView->addAction( pAction );

    m_pUTF8 = m_pEncodings->addAction( tr("&utf-8") );
    m_pUTF8->setCheckable(true);
    m_pLatin1 = m_pEncodings->addAction( tr("&latin-1") );
    m_pLatin1->setCheckable(true);

    m_aMenuLanguages.resize(LANG_MAX_ID);
    m_aMenuLanguages[LANG_NONE] = AddCheckableMenuItem(m_pLanguages, tr("none") );
    m_aMenuLanguages[LANG_MIMDCRIPT] = AddCheckableMenuItem(m_pLanguages, tr("minscript") );
    m_aMenuLanguages[LANG_CPP] = AddCheckableMenuItem(m_pLanguages, tr("C++") );
    m_aMenuLanguages[LANG_CSHARP] = AddCheckableMenuItem(m_pLanguages, tr("C#") );
    m_aMenuLanguages[LANG_JAVA] = AddCheckableMenuItem(m_pLanguages, tr("Java") );
    m_aMenuLanguages[LANG_JAVASCRIPT] = AddCheckableMenuItem(m_pLanguages, tr("JavaScript") );
    m_aMenuLanguages[LANG_D] = AddCheckableMenuItem(m_pLanguages, tr("D") );
    m_aMenuLanguages[LANG_PYTHON] = AddCheckableMenuItem(m_pLanguages, tr("Python") );
    m_aMenuLanguages[LANG_RUBY] = AddCheckableMenuItem(m_pLanguages, tr("Ruby") );
    m_aMenuLanguages[LANG_PERL] = AddCheckableMenuItem(m_pLanguages, tr("Perl") );
    m_aMenuLanguages[LANG_PHP] = AddCheckableMenuItem(m_pLanguages, tr("PHP") );
    m_aMenuLanguages[LANG_LUA] = AddCheckableMenuItem(m_pLanguages, tr("Lua") );
    m_aMenuLanguages[LANG_LISP] = AddCheckableMenuItem(m_pLanguages, tr("Lisp") );
    m_aMenuLanguages[LANG_TCL] = AddCheckableMenuItem(m_pLanguages, tr("Tcl") );
    m_aMenuLanguages[LANG_IDL] = AddCheckableMenuItem(m_pLanguages, tr("IDL") );
    m_aMenuLanguages[LANG_SQL] = AddCheckableMenuItem(m_pLanguages, tr("SQL") );
    m_aMenuLanguages[LANG_HTML] = AddCheckableMenuItem(m_pLanguages, tr("HTML") );
    m_aMenuLanguages[LANG_CSS] = AddCheckableMenuItem(m_pLanguages, tr("CSS") );
    m_aMenuLanguages[LANG_XML] = AddCheckableMenuItem(m_pLanguages, tr("XML") );
    m_aMenuLanguages[LANG_TEX] = AddCheckableMenuItem(m_pLanguages, tr("TeX") );
    m_aMenuLanguages[LANG_VHDL] = AddCheckableMenuItem(m_pLanguages, tr("VHDL") );
    m_aMenuLanguages[LANG_MATLAB] = AddCheckableMenuItem(m_pLanguages, tr("Matlab") );
    m_aMenuLanguages[LANG_OCTAVE] = AddCheckableMenuItem(m_pLanguages, tr("Octave") );
    m_aMenuLanguages[LANG_MAKEFILE] = AddCheckableMenuItem(m_pLanguages, tr("Makefile") );
    m_aMenuLanguages[LANG_DIFF] = AddCheckableMenuItem(m_pLanguages, tr("diff") );
    m_aMenuLanguages[LANG_SCHEME] = AddCheckableMenuItem(m_pLanguages, tr("Scheme") );
    m_aMenuLanguages[LANG_PASCAL] = AddCheckableMenuItem(m_pLanguages, tr("Pascal") );
    m_aMenuLanguages[LANG_FORTRAN] = AddCheckableMenuItem(m_pLanguages, tr("Fortran") );
    m_aMenuLanguages[LANG_HASKELL] = AddCheckableMenuItem(m_pLanguages, tr("Haskell") );
    m_aMenuLanguages[LANG_BATCH] = AddCheckableMenuItem(m_pLanguages, tr("batch") );
    m_aMenuLanguages[LANG_BASH] = AddCheckableMenuItem(m_pLanguages, tr("bash") );
    m_aMenuLanguages[LANG_YAML] = AddCheckableMenuItem(m_pLanguages, tr("yaml") );
    m_aMenuLanguages[LANG_PO] = AddCheckableMenuItem(m_pLanguages, tr("po") );
    m_aMenuLanguages[LANG_PROPERTIES] = AddCheckableMenuItem(m_pLanguages, tr("properties") );
    m_aMenuLanguages[LANG_CMAKE] = AddCheckableMenuItem(m_pLanguages, tr("CMake") );
    m_aMenuLanguages[LANG_SPICE] = AddCheckableMenuItem(m_pLanguages, tr("spice") );
    m_aMenuLanguages[LANG_POSTSCRIPT] = AddCheckableMenuItem(m_pLanguages, tr("Postscript") );
    m_aMenuLanguages[LANG_COFFEESCRIPT] = AddCheckableMenuItem(m_pLanguages, tr("CoffeeScript") );
    m_pView->addSeparator();

    pAction = CreateNewAction( tr( "Zoom &in" ), 0, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltZoomIn() ) );
    m_pView->addAction( pAction );
    pAction = CreateNewAction( tr( "Zoom &out" ), 0, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltZoomOut() ) );
    m_pView->addAction( pAction );

    m_pView->addSeparator();

    m_pMaximizeScript = CreateNewAction( tr( "Ma&ximize script" ), LoadFromMem(":/icons/add.png",":/svgs/1024px-1uparrow_green.svg.png",0,0,m_bUseNewIcons,m_bUseSvgIcons), 0, this );
    AddActionToToolbarOrMenubar( m_pMaximizeScript );
    connect( m_pMaximizeScript, SIGNAL( triggered() ), this, SLOT( sltMaximizeScript() ) );
    m_pView->addAction( m_pMaximizeScript );

    m_pMaximizeOutput = CreateNewAction( tr( "Maximize ou&tput" ), LoadFromMem(":/icons/sub.png",":/svgs/1024px-1downarrow_red.svg.png",0,0,m_bUseNewIcons,m_bUseSvgIcons), 0, this );
    AddActionToToolbarOrMenubar( m_pMaximizeOutput );
    connect( m_pMaximizeOutput, SIGNAL( triggered() ), this, SLOT( sltMaximizeOutput() ) );
    m_pView->addAction( m_pMaximizeOutput );

    m_pUserSizeOutput = CreateNewAction( tr( "&Split script/output" ), LoadFromMem(":/icons/pass.png",":/svgs/1024px-updown.svg.png",0,0,m_bUseNewIcons,m_bUseSvgIcons), 0, this );
    AddActionToToolbarOrMenubar( m_pUserSizeOutput );
    connect( m_pUserSizeOutput, SIGNAL( triggered() ), this, SLOT( sltSplitScriptOutput() ) );
    m_pView->addAction( m_pUserSizeOutput );

    AddActionToToolbarOrMenubar( m_pKillOrClearOutput );
    AddActionToToolbarOrMenubar( m_pRunScript );

    m_pView->addSeparator();

//#if defined(Q_OS_MAC)
//    QKeySequence aKeySeq = Qt::META+Qt::SHIFT+Qt::Key_Tab;
//#else
//    QKeySequence aKeySeq = Qt::ALT+Qt::CTRL+Qt::Key_Tab;
//#endif
    QKeySequence aKeySeq = Qt::Key_F2;
    pAction = CreateNewAction( tr( "&Switch to tab..." ), aKeySeq, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltSwitchToTab() ) );
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    m_pTab->addAction( pAction );
#else
    m_pView->addAction( pAction );
#endif

#if defined(Q_OS_MAC)
    aKeySeq = Qt::META+Qt::Key_Tab;
#else
    aKeySeq = Qt::CTRL+Qt::Key_Tab;
#endif
#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    m_pSwitchTab = CreateNewAction( tr( "&Switch to last activated tab" ), aKeySeq, this );
    connect( m_pSwitchTab, SIGNAL( triggered() ), this, SLOT( sltSwitchToLastTab() ) );
    m_pView->addAction( m_pSwitchTab );

    m_pView->addSeparator();
#endif
    m_pToggleWordWrap = CreateNewAction( tr( "&Word wrap" ), 0, this );
    m_pToggleWordWrap->setCheckable(true);
    connect( m_pToggleWordWrap, SIGNAL( triggered() ), this, SLOT( sltToggleWordWrap() ) );
    m_pView->addAction( m_pToggleWordWrap );

    m_pToggleOverwrite = CreateNewAction( tr( "O&verwrite" ), 0, this );
    m_pToggleOverwrite->setCheckable(true);
    connect( m_pToggleOverwrite, SIGNAL( triggered() ), this, SLOT( sltToggleOverwrite() ) );
    m_pView->addAction( m_pToggleOverwrite );

    m_pCodeFolding = CreateNewAction( tr( "Code &folding" ), 0, this );
    m_pCodeFolding->setCheckable(true);
    connect( m_pCodeFolding, SIGNAL( triggered() ), this, SLOT( sltCodeFolding() ) );
    m_pView->addAction( m_pCodeFolding );

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    m_pMarkAsMain = CreateNewAction( tr( "M&ark as main" ), 0, this );
    connect( m_pMarkAsMain, SIGNAL( triggered() ), this, SLOT( sltViewMarkActAsMain() ) );
    m_pView->addAction( m_pMarkAsMain );
#endif

    m_pView->addSeparator();

    // in Eclipse: CTRL+'/'
    m_pCommentAction = CreateNewAction( tr( "Co&mment block" ), Qt::CTRL+Qt::Key_M, this );
    connect( m_pCommentAction, SIGNAL( triggered() ), this, SLOT( sltCommentSelection() ) );
    m_pView->addAction( m_pCommentAction );

    // in Eclipse: CTRL+'\'
    m_pUnCommentAction = CreateNewAction( tr( "&Uncomment block" ), Qt::CTRL+Qt::Key_E, this );
    connect( m_pUnCommentAction, SIGNAL( triggered() ), this, SLOT( sltUnCommentSelection() ) );
    m_pView->addAction( m_pUnCommentAction );

    pAction = CreateNewAction( tr( "&Help..." ), Qt::SHIFT+Qt::Key_F1, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltShowVisiscriptHelp() ) );
    m_pHelp->addAction( pAction );

    pAction = CreateNewAction( tr( "Help for &interpreter..." ), Qt::Key_F1, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltShowInterpreterHelp() ) );
    m_pHelp->addAction( pAction );

    m_pHelp->addSeparator();

    pAction = CreateNewAction( tr( "&About..." ), 0, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltShowAbout() ) );
    m_pHelp->addAction( pAction );

    pAction = CreateNewAction( tr( "&Extensions..." ), 0, this );
    connect( pAction, SIGNAL( triggered() ), this, SLOT( sltShowExtensions() ) );
    m_pHelp->addAction( pAction );
    m_pMenuBar->setVisible(false);//ZJ ADD
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    m_pHelp->addSeparator();

    m_pHelp->addAction( pConfigureAction );
#endif

    // *************************************************************
    // *************************************************************

    QWidget * centralWidget = new QWidget(this);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    QGridLayout * gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(1);
    gridLayout->setContentsMargins(1, 1, 1, 1);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    setCentralWidget( centralWidget );
    m_pTabWidget = new TabWidget( centralWidget );
    m_pTabWidget->setMovable(true);
    m_pTabWidget->setTabsClosable(m_bCloseButtonInTabPage);
    m_pTabWidget->setContextMenuPolicy(Qt::DefaultContextMenu);
    m_pTabWidget->setUsesScrollButtons(true);
    m_pTabWidget->GetTabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_pTabWidget->GetTabBar(),SIGNAL(customContextMenuRequested( const QPoint & )),this,SLOT(sltCustomContextMenuRequested( const QPoint & )));
    connect(m_pTabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(sltTabCloseRequested(int)));
    gridLayout->addWidget( m_pTabWidget );

    // connect singnals with slots
    connect( m_pTabWidget, SIGNAL(currentChanged(int)), this, SLOT(sltCurrentTabPageChanged(int)));
    connect( m_pFileSystemWatcher, SIGNAL( fileChanged(const QString &) ), this, SLOT( sltFileChanged(const QString &) ) );

    connect(&m_aScriptProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(sltFinishedScript(int,QProcess::ExitStatus)));
    connect(&m_aScriptProcess,SIGNAL(error(QProcess::ProcessError)),this,SLOT(sltErrorScript(QProcess::ProcessError)));
    ConnectScriptProcessSignals();

    QString sRegisterPluginsProtocol;
    // load extensions
    foreach( IVisiScriptExtension * pExtension, aExtensions )
    {
        if( !AddExtension(pExtension) )
        {
            sRegisterPluginsProtocol += tr("Error registering extension ")+pExtension->getName()+"\n";
        }
    }

    // open files of last session
    if( m_bAutoFileLoad && m_aLastOpendFilesList.count()>0 )
    {
        int i = 0;
        bool bAnyFileOpend = false;
        for( i=0; i<m_aLastOpendFilesList.count(); i++ )
        {
            QPair<int,int> pCursorPos = m_aLastCursorPos.at(i);
            int x = pCursorPos.first;
            int y = pCursorPos.second;
            int iLastEncodingUtf8 = m_aLastEncodingUtf8.at(i);
            bool bImportOk = DoImportFile( m_aLastOpendFilesList.at(i), x, y, iLastEncodingUtf8 );
            bAnyFileOpend = bAnyFileOpend || bImportOk;
        }
        if( !bAnyFileOpend )
        {
            AddDocument( c_strUnknownFileName );
        }

        i = 0;
        for( QList<QList<int> >::iterator iter = m_aLastSpliterPos.begin(); iter!=m_aLastSpliterPos.end(); ++iter )
        {
            QWidget * pPage = m_pTabWidget->widget( i );
            if( pPage )
            {
                QSplitter * pSplitter = (QSplitter *)pPage->findChild<QSplitter *>( QString(_SPLITTER_NAME) );
                if( pSplitter )
                {
                    pSplitter->setSizes(*iter);
                }
            }
            i++;
        }
    }
    else
    {
        if(sFileName.isEmpty() && !m_sText.isEmpty())
            m_bFirstLaunch = false;
        if( m_bFirstLaunch )
        {
            QString sPath = QCoreApplication::applicationDirPath() + QDir::separator() + QString(FILES_DIR) + QDir::separator();
#if defined( Q_OS_ANDROID)
            sPath = QString(FILES_DIR);
#endif
            DoImportFile(sFileName );
        }
        else
        {
            AddDocument( c_strUnknownFileName );
        }
    }

    m_pTabWidget->setCurrentIndex( m_iLastActFileIndex );

    ProcessReadIntent();

    if( m_iLastMainScriptIndex>=0 && GetScriptFor( m_iLastMainScriptIndex ) )
    {
        GetScriptFor( m_iLastMainScriptIndex )->setMainFile(true);
        // und alles aktualisieren
        ForAllDocumentScripts( &CodeEdit::sltUpdate );
    }

    if( m_iShowStartupFileDialog )
    {
        sltFileOpen();
    }
    else
    {
        //sltShowScript(); //2015.9.7
        UpdateCaption();
    }

    setMenuBar( m_pMenuBar );

    // check if restored window position is out of screen size --> switch multiple monitor configuration back to single monitor configuration
    int iWidth = QApplication::desktop()->width();
    int iHeight = QApplication::desktop()->height();
    if( m_iLastPosX>iWidth )
    {
        m_iLastPosX = iWidth-m_iLastPosDX;
    }
    if( m_iLastPosY>iHeight )
    {
        m_iLastPosY = iHeight-m_iLastPosDY;
    }

    resize( m_iLastPosDX, m_iLastPosDY );
    move( m_iLastPosX, m_iLastPosY );

    m_bFirstLaunch = false;

    if( sRegisterPluginsProtocol.length()>0 )
    {
        AddToCurrentOutput(sRegisterPluginsProtocol);
    }
}

VisiScript::VisiScript(const QString &sText, QWidget *parent, Qt::WindowFlags fl)
{
    m_sText = sText;
    QList<IVisiScriptExtension *> aExtensions;
    VisiScript(aExtensions,"");
}

VisiScript::~VisiScript()
{
    if( m_pQScriptProxy )
    {
        delete m_pQScriptProxy;
        m_pQScriptProxy = 0;
    }
    if( m_pFindDlg )
    {
        delete m_pFindDlg;
        m_pFindDlg = 0;
    }
    if( m_pReplaceDlg )
    {
        delete m_pReplaceDlg;
        m_pReplaceDlg = 0;
    }
    if( m_pFileSystemWatcher )
    {
        delete m_pFileSystemWatcher;
        m_pFileSystemWatcher = 0;
    }
    // no need to delete child widgets, Qt does it all for us
}

void VisiScript::ConnectScriptProcessSignals()
{
    connect(&m_aScriptProcess,SIGNAL(readyReadStandardError()),this,SLOT(sltReadyReadStandardErrorScript()));
    connect(&m_aScriptProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(sltReadyReadStandardOutputScript()));
    connect(&m_aScriptProcess,SIGNAL(readyRead()),this,SLOT(sltReadyRead()));
}

void VisiScript::DisconnectScriptProcessSignals()
{
    disconnect(&m_aScriptProcess,SIGNAL(readyReadStandardError()),this,SLOT(sltReadyReadStandardErrorScript()));
    disconnect(&m_aScriptProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(sltReadyReadStandardOutputScript()));
    disconnect(&m_aScriptProcess,SIGNAL(readyRead()),this,SLOT(sltReadyRead()));
}

// ************************************************************************

bool VisiScript::Register( IDebugger * pExtension )
{
    m_aDebugger.append( pExtension );
    return true;
}

bool VisiScript::Unregister( IDebugger * pExtension )
{
    return m_aDebugger.removeOne( pExtension );
}

int VisiScript::StartDebugging()
{
    return DoRun( /*bUseMainScript*/true, false, true );
}

int VisiScript::GetLanguageIdOfCurrentScript() const
{
    CodeEdit * pCurrentScript = GetActScript();
    return pCurrentScript!=0 ? pCurrentScript->languageID() : LANG_UNDEFINED;
}

// ************************************************************************

void VisiScript::ProcessReadIntent()
{
#if defined( Q_OS_ANDROID )
    QString sIntentData = GetIntentData();
    if( !sIntentData.isEmpty() )
    {
        QUrl aIntentData(sIntentData);
        if( aIntentData.isLocalFile() )
        {
            bool ok = DoImportFile( aIntentData.toLocalFile() );
            if( !ok )
            {
                GetActOutput()->append(QString(tr("WARNING: can not load file %1 !\n")).arg(aIntentData.toLocalFile()));
            }
        }
    }
#endif
}

void VisiScript::UnregisterExtensions( const QList<IVisiScriptExtension *> aExtensions )
{
    // unload extensions
    foreach( IVisiScriptExtension * pExtension, aExtensions )
    {
        RemoveExtension(pExtension);
    }
}

IAdvancedVisiScript * VisiScript::GetAdvancedInterface( int id )
{
    if( id==INTERFACE_DEBUGGER )
    {
        return this;
    }
    return 0;
}

QMainWindow * VisiScript::GetWindow()
{
    return static_cast<QMainWindow *>(this);
}

QMenuBar * VisiScript::GetMenuBar()
{
    return m_pMenuBar;
}

OutputAreaMultiLineEdit * VisiScript::GetAndLockCurrentOutput()
{
    OutputAreaMultiLineEdit * pRet = GetActOutput();
    pRet->setLocked( true );
    return pRet;
}

void VisiScript::UnlockOutput( OutputAreaMultiLineEdit * pOutput )
{
    if( pOutput )
    {
        pOutput->setLocked( false );
    }
}

CodeEdit * VisiScript::GetCurrentScript()
{
    return GetActScript();
}

QProcess & VisiScript::GetCurrentScriptProcess()
{
    return m_aScriptProcess;
}

QScriptEngine * VisiScript::GetCurrentScriptEngine()
{
    return m_pEngine;
}

void VisiScript::ScriptEnginePushLineToInputQueue( const QString & sValue )
{
    if( m_pEngine )
    {
        QVariant inputQueue = m_pEngine->property( INPUT_QUEUE );
        QStringList queue = inputQueue.toStringList();
        queue.push_back( sValue );
        m_pEngine->setProperty( INPUT_QUEUE, QVariant( queue ) );
    }
}

void VisiScript::ScriptEngineClearInputQueue()
{
    if( m_pEngine )
    {
        m_pEngine->setProperty( INPUT_QUEUE, QVariant( QStringList() ) );
    }
}

bool VisiScript::Register( IExtendQScript * pExtension )
{
    m_aExtendQScript.append( pExtension );
    return true;
}

bool VisiScript::Unregister( IExtendQScript * pExtension )
{
    return m_aExtendQScript.removeOne( pExtension );
}

bool VisiScript::Register( IExecuteScript * pExtension )
{
    m_aExecuteScript.append( pExtension );
    return true;
}

bool VisiScript::Unregister( IExecuteScript * pExtension )
{
    return m_aExecuteScript.removeOne( pExtension );
}

bool VisiScript::AddScriptInterpreterPath( const QString & sInterpreter )
{
    return PushToList( sInterpreter, m_aExternalProgsList, MAX_EXTERNAL_PROGRAMS );
}

QString VisiScript::SelectExistingDirectory( const QString & sCurrentDir )
{
    QString ret;
#ifdef _WITH_MOBILE_FILEDIALOG
    MobileFileSelection aDlg(this,sCurrentDir,FILES_DIR,GetSDCardPath(m_bUseRootForFileDialog),true,false);
#if defined(Q_OS_ANDROID)
    aDlg.showMaximized();
#endif
    // update labels after show
    aDlg.update();
    if( aDlg.exec()==QDialog::Accepted )
    {
        ret = aDlg.getFullFileName();
    }
#else
    ret = QFileDialog::getExistingDirectory( this, "", sCurrentDir );
#endif
    return ret;
}

QString VisiScript::GetFileTag() const
{
    return c_strFileTag;
}

QString VisiScript::GetLineTag() const
{
    return c_strLineTag;
}

void VisiScript::SetCurrentCursorPosition( int iLine, int iIndex )
{
    GetActScript()->setCursorPosition(iLine,iIndex);
    setFocus();
    GetActScript()->setFocus();     // TEST ? TODO
}

bool VisiScript::IsCurrentScriptModified() const
{
    return GetActScript()->isModified();
}

void VisiScript::SetCurrentScriptModified( bool value )
{
    GetActScript()->setModified( value );
}

QString VisiScript::GetCurrentScriptText() const
{
    return GetActScript()->text();
}

QString VisiScript::GetCurrentSearchText() const
{
    CodeEdit * pActScript = GetActScript();
    return pActScript->hasSelectedText() ? pActScript->selectedText() : GetCurrentWordUnderCursor();
}

QString VisiScript::GetSelectedText() const
{
    return GetActScript()->selectedText();
}

void VisiScript::AddToCurrentOutput( const QString & sText )
{
    AddToOutput( GetActOutput(), sText );
}

void VisiScript::AddToOutput( OutputAreaMultiLineEdit * pOutput, const QString & sText )
{
    if( pOutput )
    {
        pOutput->append( sText );
        pOutput->moveCursor( QTextCursor::End );
    }
}

void VisiScript::PushToLastSearchStringList( const QString & sText )
{
    PushToList( sText, Property_LastSearchStringList(), MAX_LAST_SEARCH_STRINGS );
}

void VisiScript::PushToLastSearchFilesList( const QString & sText )
{
    PushToList( sText, Property_LastSearchFilesList(), MAX_LAST_SEARCH_STRINGS );
}

void VisiScript::PushToLastSearchDirList( const QString & sText )
{
    PushToList( sText, Property_LastSearchDirList(), MAX_LAST_SEARCH_STRINGS );
}

QStringList & VisiScript::Property_LastSearchStringList()
{
    return m_aLastSearchStringList;
}

QStringList & VisiScript::Property_LastSearchFilesList()
{
    return m_aLastSearchFilesList;
}

QStringList & VisiScript::Property_LastSearchDirList()
{
    return m_aLastSearchDirList;
}

QString & VisiScript::Property_LastSearchText()
{
    return m_sLastFindText;
}

bool & VisiScript::Property_RegularExpr()
{
    return m_bRegularExpr;
}

bool & VisiScript::Property_Wildcard()
{
    return m_bWildcard;
}

bool & VisiScript::Property_CaseSensitive()
{
    return m_bCaseSensitive;
}

bool & VisiScript::Property_OnlyWholeWords()
{
    return m_bOnlyWholeWords;
}

bool & VisiScript::Property_SearchSubDirectories()
{
    return m_bSearchSubDirectories;
}

bool VisiScript::OnSave()
{
    if(m_bUseFile == false)
    {
        GetActScript()->setEdited(false);
        return false;
    }
    return true;
}

// ************************************************************************

bool VisiScript::AddExtension( IVisiScriptExtension * pExtension )
{
    if( pExtension )
    {
        m_aExtensions.append(pExtension);
        return pExtension->registerExt(this);
    }
    return false;
}

bool VisiScript::RemoveExtension( IVisiScriptExtension * pExtension )
{
    if( pExtension )
    {
        m_aExtensions.removeOne(pExtension);
        return pExtension->unregisterExt();
    }
    return false;
}

void VisiScript::CheckTempFile()
{
    if( m_bUseTempFile )
    {
        QFile aFile(m_sTempFileName);
        if( aFile.exists() )
        {
            /*bool ok =*/ aFile.remove();
        }
    }
}

void VisiScript::sltErrorScript(QProcess::ProcessError error)
{
    QString errText = m_aScriptProcess.readAllStandardError();
    if( errText.length()>0 )
    {
        sltAddLineToOutput(errText);
    }
    errText = "";
    switch( error )
    {
        case QProcess::FailedToStart:
            errText = tr("Error: failed to start script.");
            break;
        case QProcess::Crashed:
            errText = tr("Error: script crashed.");
            break;
        case QProcess::Timedout:
            errText = tr("Error: script timeout.");
            break;
        default:
            errText = tr("Error: Unknown error.");
            break;
    }
    if( errText.length()>0 )
    {
        sltAddLineToOutput(errText);
    }
    CheckTempFile();
    m_iElapsedTime = g_aScriptTimer.elapsed();
    if( m_bMeasureExecutionTime )
    {
        sltAddLineToOutput( GetExecutionTimeStrg() );
    }
    ProcessScriptFinished();
}

void VisiScript::sltFinishedScript(int exitValue, QProcess::ExitStatus status)
{
    Q_UNUSED(exitValue);
    if( status==QProcess::NormalExit)
    {
        QString sResult = m_aScriptProcess.readAll();
        QString errText = m_aScriptProcess.readAllStandardError();
        if( sResult.length()>0 )
        {
            sltAddLineToOutput(sResult);
        }
        if( errText.length()>0 )
        {
            sltAddLineToOutput(errText);
        }
    }
    CheckTempFile();
    // set current directory back, if needed
    if( m_sLastCurrentDir.length()>0 )
    {
        QDir::setCurrent(m_sLastCurrentDir);
    }
    m_iElapsedTime = g_aScriptTimer.elapsed();
    if( m_bMeasureExecutionTime )
    {
        sltAddLineToOutput( GetExecutionTimeStrg() );
    }
    m_aScriptProcess.closeWriteChannel();
    ProcessScriptFinished();
}

void VisiScript::sltReadyReadStandardErrorScript()
{
    if( m_aScriptProcess.state()!=QProcess::NotRunning )
    {
        QString sResult = /*QString::fromLocal8Bit?*/(m_aScriptProcess.readAllStandardError());
        QStringList lstLines = sResult.split("\n");
        foreach( const QString & s, lstLines )
        {
            bool bProcessed = CallProcessStdErrLineCallback( s );
            if( !bProcessed )
            {
                sltAddToOutput(s);
            }
            QApplication::processEvents();
        }
    }
}

void VisiScript::sltReadyReadStandardOutputScript()
{
    while( m_aScriptProcess.state()!=QProcess::NotRunning && m_aScriptProcess.canReadLine() )
    {
        QString sLine = m_aScriptProcess.readLine();
        bool bProcessed = CallProcessStdOutLineCallback( sLine );
        if( !bProcessed )
        {
            sltAddToOutput(sLine);
        }
        QApplication::processEvents();
    }
}

void VisiScript::sltReadyRead()
{
    while( m_aScriptProcess.state()!=QProcess::NotRunning && m_aScriptProcess.bytesAvailable()>0 )
    {
        QString sLine = m_aScriptProcess.readAllStandardOutput();
        QStringList lstLines = sLine.split("\n");
        foreach( const QString & s, lstLines )
        {
            QString sWithCR = s + "\n";
            bool bProcessed = CallProcessStdOutLineCallback( sWithCR );
            if( !bProcessed )
            {
                sltAddToOutput( sWithCR );
            }
            QApplication::processEvents();
        }
    }
}

void VisiScript::sltFileNew()
{
    AddDocument( c_strUnknownFileName );
}

void VisiScript::AddActionToToolbarOrMenubar(QAction * pNewAction )
{
    if( m_pButtonBar )
    {
        m_pButtonBar->addAction(pNewAction);
    }
    else if( m_pMenuBar )
    {
        m_pMenuBar->addAction(pNewAction);
    }
}

void VisiScript::sltFileOpen()
{
    QString sFileName = OpenFileName( this, QString::null, m_sLastDirectory, "", m_bUseRootForFileDialog );

    if( !sFileName.isEmpty() )
    {
        m_sLastDirectory = GetDirectoryFromFilePath( sFileName );

        DoImportFile( sFileName );
    }
}

void VisiScript::sltFileSave()
{
    if(false == OnSave())
        return;
    CodeEdit * pActScript = GetActScript();
    if( pActScript->fileName()==c_strUnknownFileName )
    {
        sltFileSaveAs();
    }
    else
    {
        DoExportFile( pActScript, /*bShowError=*/true );
    }
}

void VisiScript::sltFileSaveAs()
{
    if(false == OnSave())
        return;

    QString sFileName = SaveFileName( this, QString::null, m_sLastDirectory, "", m_bUseRootForFileDialog );

    if( !sFileName.isEmpty() )
    {
        bool bSave = true;

        if( QFile::exists(sFileName) )
        {
            if( QMessageBox::Ok != QMessageBox::question(this, tr("Question"), QString(tr("File already exists !\nOverwrite file %1?")).arg(sFileName)) )
            {
                bSave = false;
            }
        }

        if( bSave )
        {
            CodeEdit * pActScript = GetActScript();
            pActScript->setFileName( sFileName );

            // change language if extension has changed
            int iActLanguageID = GetLanguageIDFromExtension(QFileInfo(sFileName).suffix(),m_aExtensionToLanguageMap);
            if( iActLanguageID!=pActScript->languageID() )
            {
                pActScript->updateLanguage( iActLanguageID );
            }

            if( DoExportFile( pActScript, /*bShowError=*/true ) )
            {
                m_sLastDirectory = GetDirectoryFromFilePath( sFileName );
                PushLastFileName( sFileName );
            }
        }
    }
}

void VisiScript::sltFileOpenProject()
{
    QString sFileName = OpenFileName( this, QString::null, m_sLastDirectory, "*.vpl", m_bUseRootForFileDialog );

    if( !sFileName.isEmpty() )
    {
        QString sText;
        if( ReadTextFile( sFileName, sText, /*bUtf8*/true ) )
        {
            QStringList aFileNotLoadedLst;
            QStringList aList = sText.split("\n");
            for( int i=0; i<aList.size(); i++ )
            {
                bool bImportOk = DoImportFile( aList[i] );
                if( !bImportOk )
                {
                    aFileNotLoadedLst.push_back( aList[i] );
                }
            }
            if( aFileNotLoadedLst.size()>0 )
            {
                QString sMsg(tr("Could not load this files:\n"));
                sMsg += aFileNotLoadedLst.join(",\n");
                QMessageBox::warning(this,tr("Load"),sMsg);
            }
        }
    }
}

void VisiScript::sltFileSaveProject()
{
    if(false == OnSave())
        return;

    QString sFileName = SaveFileName( this, QString::null, m_sLastDirectory, "*.vpl", m_bUseRootForFileDialog );   // vpl = visiscript project list

    if( !sFileName.isEmpty() )
    {
        // on MAC the filter is not appended --> automatically add file extension if needed
        if( !sFileName.endsWith(".vpl") )
        {
            sFileName += ".vpl";
        }

        QString sFileList;

        for( int i=0; i<GetDocumentCount(); i++ )
        {
            sFileList += GetScriptFor(i)->fileName() + QString("\n");
        }
        if( !WriteTextFile( sFileName, sFileList, /*bAppend*/false, /*bUtf8*/true ) )
        {
            QMessageBox::warning(this,tr("Save"),tr("Error saving project file!"));
        }
    }
}

void VisiScript::sltFileSaveOutputAs()
{
    if(false == OnSave())
        return;

    QString sFileName = SaveFileName( this, QString::null, m_sLastDirectory, "", m_bUseRootForFileDialog );

    if( !sFileName.isEmpty() )
    {
        QString sText = GetActOutput()->toPlainText();
        /*bool bOk =*/ WriteTextFile( sFileName, sText, /*append=*/false, GetActScript()->isUtf8() );
    }
}

void VisiScript::sltFilePrint()
{
    CodeEdit * pActScript = GetActScript();
    if( pActScript )
    {
        pActScript->PrintContent();
    }
}

void VisiScript::sltOutputPrint()
{
    if( GetActOutput() )
    {
        GetActOutput()->PrintContent();
    }
}

void VisiScript::UpdateExternalProgs( QComboBox * pCombo )
{
    pCombo->setDuplicatesEnabled( false );
    pCombo->clear();

    for( int i=0; i<m_aExternalProgsList.count(); i++ )
    {
        QString sExternalProg = m_aExternalProgsList.at( m_aExternalProgsList.count()-i-1 );

        pCombo->insertItem( i, ToMetaPath( sExternalProg ) );
    }
}

static void ShowAction( QAction * pAction, bool value )
{
    pAction->setEnabled(value);
#if !defined(Q_OS_ANDROID)
    pAction->setVisible(value);
#endif
}

void VisiScript::UpdateOutputArea()
{
    if( !m_bShowOutputArea )
    {
        m_dOutputRatio = 0;
        SetOutputSizeForAll(0);
    }
    else
    {
        m_dOutputRatio = _INIT_OUTPUT_RATIO;
        SetOutputSizeForAll(m_dOutputRatio);
    }
    m_pMaximizeScript->setVisible(m_bShowOutputArea);
    m_pMaximizeOutput->setVisible(m_bShowOutputArea);
    ShowAction(m_pClearOutput,m_bShowOutputArea);
    ShowAction(m_pRunScript,m_bShowOutputArea);
#if !defined(Q_OS_ANDROID)
    ShowAction(m_pRunActScript,m_bShowOutputArea);
#endif
    ShowAction(m_pKillScript,m_bShowOutputArea);
    // show hide menu items first, than containing menu !
    m_pRun->setVisible(m_bShowOutputArea);
    m_pRun->setEnabled(m_bShowOutputArea);
    if(m_bShowOutputArea)
    {
        m_pRun->hide();
    }
}

void VisiScript::sltConfiguration()
{
    ConfigureDlgImpl * pDlg = new ConfigureDlgImpl( m_aUserInterpreters, this );

    int iScriptInterpreterConfigurationIndex = m_aUserInterpreters.GetIndexFor(m_sScriptCmd,0);
    if( iScriptInterpreterConfigurationIndex!=-1 )
    {
        pDlg->ui.btnAssignExtension->setText("*."+m_aUserInterpreters[iScriptInterpreterConfigurationIndex].m_sFileExtension);
    }

    pDlg->ui.m_pScriptCmd->setText( ToMetaPath(m_sScriptCmd) );

    UpdateExternalProgs( pDlg->ui.m_pScriptCmdList );

    SetComboBoxToText( pDlg->ui.m_pScriptCmdList, ToMetaPath(m_sScriptCmd) );

    // update the combo box for the last script arguments strings
    for( int i=0; i<m_aLastScriptArgumentsList.count(); i++ )
    {
        pDlg->ui.m_pScriptArguments->insertItem( 0, m_aLastScriptArgumentsList.at( i ) );
    }

    SetComboBoxToText( pDlg->ui.m_pScriptArguments, m_sScriptArguments );

#if defined(Q_OS_ANDROID)
    pDlg->ui.m_pUseRootForFileDialog->setChecked( (bool)m_bUseRootForFileDialog );
#else
    pDlg->ui.m_pUseRootForFileDialog->setVisible( false );
#endif
    pDlg->ui.m_pTabSpace->setText( QString::number( m_iTabSpace ) );
    pDlg->ui.m_pSpacesForTabs->setChecked( (bool)m_iSpacesForTabs );
    pDlg->ui.m_pShowCurrentLine->setChecked( (bool)m_iShowCurrentLine );
    pDlg->ui.m_pShowWhitespaces->setChecked( (bool)m_iShowWhitespaces );
    pDlg->ui.m_pShowLineNumbers->setChecked( (bool)m_iShowLineNumbers );
    pDlg->ui.m_pAutoIndent->setChecked( (bool)m_iAutoIndent );
    pDlg->ui.m_pCodeFolding->setChecked( (bool)m_iCodeFolding );
    pDlg->ui.m_pOutputRatio->setValue( m_dOutputRatio );
    pDlg->ui.m_pInteractionModus->setChecked( (bool)m_bInteractiveModus );
    pDlg->ui.m_pChangeActDir->setChecked( (bool)m_bChangeActDir );
    pDlg->ui.m_pShowOutputArea->setChecked( (bool)m_bShowOutputArea );
    pDlg->ui.m_pAutoSave->setChecked( (bool)m_bAutoSave );
    pDlg->ui.m_pPipeScriptToInterpreter->setChecked( (bool)m_bPipeScriptToInterpreter );
    pDlg->ui.m_pMeasureExecutionTime->setChecked( (bool)m_bMeasureExecutionTime );
    pDlg->ui.m_pBraceMatching->setChecked( (bool)m_bBraceMatching );
    pDlg->ui.m_pAutoCompleteThresh->setText( QString::number( m_iAutoCompletionThresh ) );
    pDlg->ui.m_pAutoClearOutput->setChecked( (bool)m_bAutoClearOutput );
    pDlg->ui.m_pFlagUtf8->setChecked( (bool)m_bIsDefaultUtf8 );
    pDlg->ui.m_pUseNewIcons->setChecked( m_bUseNewIcons );
    pDlg->ui.m_pUseToolbar->setChecked( m_bUseToolbar );
    pDlg->ui.m_pUseUserInterpreter->setChecked( m_bUseUserInterpreter );
    pDlg->ui.m_pCloseButtonInTabHeader->setChecked( m_bCloseButtonInTabPage );

    pDlg->ui.m_pAutoCompletion->setText( m_sAutoCompletionFileName );

    if( m_iShowStartupFileDialog==0 )
    {
        pDlg->ui.m_pShowFileDialog->setChecked( false );
    }
    else
    {
        pDlg->ui.m_pShowFileDialog->setChecked( true );
    }
    if( m_bAutoFileLoad )
    {
        pDlg->ui.m_pAutoFileLoad->setChecked( true );
    }
    else
    {
        pDlg->ui.m_pAutoFileLoad->setChecked( false );
    }

    if( m_iEolMode==QsciScintilla::EolWindows )
    {
        pDlg->ui.m_pCRLFModus->setCurrentIndex( 0 );
    }
    else if( m_iEolMode==QsciScintilla::EolUnix )
    {
        pDlg->ui.m_pCRLFModus->setCurrentIndex( 1 );
    }
    else if( m_iEolMode==QsciScintilla::EolMac )
    {
        pDlg->ui.m_pCRLFModus->setCurrentIndex( 2 );
    }
    QString sIconSize = QString::number(m_iIconSize);
    int iIndex = pDlg->ui.m_pIconSize->findText(sIconSize);
    if( iIndex>=0 )
    {
        pDlg->ui.m_pIconSize->setCurrentIndex(iIndex);
    }

#if defined(Q_OS_ANDROID)
    //pDlg->ui.m_pExecuteModus->hide();
    //pDlg->adjustSize();
    pDlg->showMaximized();
#endif

    int iRet = pDlg->exec();

    if( iRet == 1 )
    {
        // ok, pressed
        m_sScriptCmd = FromMetaPath( pDlg->ui.m_pScriptCmd->text() );

        PushToList( m_sScriptCmd, m_aExternalProgsList, MAX_EXTERNAL_PROGRAMS );

        m_sScriptArguments = pDlg->ui.m_pScriptArguments->currentText();
        PushToList( m_sScriptArguments, m_aLastScriptArgumentsList, MAX_LAST_SEARCH_STRINGS );

        QString sExtension = pDlg->ui.btnAssignExtension->text().mid(2);
        int iScriptInterpreterConfigurationIndex = m_aUserInterpreters.GetIndexFor(m_sScriptCmd,0);
        if( iScriptInterpreterConfigurationIndex!=-1 )
        {
            // remove user interpreter item if the extension is removed !
            if( sExtension=="???" )
            {
                m_aUserInterpreters.removeAt(iScriptInterpreterConfigurationIndex);
            }
            else
            {
                m_aUserInterpreters[iScriptInterpreterConfigurationIndex].m_sFileExtension = sExtension;
            }
        }

        // do we need to create a new user interpreter item ?
        if( iScriptInterpreterConfigurationIndex==-1 && sExtension!="???" )
        {
            ScriptInterpreterConfiguration aNewUserInterpreterItem;
            aNewUserInterpreterItem.m_sInterpreterPath = m_sScriptCmd;
            aNewUserInterpreterItem.m_sInterpreterArguments = m_sScriptArguments;
            aNewUserInterpreterItem.m_sFileExtension = sExtension;
            m_aUserInterpreters.append(aNewUserInterpreterItem);
        }

        m_sAutoCompletionFileName = pDlg->ui.m_pAutoCompletion->text();

#if defined(Q_OS_ANDROID)
        m_bUseRootForFileDialog = pDlg->ui.m_pUseRootForFileDialog->isChecked();
#endif
        m_iSpacesForTabs = (int)pDlg->ui.m_pSpacesForTabs->isChecked();
        m_iShowCurrentLine = (int)pDlg->ui.m_pShowCurrentLine->isChecked();
        m_iShowWhitespaces = (int)pDlg->ui.m_pShowWhitespaces->isChecked();
        m_iShowLineNumbers = (int)pDlg->ui.m_pShowLineNumbers->isChecked();
        m_iAutoIndent = (int)pDlg->ui.m_pAutoIndent->isChecked();
        m_iCodeFolding = (int)pDlg->ui.m_pCodeFolding->isChecked();
        m_dOutputRatio = pDlg->ui.m_pOutputRatio->value();
        m_bInteractiveModus = pDlg->ui.m_pInteractionModus->isChecked();
        m_bChangeActDir = pDlg->ui.m_pChangeActDir->isChecked();
        bool bCurrentShowOutputArea = pDlg->ui.m_pShowOutputArea->isChecked();
        if( bCurrentShowOutputArea!=m_bShowOutputArea )
        {
            m_bShowOutputArea = bCurrentShowOutputArea;
            UpdateOutputArea();
        }
        m_bAutoSave = pDlg->ui.m_pAutoSave->isChecked();
        m_bPipeScriptToInterpreter = pDlg->ui.m_pPipeScriptToInterpreter->isChecked();
        m_bMeasureExecutionTime = pDlg->ui.m_pMeasureExecutionTime->isChecked();
        m_bBraceMatching = pDlg->ui.m_pBraceMatching->isChecked();
        m_bAutoClearOutput = pDlg->ui.m_pAutoClearOutput->isChecked();
        m_bIsDefaultUtf8 = pDlg->ui.m_pFlagUtf8->isChecked();
        m_bUseNewIcons = pDlg->ui.m_pUseNewIcons->isChecked();
        m_bUseToolbar = pDlg->ui.m_pUseToolbar->isChecked();
        m_bUseUserInterpreter = pDlg->ui.m_pUseUserInterpreter->isChecked();
        m_bCloseButtonInTabPage = pDlg->ui.m_pCloseButtonInTabHeader->isChecked();
        m_bAutoFileLoad = pDlg->ui.m_pAutoFileLoad->isChecked();
        m_iAutoCompletionThresh = pDlg->ui.m_pAutoCompleteThresh->text().toInt();
        int iMode = pDlg->ui.m_pCRLFModus->currentIndex();
        if( iMode == 0 )
        {
            m_iEolMode = QsciScintilla::EolWindows;
        }
        else if( iMode == 1 )
        {
            m_iEolMode = QsciScintilla::EolUnix;
        }
        else if( iMode == 2 )
        {
            m_iEolMode = QsciScintilla::EolMac;
        }
        QString sIconSize = pDlg->ui.m_pIconSize->currentText();
        m_iIconSize = sIconSize.toInt();
        // update the size of the icons
        if( m_pButtonBar )
        {
            m_pButtonBar->setIconSize(QSize(m_iIconSize,m_iIconSize));
        }

        m_iTabSpace = pDlg->ui.m_pTabSpace->text().toInt();
        ForAllDocumentOutput( m_iTabSpace, (void (OutputAreaMultiLineEdit::*)(int))&OutputAreaMultiLineEdit::setTabStopWidth );
        ForAllDocumentScripts( m_iTabSpace, &CodeEdit::setTabWidth );
        ForAllDocumentScripts( &CodeEdit::doRepaint );

        if( pDlg->ui.m_pShowFileDialog->isChecked() )
        {
            m_iShowStartupFileDialog = 1;
        }
        else
        {
            m_iShowStartupFileDialog = 0;
        }

        UpdateFlags();

        SaveInit();
    }
    delete pDlg;

    // Workaround for Windows...
    GetActScript()->setFocus();
}

void VisiScript::sltShowVisiscriptHelp()
{
    QUrl aUrl("http://www.mneuroth.de/projects/Visiscript.html");
    QDesktopServices::openUrl(aUrl);
}

void VisiScript::sltShowInterpreterHelp()
{
    QUrl aUrl("http://www.mneuroth.de/projects/Visiscript.html");   // default help link
    int iLanguageId = GetActScript()->languageID();

    // first check extension modules for help about interpreter
    bool bFound = false;
    foreach( IVisiScriptExtension * pExt, m_aExtensions )
    {
        QList<int> lstIds = pExt->getSupportedLanguageIds();
        if( lstIds.contains(iLanguageId) )
        {
            aUrl = QUrl( pExt->getHelpUrl() );
            bFound = true;
        }
    }

    if( !bFound )
    {
        if( iLanguageId==LANG_JAVASCRIPT )
        {
            aUrl = QUrl("http://qt-project.org/doc/qt-5/ecmascript.html");
        }
        else if( iLanguageId==LANG_PYTHON )
        {
            aUrl = QUrl("http://docs.python.org");
        }
        else if( iLanguageId==LANG_LUA )
        {
            aUrl = QUrl("http://www.lua.org/manual");
        }
        else if( iLanguageId==LANG_RUBY )
        {
            aUrl = QUrl("http://ruby-doc.org/");
        }
        else if( iLanguageId==LANG_SCHEME )
        {
            aUrl = QUrl("http://s48.org/1.9.1/manual/manual-Z-H-1.html#node_toc_start");
        }
    }

    QDesktopServices::openUrl(aUrl);
}

void VisiScript::sltShowAbout()
{
    AboutDlgImpl * pDlg = new AboutDlgImpl( this );

    pDlg->adjustSize();

#ifndef _WITH_QSCINTILLA
    QString sSupportedBy = tr("powerd by Qt ")+QString(QT_VERSION_STR);
    //sSupportedBy += tr(" and <a href=http://pyqt.sourceforge.net/Docs/QScintilla2/>QScintilla</a> ")+QString(QSCINTILLA_VERSION_STR);
    pDlg->ui.m_pPowerdBy->setText("<html><head/><body><p>"+sSupportedBy+"</p></body></html>");
#else
    pDlg->ui.m_pPowerdBy->setText(pDlg->ui.m_pPowerdBy->text().arg(QString(QT_VERSION_STR)).arg(QString(QSCINTILLA_VERSION_STR)));
#endif
    pDlg->ui.m_pVersionLabel->setText(tr("Version: ")+QString(VISISCRIPT_VERSION_STR)+" from: "+__DATE__);

#if defined(Q_OS_ANDROID)
    pDlg->showMaximized();
#endif
    /*int iRet =*/ pDlg->exec();

    delete pDlg;
}

void VisiScript::sltShowExtensions()
{
    ExtensionsDialog * pDlg = new ExtensionsDialog( m_aExtensions, this );

    pDlg->adjustSize();

#if defined(Q_OS_ANDROID)
    pDlg->showMaximized();
#endif
    /*int iRet =*/ pDlg->exec();

    delete pDlg;
}

void VisiScript::sltUpdateLastFilesMenu()
{
    m_pLastOpendFiles->clear();
    m_aMenuLastFiles.clear();

    for( int i=0; i<m_aLastFilesList.count(); i++ )
    {
        QString sLastFileName = m_aLastFilesList.at( m_aLastFilesList.count()-i-1 );

        // ** show filename with number

        sLastFileName = QString( "&" ) + QString::number( i ) + QString( " " ) + sLastFileName;

        QAction * pAction = m_pLastOpendFiles->addAction(sLastFileName);
        m_aMenuLastFiles.push_back(pAction);
    }
}

void VisiScript::sltLastFilesMenuTriggered(QAction * pAction)
{
    QString sFileName = pAction->text();

    sFileName = sFileName.right( sFileName.length()-3 );

    DoImportFile( sFileName );
}

void VisiScript::sltViewMarkActAsMain()
{
    CodeEdit * pActScript = GetActScript();
    if( pActScript->isMainFile() )
    {
        // das aktuelle Skript als main zuruecksetzen
        pActScript->setMainFile(false);
    }
    else
    {
        // erst alle Skripte als nicht main setzen
        ForAllDocumentScripts( false, &CodeEdit::setMainFile );

        // das aktuelle Skript als main markieren
        pActScript->setMainFile(true);
    }
    // und alles aktualisieren
    ForAllDocumentScripts( &CodeEdit::sltUpdate );
}

void VisiScript::sltUpdateChilds()
{
    UpdateFont( true );
}

void VisiScript::sltOutputDoubleClicked( int iPara, int iPos)
{
    Q_UNUSED(iPos);
    // search for the line tag in the output window and try to find a line-number
    // example for Python:   File "_tsf.xyz", line 3, in ?
    // example for ruby: Error: _tsf.xyz:2: undefined local variable or method `x' for #<Object:0x949618> (NameError)
    // example for perl: Error: Scalar found where operator expected at _tsf.xyz line 3, near "$x" ...
    QString strCont = GetActOutput()->toPlainText();
    const QString strLine = strCont.split("\n")[iPara];
    int iFoundFile = strLine.indexOf(c_strFileTag,0,/*case_sensitive*/Qt::CaseInsensitive);
    if( iFoundFile>=0 )
    {
        iFoundFile = iFoundFile + c_strFileTag.length();
    }
    else
    {
        // try it with the english text...
        iFoundFile = strLine.indexOf(c_strFileTagOrg,0,/*case_sensitive*/Qt::CaseInsensitive);
        if( iFoundFile>=0 )
        {
            iFoundFile = iFoundFile + c_strFileTagOrg.length();
        }
    }
    if( iFoundFile>=0 )
    {
        int i = FindSeparatorChar(strLine,iFoundFile,true);
        if( i>=0 )
        {
            int iIndex = 0;
            QString strNewFile = StripPath( strLine.mid(iFoundFile,i-iFoundFile) );
            // is the script allready loaded ?
            CodeEdit * pScript = GetScriptForFileName(strNewFile,&iIndex);
            if( pScript==0 )
            {
                // no, load it !
                DoImportFile( strNewFile );
            }
            else
            {
                // yes, activate the allready loaded script to update the cursor position
                m_pTabWidget->setCurrentIndex(iIndex);
            }
        }
    }
    int iFound = strLine.indexOf(c_strLineTag,0,/*case_sensitive*/Qt::CaseInsensitive);
    if( iFound>=0 )
    {
        iFound = iFound + c_strLineTag.length();
    }
    else
    {
        // try it with the english text...
        iFound = strLine.indexOf(c_strLineTagOrg,0,/*case_sensitive*/Qt::CaseInsensitive);
        if( iFound>=0 )
        {
            iFound = iFound + c_strLineTagOrg.length();
        }
    }
    if( iFound>=0 )
    {
        int i = FindSeparatorChar(strLine,iFound);
        if( i>=0 )
        {
            int iFoundPos = strLine.mid(iFound,i-iFound).toInt();
            int iLineNo = iFoundPos-1;
            CodeEdit * pScript = GetActScript();
            pScript->setFocus();
            pScript->setCursorPosition( iLineNo, 0 );
            // damit die selektierte Zeile niemals am Ende des Fensters steht !
            pScript->ensureLineVisible( iLineNo+5 );
            // select the complete line...
            pScript->setSelection( iLineNo, 0, iLineNo, pScript->text(iLineNo).length() );
        }
    }
}

void VisiScript::sltPositionChanged( int iLine, int iPos )
{
    CodeEdit * pActScript = GetActScript();
    bool bUtf8 = pActScript->isUtf8();
    int iLength = pActScript->length();
    int iTotalLines = pActScript->lines();
    QString s = QString(tr("line=%1/%3 pos=%2 bytes=%4 encoding=%5")).arg(iLine+1).arg(iPos+1).arg(iTotalLines).arg(iLength).arg(bUtf8 ? "utf-8" : "latin-1");

    //if( !pActScript->isNavigating() )
    {
        pActScript->GetNavigationStack().push_pos(iLine,iPos);
    }

    m_pStatusBarMsg->setText( s );
}

void VisiScript::sltSelectFont()
{
    QFont aFont( m_sFontName, m_iFontSize );

    QFontDialog aDlg(aFont,this);
    aDlg.setOption(QFontDialog::DontUseNativeDialog);
#if defined(Q_OS_ANDROID)
    aDlg.showMaximized();
#endif
    if( aDlg.exec()==QDialog::Accepted )
    {
        aFont = aDlg.selectedFont();

        m_sFontName = aFont.family();
        m_sFontStyle = aFont.styleHint();
        m_iFontSize = aFont.pointSize();
        UpdateFont( true );
        UpdateFlags();  // update width for linenumbers
        SaveInit();
    }
}

void VisiScript::sltZoomIn()
{
    m_iFontSize++;
    UpdateFont( true );
    SaveInit();
}

void VisiScript::sltZoomOut()
{
    m_iFontSize--;
    UpdateFont( true );
    SaveInit();
}

void VisiScript::sltMaximizeScript()
{
    SetOutputSizeForAll( c_iMinOutputRatio );
}

void VisiScript::sltMaximizeOutput()
{
    SetOutputSizeForAll( c_iMaxOutputRatio );
}

void VisiScript::sltSplitScriptOutput()
{
    SetOutputSizeForAll( m_dOutputRatio );
}

void VisiScript::sltSwitchToTab()
{
    SelectFromListDlg aDlg(this);

#if defined(Q_OS_ANDROID)
    aDlg.showMaximized();
#else
    //aDlg.adjustSize();
#endif

    for( int i=0; i<(int)GetDocumentCount(); i++ )
    {
        CodeEdit * pScript = GetScriptFor( i );
        aDlg.AddItem( pScript->onlyFileName(), i );
    }
    aDlg.SelectItem( GetActScript()->onlyFileName() );

    if( aDlg.exec()==1 )
    {
        int iIndex = aDlg.GetSelectedItemIndex();
        if( iIndex>=0 )
        {
            m_pTabWidget->setCurrentIndex(iIndex);
        }
    }
}

void VisiScript::sltSwitchToLastTab()
{
    if( m_iLastPageIndex>=0 )
    {
        int iActPageIndex = m_pTabWidget->currentIndex();
        m_pTabWidget->setCurrentIndex(m_iLastPageIndex);
        m_iLastPageIndex = iActPageIndex;
    }
}

void VisiScript::sltToggleWordWrap()
{
    if( !m_pToggleWordWrap->isChecked() )
    {
        ForAllDocumentScripts( false, &CodeEdit::setWordWrap );
        m_iWordWrap = 0;
    }
    else
    {
        ForAllDocumentScripts( true, &CodeEdit::setWordWrap );
        m_iWordWrap = 1;
    }
}

void VisiScript::sltToggleOverwrite()
{
    if( !m_pToggleOverwrite->isChecked() )
    {
        ForAllDocumentScripts( false, &CodeEdit::setOverwriteMode );
        m_iOverwrite = 0;
    }
    else
    {
        ForAllDocumentScripts( true, &CodeEdit::setOverwriteMode );
        m_iOverwrite = 1;
    }
}

void VisiScript::sltCodeFolding()
{
    GetActScript()->foldAll();
}

QFont VisiScript::GetCurrentFont()
{
    QFontDatabase aFontDB;
    int iWeight = aFontDB.weight( m_sFontName, m_sFontStyle );
    bool bItalic = aFontDB.italic( m_sFontName, m_sFontStyle );
    QFont aTempFont( m_sFontName, m_iFontSize, iWeight, bItalic );
    return aTempFont;
}

void VisiScript::UpdateFont( CodeEdit * pScript, OutputAreaMultiLineEdit * pOutput )
{
    QFont aTempFont = GetCurrentFont();

    UpdateFont( pScript, pOutput, aTempFont );
}

void VisiScript::UpdateFont( CodeEdit * pScript, OutputAreaMultiLineEdit * pOutput, QFont & aFont )
{
    if( pScript )
    {
        pScript->updateFontForLexer( aFont );
        pScript->setFont( aFont );
        pScript->setMarginsFont( aFont );
    }
    if( pOutput )
    {
        pOutput->setFont( aFont );
    }
}

void VisiScript::UpdateFont( bool bForAllDocuments )
{
    QFont aTempFont = GetCurrentFont();

    if( bForAllDocuments )
    {
        for( int i=0; i<GetDocumentCount(); i++ )
        {
            UpdateFont( GetScriptFor( i ), GetOutputFor( i ), aTempFont );
        }
    }
    else
    {
        UpdateFont( GetActScript(), GetActOutput(), aTempFont );
    }
}

void VisiScript::SetAutoCompletionSource( QsciScintilla::AutoCompletionSource aSrc, int iThresh, const QString & sAPIFileName )
{
    ForAllDocumentScripts( aSrc, &CodeEdit::setAutoCompletionSource );
    ForAllDocumentScripts( iThresh, &CodeEdit::setAutoCompletionThreshold );
    ForAllDocumentScripts( sAPIFileName, &CodeEdit::setAPIFile );
}

void VisiScript::UpdateFlags()
{
    // restore values form the init/configuration file
    if( m_iWordWrap==0 )
    {
        m_pToggleWordWrap->setChecked( false );
        ForAllDocumentScripts( false, &CodeEdit::setWordWrap );
    }
    else
    {
        m_pToggleWordWrap->setChecked( true );
        ForAllDocumentScripts( true, &CodeEdit::setWordWrap );
    }
    if( m_iOverwrite==0 )
    {
        m_pToggleOverwrite->setChecked( false );
        ForAllDocumentScripts( false, &CodeEdit::setOverwriteMode );
    }
    else
    {
        m_pToggleOverwrite->setChecked( true );
        ForAllDocumentScripts( true, &CodeEdit::setOverwriteMode );
    }
    if( m_iSpacesForTabs==0 )
    {
        ForAllDocumentScripts( true, &CodeEdit::setIndentationsUseTabs );
    }
    else
    {
        ForAllDocumentScripts( false, &CodeEdit::setIndentationsUseTabs );
    }
    if( m_iShowWhitespaces==0 )
    {
        ForAllDocumentScripts( QsciScintilla::WsInvisible, &CodeEdit::setWhitespaceVisibility );
        ForAllDocumentScripts( false, &CodeEdit::setEolVisibility );
    }
    else
    {
        ForAllDocumentScripts( QsciScintilla::WsVisible, &CodeEdit::setWhitespaceVisibility );
        ForAllDocumentScripts( true, &CodeEdit::setEolVisibility );
    }
    if( m_iShowCurrentLine==0 )
    {
        ForAllDocumentScripts( false, &CodeEdit::setCaretLineVisible );
    }
    else
    {
        ForAllDocumentScripts( true, &CodeEdit::setCaretLineVisible );
    }
    if( m_iShowLineNumbers==0 )
    {
        ForAllDocumentScripts( 1, true, &CodeEdit::setMarginLineNumbers );
        ForAllDocumentScripts( 1, 0, (void (CodeEdit::*)(int,int))&CodeEdit::setMarginWidth );
    }
    else
    {
        ForAllDocumentScripts( 1, true, &CodeEdit::setMarginLineNumbers );
        ForAllDocumentScripts( 1, QString("99999"), (void (CodeEdit::*)(int,const QString &))&CodeEdit::setMarginWidth );
    }
    if( m_iAutoIndent==0 )
    {
        ForAllDocumentScripts( false, &CodeEdit::setAutoIndent );
    }
    else
    {
        ForAllDocumentScripts( true, &CodeEdit::setAutoIndent );
    }
    if( m_iCodeFolding==0 )
    {
        ForAllDocumentScripts( QsciScintilla::NoFoldStyle, 2, &CodeEdit::setFolding );
    }
    else
    {
        ForAllDocumentScripts( QsciScintilla::BoxedTreeFoldStyle, 2, &CodeEdit::setFolding );
    }
    if( m_bBraceMatching )
    {
        ForAllDocumentScripts( QsciScintilla::SloppyBraceMatch, &CodeEdit::setBraceMatching );
        ForAllDocumentScripts( Qt::blue, &CodeEdit::setMatchedBraceForegroundColor );
        ForAllDocumentScripts( Qt::red, &CodeEdit::setUnmatchedBraceForegroundColor );
    }
    else
    {
        ForAllDocumentScripts( QsciScintilla::NoBraceMatch, &CodeEdit::setBraceMatching );
    }
    if( !m_sAutoCompletionFileName.isEmpty() )
    {
        if( m_sAutoCompletionFileName == "none" || m_sAutoCompletionFileName == "0" || m_sAutoCompletionFileName == "-1" )
        {
            SetAutoCompletionSource( QsciScintilla::AcsAll, -1, QString::null );
        }
        else
        {
            SetAutoCompletionSource( QsciScintilla::AcsAll, m_iAutoCompletionThresh, m_sAutoCompletionFileName );
        }
    }
    else
    {
        SetAutoCompletionSource( QsciScintilla::AcsAll, m_iAutoCompletionThresh, QString::null );
    }
    ForAllDocumentScripts( (QsciScintilla::EolMode)m_iEolMode, &CodeEdit::setEolMode );
    // BUG: WARNING: im "Windows" Modus werden die Dateien 'geaendert' !
    ForAllDocumentScripts( (QsciScintilla::EolMode)m_iEolMode, &CodeEdit::convertEols );
}

void VisiScript::UpdateLanguageForCurrent( int iActLanguageID )
{
    CodeEdit * pScript = GetActScript();
    pScript->updateLanguage( iActLanguageID );
    // update the language extension map:
    m_aExtensionToLanguageMap[pScript->extension()] = iActLanguageID;
    UpdateFont( pScript, 0 );
}

void VisiScript::InsertInSelection( bool bDoInsert, const QString & sToken, const QString & sTokenStop )
{
    CodeEdit * pScript = GetActScript();

    if( pScript && pScript->hasSelectedText() )
    {
        int iFromLine,iFromPos,iToLine,iToPos;
        int iTokenLen = sToken.length();
        int iTokenStopLen = sTokenStop.length();
        bool bHasStopToken = iTokenStopLen>0;

        pScript->getSelection( &iFromLine, &iFromPos, &iToLine, &iToPos );

        pScript->beginUndoAction();
        for( int i=iFromLine; i<=iToLine; i++ )
        {
            int iStartIndex = 0;
            if( i==iFromLine )
            {
                iStartIndex = iFromPos;
            }
            if( i==iToLine && iToPos==0 )
            {
                break;
            }
            if( bDoInsert )
            {
                pScript->insertAt( sToken, i, iStartIndex );
                if( bHasStopToken )
                {
                    pScript->insertAt( sTokenStop, i, pScript->text(i).length()-1 );
                }
            }
            else
            {
                QString s = pScript->text( i );
                if( bHasStopToken && s.mid( s.length()-iTokenStopLen-1, iTokenStopLen ) == sTokenStop )
                {
                    pScript->setSelection( i, s.length()-iTokenStopLen-1, i, s.length()-1 );    // -1 because of new line
                    pScript->removeSelectedText();
                }
                s = pScript->text( i );
                if( s.mid( iStartIndex, iTokenLen ) == sToken )
                {
                    pScript->setSelection( i, iStartIndex, i, iStartIndex+iTokenLen );
                    pScript->removeSelectedText();
                }
            }
        }
        pScript->endUndoAction();

        pScript->setSelection( iFromLine, iFromPos, iToLine, iToPos );
    }
}

int VisiScript::GetHeightForOutput( double dPercent )
{
    int iValue = (int)((double)height()*dPercent/100.0);
    return iValue;
}

void VisiScript::PushLastFileName( const QString & sName )
{
    PushToList( sName, m_aLastFilesList, MAX_LAST_FILES );
}

bool VisiScript::ExistsLastFileName( const QString & sName )
{
    return ExistsInList( sName, m_aLastFilesList );
}

bool VisiScript::DoImportFile( const QString & sFileName, int x, int y, int iLastEncodingUtf8 )
{
    bool bProcessed = false;

    if( !sFileName.isNull() )
    {
        AddDocument( sFileName );

        QPair<bool,bool> bPeekedIsUtf8;
        QString sText;

        CodeEdit * pActScript = GetActScript();

        // call extension callbacks
        foreach( IVisiScriptExtension * pExtension, m_aExtensions )
        {
            QString sSymbol;
            bProcessed = pExtension->Callback_ReadFile( pActScript, sFileName, sText, (bool)iLastEncodingUtf8, sSymbol );
            if( bProcessed )
            {
                pActScript->setUserSymbol(sSymbol);
                break;
            }
        }

        if( !bProcessed )
        {
            bProcessed = ReadTextFile( sFileName, sText, (bool)iLastEncodingUtf8, &bPeekedIsUtf8 );
        }
        if( bProcessed )
        {
            pActScript->setUtf8( bPeekedIsUtf8.first ? bPeekedIsUtf8.second :  (bool)iLastEncodingUtf8 );
            pActScript->setText( sText );
            pActScript->setEdited( false );
            pActScript->setCursorPosition(y,x);
            //pActScript->ensureLineVisible(y);
            //pActScript->ensureCursorVisible();
            pActScript->horizontalScrollBar()->setValue(0);
            pActScript->setFileName(sFileName);
            //UpdateCaption( sFileName+" #" );

            PushLastFileName( sFileName );
        }
        else
        {
            // because some problems reading the file occured --> close the just opend edit tab page
            sltCloseActDocument();
        }
    }
    return bProcessed;
}

bool VisiScript::ImportText(const QString &sTitleName, const QString &sText)
{
    m_bUseFile = false;
    AddDocument( sTitleName );

    CodeEdit * pActScript = GetActScript();

    pActScript->setUtf8( 1 );
    pActScript->setText( sText );
    pActScript->setEdited( false );
    pActScript->setCursorPosition(0,0);
    //pActScript->ensureLineVisible(y);
    //pActScript->ensureCursorVisible();
    pActScript->horizontalScrollBar()->setValue(0);
   // pActScript->setFileName(sFileName);
    UpdateCaption( sTitleName);

    //PushLastFileName( sFileName );
    return true;
}

bool VisiScript::DoExportFile( CodeEdit * pScript, bool bShowError )
{
    bool bOk = false;
    QString sText = pScript->text();
    pScript->setIsWriting(true);

    // call extension callbacks
    bool bProcessed = false;
    foreach( IVisiScriptExtension * pExtension, m_aExtensions )
    {
        bProcessed = pExtension->Callback_WriteFile( pScript, pScript->fileName(), sText, pScript->isUtf8() );
        if( bProcessed )
        {
            break;
        }
    }

    if( !bProcessed )
    {
        bOk = WriteTextFile( pScript->fileName(), sText, /*append=*/false, pScript->isUtf8() );
    }

    if( bOk )
    {
        pScript->setEdited(false);
        //pScript->setIsWriting(false);  // will be done in event handling of FileSystemWatcher
    }

    if( bShowError && !bOk )
    {
        QMessageBox::warning(this,tr("Warning"),QString(tr("Error writing file %1 !")).arg(pScript->fileName()));
    }

    return bOk;
}

void VisiScript::SetOutputSize( OutputAreaMultiLineEdit * pOutput, double dPercentSize )
{
    QSplitter * pSplitter = (QSplitter *)pOutput->parentWidget();
    QList<int> aSizes;
    aSizes.push_back(GetHeightForOutput( 100.0-dPercentSize ));
    aSizes.push_back(GetHeightForOutput( dPercentSize ));
    pSplitter->setSizes(aSizes);
}

void VisiScript::SetOutputSizeForAll( double dPercentSize )
{
    for( int i=0; i<GetDocumentCount(); i++ )
    {
        SetOutputSize( GetOutputFor( i ), dPercentSize );
    }
}

void VisiScript::sltEditUndo()
{
    CodeEdit * pActScript = GetActScript();

    pActScript->undo();

    pActScript->setEdited( pActScript->isModified() );
}

void VisiScript::sltEditRedo()
{
    CodeEdit * pActScript = GetActScript();

    pActScript->redo();

    pActScript->setEdited( pActScript->isModified() );
}

void VisiScript::sltEditCut()
{
    CodeEdit * pActScript = GetActScript();
    if( pActScript->hasSelectedText() )
    {
        pActScript->cut();
    }
}

void VisiScript::sltEditForward()
{
    CodeEdit * pActScript = GetActScript();
    pActScript->setFocus();
    pActScript->forward();
}

void VisiScript::sltEditBackward()
{
    CodeEdit * pActScript = GetActScript();
    pActScript->setFocus();
    pActScript->backward();
}

void VisiScript::sltEditCopy()
{
    CodeEdit * pActScript = GetActScript();
    if( pActScript->hasSelectedText() )
    {
        pActScript->copy();
    }
    else if( GetActOutput()->hasMarkedTxt() )
    {
        GetActOutput()->copy();
    }
}

void VisiScript::sltEditPaste()
{
    GetActScript()->paste();
}

void VisiScript::sltEditSelectAll()
{
    GetActScript()->selectAll(true);
}

void VisiScript::sltClearOutput()
{
    GetActOutput()->setText( "" );
}

void VisiScript::sltKillOrClearOutput()
{
    if( isScriptRunning() )
    {
        sltKill();
    }
    else
    {
        sltClearOutput();
    }
}

void VisiScript::sltCommentSelection()
{
    QPair<QString,QString> aCommentToken = GetCommentTokenForLine( GetActScript()->languageID() );
    InsertInSelection( true, aCommentToken.first, aCommentToken.second );
}

void VisiScript::sltUnCommentSelection()
{
    QPair<QString,QString> aCommentToken = GetCommentTokenForLine( GetActScript()->languageID() );
    InsertInSelection( false, aCommentToken.first, aCommentToken.second );
}

QString VisiScript::GetStringForTab() const
{
    if( m_iSpacesForTabs )
    {
        QString s;
        /*s =*/ s.fill( ' ', m_iTabSpace );
        return s;
    }
    else
    {
        return "\t";
    }
}

void VisiScript::sltTabWithSelectedTextPressed()
{
    CodeEdit * pScript = GetActScript();

    if( pScript && pScript->hasSelectedText() )
    {
        InsertInSelection( true, GetStringForTab(), "" );
    }
}

void VisiScript::sltShiftTabWithSelectedTextPressed()
{
    CodeEdit * pScript = GetActScript();

    if( pScript && pScript->hasSelectedText() )
    {
        InsertInSelection( false, GetStringForTab(), "" );
    }
}

void VisiScript::sltAddToOutput( const QString & sText )
{
    GetActOutput()->setPlainText(GetActOutput()->toPlainText()+sText);
    GetActOutput()->moveCursor( QTextCursor::End );
}

void VisiScript::sltAddLineToOutput( const QString & sLine )
{
    sltAddToOutput( sLine+QString("\n") );
}

QString VisiScript::GetCurrentWordUnderCursor() const
{
    int iLine,iIndex;
    // analyze the word under the cursor for default value in find dialog
    CodeEdit * pActScript = GetActScript();
    pActScript->getCursorPosition(&iLine,&iIndex);
    QString s = pActScript->text(iLine);
    if( s.length()>0 )
    {
        if(iIndex>=s.length() && s.length()>0)      // bugfix 10.1.2010: handle crash if cursor is at the end of the document
        {
            iIndex = s.length()-1;
        }
        if(!s[iIndex].isSpace())
        {
            return GetWordForPosition(s,iIndex);		// .section() with regexpr
        }
        else if( iIndex>0 && !s[iIndex-1].isSpace() )   // handle situation where cursor is at the end of a word
        {
            return GetWordForPosition(s,iIndex-1);		// .section() with regexpr
        }
    }
    return m_sLastFindText;
}

bool VisiScript::DoFindFirst( CodeEdit * pActScript, int iLine, int iIndex )
{
    bool bFound = pActScript->findFirst( m_sLastFindText,
                               m_bRegularExpr,
                               m_bCaseSensitive,
                               m_bOnlyWholeWords,
                               /*bWrap*/false,
                               !m_bSearchBackward,
                               iLine, iIndex );

    if( !bFound )
    {
        /*int iRet =*/ QMessageBox::information( this, tr( "Find Text" ), tr( "Text was not found!" ), QMessageBox::Ok );
    }

    return bFound;
}

template <class T>
T * GetInterface(QList<IVisiScriptExtension *> aExensions)
{
    foreach( IVisiScriptExtension * pExtension, aExensions )
    {
        T* pRet = dynamic_cast<T *>(pExtension);
        if( pRet!=0 )
        {
            return pRet;
        }
    }
    return 0;
}

void VisiScript::sltEditFindText()
{
    IFindInFilesService * pFindInFileService = GetInterface<IFindInFilesService>(m_aExtensions);

    if( m_pFindDlg==0 )
    {
        m_pFindDlg = new FindDlgImpl( this );
        m_pFindDlg->ui.m_pShowAllOccurrences->setVisible( pFindInFileService!=0 );
#if defined(Q_OS_ANDROID)
        m_pFindDlg->ui.m_pFindText->setBaseSize(0,c_iComboBoxHeight);
        m_pFindDlg->ui.m_pFindText->setMinimumHeight(c_iComboBoxHeight);
        m_pFindDlg->showMaximized();
#else
        m_pFindDlg->adjustSize();
#endif
    }


    // update the combo box for the old search strings
    m_pFindDlg->ui.m_pFindText->clear();
    // change order because last used string was pushed to the end of the list but should apear at the beginning of the list !
    for( int i=m_aLastSearchStringList.count()-1; i>=0; i-- )
    {
        m_pFindDlg->ui.m_pFindText->addItem( m_aLastSearchStringList.at( i ) );
    }
    m_pFindDlg->ui.m_pFindText->setCompleter(0);		// 30.8.2009: be compatible with old implementation in Qt3

    CodeEdit * pActScript = GetActScript();

    // the following code block is very similar to the code in find and replace dialog --> NOT DRY code !
    // if any text is marked, use this text for searching
    if( pActScript->hasSelectedText() )
    {
        m_pFindDlg->ui.m_pFindText->setEditText( pActScript->selectedText() );
        m_pFindDlg->ui.m_pFindText->lineEdit()->selectAll();
    }
    else
    {
        m_pFindDlg->ui.m_pFindText->setEditText( GetCurrentWordUnderCursor() );
        m_pFindDlg->ui.m_pFindText->lineEdit()->selectAll();
    }
    m_pFindDlg->ui.m_pRegularExpr->setChecked( m_bRegularExpr );
    m_pFindDlg->ui.m_pCaseSensitive->setChecked( m_bCaseSensitive );
    m_pFindDlg->ui.m_pOnlyWholeWord->setChecked( m_bOnlyWholeWords );
    m_pFindDlg->ui.m_pSearchBackward->setChecked( m_bSearchBackward );
    m_pFindDlg->ui.m_pShowAllOccurrences->setChecked( m_bShowAllOccurrences );

    m_pFindDlg->ui.m_pFindText->setFocus(Qt::PopupFocusReason);

    int iRet = m_pFindDlg->exec();

    if( iRet == 1 )
    {
        m_bIsReplace = false;

        m_sLastFindText = m_pFindDlg->ui.m_pFindText->currentText();
        m_bRegularExpr = m_pFindDlg->ui.m_pRegularExpr->isChecked();
        m_bCaseSensitive = m_pFindDlg->ui.m_pCaseSensitive->isChecked();
        m_bOnlyWholeWords = m_pFindDlg->ui.m_pOnlyWholeWord->isChecked();
        m_bSearchBackward = m_pFindDlg->ui.m_pSearchBackward->isChecked();
        if( pFindInFileService!=0 )
        {
            m_bShowAllOccurrences = m_pFindDlg->ui.m_pShowAllOccurrences->isChecked();
            if( m_bShowAllOccurrences )
            {
                pFindInFileService->GetAllOccurencesInFile( GetActScript()->fileName(), m_sLastFindText, m_bCaseSensitive, m_bOnlyWholeWords, m_bRegularExpr );
            }
        }

        // update the list of the last searched items
        PushToList( m_sLastFindText, m_aLastSearchStringList, MAX_LAST_SEARCH_STRINGS );

        DoFindFirst(pActScript);
    }
#if defined( Q_OS_ANDROID )
    delete m_pFindDlg;
    m_pFindDlg = 0;
#endif
}

void VisiScript::sltEditFindNext()
{
    CodeEdit * pActScript = GetActScript();

    bool bFound = pActScript->findNext();

    if( m_bIsReplace && bFound )
    {
        pActScript->replace( m_sReplaceText );
    }

    if( !bFound )
    {
        int iRet = QMessageBox::information( this, tr( "Find Text" ),
                                             tr( "Searched to the end of the text.\nStart search again?" ),
                                             QMessageBox::Yes, QMessageBox::No );

        if( iRet == QMessageBox::Yes )
        {
            bFound = DoFindFirst( pActScript, 0, 0 );
            if( m_bIsReplace && bFound )
            {
                pActScript->replace( m_sReplaceText );
            }
        }
    }
}

void VisiScript::sltEditReplaceText()
{
    IFindInFilesService * pFindInFileService = GetInterface<IFindInFilesService>(m_aExtensions);

    if( m_pReplaceDlg==0 )
    {
        m_pReplaceDlg = new ReplaceDlgImpl( this );
        m_pReplaceDlg->ui.m_pShowAllOccurrences->setVisible( pFindInFileService!=0 );
#if defined(Q_OS_ANDROID)
        m_pReplaceDlg->ui.m_pReplaceText->setBaseSize(0,c_iComboBoxHeight);
        m_pReplaceDlg->ui.m_pReplaceText->setMinimumHeight(c_iComboBoxHeight);
        m_pReplaceDlg->ui.m_pFindText->setBaseSize(0,c_iComboBoxHeight);
        m_pReplaceDlg->ui.m_pFindText->setMinimumHeight(c_iComboBoxHeight);
        m_pReplaceDlg->showMaximized();
#else
        m_pReplaceDlg->adjustSize();
#endif
    }

    // update the combo box for the old search strings
    m_pReplaceDlg->ui.m_pFindText->clear();
    m_pReplaceDlg->ui.m_pReplaceText->clear();
    int i = 0;
    for( i=m_aLastSearchStringList.count()-1; i>=0; i-- )
    {
        m_pReplaceDlg->ui.m_pFindText->addItem( m_aLastSearchStringList.at( i ) );
    }
    for( i=m_aLastReplaceStringList.count()-1; i>=0; i-- )
    {
        m_pReplaceDlg->ui.m_pReplaceText->addItem( m_aLastReplaceStringList.at( i ) );
    }
    m_pReplaceDlg->ui.m_pFindText->setCompleter(0);		// 30.8.2009: be compatible with old implementation in Qt3

    CodeEdit * pActScript = GetActScript();

    // the following code block is very similar to the code in find text dialog --> NOT DRY code !
    // if any text is marked, use this text for searching
    if( pActScript->hasSelectedText() )
    {
        m_pReplaceDlg->ui.m_pFindText->setEditText( pActScript->selectedText() );
        m_pReplaceDlg->ui.m_pFindText->lineEdit()->selectAll();
    }
    else
    {
        m_pReplaceDlg->ui.m_pFindText->setEditText( GetCurrentWordUnderCursor() );
        m_pReplaceDlg->ui.m_pFindText->lineEdit()->selectAll();
    }
    m_pReplaceDlg->ui.m_pReplaceText->setEditText( m_sReplaceText );
    m_pReplaceDlg->ui.m_pRegularExpr->setChecked( m_bRegularExpr );
    m_pReplaceDlg->ui.m_pCaseSensitive->setChecked( m_bCaseSensitive );
    m_pReplaceDlg->ui.m_pOnlyWholeWord->setChecked( m_bOnlyWholeWords );
    m_pReplaceDlg->ui.m_pSearchBackward->setChecked( m_bSearchBackward );
    m_pReplaceDlg->ui.m_pReplaceAll->setChecked( m_bReplaceAll );
    m_pReplaceDlg->ui.m_pShowAllOccurrences->setChecked( m_bShowAllOccurrences );

    m_pReplaceDlg->ui.m_pFindText->setFocus(Qt::PopupFocusReason);

    int iRet = m_pReplaceDlg->exec();

    if( iRet == 1 )
    {
        m_bIsReplace = true;

        m_sLastFindText = m_pReplaceDlg->ui.m_pFindText->currentText();
        m_sReplaceText = m_pReplaceDlg->ui.m_pReplaceText->currentText();
        m_bRegularExpr = m_pReplaceDlg->ui.m_pRegularExpr->isChecked();
        m_bCaseSensitive = m_pReplaceDlg->ui.m_pCaseSensitive->isChecked();
        m_bOnlyWholeWords = m_pReplaceDlg->ui.m_pOnlyWholeWord->isChecked();
        m_bSearchBackward = m_pReplaceDlg->ui.m_pSearchBackward->isChecked();
        m_bReplaceAll = m_pReplaceDlg->ui.m_pReplaceAll->isChecked();
        if( pFindInFileService!=0 )
        {
            m_bShowAllOccurrences = m_pReplaceDlg->ui.m_pShowAllOccurrences->isChecked();
            if( m_bShowAllOccurrences )
            {
                pFindInFileService->GetAllOccurencesInFile( GetActScript()->fileName(), m_sLastFindText, m_bCaseSensitive, m_bOnlyWholeWords, m_bRegularExpr );
            }
        }

        // update the list of the last searched items
        PushToList( m_sLastFindText, m_aLastSearchStringList, MAX_LAST_SEARCH_STRINGS );
        PushToList( m_sReplaceText, m_aLastReplaceStringList, MAX_LAST_SEARCH_STRINGS );

        bool bFound = DoFindFirst(pActScript);

        if( bFound )
        {
            bool bCacheValue = pActScript->isMarkAllOccurencesOfSelection();

            pActScript->beginUndoAction();
            pActScript->setMarkAllOccurencesOfSelection( false );

            pActScript->replace( m_sReplaceText );

            while( m_bReplaceAll && bFound )
            {
                bFound = pActScript->findNext();

                if( m_bIsReplace && bFound )
                {
                    pActScript->replace( m_sReplaceText );
                }
            }

            pActScript->setMarkAllOccurencesOfSelection( bCacheValue );
            pActScript->endUndoAction();

            // update mark all texts of selection
            pActScript->sltSelectionChanged();
        }
    }
#if defined( Q_OS_ANDROID )
    delete m_pReplaceDlg;
    m_pReplaceDlg = 0;
#endif
}

void VisiScript::sltEditGotoLine()
{
    bool bOk = false;
    int iActLine = 0;
    int iIndex = 0;
    CodeEdit * pActScript = GetActScript();
    pActScript->getCursorPosition(&iActLine,&iIndex);
    QString strLabel = QString(tr("Goto line (max. %1):")).arg(pActScript->lines());
    QString strTitle = tr("Goto line");
    int iLine = QInputDialog::getInt(this,strTitle,strLabel,iActLine+1,0,pActScript->lines(),1,&bOk);
    if( bOk )
    {
        pActScript->setFocus();
        pActScript->setCursorPosition(iLine-1,0);
    }
}

void VisiScript::ToggleRecordMacro( bool bStart )
{
    CodeEdit * pActScript = GetActScript();
    if( bStart )
    {
        m_bIsMacroRecording = true;
        statusBar()->showMessage( tr("Start recording macro..."), g_iStatusBarTimeoutInMs );
        m_pRecordMacro->setText(tr("Sto&p record macro"));
        pActScript->sltToggleRecordMacro( true );
    }
    else
    {
        m_bIsMacroRecording = false;
        statusBar()->showMessage( tr("Stop recording macro"), g_iStatusBarTimeoutInMs );
        m_pRecordMacro->setText(tr("S&tart record macro"));
        pActScript->sltToggleRecordMacro( false );
        m_sCurrentMacro = pActScript->getMacro();
    }
}

void VisiScript::sltToggleRecordMacro()
{
    ToggleRecordMacro( m_pRecordMacro->isChecked() );
}

void VisiScript::sltPlayMacro()
{
    CodeEdit * pActScript = GetActScript();
    pActScript->setMacro( m_sCurrentMacro );
    pActScript->sltPlayMacro();
}

void VisiScript::sltClearScript()
{
    CodeEdit * pActScript = GetActScript();
    if( CheckData( pActScript ) )
    {
        pActScript->setText( "" );
        pActScript->setEdited( false );
    }
}

void VisiScript::sltKill()
{
    bool bAborted = false;
    if( m_aScriptProcess.state()==QProcess::Running )
    {
        m_aScriptProcess.kill();
        bAborted = true;
    }
    if( m_pEngine!=0 )
    {
        m_pEngine->abortEvaluation();
        bAborted = true;
    }
    if( bAborted )
    {
        GetActOutput()->append(tr("script aborted !\n"));
    }
}

void VisiScript::sltRun()
{
    DoRun( /*bUseMainScript*/true );
}

void VisiScript::sltActRun()
{
    DoRun( /*bUseMainScript*/false );
}

void VisiScript::sltShowInterpreterVersion()
{
    DoRun( /*bUseMainScript*/false, true );
}

void VisiScript::WriteTempFileIfNeeded( const QString & sScript, bool bIsNotQScript, bool bUtf8 )
{
    if( !sScript.isNull() && m_bUseTempFile && bIsNotQScript )
    {
        // write the temporary file (if neccassary)
        if( !WriteTextFile( m_sTempFileName, sScript, /*append=*/false, bUtf8 ) )
        {
            GetActOutput()->append(QString(tr("Error: can not write temporary script file %1")).arg(m_sTempFileName));
        }
    }
}

bool VisiScript::isScriptRunning() const
{
    return (m_pEngine!=0 && m_pEngine->isEvaluating()) || (m_aScriptProcess.state()==QProcess::Running);
}

void VisiScript::EnableRunCommand( bool bEnable )
{
    m_pRunScript->setEnabled( bEnable );
}

void VisiScript::ProcessScriptFinished()
{
    CallFinishedScriptCallback();
    EnableRunCommand( true );
    if( m_bIsDebugging )
    {
        ConnectScriptProcessSignals();
        m_bIsDebugging = false;
    }
}

QString VisiScript::GetExecutionTimeStrg() const
{
    return QString(tr("Execution time: %1 ms")).arg(m_iElapsedTime)+"\n";
}

void VisiScript::CallFinishedScriptCallback()
{
    // ask extension modules for processing when script finished
    foreach( IExecuteScript * pExec, m_aExecuteScript )
    {
        pExec->ScriptFinished();
    }
}

void VisiScript::CallAboutToStartScriptCallback( const QString & sScriptFileName )
{
    // ask extension modules for processing when script wants to start execution
    foreach( IExecuteScript * pExec, m_aExecuteScript )
    {
        pExec->ScriptAboutToStart( sScriptFileName );
    }
}

bool VisiScript::CallProcessStdOutLineCallback( const QString & sLine )
{
    // ask extension modules for processing of output
    foreach( IExecuteScript * pExec, m_aExecuteScript )
    {
        if( pExec->ProcessStdOutLine( sLine ) )
        {
            return true;
        }
    }
    return false;
}

bool VisiScript::CallProcessStdErrLineCallback( const QString & sLine )
{
    // ask extension modules for processing of output
    foreach( IExecuteScript * pExec, m_aExecuteScript )
    {
        if( pExec->ProcessStdErrLine( sLine ) )
        {
            return true;
        }
    }
    return false;
}

int VisiScript::DoRun( bool bUseMainScript, bool bShowOnlyVersion, bool bDebug )
{
    CodeEdit * pRunScript = bUseMainScript ? GetActMainScript() : GetActScript();

    EnableRunCommand( false );

    bool    bUtf8       = pRunScript->isUtf8();
    bool    bModified	= pRunScript->isModified();
    QString sScript		= pRunScript->text();
    m_sTempFileName     = QString(_TEMP_SCRIPT_FILE)+"."+QFileInfo( pRunScript->fileName() ).suffix();
    QString strScriptFileName = m_sTempFileName;

    m_bUseTempFile = true;

    // handle auto save for script, if checked
    if( m_bAutoSave && bModified )
    {
        sltFileSave();
        bModified = pRunScript->isModified();
    }

    if( m_bAutoClearOutput )
    {
        sltClearOutput();
    }

    if( !bModified )
    {
        strScriptFileName = pRunScript->fileName();
        // we are using the original script file, do not remove this file after execution !
        m_bUseTempFile = false;
    }

    // do we have to change the actual directory before we execute the script ?
    if( m_bChangeActDir )
    {
        // save the actual directory
        m_sLastCurrentDir = QDir::currentPath();
        // get the path to directory of the actual script
        QString strScriptActDir = QFileInfo(pRunScript->fileName()).absolutePath();
        QDir::setCurrent(strScriptActDir);
    }
    else
    {
        m_sLastCurrentDir = QString();
    }

    QString sOutput;
    QString sFileExtension = QFileInfo(strScriptFileName).suffix();

    // GetScriptCmdFromFileExtension
    int iActLanguageID = GetLanguageIDFromExtension(sFileExtension,m_aExtensionToLanguageMap);
    ScriptInfo aScriptInfo = GetScriptCommandFromLanguageID(iActLanguageID,m_aExtensions);
    QString sCmd = aScriptInfo.sCmd;
    QString sVersion = aScriptInfo.sVersion;
    QStringList lstArguments = aScriptInfo.lstArguments;

    // after default mapping, check for user registered interpreters
    int iUserInterpreterIndex = m_aUserInterpreters.GetIndexFor(sFileExtension,2);
    if( iUserInterpreterIndex!=-1 )
    {
        const ScriptInterpreterConfiguration & aItem = m_aUserInterpreters[iUserInterpreterIndex];
        sCmd = aItem.m_sInterpreterPath;
        QStringList scriptArgs = SplitString(aItem.m_sInterpreterArguments);
        foreach( const QString & s, scriptArgs )
        {
            if( s.length()>0 )
            {
                lstArguments << s;
            }
        }
    }

    // then check if the user wants to use a specific interpreter
    QString sScriptCommand = m_bUseUserInterpreter ? m_sScriptCmd : sCmd;

    if( m_bMeasureExecutionTime )
    {
        sltAddLineToOutput( "Measure execution time for interpreter: "+(sScriptCommand.length()==0 ? "<none>" : sScriptCommand) );
    }

    if( sScriptCommand.length()==0 )
    {
        sltAddLineToOutput(QString(tr("Warning: no interpreter found for script type '%1' (id=%2)")).arg(GetActScript()->language()).arg(iActLanguageID));
        EnableRunCommand( true );
        return iActLanguageID;
    }

    CallAboutToStartScriptCallback( strScriptFileName );

    if( m_bUseUserInterpreter )
    {
        lstArguments.clear();
        QStringList scriptArgs = SplitString(m_sScriptArguments);
        foreach( const QString & s, scriptArgs )
        {
            if( s.length()>0 )
            {
                lstArguments << s;
            }
        }
    }

    WriteTempFileIfNeeded(sScript,sScriptCommand!=_QSCRIPT,bUtf8);

    m_iElapsedTime = 0;
    g_aScriptTimer.start();

    if( sScriptCommand == _QSCRIPT )
    {
        if( bShowOnlyVersion )
        {
            sOutput = "QScript of Qt version "+QString( qVersion() );
            EnableRunCommand( true );
        }
        else
        {
            if( m_pEngine == 0 )
            {
                m_pEngine = new QScriptEngine();

                if( bDebug )
                {
                    DisconnectScriptProcessSignals();

                    // ask extension modules for debugger interface
                    foreach( IDebugger * pDebugger, m_aDebugger )
                    {
                        if( pDebugger->AttachToQScriptEngine( m_pEngine ) )
                        {
                            break;
                        }
                    }
                }

                m_pEngine->setProperty("inputQueue",QVariant(QStringList()));

                // QScript has this symbols:
                //  print, input, clockms, sleep, app, app.GetActScriptText(), app.GetActOutputText(), app.AddToOutput()
                //  messageBox(title,text)
                QScriptValue printFcnValue = m_pEngine->newFunction(qscript_print);
                m_pEngine->globalObject().setProperty("print",printFcnValue);
                QScriptValue inputFcnValue = m_pEngine->newFunction(qscript_input);
                m_pEngine->globalObject().setProperty("input",inputFcnValue);
                QScriptValue clockFcnValue = m_pEngine->newFunction(qscript_clockms);
                m_pEngine->globalObject().setProperty("clockms",clockFcnValue);
                QScriptValue sleepFcnValue = m_pEngine->newFunction(qscript_sleep);
                m_pEngine->globalObject().setProperty("sleep",sleepFcnValue);
                QScriptValue setdefaultsizeFcnValue = m_pEngine->newFunction(qscript_setdefaultsize);
                m_pEngine->globalObject().setProperty("setDefaultSize",setdefaultsizeFcnValue);
                QScriptValue messageboxFcnValue = m_pEngine->newFunction(qscript_messagebox);
                m_pEngine->globalObject().setProperty("messageBox",messageboxFcnValue);

                // ask extension modules for additional qscript functions
                foreach( IExtendQScript * pExt, m_aExtendQScript )
                {
                    pExt->RegisterFunctions( m_pEngine );
                }

                if( m_pQScriptProxy==0 )
                {
                    m_pQScriptProxy = new QScriptProxy(this);
                }
                QScriptValue appValue = m_pEngine->newQObject(m_pQScriptProxy);
                m_pEngine->globalObject().setProperty("app",appValue);
// TODO ggf. hier fuer alle IVisiScript methoden registrieren --> http://qt-project.org/doc/qt-4.8/scripting.html
//                qScriptRegisterMetaType(m_pEngine,);
            }

            if( !m_pEngine->isEvaluating() )
            {
                QScriptValue result = m_pEngine->evaluate(sScript);
                sOutput = QString(tr("QScript return="))+result.toString();
                if (m_pEngine->hasUncaughtException())
                {
                     int line = m_pEngine->uncaughtExceptionLineNumber();
                     sOutput += "\n";
                     sOutput += QString(tr("Error: uncaught exception at line %1 %2\n")).arg(line).arg(result.toString());
                }
                // remove (unused) temp file, QScripts are exceucted synchroniously !
                CheckTempFile();

                if( bDebug )
                {
                    // ask extension modules for debugger interface
                    foreach( IDebugger * pDebugger, m_aDebugger )
                    {
                        if( pDebugger->DetachFromQScriptEngine( m_pEngine ) )
                        {
                            break;
                        }
                    }
                }

                // destroy script engine
                delete m_pEngine;
                m_pEngine = 0;

                m_iElapsedTime = g_aScriptTimer.elapsed();
                if( m_bMeasureExecutionTime )
                {
                    sOutput += GetExecutionTimeStrg();
                }

                ProcessScriptFinished();

                // Workaround for Mac ?
                GetActScript()->setFocus();
            }
            else
            {
                sOutput += tr("Warning: could not start script because script is already running !");
            }
        }
    }
    else
    {
        if( m_aScriptProcess.state()==QProcess::NotRunning )
        {
            bool bPipeScriptToStdInput = aScriptInfo.bNeedPipeWriting;
            QStringList args;

            if( !bShowOnlyVersion )
            {
                if( bDebug )
                {
                    DisconnectScriptProcessSignals();

                    // ask extension modules for debugger interface
                    foreach( IDebugger * pDebugger, m_aDebugger )
                    {
                        if( pDebugger->canDebug( iActLanguageID ) )
                        {
                            args += pDebugger->getDebugInterpreterArguments( iActLanguageID );
                        }
                    }
                }

                foreach( const QString & sArg, lstArguments )
                {
                    if( sArg.length()>0 )
                    {
                        args << sArg;
                    }
                }

                if( !bPipeScriptToStdInput )
                {
                    args << strScriptFileName;              // AddStringKommasIfNeededAndConvertSeparators(strScriptFileName);
                }
            }
            else
            {
                args << sVersion;
            }

            // set system environment to find all available script engines
            QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
            m_aScriptProcess.setProcessEnvironment(env);

            QFileInfo aInfo(sScriptCommand);
            m_aScriptProcess.setWorkingDirectory(aInfo.absolutePath());
            m_aScriptProcess.start(sScriptCommand,args,QIODevice::Unbuffered|QIODevice::ReadWrite);

            bool ok = m_aScriptProcess.waitForStarted();    // AddStringKommasIfNeededAndConvertSeparators

            if( ok && bPipeScriptToStdInput )
            {
                m_aScriptProcess.write( sScript.toLocal8Bit() );
            }
        }
        else
        {
            sOutput += tr("Warning: could not start script because script is already running !");
        }
    }

    // show the output of the script in any case
    if( sOutput.length()>0 )
    {
        sltAddLineToOutput( sOutput );
    }

    return iActLanguageID;
}

bool VisiScript::CheckData( CodeEdit * pScript )
{
    if( (pScript!=0) && pScript->edited() )
    {
        int iRet = QMessageBox::question( this, tr( "Save file" ),
                                             tr( "File was modified, save now ?" ),
                                             QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Yes );

        if( iRet == QMessageBox::Yes )
        {
            sltFileSave();//zj
        }
        if( iRet == QMessageBox::Cancel )
        {
            return false;
        }
    }
    return true;
}

bool VisiScript::CheckAllData( bool bAskForSave )
{
    QMessageBox::StandardButton result = QMessageBox::Yes;
    for( int i=0; i<GetDocumentCount(); i++ )
    {
        CodeEdit * pScript = GetScriptFor( i );

        if( pScript->edited() )
        {
            if( !bAskForSave || (result=QMessageBox::question( this, tr( "Save file" ),
                                                 tr( "File %1 was modified, save now ?" ).arg(pScript->fileName()),
                                                 QMessageBox::Abort|QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes )) == QMessageBox::Yes )
            {
                if( pScript->fileName()==c_strUnknownFileName )
                {
                    QString sFileName = SaveFileName( this, QString::null, m_sLastDirectory, "", m_bUseRootForFileDialog );
                    pScript->setFileName( sFileName );
                }
                sltFileSave();//zj
                //DoExportFile( pScript );
            }
        }
    }
    return result!=QMessageBox::Abort;
}

void VisiScript::sltShowScript()
{
    CodeEdit * pActScript = GetActScript();
    if( pActScript )
    {
        pActScript->setFocus();
    }
}

void VisiScript::sltFileAboutToShow()
{
}

void VisiScript::sltEditAboutToShow()
{
    bool bEnableCopy = GetActScript()->hasSelectedText() || GetActOutput()->hasMarkedTxt();
    m_pCopyAction->setEnabled( bEnableCopy );
    m_pCutAction->setEnabled( bEnableCopy );

    QClipboard * pClipboard = QApplication::clipboard();
    QString sTemp = pClipboard->text();
    bool bEnablePaste = !sTemp.isNull();
    m_pPasteAction->setEnabled( bEnablePaste );
}

void VisiScript::sltRunAboutToShow()
{
}

void VisiScript::sltViewAboutToShow()
{
    CodeEdit * pActScript = GetActScript();
    m_pToggleOverwrite->setChecked( pActScript->overwriteMode() );
    bool bEnable = pActScript->hasSelectedText();
    m_pCommentAction->setEnabled( bEnable );
    m_pUnCommentAction->setEnabled( bEnable );
}

void VisiScript::sltLanguagesAboutToShow()
{
    // deselect all languages
    for( int i=LANG_NONE; i<LANG_MAX_ID; i++ )
    {
        m_aMenuLanguages.at(i)->setChecked(false);
    }
    // and select the new language
    CodeEdit * pActScript = GetActScript();
    if( pActScript!=0 )
    {
        m_aMenuLanguages.at(pActScript->languageID())->setChecked(true);
    }
}

int VisiScript::GetLanguageIdForAction( QAction * pAction )
{
    for( int i=LANG_NONE; i<LANG_MAX_ID; i++ )
    {
        if( pAction == m_aMenuLanguages.at(i) )
        {
            return i;
        }
    }
    return LANG_NONE;      // not found
}

void VisiScript::sltLanguagesTriggered( QAction * pAction )
{
    UpdateLanguageForCurrent( GetLanguageIdForAction(pAction) );
    // update menu
    sltLanguagesAboutToShow();
}

void VisiScript::sltEncodingsAboutToShow()
{
    CodeEdit * pActScript = GetActScript();
    if( pActScript!=0 )
    {
        if( pActScript->isUtf8() )
        {
            m_pUTF8->setChecked( true );
            m_pLatin1->setChecked( false );
        }
        else
        {
            m_pUTF8->setChecked( false );
            m_pLatin1->setChecked( true );
        }
    }
}

void VisiScript::sltEncodingsTriggered( QAction * pAction )
{
    GetActScript()->setUtf8( pAction==m_pUTF8 );
    sltEncodingsAboutToShow();
}

void VisiScript::UpdateCaption( const QString & sName )
{
    if( sName.isNull() )
    {
        setWindowTitle( tr( "脚本编辑器" ) );
    }
    else
    {
        setWindowTitle( sName + " - " + tr( "VisiScript" ) );
    }
}

void VisiScript::LoadInit()
{
    QSettings aCfg(QSettings::UserScope, _MNEUROTH_DE, _VISISCRIPT_NAME);

    aCfg.beginGroup(_VISISCRIPT_NAME);

    m_bFirstLaunch = aCfg.value( c_sFirstLaunch, m_bFirstLaunch ).toBool();
    m_sScriptCmd = aCfg.value( c_sScriptCmdKey, m_sScriptCmd ).toString();
    m_sScriptArguments = aCfg.value( c_sScriptArgumentsKey, m_sScriptArguments ).toString();
    m_sFontName = aCfg.value( c_sFontNameKey, m_sFontName ).toString();
    m_sFontStyle = aCfg.value( c_sFontStyleKey, m_sFontStyle ).toString();
    m_iFontSize = aCfg.value( c_sFontSizeKey, m_iFontSize ).toInt();
    m_iWordWrap = aCfg.value( c_sWordWrapKey, m_iWordWrap ).toInt();
    m_iOverwrite = aCfg.value( c_sOverwriteKey, m_iOverwrite ).toInt();
    m_iTabSpace = aCfg.value( c_sTabSpaceKey, m_iTabSpace ).toInt();
    m_iEolMode = aCfg.value( c_sEolMode, m_iEolMode ).toInt();
    m_dOutputRatio = aCfg.value( c_sOutputRatio, m_dOutputRatio ).toInt();
    m_iSpacesForTabs = aCfg.value( c_sSpacesForTabs, m_iSpacesForTabs ).toInt();
    m_iShowCurrentLine = aCfg.value( c_sShowCurrentLine, m_iShowCurrentLine ).toInt();
    m_iShowWhitespaces = aCfg.value( c_sShowWhitespaces, m_iShowWhitespaces ).toInt();
    m_iShowLineNumbers = aCfg.value( c_sShowLineNumbers, m_iShowLineNumbers ).toInt();
    m_iAutoIndent = aCfg.value( c_sAutoIndent, m_iAutoIndent ).toInt();
    m_iCodeFolding = aCfg.value( c_sCodeFolding, m_iCodeFolding ).toInt();
    m_sLastDirectory = aCfg.value( c_sLastDirectoryKey, m_sLastDirectory ).toString();
    m_bInteractiveModus = aCfg.value( c_sInteractiveModusKey, m_bInteractiveModus ).toBool();
    m_bChangeActDir = aCfg.value( c_sChangeActDirKey, m_bChangeActDir ).toBool();
    m_bShowOutputArea = aCfg.value( c_sShowOutputArea, m_bShowOutputArea ).toBool();
    m_bAutoSave = aCfg.value( c_sAutoSaveKey, m_bAutoSave ).toBool();
    m_bPipeScriptToInterpreter = aCfg.value( c_sPipeScriptToInterpreter, m_bPipeScriptToInterpreter ).toBool();
    m_bMeasureExecutionTime = aCfg.value( c_sMeasureExecutionTimeKey, m_bMeasureExecutionTime ).toBool();
    m_bBraceMatching = aCfg.value( c_sBraceMatchingKey, m_bBraceMatching ).toBool();
    m_bAutoClearOutput = aCfg.value( c_sAutoClearOutputKey, m_bAutoClearOutput ).toBool();
    m_bIsDefaultUtf8 = aCfg.value( c_sSetUtf8, m_bIsDefaultUtf8 ).toBool();
    m_bUseNewIcons = aCfg.value( c_sUseNewIcons, m_bUseNewIcons ).toBool();
    m_bUseToolbar = aCfg.value( c_sUseToolbar, m_bUseToolbar ).toBool();
    m_bUseUserInterpreter = aCfg.value( c_sUseUserInterpreter, m_bUseUserInterpreter ).toBool();
    m_bCloseButtonInTabPage = aCfg.value( c_sCloseButtonInTabPage, m_bCloseButtonInTabPage ).toBool();
    m_iShowStartupFileDialog = aCfg.value( c_sShowStaruptFileDialog, m_iShowStartupFileDialog ).toInt();
    m_iIconSize = aCfg.value( c_sIconSize, m_iIconSize ).toInt();
    m_iAutoCompletionThresh = aCfg.value( c_sAutoCompletionThresh, m_iAutoCompletionThresh ).toInt();
    m_sAutoCompletionFileName = aCfg.value( c_sAutoCompletionFileKey, m_sAutoCompletionFileName ).toString();
    m_bRegularExpr = aCfg.value( c_sRegExpr, m_bRegularExpr ).toBool();
    m_bWildcard = aCfg.value( c_sUseWildcard, m_bWildcard ).toBool();
    m_bCaseSensitive = aCfg.value( c_sCaseSensitive, m_bCaseSensitive ).toBool();
    m_bOnlyWholeWords = aCfg.value( c_sWholeWord, m_bOnlyWholeWords ).toBool();
    m_bSearchBackward = aCfg.value( c_sSearchBackward, m_bSearchBackward ).toBool();
    m_bShowAllOccurrences = aCfg.value( c_sShowAllOccurrences, m_bShowAllOccurrences ).toBool();
    m_bSearchSubDirectories = aCfg.value( c_sSearchSubDirs, m_bSearchSubDirectories ).toBool();
    m_bReplaceAll = aCfg.value( c_sReplaceAll, m_bReplaceAll ).toBool();
    m_bAutoFileLoad = aCfg.value( c_sAutoFileLoad, m_bAutoFileLoad ).toBool();
    m_iLastMainScriptIndex = aCfg.value( c_sLastMainScriptIndex, -1 ).toInt();
    m_iLastActFileIndex = aCfg.value( c_sLastActiveFileIndex, 0 ).toInt();
#if defined(Q_OS_ANDROID)
    m_bUseRootForFileDialog = aCfg.value( c_sUseRootForSDCard, m_bUseRootForFileDialog ).toBool();
#endif

    m_iLastPosX = aCfg.value( c_sLastPosX, m_iLastPosX ).toInt();
    m_iLastPosY = aCfg.value( c_sLastPosY, m_iLastPosY ).toInt();
    m_iLastPosDX = aCfg.value( c_sLastPosDX, m_iLastPosDX ).toInt();
    m_iLastPosDY = aCfg.value( c_sLastPosDY, m_iLastPosDY ).toInt();

    int i = 0;
    int n = 0;
    bool bDone = false;
    QString sBuffer;
    while( !bDone )
    {
        QString sLastOpendFileName;

        QString sBuffer = QString("%0.%1").arg(c_sLastOpendFileName).arg(n);
        sLastOpendFileName = aCfg.value( sBuffer, sLastOpendFileName ).toString();

        if( sLastOpendFileName.size()==0 )
        {
            m_iCountLastOpendFiles = n;
            bDone = true;
        }
        else
        {
            int x,y,iLen,iVal,iEncodingUtf8;
            sBuffer = QString("%0.%1").arg(c_sLastCursorPosX).arg(n);
            x = aCfg.value( sBuffer, 0 ).toInt();
            sBuffer = QString("%0.%1").arg(c_sLastCursorPosY).arg(n);
            y = aCfg.value( sBuffer, 0 ).toInt();
            m_aLastCursorPos.append( QPair<int,int>(x,y) );

            sBuffer =QString("%0.%1").arg(c_sLastEncodingUtf8).arg(n);
            iEncodingUtf8 = aCfg.value( sBuffer, 0 ).toInt();
            m_aLastEncodingUtf8.append( iEncodingUtf8 );

            // read in the position of the splitter for editor and output widget
            sBuffer = QString("%0Len.%1").arg(c_sLastSizerPos).arg(n);
            iLen = aCfg.value( sBuffer, 0 ).toInt();
            QList<int> aSizeList;
            for( i=0; i<iLen; i++ )
            {
                sBuffer = QString("%0.%1.%2").arg(c_sLastSizerPos).arg(n).arg(i);
                iVal = aCfg.value( sBuffer, 0 ).toInt();
                aSizeList.append(iVal);
            }
            m_aLastSpliterPos.append( aSizeList );

            PushToList( sLastOpendFileName, m_aLastOpendFilesList );
            n++;
        }
    }
    for( i=0; i<MAX_LAST_FILES; i++ )
    {
        QString sLastFileName;

        sBuffer = QString("%0.%1").arg(c_sLastFileName).arg(i);
        sLastFileName = aCfg.value( sBuffer, sLastFileName ).toString();

        if( !sLastFileName.isEmpty() )
        {
            PushLastFileName( sLastFileName );
        }
    }
    for( i=0; i<MAX_LAST_SEARCH_STRINGS; i++ )
    {
        QString sText;
        QString sTextRepl;
        QString sFile;
        QString sDir;
        QString sScriptArguments;

        sBuffer = QString("%0.%1").arg(c_sLastSearchString).arg(i);
        sText = aCfg.value( sBuffer, sText ).toString();

        sBuffer = QString("%0.%1").arg(c_sLastReplaceString).arg(i);
        sTextRepl = aCfg.value( sBuffer, sTextRepl ).toString();

        sBuffer = QString("%0.%1").arg(c_sLastSearchFiles).arg(i);
        sFile = aCfg.value( sBuffer, sFile ).toString();

        sBuffer = QString("%0.%1").arg(c_sLastSearchDirs).arg(i);
        sDir = aCfg.value( sBuffer, sDir ).toString();

        sBuffer = QString("%0.%1").arg(c_sLastScriptArguments).arg(i);
        sScriptArguments = aCfg.value( sBuffer, sScriptArguments ).toString();

        if( !sText.isEmpty() )
        {
            PushToList( sText, m_aLastSearchStringList, MAX_LAST_SEARCH_STRINGS );
        }
        if( !sTextRepl.isEmpty() )
        {
            PushToList( sTextRepl, m_aLastReplaceStringList, MAX_LAST_SEARCH_STRINGS );
        }
        if( !sFile.isEmpty() )
        {
            PushToList( sFile, m_aLastSearchFilesList, MAX_LAST_SEARCH_STRINGS );
        }
        if( !sDir.isEmpty() )
        {
            PushToList( sDir, m_aLastSearchDirList, MAX_LAST_SEARCH_STRINGS );
        }
        if( !sScriptArguments.isEmpty() )
        {
            PushToList( sScriptArguments, m_aLastScriptArgumentsList, MAX_LAST_SEARCH_STRINGS );
        }
    }
    for( i=0; i<MAX_EXTERNAL_PROGRAMS; i++ )
    {
        QString sExternalProg;

        sBuffer = QString("%0.%1").arg(c_sExternalProg).arg(i);
        sExternalProg = aCfg.value( sBuffer, sExternalProg ).toString();

        if( !sExternalProg.isEmpty() )
        {
            PushToList( sExternalProg, m_aExternalProgsList, MAX_EXTERNAL_PROGRAMS );
        }
    }
    if( m_aExternalProgsList.count()==0 )
    {
        // create default values for first start
#if !defined(Q_OS_ANDROID)
        PushToList( "chicken", m_aExternalProgsList, MAX_EXTERNAL_PROGRAMS );
        PushToList( "scheme48", m_aExternalProgsList, MAX_EXTERNAL_PROGRAMS );
        PushToList( "lush", m_aExternalProgsList, MAX_EXTERNAL_PROGRAMS );
        PushToList( "lua", m_aExternalProgsList, MAX_EXTERNAL_PROGRAMS );
        PushToList( "tclsh", m_aExternalProgsList, MAX_EXTERNAL_PROGRAMS );
        PushToList( "ruby", m_aExternalProgsList, MAX_EXTERNAL_PROGRAMS );
        PushToList( "perl", m_aExternalProgsList, MAX_EXTERNAL_PROGRAMS );
        //PushToList( "python -i", m_aExternalProgsList, MAX_EXTERNAL_PROGRAMS );
        PushToList( "python", m_aExternalProgsList, MAX_EXTERNAL_PROGRAMS );
        PushToList( "minscript", m_aExternalProgsList, MAX_EXTERNAL_PROGRAMS );
#endif
        PushToList( _QSCRIPT, m_aExternalProgsList, MAX_EXTERNAL_PROGRAMS );
    }
    // handle the file extension to language id map
    {
        // read size
        sBuffer = QString("%0_size").arg(c_sExtensionMap);
        int iSize = aCfg.value( sBuffer, 0 ).toInt();
        // read every item for the map
        for( int i=0; i<iSize; i++ )
        {
            sBuffer = QString("%0_key.%1").arg(c_sExtensionMap).arg(i);
            QString sExt = aCfg.value( sBuffer, "" ).toString();
            sBuffer = QString("%0_val.%1").arg(c_sExtensionMap).arg(i);
            int iLanguageID = aCfg.value( sBuffer, LANG_NONE ).toInt();
            // and add item
            m_aExtensionToLanguageMap[sExt] = iLanguageID;
        }
    }

    {
        sBuffer = QString("%0_size").arg(c_sUserInterpreter);
        int iSize = aCfg.value( sBuffer, 0 ).toInt();
        for( int i=0; i<iSize; i++ )
        {
            ScriptInterpreterConfiguration aItem;

            sBuffer = QString("%1.%2.%3").arg(c_sUserInterpreter).arg(c_sPath).arg(i);
            aItem.m_sInterpreterPath = aCfg.value(sBuffer,aItem.m_sInterpreterPath).toString();
            sBuffer = QString("%1.%2.%3").arg(c_sUserInterpreter).arg(c_sArguments).arg(i);
            aItem.m_sInterpreterArguments = aCfg.value( sBuffer, aItem.m_sInterpreterArguments ).toString();
            sBuffer = QString("%1.%2.%3").arg(c_sUserInterpreter).arg(c_sExtension).arg(i);
            aItem.m_sFileExtension = aCfg.value( sBuffer, aItem.m_sFileExtension ).toString();
            m_aUserInterpreters.append( aItem );
        }
    }

    aCfg.endGroup();
}

void VisiScript::SaveInit()
{
    QSettings aCfg(QSettings::UserScope, _MNEUROTH_DE, _VISISCRIPT_NAME);

    aCfg.beginGroup(_VISISCRIPT_NAME);

    aCfg.setValue( c_sFirstLaunch, m_bFirstLaunch );
    aCfg.setValue( c_sScriptCmdKey, m_sScriptCmd );
    aCfg.setValue( c_sScriptArgumentsKey, m_sScriptArguments );
    aCfg.setValue( c_sFontNameKey, m_sFontName );
    aCfg.setValue( c_sFontStyleKey, m_sFontStyle );
    aCfg.setValue( c_sFontSizeKey, m_iFontSize );
    aCfg.setValue( c_sWordWrapKey, m_iWordWrap );
    aCfg.setValue( c_sOverwriteKey, (int)GetActScript()->overwriteMode()/*m_iOverwrite*/ );
    aCfg.setValue( c_sTabSpaceKey, m_iTabSpace );
    aCfg.setValue( c_sEolMode, m_iEolMode );
    aCfg.setValue( c_sOutputRatio, m_dOutputRatio );
    aCfg.setValue( c_sSpacesForTabs, m_iSpacesForTabs );
    aCfg.setValue( c_sShowCurrentLine, m_iShowCurrentLine );
    aCfg.setValue( c_sShowWhitespaces, m_iShowWhitespaces );
    aCfg.setValue( c_sShowLineNumbers, m_iShowLineNumbers );
    aCfg.setValue( c_sAutoIndent, m_iAutoIndent );
    aCfg.setValue( c_sCodeFolding, m_iCodeFolding );
    aCfg.setValue( c_sLastDirectoryKey, m_sLastDirectory );
    aCfg.setValue( c_sInteractiveModusKey, (int)m_bInteractiveModus );
    aCfg.setValue( c_sChangeActDirKey, (int)m_bChangeActDir );
    aCfg.setValue( c_sShowOutputArea, (int)m_bShowOutputArea );
    aCfg.setValue( c_sAutoSaveKey, (int)m_bAutoSave );
    aCfg.setValue( c_sPipeScriptToInterpreter, (int)m_bPipeScriptToInterpreter );
    aCfg.setValue( c_sMeasureExecutionTimeKey, (int)m_bMeasureExecutionTime );
    aCfg.setValue( c_sBraceMatchingKey, (int)m_bBraceMatching );
    aCfg.setValue( c_sAutoClearOutputKey, (int)m_bAutoClearOutput );
    aCfg.setValue( c_sSetUtf8, (int)m_bIsDefaultUtf8 );
    aCfg.setValue( c_sUseNewIcons, (int)m_bUseNewIcons );
    aCfg.setValue( c_sUseToolbar, (int)m_bUseToolbar );
    aCfg.setValue( c_sUseUserInterpreter, (int)m_bUseUserInterpreter );
    aCfg.setValue( c_sCloseButtonInTabPage, (int)m_bCloseButtonInTabPage );
    aCfg.setValue( c_sShowStaruptFileDialog, m_iShowStartupFileDialog );
    aCfg.setValue( c_sIconSize, m_iIconSize );
    aCfg.setValue( c_sAutoCompletionThresh, m_iAutoCompletionThresh );
    aCfg.setValue( c_sAutoCompletionFileKey, m_sAutoCompletionFileName );
    aCfg.setValue( c_sRegExpr, (int)m_bRegularExpr );
    aCfg.setValue( c_sUseWildcard, (int)m_bWildcard );
    aCfg.setValue( c_sCaseSensitive, (int)m_bCaseSensitive );
    aCfg.setValue( c_sWholeWord, (int)m_bOnlyWholeWords );
    aCfg.setValue( c_sSearchBackward, (int)m_bSearchBackward );
    aCfg.setValue( c_sShowAllOccurrences, (int)m_bShowAllOccurrences );
    aCfg.setValue( c_sSearchSubDirs, (int)m_bSearchSubDirectories );
    aCfg.setValue( c_sReplaceAll, (int)m_bReplaceAll );
    aCfg.setValue( c_sLastActiveFileIndex, m_pTabWidget->currentIndex() );
    aCfg.setValue( c_sAutoFileLoad, (int)m_bAutoFileLoad );
#if defined(Q_OS_ANDROID)
    aCfg.setValue( c_sUseRootForSDCard, (int)m_bUseRootForFileDialog );
#endif

    int iFoundIndex = -1;
    GetActMainScript( &iFoundIndex );
    aCfg.setValue( c_sLastMainScriptIndex, iFoundIndex );

    m_iLastPosX = x();
    m_iLastPosY = y();
    m_iLastPosDX = width();
    m_iLastPosDY = height();
    aCfg.setValue( c_sLastPosX, m_iLastPosX );
    aCfg.setValue( c_sLastPosY, m_iLastPosY );
    aCfg.setValue( c_sLastPosDX, m_iLastPosDX );
    aCfg.setValue( c_sLastPosDY, m_iLastPosDY );

    QString sBuffer;
    int i = 0;
    // before writing the actual opend files delete the actual configuration data,
    // because some files could be closed since the last saving of configuration data
    for( i=0; i<m_iCountLastOpendFiles; i++ )
    {
        sBuffer = QString("%0.%1").arg(c_sLastOpendFileName).arg(i);
        aCfg.remove( sBuffer );
        sBuffer = QString("%0.%1").arg(c_sLastCursorPosX).arg(i);
        aCfg.remove( sBuffer );
        sBuffer = QString("%9.%1").arg(c_sLastCursorPosY).arg(i);
        aCfg.remove( sBuffer );
        sBuffer = QString("%0.%1").arg(c_sLastEncodingUtf8).arg(i);
        aCfg.remove( sBuffer );

        // handle the spliter positions
        sBuffer = QString("%0Len.%1").arg(c_sLastSizerPos).arg(i);
        int iLen = aCfg.value( sBuffer, 0 ).toInt();
        aCfg.remove( sBuffer );
        for( int j=0; j<iLen; j++ )
        {
            sBuffer = QString("%0.%1.%2").arg(c_sLastSizerPos).arg(i).arg(j);
            aCfg.remove( sBuffer );
        }
    }
    m_iCountLastOpendFiles = GetDocumentCount();
    for( i=0; i<(int)GetDocumentCount(); i++ )
    {
        int x,y,iEncodingUtf8;
        CodeEdit * pScript = GetScriptFor( i );
        pScript->getCursorPosition( &y, &x );
        iEncodingUtf8 = (int)pScript->isUtf8();

        QString sLastOpendFileName = pScript->fileName();
        sBuffer = QString("%0.%1").arg(c_sLastOpendFileName).arg(i);
        aCfg.setValue( sBuffer, sLastOpendFileName );

        sBuffer = QString("%0.%1").arg(c_sLastCursorPosX).arg(i);
        aCfg.setValue( sBuffer, x );
        sBuffer = QString("%0.%1").arg(c_sLastCursorPosY).arg(i);
        aCfg.setValue( sBuffer, y );

        sBuffer = QString("%0.%1").arg(c_sLastEncodingUtf8).arg(i);
        aCfg.setValue( sBuffer, iEncodingUtf8 );

        // save the position of the splitter between edit- and output-widget
        QWidget * pPage = m_pTabWidget->widget( i );
        QSplitter * pSplitter = (QSplitter *)pPage->findChild<QSplitter *>( QString(_SPLITTER_NAME) );
        QList<int> aSizeList = pSplitter->sizes();
        sBuffer = QString("%0Len.%1").arg(c_sLastSizerPos).arg(i);
        aCfg.setValue( sBuffer, aSizeList.size() );
        int n = 0;
        for( QList<int>::iterator it = aSizeList.begin(); it!=aSizeList.end(); it++ )
        {
            sBuffer = QString("%0.%1.%2").arg(c_sLastSizerPos).arg(i).arg(n);
            aCfg.setValue( sBuffer, *it );
            n++;
        }
    }
    for( i=0; i<m_aLastFilesList.count(); i++ )
    {
        QString sLastFileName = m_aLastFilesList.at( i );

        sBuffer = QString("%0.%1").arg(c_sLastFileName).arg(i);
        aCfg.setValue( sBuffer, sLastFileName );
    }
    for( i=0; i<m_aLastSearchStringList.count(); i++ )
    {
        QString sText = m_aLastSearchStringList.at( i );

        sBuffer = QString("%0.%1").arg(c_sLastSearchString).arg(i);
        aCfg.setValue( sBuffer, sText );
    }
    for( i=0; i<m_aLastReplaceStringList.count(); i++ )
    {
        QString sText = m_aLastReplaceStringList.at( i );

        sBuffer = QString("%0.%1").arg(c_sLastReplaceString).arg(i);
        aCfg.setValue( sBuffer, sText );
    }
    for( i=0; i<m_aLastSearchFilesList.count(); i++ )
    {
        QString sText = m_aLastSearchFilesList.at( i );

        sBuffer = QString("%0.%1").arg(c_sLastSearchFiles).arg(i);
        aCfg.setValue( sBuffer, sText );
    }
    for( i=0; i<m_aLastSearchDirList.count(); i++ )
    {
        QString sText = m_aLastSearchDirList.at( i );

        sBuffer = QString("%0.%1").arg(c_sLastSearchDirs).arg(i);
        aCfg.setValue( sBuffer, sText );
    }
    for( i=0; i<m_aLastScriptArgumentsList.count(); i++ )
    {
        QString sText = m_aLastScriptArgumentsList.at( i );

        sBuffer = QString("%0.%1").arg(c_sLastScriptArguments).arg(i);
        aCfg.setValue( sBuffer, sText );
    }
    for( i=0; i<m_aExternalProgsList.count(); i++ )
    {
        QString sExternalProg = m_aExternalProgsList.at( i );

        sBuffer = QString("%0.%1").arg(c_sExternalProg).arg(i);
        aCfg.setValue( sBuffer, sExternalProg );
    }
    // handle the file extension to language id map
    {
        // save the size of the map
        sBuffer = QString("%0_size").arg(c_sExtensionMap);
        aCfg.setValue( sBuffer, (int)m_aExtensionToLanguageMap.count() );

        QMap<QString,int>::ConstIterator aIter;
        int iIndex = 0;
        for( aIter=m_aExtensionToLanguageMap.begin(); aIter!=m_aExtensionToLanguageMap.end(); ++aIter )
        {
            QString sExt = aIter.key();
            int iLanguageID = aIter.value();

            sBuffer = QString("%0_key.%1").arg(c_sExtensionMap).arg(iIndex);
            aCfg.setValue( sBuffer, sExt );
            sBuffer = QString("%0_val.%1").arg(c_sExtensionMap).arg(iIndex);
            aCfg.setValue( sBuffer, iLanguageID );

            iIndex++;
        }
    }

    sBuffer = QString("%1_size").arg(c_sUserInterpreter);
    aCfg.setValue( sBuffer, (int)m_aUserInterpreters.size() );
    for( i=0; i<m_aUserInterpreters.count(); i++ )
    {
        const ScriptInterpreterConfiguration & aItem = m_aUserInterpreters.at( i );

        sBuffer = QString("%1.%2.%3").arg(c_sUserInterpreter).arg(c_sPath).arg(i);
        aCfg.setValue( sBuffer, aItem.m_sInterpreterPath );
        sBuffer = QString("%1.%2.%3").arg(c_sUserInterpreter).arg(c_sArguments).arg(i);
        aCfg.setValue( sBuffer, aItem.m_sInterpreterArguments );
        sBuffer = QString("%1.%2.%3").arg(c_sUserInterpreter).arg(c_sExtension).arg(i);
        aCfg.setValue( sBuffer, aItem.m_sFileExtension );
    }

    aCfg.endGroup();
}

bool VisiScript::HandleClose( bool bAskForSave )
{
    bool ret = CheckAllData(bAskForSave);

    if( ret )
    {
        SaveInit();

        if( isScriptRunning() )
        {
            sltKill();
        }

        foreach( IVisiScriptExtension * pExtension, m_aExtensions )
        {
            pExtension->Callback_ShutdownApplication();
        }
    }

    return ret;
}

bool VisiScript::WasSaveAlreadyAsked() const
{
    return m_bSaveAlreadyAsked;
}

void VisiScript::dragEnterEvent( QDragEnterEvent * pEvent )
{
    if( pEvent->mimeData()->hasUrls() )
    {
        pEvent->accept();
    }
}

void VisiScript::dropEvent( QDropEvent * pEvent )
{
    QString sFileName;

    if( IsTextFileDrag( pEvent, sFileName ) )
    {
        DoImportFile( sFileName );
        pEvent->accept();
    }
}

void VisiScript::customEvent(QEvent * pEvent)
{
    if( pEvent && pEvent->type()==_USER_EVENT_CHECK_RELOAD )
    {
        sltCheckReloadFiles();
        pEvent->accept();
    }
    else
    {
        // unknown event --> apple events ?
    }
}

void VisiScript::keyPressEvent( QKeyEvent * pKeyEvent )
{
    if( pKeyEvent->key()==Qt::Key_Shift || pKeyEvent->key()==Qt::Key_Control )
    {
        m_bIsMoveAllKeyPressed = true;
    }
    QMainWindow::keyPressEvent( pKeyEvent );
}

void VisiScript::keyReleaseEvent( QKeyEvent * pKeyEvent )
{
    if( pKeyEvent->key()==Qt::Key_Shift || pKeyEvent->key()==Qt::Key_Control )
    {
        m_bIsMoveAllKeyPressed = false;
    }
    QMainWindow::keyReleaseEvent( pKeyEvent );
}

void VisiScript::closeEvent( QCloseEvent * pCloseEvent )
{
    m_bSaveAlreadyAsked = true;
    if( HandleClose(true) )
    {
        pCloseEvent->accept();
    }
    else
    {
        pCloseEvent->ignore();
    }
}

#if !defined(Q_OS_ANDROID)
void VisiScript::focusInEvent( QFocusEvent * /*pEvent*/ )
{
    QEvent * pUserEvent = new CheckForReloadEvent();
    QApplication::postEvent(this,pUserEvent);
}
#endif

void VisiScript::sltCheckReloadFiles()
{
    // we do not want to call this method recursively,
    // because than we might get a crash when multiple
    // files have changed externaly
    if( m_iCheckModifiedFilesCounter==0 )
    {
        m_iCheckModifiedFilesCounter++;
        m_aModifiedFilesLock.lockForWrite();
        bool bYesToAll = false;
        for( int i=0; i<m_aModifiedFiles.size(); i++ )
        {
            CodeEdit * pEdit = GetScriptForFileName(m_aModifiedFiles[i]);
            if( pEdit )
            {
                pEdit->sltFileChanged(m_aModifiedFiles[i],bYesToAll);
            }
            // 27.1.2012: register again for changes (needed for Mac!?)
            m_pFileSystemWatcher->addPath(m_aModifiedFiles[i]);
        }
        m_aModifiedFiles.clear();
        m_aModifiedFilesLock.unlock();
        m_iCheckModifiedFilesCounter--;
    }
}

CodeEdit * VisiScript::GetActMainScript( int * pIndexFound ) const
{
    for( int i=0; i<GetDocumentCount(); i++ )
    {
        QWidget * pPage = m_pTabWidget->widget( i );
        QObject * pChild = pPage->findChild<QObject *>( QString(_SCRIPT_NAME) );
        CodeEdit * pScript = (CodeEdit *)pChild;
        if( pScript )
        {
            if( pScript->isMainFile() )
            {
                if( pIndexFound )
                {
                    *pIndexFound = i;
                }
                return pScript;
            }
        }
    }

    if( pIndexFound )
    {
        *pIndexFound = -1;
    }
    // if no main file is found, return the actual script !
    return GetActScript();
}

CodeEdit * VisiScript::GetActScript() const
{
    if( GetActDocumentTabPage() )
    {
        QObject * pChild = GetActDocumentTabPage()->findChild<QObject *>( QString(_SCRIPT_NAME) );

        return (CodeEdit *)pChild;
    }
    return 0;
}

OutputAreaMultiLineEdit * VisiScript::GetActOutput() const
{
    QObject * pChild = GetActDocumentTabPage()->findChild<QObject *>( QString(_OUTPUT_NAME) );

    return (OutputAreaMultiLineEdit *)pChild;
}

CodeEdit * VisiScript::GetScriptForFileName( const QString & strFileName, int * pIndex, bool bCompareOnlyFilename ) const
{
    for( int i=0; i<GetDocumentCount(); i++ )
    {
        QWidget * pPage = m_pTabWidget->widget( i );
        QObject * pChild = pPage->findChild<QObject *>( QString(_SCRIPT_NAME) );
        CodeEdit * pScript = (CodeEdit *)pChild;
        if( pScript )
        {
            QString strTemp = bCompareOnlyFilename ? pScript->onlyFileName() : pScript->fileName();
            if( strTemp == strFileName )
            {
                if( pIndex )
                {
                    *pIndex = i;
                }
                return pScript;
            }
        }
    }
    if( pIndex )
    {
        *pIndex = -1;
    }
    return 0;	// not found !
}

int VisiScript::GetIndexForScript( CodeEdit * pScript ) const
{
    for( int i=0; i<GetDocumentCount(); i++ )
    {
        QWidget * pPage = m_pTabWidget->widget( i );
        QObject * pChild = pPage->findChild<QObject *>( QString(_SCRIPT_NAME) );
        if( pChild==pScript )
        {
            return i;
        }
    }
    return -1;      // not found
}

CodeEdit * VisiScript::GetScriptFor( int iIndex ) const
{
    if( iIndex>=0 && iIndex<GetDocumentCount() )
    {
        QWidget * pPage = m_pTabWidget->widget( iIndex );
        QObject * pChild = pPage->findChild<QObject *>( QString(_SCRIPT_NAME) );
        return (CodeEdit *)pChild;
    }
    return 0;
}

OutputAreaMultiLineEdit * VisiScript::GetOutputFor( int iIndex ) const
{
    if( iIndex>=0 && iIndex<GetDocumentCount() )
    {
        QWidget * pPage = m_pTabWidget->widget( iIndex );
        QObject * pChild = pPage->findChild<QObject *>( QString(_OUTPUT_NAME) );
        return (OutputAreaMultiLineEdit *)pChild;
    }
    return 0;
}

QWidget * VisiScript::GetActDocumentTabPage() const
{
    return m_pTabWidget->currentWidget();
}

int VisiScript::GetDocumentCount() const
{
    return m_pTabWidget->count();
}

void VisiScript::sltCustomContextMenuRequested( const QPoint & pos )
{
    QWidget * pWidget = m_pTabWidget->widget( m_pTabWidget->GetTabBar()->tabAt( pos ) );
    QObject * pChild = pWidget->findChild<QObject *>( QString(_SCRIPT_NAME) );
    CodeEdit * pCodeEdit = (CodeEdit *)pChild;

    TabContextMenu aMenu( pCodeEdit );
    aMenu.exec( m_pTabWidget->GetTabBar()->mapToGlobal(pos) );
}

void VisiScript::sltOutputSplitterMoved( int pos, int index )
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    QSplitter * pSplitter = (QSplitter *)QObject::sender();
    QList<int> aLastSplitterPos = pSplitter->sizes();
    // user interaction updates default value for output ratio !
    m_dOutputRatio = 100.0 * (double)aLastSplitterPos.at(1) / (double)(aLastSplitterPos.at(1) + aLastSplitterPos.at(0));
    // update splitter size for all tab pages if needed
    if( m_bIsMoveAllKeyPressed )
    {
        SetOutputSizeForAll( m_dOutputRatio );
    }
}

void VisiScript::AddDocument( const QString & sFileName )
{
    QString sFileNameTemp = StripPath( sFileName );
    int iCount = m_pTabWidget->count();
    if(iCount >0)//ZJ ADD
    {
        m_pTabWidget->removeTab(0);
    }
    QWidget * pTab = new QWidget( m_pTabWidget );
    /*int index =*/ m_pTabWidget->addTab( pTab, sFileNameTemp );
    m_pTabWidget->setTabToolTip( m_pTabWidget->indexOf(pTab), sFileNameTemp );

    QVBoxLayout * pLayout =  new QVBoxLayout( pTab );
    pLayout->setSpacing(1);
    pLayout->setContentsMargins(1, 1, 1, 1);

    QSplitter * pSplitter = new QSplitter( Qt::Vertical, pTab );
    pSplitter->setObjectName(_SPLITTER_NAME);
    pSplitter->setChildrenCollapsible(true);
#ifdef Q_OS_ANDROID
    // Maybe: use width of scrollbars for width of sizer
    pSplitter->setHandleWidth(m_iIconSize/2);  // increase size of splitter for mobile platforms...
#endif
    connect( pSplitter, SIGNAL( splitterMoved(int,int) ), this, SLOT( sltOutputSplitterMoved(int,int) ) );

    pLayout->addWidget( pSplitter );

    int iActLanguageID = GetLanguageIDFromExtension(QFileInfo(sFileNameTemp).suffix(),m_aExtensionToLanguageMap);

    // *** create edit widget ***
    CodeEdit * pScript = new CodeEdit( pSplitter, _SCRIPT_NAME, m_pTabWidget, pTab, this, m_pFileSystemWatcher, m_bIsDefaultUtf8 );
    pScript->setTabWidth( m_iTabSpace );
    pScript->setLanguage( 0, iActLanguageID );
    pScript->setMarginLineNumbers( 1, true );
    pScript->setOverwriteMode( (bool)m_iOverwrite );
    pScript->setSelectionBackgroundColor( QColor( 0,0,0 ) );
    pScript->setSelectionForegroundColor( QColor( 255,255,255 ) );
    pScript->setFileName( sFileNameTemp );

    connect( pScript, SIGNAL( cursorPositionChanged(int,int) ), this, SLOT( sltPositionChanged(int,int) ) );

    // *** create output widget ***
    OutputAreaMultiLineEdit * pOutput = new OutputAreaMultiLineEdit( pSplitter, _OUTPUT_NAME );
    pOutput->setReadOnly( true );
    pOutput->setWordWrapMode( QTextOption::NoWrap );

    connect( pOutput, SIGNAL( doubleClicked(int,int) ), this, SLOT( sltOutputDoubleClicked(int,int) ) );

    pSplitter->setCollapsible( pSplitter->indexOf(pScript), true );
    pSplitter->setCollapsible( pSplitter->indexOf(pOutput), true );

    m_pTabWidget->setCurrentIndex(m_pTabWidget->indexOf(pTab));

    pOutput->setTabStopWidth( m_iTabSpace );

    // font settings updaten
    UpdateFont( false );
    UpdateLanguageForCurrent( iActLanguageID );

    UpdateFlags();

    SetOutputSize( pOutput, m_dOutputRatio );
}

void VisiScript::sltCloseAllDocuments()
{
    int iCount = GetDocumentCount();
    // close all but the first document
    for( int i=iCount-1; i>0; i-- )
    {
        m_pTabWidget->setCurrentIndex(i);
        sltCloseActDocument();
    }
    // add one empty document
    AddDocument( c_strUnknownFileName );
    // and now close the first document
    m_pTabWidget->setCurrentIndex(0);
    sltCloseActDocument();
}

void VisiScript::sltCopyFullPath()
{
    QString sFileName = GetActScript()->fileName();
    sFileName = QDir::toNativeSeparators(sFileName);
    QClipboard * pClipboard = QApplication::clipboard();
    pClipboard->setText( sFileName, QClipboard::Clipboard );
}

void VisiScript::sltOpenContainingFolder()
{
    OpenContainingFolder( GetActScript()->fileName() );
}

void VisiScript::sltTabCloseRequested(int index)
{
    sltCloseTab( GetScriptFor( index ) );
}

void VisiScript::sltCloseTab(CodeEdit * pEdit)
{
    if( GetDocumentCount()>1 )
    {
        OutputAreaMultiLineEdit * pOutput = 0;
        int index = GetIndexForScript( pEdit );
        if( index>=0 )
        {
            pOutput = GetOutputFor( index );
        }

        if( pOutput && pOutput->isLocked() )
        {
            // can not close a window when a find-in-files search is still running
            QMessageBox::information(this,tr("Information"),tr("Can not close current document, because asynchronious find in files is still running!"));
        }
        else
        {
            if( CheckData( pEdit ) )
            {
                QWidget * pPage = pEdit->parentWidget()->parentWidget();
                m_pTabWidget->removeTab(m_pTabWidget->indexOf(pPage));
                delete pPage;
            }
        }
    }
}

void VisiScript::sltCloseActDocument()
{
    // at least one document must be open !
    int iTempLastPageIndex = m_iLastPageIndex;

    sltCloseTab( GetActScript() );

    // update last selected data
    if( m_pTabWidget->currentIndex()==iTempLastPageIndex )
    {
        m_iLastPageIndex = -1;
    }
    else if( m_pTabWidget->currentIndex()<iTempLastPageIndex )
    {
        m_iLastPageIndex = iTempLastPageIndex-1;
    }
    else
    {
        m_iLastPageIndex = iTempLastPageIndex;
    }
}

void VisiScript::sltClearActDocument()
{
    int iRet = QMessageBox::information( this, tr( "Clear tab" ),
                                         tr( "Clear the acutual document?" ),
                                         QMessageBox::Yes, QMessageBox::No );
    if( iRet == QMessageBox::Yes )
    {
        sltClearScript();
        sltShowScript();
    }
}

void VisiScript::sltCurrentTabPageChanged(int iIndex)
{
    // switch off macro recording if tab page changes !
    if(  m_bIsMacroRecording )
    {
        ToggleRecordMacro( false );
    }

    Q_UNUSED(iIndex);
    m_iLastPageIndex = m_iActPageIndex;
    m_iActPageIndex = m_pTabWidget->currentIndex();
#if defined(Q_OS_ANDROID)
    sltLanguagesAboutToShow();
    sltEncodingsAboutToShow();
#endif
    // set the input focus after tab change into the selected editor widget
    CodeEdit * pActScript = GetActScript();
    if( pActScript )
    {
        pActScript->setFocus();
        // update status bar
        int iLine, iIndex;
        pActScript->getCursorPosition(&iLine,&iIndex);
        sltPositionChanged(iLine,iIndex);
    }
}

void VisiScript::sltFileChanged(const QString & path)
{
    m_aModifiedFilesLock.lockForWrite();
    m_aModifiedFiles.push_front(path);
    m_aModifiedFiles.removeDuplicates();	// add only uniqe files
    m_aModifiedFilesLock.unlock();
}

template <class T1>
void VisiScript::ForAllDocumentScripts( T1 pFcn )
{
    for( int i=0; i<GetDocumentCount(); i++ )
    {
        (GetScriptFor( i )->*pFcn)();
    }
}

template <class T1, class T2>
void VisiScript::ForAllDocumentScripts( T1 aValue, T2 pFcn )
{
    for( int i=0; i<GetDocumentCount(); i++ )
    {
        (GetScriptFor( i )->*pFcn)( aValue );
    }
}

template <class T1, class T2, class T3>
void VisiScript::ForAllDocumentScripts( T1 aValue1, T2 aValue2, T3 pFcn )
{
    for( int i=0; i<GetDocumentCount(); i++ )
    {
        (GetScriptFor( i )->*pFcn)( aValue1, aValue2 );
    }
}

template <class T1>
void VisiScript::ForAllDocumentOutput( T1 pFcn )
{
    for( int i=0; i<GetDocumentCount(); i++ )
    {
        (GetOutputFor( i )->*pFcn)();
    }
}

template <class T1, class T2>
void VisiScript::ForAllDocumentOutput( T1 aValue, T2 pFcn )
{
    for( int i=0; i<GetDocumentCount(); i++ )
    {
        (GetOutputFor( i )->*pFcn)( aValue );
    }
}
