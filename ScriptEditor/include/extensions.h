#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QMenuBar>
#include <QMainWindow>
#include <QProcess>

class OutputAreaMultiLineEdit;
class CodeEdit;

// ************************************************************************

#define _MENU_FILE      "file"
#define _MENU_TABPAGE   "tabpage"
#define _MENU_EDIT      "edit"
#define _MENU_RUN       "run"
#define _MENU_VIEW      "view"
#define _MENU_HELP      "help"

#define _MENU_GOTO_LINE                         "goto_line"
#define _MENU_SEPARATOR_SEARCH                  "separator_search"
#define _MENU_SEPARATOR_GOTO                    "separator_goto"
#define _MENU_SEPARATOR_INTERPRETER_VERSION     "interpreter_version"
#define _MENU_ENCODING                          "encoding"
#define _MENU_STOP                              "stop"
#define _MENU_SEPARATOR_STOP                    "separator_stop"

#define INTERFACE_DEBUGGER      1001

// ************************************************************************

class QScriptEngine;
class QScriptEngineDebugger;

// ************************************************************************

class IDebuggerCallback
{
public:
    virtual ~IDebuggerCallback() {}

    virtual QString getCurrentOutput() = 0;
};

// ************************************************************************

// Interface for debugger, new since version 0.5.1
class IDebugger
{
public:
    virtual ~IDebugger() {}

    virtual bool AttachToQScriptEngine( QScriptEngine * pEngine ) = 0;
    virtual bool DetachFromQScriptEngine( QScriptEngine * pEngine ) = 0;

    virtual bool canDebug( int iLanguageId ) const = 0;
    virtual QStringList getDebugInterpreterArguments( int iLanguageId ) const = 0;
//    virtual bool StartDebug() = 0;
};

// ************************************************************************

// Interface to register additional functions at QScript interpreter
class IExtendQScript
{
public:
    virtual ~IExtendQScript() {}

    virtual void RegisterFunctions( QScriptEngine * pEngine ) = 0;
};

// ************************************************************************

// Generic callback interface to change behaviour of script execution
class IExecuteScript
{
public:
    virtual ~IExecuteScript() {}

    virtual void ScriptAboutToStart( const QString & sScriptName ) = 0;
    virtual void ScriptFinished() = 0;
    virtual bool ProcessStdOutLine( const QString & sLine ) = 0;
    virtual bool ProcessStdErrLine( const QString & sLine ) = 0;
};

// ************************************************************************

// Base-Interface for compatible improvements of the interface IVisiScript in the future
class IAdvancedVisiScript
{
public:
    virtual ~IAdvancedVisiScript() {}
};

// ************************************************************************

// Interface which can be used from a visiscript extension module
// to communicate with the visiscript application
class IVisiScript
{
public:
    virtual ~IVisiScript() {}

    virtual IAdvancedVisiScript * GetAdvancedInterface( int id ) = 0;

    virtual QMainWindow * GetWindow() = 0;
    virtual QMenuBar * GetMenuBar() = 0;
    virtual OutputAreaMultiLineEdit * GetAndLockCurrentOutput() = 0;
    virtual void UnlockOutput( OutputAreaMultiLineEdit * pOutput ) = 0;
    virtual CodeEdit * GetCurrentScript() = 0;
    virtual QProcess & GetCurrentScriptProcess() = 0;
    virtual QScriptEngine * GetCurrentScriptEngine() = 0;
    virtual void ScriptEnginePushLineToInputQueue( const QString & sValue ) = 0;
    virtual void ScriptEngineClearInputQueue() = 0;

    virtual bool Register( IExtendQScript * pExtension ) = 0;
    virtual bool Unregister( IExtendQScript * pExtension ) = 0;
    virtual bool Register( IExecuteScript * pExtension ) = 0;
    virtual bool Unregister( IExecuteScript * pExtension ) = 0;

    virtual bool AddScriptInterpreterPath( const QString & sInterpreter ) = 0;

    virtual QString SelectExistingDirectory( const QString & sCurrentDir ) = 0;
    //virtual QString SelectExistingFile( const QString & sCurrentDir ) = 0;

    virtual QString GetFileTag() const = 0;
    virtual QString GetLineTag() const = 0;

    virtual void SetCurrentCursorPosition( int iLine, int iIndex ) = 0;
    virtual bool IsCurrentScriptModified() const = 0;
    virtual void SetCurrentScriptModified( bool value ) = 0;
    virtual QString GetCurrentScriptText() const = 0;
    virtual QString GetCurrentSearchText() const = 0;
    virtual QString GetSelectedText() const = 0;
    virtual void AddToCurrentOutput( const QString & sText ) = 0;
    virtual void AddToOutput( OutputAreaMultiLineEdit * pOutput, const QString & sText ) = 0;

    virtual void PushToLastSearchStringList( const QString & sText ) = 0;
    virtual void PushToLastSearchFilesList( const QString & sText ) = 0;
    virtual void PushToLastSearchDirList( const QString & sText ) = 0;

    virtual QStringList & Property_LastSearchStringList() = 0;
    virtual QStringList & Property_LastSearchFilesList() = 0;
    virtual QStringList & Property_LastSearchDirList() = 0;

    virtual QString & Property_LastSearchText() = 0;

    virtual bool & Property_RegularExpr() = 0;
    virtual bool & Property_Wildcard() = 0;
    virtual bool & Property_CaseSensitive() = 0;
    virtual bool & Property_OnlyWholeWords() = 0;
    virtual bool & Property_SearchSubDirectories() = 0;
};

// ************************************************************************

// Base-Interface for extensions in the future
class IVisiScriptAdvancedExtension
{
public:
    virtual ~IVisiScriptAdvancedExtension() {}
};

// ************************************************************************

// Interface which must be implemented from a visiscript extension module
class IVisiScriptExtension
{
public:
    virtual ~IVisiScriptExtension() {}

    // handle extensions of this interface backward compatible
    // after initial release of visiscript IVisiScriptExtensions MUST not be changed
    // use this method to extend extension interface if needed
    virtual IVisiScriptAdvancedExtension * getAdvancedExtensionInterface( int id ) = 0;

    virtual bool isEnabled() const = 0;
    virtual bool canDisable() const = 0;

    // general informations about extension
    virtual QString getName() const = 0;
    virtual QString getVersion() const = 0;
    virtual QString getDate() const = 0;
    virtual QString getModule() const = 0;

    // script interpreter informations
    virtual QString getLicenseUrl() const = 0;
    virtual QString getHomepageUrl() const = 0;
    virtual QString getHelpUrl() const = 0;

    // return the binary for the interpreter or empty for none
    virtual QString getInterpreterPath() const = 0;
    virtual QString getInterpreterVersionOption() const = 0;
    virtual QStringList getDefaultInterpreterArguments() const = 0;
    virtual bool needsPipeWriting() const = 0;
    // which langeuage supports the module or -1 for none
    virtual QList<int> getSupportedLanguageIds() const = 0;

    virtual bool copyFilesToVisiscript() const = 0;

    virtual bool registerExt( IVisiScript * pVisiScript ) = 0;
    virtual bool unregisterExt() = 0;

    // callbacks --> return true if object processed the callback, false for not processed !
    virtual bool Callback_WriteFile( CodeEdit * pCurrentCode, const QString & sFileName, const QString & sContent, bool bIsUtf8 ) = 0;
    virtual bool Callback_ReadFile( CodeEdit * pCurrentCode, const QString & sFileName, QString & sContent, bool bIsUtf8, QString & sTabSymbol ) = 0;
    virtual void Callback_ShutdownApplication() = 0;
};

// ************************************************************************

// Base-Interface for compatible improvements of the interface IVisiScript in the future, new since version 0.5.1
class IAdvancedVisiScriptDebugger : public IAdvancedVisiScript
{
public:
    virtual ~IAdvancedVisiScriptDebugger() {}

    virtual bool Register( IDebugger * pExtension ) = 0;
    virtual bool Unregister( IDebugger * pExtension ) = 0;

    virtual int StartDebugging() = 0;

    virtual int GetLanguageIdOfCurrentScript() const = 0;
};

// ************************************************************************

// new since 25.12.2014
class IFindInFilesService
{
public:
    virtual ~IFindInFilesService() {}

    virtual void GetAllOccurencesInFile( const QString & sFileName, const QString & sSearch, bool bCaseSensitive, bool bOnlyWholeWords, bool bRegularExpr ) = 0;
};

#endif // EXTENSIONS_H
