
#include <QApplication>
#include <QThread>
#include <QTextCodec>
#include "mainwindow.h"
#include <QVariant>
#include "SingleRuntimeApp.h"
#include <QMessageBox>
#include <QLibrary>
#ifdef _WIN32
#include <shlwapi.h>
#endif
#include "Utility.h"
#include <QDir>

#include<stdio.h>
#ifdef _WIN32
    #include "getopt.h"
#else
    #include <unistd.h>
    #include "/usr/include/getopt.h"
#endif

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

#include<iostream>
#include<string>
using namespace std;

void showUsage() {
    cout << "Options:" << endl;
    cout << " -p, --project=projectpath  The path to .devx file." << endl;
    cout << " -m, --minimize             Start with minimize state." << endl;
    cout << " -i, --hide                 Start with hide state." << endl;
    cout << " -t, --test                 Test run by studio." << endl;
    cout << " -V, --virtualRun           virtual run mode." << endl;
    cout << " -v, --version              Print the version number and exit." << endl;
    cout << " -h, --help                 Print this message and exit." << endl;
    cout << "Examples:" << endl;
    cout << " ./runtime -p=~/Project/server/server.devx -m -i" << endl;
    cout << " ./runtime --project=~/Project/server/server.devx -minimize --hide" << endl;
    cout << " ./runtime -V -p=~/Project/server/server.devx -m -i" << endl;
    cout << "Reports bugs to <bosmutus@hotmail.com>" << endl;
}


void get_option(int argc, char **argv, MainWindow* w, CSingleRuntimeApp* app)
{
    int c;
    bool virtualRun = false;
    while(1) {
        int optIndex = 0;
        int lopt;
        static struct option longOpts[] = {
            { "project", required_argument, NULL, 'p' },
            { "minimize", no_argument, NULL, 'm' },
            { "maximize", no_argument, NULL, 'M' },
            { "hide", no_argument, NULL, 'i' },
            { "test", no_argument, NULL, 't' },
            { "virtualRun", no_argument, NULL, 'V' },
            //           { "log", required_argument, NULL, 'l' },
            //           { "split", required_argument, NULL, 's' },
            //           { "http-proxy", required_argument, &lopt, 1 },
            //           { "http-user", required_argument, &lopt, 2 },
            //           { "http-passwd", required_argument, &lopt, 3 },
            //           { "http-proxy-user", required_argument, &lopt, 4 },
            //           { "http-proxy-passwd", required_argument, &lopt, 5 },
            //           { "http-auth-scheme", required_argument, &lopt, 6 },
            { "version", no_argument, NULL, 'v' },
            { "help", no_argument, NULL, 'h' },
            { 0, 0, 0, 0 }
        };
        c = getopt_long(argc, argv, "p:mMitVvh", longOpts, &optIndex);
        if(c == -1) {
            break;
        }
        switch(c) {
            case 0:{
                switch(lopt) {
//                    case 1:
//                        printf("1: %s\n",optarg);
//                        break;
//                    case 2:
//                        printf("2: %s\n",optarg);
//                        break;
//                    case 3:
//                        printf("3: %s\n",optarg);
//                        break;
//                    case 4:
//                        printf("4: %s\n",optarg);
//                        break;
//                    case 5:
//                        printf("5: %s\n",optarg);
//                        break;
//                    case 6:
//                        printf("6: %s\n",optarg);
//                        break;
//                    case 7:
//                        printf("7: %s\n",optarg);
//                        break;
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                        printf("%d: %s\n", lopt, optarg);
                        break;
                }
                break;
            }
            case 'p':
            {
                printf("p: %s\n",optarg);
                QString projectFileName(optarg);
                app->LOG_RUNTIME("在启动参数中指定了加载的项目->" + projectFileName);
                //projectFileName = QDir::toNativeSeparators(projectFileName);
                projectFileName = projectFileName.replace('\\', '/');
                w->loadProject(projectFileName, virtualRun);
            }
                break;
            case 'm':
                w->showMinimized();
                break;
            case 'M':
                w->showMaximized();
                break;
            case 'i':
                w->hide();
                break;
            case 't':
                w->show();
                break;
            case 'V':
                virtualRun = true;
                break;
            case 'v':
                printf("version: v1.0.10\n");
                //showVersion();
                exit(0);
            case 'h':
                showUsage();
                exit(0);
            default:
                showUsage();
                exit(1);
        }
    }
}


int main(int argc, char *argv[])
{
    const char *szReport = "MindSCADA.dbg";

#ifdef _WIN32
    if(ExcHndlInit)
        ExcHndlInit();

    if(pfnSetLogFileNameA)
        pfnSetLogFileNameA(szReport);
#endif
    Q_INIT_RESOURCE(MDRuntime);

    CSingleRuntimeApp app(argc, argv);

    app.setOrganizationName("Mindteco");
    app.setApplicationName("MindSCADASvc");
    app.setApplicationVersion("1.0.10");
    app.setOrganizationDomain("www.mindteco.com");
#ifdef Q_OS_LINUX
#ifdef QBREAKPAD
    BreakpadHandler *breakpad_instance = BreakpadHandler::GetInstance();
    QStringList path_list2 = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    QString std_base_path =  path_list2[0];
    /////////////////////////////////////
    // 当supervisor开机自动启动时，需要手动切换至current user crashs路径[/home/[usr]/.local/share/Mindteco/MindSCADASvc/crashs]
    /////////////////////////////////////
    //QCoreApplication::applicationDirPath: Please instantiate the QApplication object first
    //QStandardPaths: XDG_RUNTIME_DIR not set, defaulting to '/tmp/runtime-pi'
    //Failed to set dump path which not exists: //.local/share/Mindteco/MindSCADASvc/crashs
    /////////////////////////////////////
    std_base_path = std_base_path.replace("//", "../");
    //std_base_path = "/home/pi/.local/share/Mindteco/MindSCADASvc";
    QString my_crash_path = std_base_path + "/crashs";
    breakpad_instance->SetupBreakPad(my_crash_path);//or just using "crashes"
#endif
#endif
    CUtility::putDependPathToEnv(qApp->applicationDirPath());
    if(app.run("Mind.Runtime"))
    {
    #ifdef Q_OS_LINUX
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    #else
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    #endif

        app.initSwitchs();
        app.LOG_RUNTIME("-----------------成功启动 MindSCADA 运行时！-----------------");
        app.LOG_RUNTIME(QString("Runtime server name:%1").arg(app.appServerName()));

        MainWindow w(app.m_lsRuntime);
#ifdef Q_OS_LINUX
#ifdef QBREAKPAD
        w.setBreakpadInstance(breakpad_instance);
#endif
#endif
        app.setAppMainWnd(&w);

        bool projected = false;
        if(argc > 1)
        {
            QString projectFileName(argv[1]);
            if(!projectFileName.startsWith('-'))
            {
                QFileInfo fi(projectFileName);
                if(fi.isFile() && fi.exists())
                {
                    app.LOG_RUNTIME("在启动参数中指定了加载的项目->" + projectFileName);
                    //projectFileName = QDir::toNativeSeparators(projectFileName);
                    projectFileName = projectFileName.replace('\\', '/');
                    w.loadProject(projectFileName);
                    projected = true;
                }
                else {
                   app.LOG_RUNTIME("error: 项目文件不存在:" + projectFileName);
                }
            }
        }

        bool minimize = false;
        if(argc > 2)
        {
            QString para2(argv[2]);
            if(!para2.startsWith('-'))
            {
                if(para2 == "minimize")
                {
                    w.showMinimized();
                    minimize = true;
                }
            }
        }
        bool hide = false;
        if(argc > 3)
        {
            QString para3(argv[3]);
            if(!para3.startsWith('-'))
            {
                if(para3 == "hide")
                {
                    w.hide();
                    hide = true;
                }
            }
        }
        //if(!minimize && !hide)
        //    w.showMaximized();

        if(projected == false)
            get_option(argc, argv, &w, &app);

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
        int result = app.exec();
        app.LOG_RUNTIME("-----------------MindSCADA 运行时已经关闭！-----------------");
        //int *p = NULL;
        //*p=1588;
        return result;
    }
    else
    {
//        QMessageBox box;
//        box.setWindowTitle(QObject::tr("警告"));
//        box.setIcon(QMessageBox::Warning);
//        box.setText("启动 MindSCADA 运行时失败，因为已经有一个实例在运行了！");
//        box.setStandardButtons(QMessageBox::Ok);
//        box.setWindowFlags(box.windowFlags() | Qt::Desktop);
//        box.exec();

//        app.LOG_RUNTIME("启动 MindSCADA 运行时失败，因为已经有一个实例在运行了！");
        return 0;
    }
}
