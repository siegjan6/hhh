#define MAIN_CPP
#ifndef MAIN_CPP
#define MAIN_CPP
#include "visiscript.h"
#include "inline_code.h"

#include <QApplication>
#include <QList>
#include <QTranslator>
#include <QLocale>
#include <QLibrary>
#include <QLibraryInfo>
#include <QFileInfo>
#include <QFile>
#include <QDir>

// ************************************************************************

static void ProcessTranslations( QTranslator * pTranslatorQt, QTranslator * pTranslator )
{
    // ** need global Translator for the other dialogs...
    QString sLanguage = QLocale::system().name();   // for tests/screenshots: "en"

    bool bOk = pTranslatorQt->load(":/translations/qt_" + sLanguage,QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    if( !bOk )
    {
        bOk = pTranslatorQt->load(":/translations/qt_" + sLanguage);
    }
    QApplication::installTranslator(pTranslatorQt);

    bOk = pTranslator->load(":/translations/visiscript_" + sLanguage);
    QApplication::installTranslator( pTranslator );
}

// ************************************************************************

class VisiScriptApp : public QApplication
{
public:
    VisiScriptApp(int & argc, char ** argv);
    ~VisiScriptApp();

    void init(const QStringList & sFileName, const QList<IVisiScriptExtension *> aExtensions, const QString &sLoadExtensionsMessages);
    void finish(const QList<IVisiScriptExtension *> aExtensions);

#if defined( Q_WS_MACX )
protected:
    bool event(QEvent *event);
#endif

private:
    VisiScript *        m_pMainWindow;
    QString             m_sMacFile;
};

// ************************************************************************

VisiScriptApp::VisiScriptApp(int & argc, char ** argv)
: QApplication(argc,argv),
  m_pMainWindow(0)
{
}

VisiScriptApp::~VisiScriptApp()
{
#if defined( Q_OS_ANDROID )
    // 2.12.2011: workaround for handle activity close with back-button
    // --> save all files without prompting when closing application
    if( !m_pMainWindow->WasSaveAlreadyAsked() )
    {
        m_pMainWindow->HandleClose(false);
    }
#endif
    delete m_pMainWindow;
}
#include "MDVisiScript.h"
void VisiScriptApp::init(const QStringList & aFileNames, const QList<IVisiScriptExtension *> aExtensions, const QString & sLoadExtensionsMessages )
{
    m_pMainWindow = new MDVisiScript("/home/connlaio/6/visiscript/src/files/demoobj.js");

    m_pMainWindow->show();

    if( sLoadExtensionsMessages.length()>0 )
    {
        m_pMainWindow->AddToCurrentOutput(sLoadExtensionsMessages);
    }

    // load files
    if( !m_sMacFile.isEmpty() )
    {
        m_pMainWindow->DoImportFile(m_sMacFile);
    }
    foreach( const QString & sFileName, aFileNames )
    {
        m_pMainWindow->DoImportFile(sFileName);
    }
}

void VisiScriptApp::finish(const QList<IVisiScriptExtension *> aExtensions)
{
    m_pMainWindow->UnregisterExtensions(aExtensions);
}

#if defined( Q_WS_MACX )
bool VisiScriptApp::event( QEvent * event )
{
    if( event->type() == QEvent::FileOpen )
    {
        QFileOpenEvent *oe = static_cast<QFileOpenEvent *>(event);
        if ( m_pMainWindow )
        {
            m_pMainWindow->DoImportFile(oe->file());
        }
        else
        {
            m_sMacFile = oe->file();
        }
    }
    return QApplication::event(event);
}
#endif

// ************************************************************************

static QString GetLibraryName( const QString & sModule )
{
    // extension library will be copied in buildscript (see QMAKE_POST_LINK in .pro)
    QString sExtension;
#if defined(Q_OS_WIN)
    sExtension = "./extensions/extensions%0.dll";
#elif defined(Q_OS_MAC)
    //sExtension = "/Users/min/Documents/home/Entwicklung/shadowbuilds/build-visiscript-Desktop_Qt_5_2_1_clang_64bit-Debug/src/visiscript.app/Contents/MacOS/extensions/libextensions%0.dylib";
    //sExtension = "/Users/min/Documents/home/Entwicklung/shadowbuilds/build-visiscript-Desktop_Qt_5_4_0_clang_64bit-Debug/src/visiscript.app/Contents/MacOS/extensions/libextensions%0.dylib";
    sExtension =  QCoreApplication::applicationDirPath()+QDir::separator()+"extensions/libextensions%0.dylib";
#elif defined(Q_OS_ANDROID)
    sExtension = "/data/data/de.mneuroth.visiscriptextensions%0/files/libextensions.so";
#elif defined(Q_OS_UNIX)
    sExtension = "../extensions/libextensions%0.so";
#else
#error platform not supported yet !
#endif
    sExtension = sExtension.arg(sModule);
    return sExtension;
}

static bool NeedsUpackExtensions( const QString & sModule )
{
#if defined(Q_OS_ANDROID)
    QFileInfo aModule( sModule );
    return QDir( aModule.absoluteDir() ).exists() && !QFile::exists( aModule.absoluteFilePath() );
#else
    // other platforms than android do not need an unpack of extension modules !
    Q_UNUSED(sModule);
    return false;
#endif
}

static QList<IVisiScriptExtension *> TryLoadExtension( const QString & sModule, QString & sLoadExtensionsMessages )
{
    QList<IVisiScriptExtension *> lstExtensions;

    if( NeedsUpackExtensions( sModule ) )
    {
        sLoadExtensionsMessages += QString(qApp->tr("Warning: extensionmodule %0 needs to be unpacked, please start corresponding visiscriptextension application.\n")).arg(sModule);
    }
    else
    {
        lstExtensions += TryLoadExtensions( sModule );
    }

    return lstExtensions;
}

static QList<IVisiScriptExtension *> TryLoadAllExtensions( QString & sLoadExtensionsMessages )
{
    QList<IVisiScriptExtension *> lstExtensions;

    lstExtensions += TryLoadExtension( GetLibraryName(""), sLoadExtensionsMessages );
    lstExtensions += TryLoadExtension( GetLibraryName("a"), sLoadExtensionsMessages );
    lstExtensions += TryLoadExtension( GetLibraryName("b"), sLoadExtensionsMessages );
    lstExtensions += TryLoadExtension( GetLibraryName("c"), sLoadExtensionsMessages );
    lstExtensions += TryLoadExtension( GetLibraryName("d"), sLoadExtensionsMessages );
    lstExtensions += TryLoadExtension( GetLibraryName("e"), sLoadExtensionsMessages );
    lstExtensions += TryLoadExtension( GetLibraryName("f"), sLoadExtensionsMessages );

    return lstExtensions;
}

//*************************************************************************

#if defined(Q_OS_ANDROID)

#define ASSETS_DIR           "assets:/files/"

#define WARNING_FILE         "_all_files_in_directory_will_be_deleted_in_update.txt"

#include "inline_code.h"

static void ExtractAssets()
{
    // extract the script interpreter binary to call script interperter as external process
    QString sAsset,sOutput;
    sAsset = QString(ASSETS_DIR)+QString(DEMO_JS);
    sOutput = QString(FILES_DIR)+QString(DEMO_JS);
    /*ok =*/ extractAssetFile(sAsset,sOutput,true);
    sAsset = QString(ASSETS_DIR)+QString(DEMOOBJ_JS);
    sOutput = QString(FILES_DIR)+QString(DEMOOBJ_JS);
    /*ok =*/ extractAssetFile(sAsset,sOutput,true);
    sAsset = QString(ASSETS_DIR)+QString(WARNING_FILE);
    sOutput = QString(FILES_DIR)+QString(WARNING_FILE);
    /*ok =*/ extractAssetFile(sAsset,sOutput,true);
    sAsset = QString(ASSETS_DIR)+QString(BENCHMARK_JS);
    sOutput = QString(FILES_DIR)+QString(BENCHMARK_JS);
    /*ok =*/ extractAssetFile(sAsset,sOutput,true);
}

#endif

// ************************************************************************

static int private_main( int argc, char** argv, const QList<IVisiScriptExtension *> * pExtensions )
{
    VisiScriptApp aAppl( argc, argv );

    // ** prepare application for different languages...**
    QTranslator * pTranslator = new QTranslator(0);
    QTranslator * pTranslatorQt = new QTranslator(0);
    ProcessTranslations( pTranslatorQt, pTranslator );

    QStringList aFileNames;
    for( int i=1; i<argc; i++ )
    {
        aFileNames.append(argv[i]);
    }

#if defined(Q_OS_ANDROID)
    ExtractAssets();
#endif

    QString sLoadExtensionsMessages;
    QList<IVisiScriptExtension *> aExtensions;
    if( pExtensions==0 )
    {
        aExtensions = TryLoadAllExtensions( sLoadExtensionsMessages );
    }
    else
    {
        aExtensions = *pExtensions;
    }
    aAppl.init( aFileNames, aExtensions, sLoadExtensionsMessages );

    int iRet = aAppl.exec();

    aAppl.finish( aExtensions );

    delete pTranslatorQt;
    delete pTranslator;

    foreach( IVisiScriptExtension * pExtension, aExtensions )
    {
        delete pExtension;
    }
    aExtensions.clear();

    return iRet;
}

// ************************************************************************

int main( int argc, char** argv )
{
    return private_main( argc, argv, 0 );
}

#endif //MAIN_CPP
