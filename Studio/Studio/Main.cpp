#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLibraryInfo>
#include "MainWindow.h"
#include "MDSingleApplication.h"
#include <QTextCodec>
#include <QLibrary>
#ifdef _WIN32
#include <shlwapi.h>
#endif
#include "Utility.h"

#ifdef _WIN32
typedef void (*DrMingwPrototype)();
DrMingwPrototype ExcHndlInit =
        (DrMingwPrototype) QLibrary::resolve("exchndl", "ExcHndlInit");

typedef BOOL (APIENTRY * PFN_SETLOGFILENAMEA)(const char *szLogFileName);
   PFN_SETLOGFILENAMEA pfnSetLogFileNameA =
           (PFN_SETLOGFILENAMEA) QLibrary::resolve("exchndl", "ExcHndlSetLogFileNameA");
#elif defined(Q_OS_LINUX)
#ifdef QBREAKPAD
    #include "BreakpadHandler.h"
    #include "BreakpadHttpUploader.h"
    #include <QUrl>
    #include <QProcess>
    #include <QDir>
    #include <QDesktopServices>
    #include <QObject>
#endif
#endif

int main(int argc, char *argv[])
{
    const char *szReport = "MindStudio.dbg";
#ifdef _WIN32
    if(ExcHndlInit)
        ExcHndlInit();

    if(pfnSetLogFileNameA)
        pfnSetLogFileNameA(szReport);
#endif



    CMDSingleApplication app(argc, argv);

	//QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	//QTextCodec::setCodecForLocale(codec);
	//QTextCodec::setCodecForCStrings(codec);
	//QTextCodec::setCodecForTr(codec);
	
#ifdef _WIN32
#include <winsock.h>
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    if(app.run())
    {
        QString m_locale = QLocale::system().name();

        // Qt translator
        QTranslator qtTranslator;
        qtTranslator.load("qt_" + m_locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        app.installTranslator(&qtTranslator);
        /*=========================================*/
        app.setOrganizationName("Mindteco");
        app.setApplicationName("MindStudioSvc");
        app.setApplicationVersion("1.0.10");
        app.setOrganizationDomain("www.mindteco.com");
#ifdef Q_OS_LINUX
#ifdef QBREAKPAD
        BreakpadHandler *breakpad_instance = BreakpadHandler::GetInstance();
        QStringList path_list2 = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
        QString std_base_path =  path_list2[0];
        std_base_path = std_base_path.replace("//", "../");
        //std_base_path = "/home/pi/.local/share/Mindteco/MindStudioSvc";
        QString my_crash_path = std_base_path + "/crashs";
        breakpad_instance->SetupBreakPad(my_crash_path);//or just using "crashes"
#endif
#endif
        CUtility::putDependPathToEnv(qApp->applicationDirPath());

//    #ifdef Q_OS_LINUX
//        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
//    #else
//        QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
//		//QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
//		//QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
//    #endif


#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
#if _MSC_VER
    QTextCodec *codec = QTextCodec::codecForName("gbk");
#else
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
#endif
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#else
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
#endif

        MainWindow w;
#ifdef Q_OS_LINUX
#ifdef QBREAKPAD
        w.setBreakpadInstance(breakpad_instance);
#endif
#endif
        app.setAppMainWnd(&w);
        w.showMaximized();

#ifdef Q_OS_LINUX
#ifdef QBREAKPAD
        if(breakpad_instance->IsHadCrashedDumpFiles()) { //last time running was crashed
            //1. copy out the dmp files
            //QString desktop_path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
            //QString dump_file_dst_path = desktop_path + "/" + "ReportBug";
            //QDir().mkpath(dump_file_dst_path);
            //if (QDir().exists(dump_file_dst_path))
            {

                QStringList dump_list = breakpad_instance->GetDumpFileList();
                QString base_path = breakpad_instance->GetBreakPadDumpPath();
                foreach(QString itDmpFileName, dump_list) {
                    //can log this "itDmpFileName" name into log
                    QString src_file_full_path = base_path + "/" + itDmpFileName ;
                    //QString des_file_full_path = dump_file_dst_path  + "/" + itDmpFileName ;
                    if(QFile::exists(src_file_full_path)) {
                        //QFile::copy(src_file_full_path, des_file_full_path);
                        qDebug() << "dump file path:" << src_file_full_path;
                    }
                }
            }

            //2. show user the error report dialog (model dialog)
//            CrashReportDialog * crash_report_dialog = new CrashReportDialog();
//            crash_report_dialog->setModal(true);
//            crash_report_dialog->exec();

            //3. show copyed dir
//            ShowDir(dump_file_dst_path);

            //4. delete all of the dmp files
//            breakpad_instance->DeleteAllOfTheDumpFiles();

            //5. you may choose to upload this dump file to your server by http
            //QBreakpadHttpUploader uploder;
            //uploder.setUrl(QUrl("http://xxx.abc.net/upload_log"));
            //QString fake_dump_file_path = "";
            //uploder.uploadDump(fake_dump_file_path);
        }
#endif
#endif
        return app.exec();
    }

    return 0;
}
