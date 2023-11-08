#include "procdockerwidget.h"
#include "ui_procdockerwidget.h"
#include <QMessageBox>
#include <QWindow>
#include <QProcess>
#include <QLabel>
#include <QFile>
#include <QDir>
#include <QThread>
#include <QDebug>
#include "../../include/MDUtility/rs_debug.h"

#ifdef Q_OS_LINUX
    #include <xdo.h>
    #define window_print(window) (printf("%ld\n", window))
#endif

ProcDockerWidget::ProcDockerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProcDockerWidget)
{
    ui->setupUi(this);
    label_tip.setStyleSheet("color:white;");
    ui->verticalLayout->addWidget(&label_tip);
    connect(&process, SIGNAL(started()), this, SLOT(started()));
    connect(&process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(onMainProcErrorOccurred(QProcess::ProcessError)));
    connect(&process, SIGNAL(stateChanged(QProcess::ProcessState state)), this, SLOT(onMainProcStateChanged(QProcess::ProcessState state)));
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onMainProcFinished(int , QProcess::ExitStatus)));

    timer.setInterval(200);
    connect(&timer, SIGNAL(timeout()), this, SLOT(checkProcMainWnd()));

    timerDemonmainProc.setInterval(200);
    connect(&timerDemonmainProc, SIGNAL(timeout()), this, SLOT(demonMainProc()));

    connect(&m_getProcMainWnd, SIGNAL(readyReadStandardError()), this, SLOT(onStandardError()));
    connect(&m_getProcMainWnd, SIGNAL(readyReadStandardOutput()), this, SLOT(onStandardOutput()));
    connect(&m_getProcMainWnd, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinished(int , QProcess::ExitStatus)));
    m_mainWnd = 0;
    m_lastcmd = "";
    m_lastpid = 0;
    m_usercustomWnd=0;
}

ProcDockerWidget::~ProcDockerWidget()
{
    timer.stop();
    if(process.isOpen())
    {
//        QMessageBox::information(this, "process kill", QString("%1").arg(process.processId()));
        process.kill();
    }
    if(m_getProcMainWnd.isOpen())
        m_getProcMainWnd.close();

    delete ui;
}

void ProcDockerWidget::init(QString cmd)
{
    if(process.isOpen())
    {
//        QMessageBox::information(this, "process kill", QString("%1").arg(process.processId()));
        process.close();
        process.waitForFinished(3000);

    }
    bool isWinid = false;
    ulong winid = cmd.toULong(&isWinid);
    if(!isWinid)
        process.start(cmd);
    //创建进程
    //    process.setProgram(cmd);
    //    process.setArguments(arguments);
    //process.start(cmd);
    //process.setProgram("Z:/D/work/OpenSource/godot/bin/MindSCADA.windows.tools.64.exe");
    //process.setArguments(QStringList() << "--path" << "C:/Users/Administrator/Documents/新建游戏项目");

    //qint64 pid;
    //process.startDetached(&pid);
    //QProcess::startDetached("Z:/D/work/OpenSource/godot/bin/godot.windows.opt.64.exe",QStringList() << "--path" << "C:/Users/Administrator/Documents/新建游戏项目","",&pid);
    //QProcess::startDetached("Z:/D/work/OpenSource/godot/bin/godot.windows.opt.64.exe",QStringList() << "--main-pack" << "C:/Users/Administrator/Documents/gdtest.pck","",&pid);
    //QMessageBox::information(this,QString("%1").arg(pid),"");
    //m_pid = pid;
    m_lastcmd = cmd;
    //process.waitForStarted();
    //timer.start(300);
    if(isWinid){
        m_usercustomWnd = winid;
        timer.start(300);
    }
}

void ProcDockerWidget::started()
{
    // start to check proc cyclely every 300ms
    timer.start(300);
}

void ProcDockerWidget::replaceWidget(QWidget *pluginWidget)
{
    pluginWidget->setObjectName(QStringLiteral("plugin_widget"));
    ui->verticalLayout->replaceWidget(ui->widget, pluginWidget);
    ui->widget = pluginWidget;
}

#ifdef Q_OS_WIN

struct handle_data {
	unsigned long process_id;
	HWND best_handle;
};

bool ProcDockerWidget::IsMainWindow(HWND handle)
{
	return ::GetWindow(handle, GW_OWNER) == (HWND)0/* && ::IsWindowVisible(handle)*/;
}

int ProcDockerWidget::EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	handle_data& data = *(handle_data*)lParam;
	unsigned long process_id = 0;
    ::GetWindowThreadProcessId(handle, &process_id);
	if (data.process_id != process_id || !IsMainWindow(handle)) {
		return TRUE;
	}
	data.best_handle = handle;
	return FALSE;
}
#endif

unsigned long ProcDockerWidget::FindMainWindow(unsigned long process_id)
{
    if(m_usercustomWnd!=0)
        return m_usercustomWnd;
#ifdef Q_OS_WIN
    handle_data data;
    data.process_id = process_id;
    data.best_handle = 0;
    ::EnumWindows(EnumWindowsCallback, (LPARAM)&data);
    return (unsigned long)data.best_handle;
#elif defined(Q_OS_LINUX)
    //TODO: linux os windocker
//    Window *list = NULL;
//    xdo_search_t search;
//    unsigned int nwindows;
//    memset(&search, 0, sizeof(xdo_search_t));
//    search.max_depth = -1;
//    search.require = xdo_search::SEARCH_ALL;
//    search.pid = (int)process_id;
//    search.searchmask |= SEARCH_PID;
//    search.only_visible = True;
//    search.searchmask |= SEARCH_ONLYVISIBLE;
//    search.searchmask |= (SEARCH_NAME | SEARCH_CLASS | SEARCH_CLASSNAME
//            | SEARCH_ROLE);
//    search.searchmask = 24;
//    search.title = 0;
//    search.winname = 0;
//    search.winclass = 0;
//    search.winclassname = 0;
//    search.winrole = 0;
//    xdo_t* xdo = xdo_new(NULL);
//    do {
//        free(list);

//        printf("nwindows=%d\n",nwindows);
//            printf("searchmask=%d\n",search.searchmask);
//            printf("pid=%d\n",search.pid);
//            window_print(search.winname);
//            window_print(search.winclass);
//            window_print(search.winclassname);
//            window_print(search.winrole);
//            xdo->keycode_high = 255;
//            xdo->keycode_low = 8;
//            xdo->keysyms_per_keycode = 7;
//            xdo->close_display_when_freed = 1;
//            xdo->debug = 0;
//            xdo->quiet = 0;
//            xdo->features_mask = 0;
//            QMessageBox::information(this,"",QString("xdo%1-%2-%3-%4-%5-%6-%7-%8-%9-%10-%11").arg((qlonglong)xdo->xdpy).arg(xdo->display_name).arg((qlonglong)xdo->charcodes).arg(xdo->charcodes_len)
//                                     .arg(xdo->keycode_high).arg(xdo->keycode_low).arg(xdo->keysyms_per_keycode).arg(xdo->close_display_when_freed).arg(xdo->quiet).arg(xdo->debug).arg(xdo->features_mask));

//            QMessageBox::information(this,"",QString("search%1-%2-%3-%4-%5-%6-%7-%8-%9-%10-%11-%12-%13").arg(search.title).arg(search.winclass).arg(search.winclassname).arg(search.winname).arg(search.winrole)
//                                     .arg(search.pid).arg(search.max_depth).arg(search.only_visible).arg(search.screen).arg(search.require)
//                                     .arg(search.searchmask).arg(search.desktop).arg(search.limit));
//        xdo_search_windows(xdo, &search, &list, &nwindows);
//        printf("nwindows=%d\n",nwindows);
//        QMessageBox::information(this,"",QString("%1").arg(nwindows));
//        for (int i = 0; i < nwindows; i++) {
//            window_print(list[i]);
//        }

//        if(nwindows == 0){
//            /* TODO(sissel): Make this tunable */
//            usleep(500000);
//        }

//        if(nwindows == 1) {
//            if(xdo)
//                xdo_free(xdo);
//            return list[0];
//        }

//    } while (nwindows == 0);


    // case 2:
    if(m_mainWnd == 0) {
        m_getProcMainWnd.close();
        QString cmd = QString("xdotool search --all --onlyvisible --pid %1").arg(process_id);
        //QMessageBox::information(this,"",cmd);
        m_getProcMainWnd.start(cmd);
        m_getProcMainWnd.waitForStarted(10000);
        QThread::sleep(5);
    }
    else {
        return m_mainWnd;
    }

    return 0;
#endif
}

bool ProcDockerWidget::isProcExistByPid(unsigned long pid)
{
#ifdef Q_OS_WIN
    bool bResult = false;
    // 进程ID有效（非零）
    if (0 != pid)
    {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
        if (NULL == hProcess)
        {
            qInfo() << "exit ProcessID:" << pid << "OpenProcess fail" << GetLastError();
        }
        else
        {
            DWORD dwExitCode = 0;
            BOOL bGet = GetExitCodeProcess(hProcess, &dwExitCode);
            if (bGet && (STILL_ACTIVE == dwExitCode)) // 正在运行
            {
                qInfo() << "check process";
                bResult = true;
            }
            else
            {
                qInfo() << "exit ProcessID:" << pid << "flag:" << bGet << "code:" << dwExitCode;
            }

            CloseHandle(hProcess);
            hProcess = NULL;
        }
    }
    else
    {
        qInfo() << "exit ProcessID invalid";
    }

    return bResult;
#elif defined(Q_OS_LINUX)
    QString proc_pid_path = QString("/proc/%1/status").arg(pid);
    return QFile::exists(proc_pid_path);
#endif
}

void ProcDockerWidget::checkProcMainWnd()
{
    // Sleep(2000);
    unsigned long pid = process.processId();
    //pid = m_pid;
    unsigned long hwnd = FindMainWindow(pid);
    if(!hwnd)
    {
        // 0,未找到主窗口句柄
        label_tip.setVisible(true);
        label_tip.setText(QString(tr("can't find main window of process(id=%1),count=%2")).arg(pid).arg(count++));
        m_mainWnd = 0;
        return;
    }
    // 找到主窗口后,延时1s,等待其他窗口准备完成
    //Sleep(1000);
    QThread::sleep(1);
    QWindow *childWindow = QWindow::fromWinId((int)hwnd);
    if(childWindow)
    {
        childWindow->setFlags(childWindow->flags() | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

        QWidget *m_widget = QWidget::createWindowContainer(childWindow, this, Qt::Widget);
        if(m_widget)
        {
            timer.stop();
            label_tip.setVisible(false);
//            ui->verticalLayout->removeWidget(label_tip);
            replaceWidget(m_widget);
            m_widget->show();

            m_mainWnd = 0;
            m_lastpid = pid;
            timerDemonmainProc.start(300);
        }
    }
}

void ProcDockerWidget::demonMainProc()
{
    //RS_DEBUG->print("demonMainProc\n");
    if(m_lastpid!=0){
        //RS_DEBUG->print("m_lastpid=%d \n", m_lastpid);
        if(!isProcExistByPid(m_lastpid)) {
            //RS_DEBUG->print("m_lastcmd=%s \n", m_lastcmd.toUtf8().data());
            timerDemonmainProc.stop();
            init(m_lastcmd);
        }
    }
}

void ProcDockerWidget::setDrawEnvrionment(IDrawEnvrionment *value)
{
    m_pEnv = value;
}

QString ProcDockerWidget::executeCommand(const QString& cmdType, const QString& cmd) {
    if(cmdType == "pdfreader") {
        if(m_pEnv) {
            RS_DEBUG->print("ProcDockerWidget::executeCommand(%s, %s): Enter\n", cmdType.toUtf8().data(), cmd.toUtf8().data());
            if(!m_pEnv->isStudioMode()) {
                QString pdffile = cmd;
                if(QDir::isRelativePath(pdffile))
                    pdffile = QDir::toNativeSeparators(m_pEnv->projectPath()) + QDir::separator() + "Report_export" + QDir::separator() + pdffile;
                QString pdfShellPath = "C:/MindTech/MindSCADA/MindReportSln/ref/pdfreader/MindPDF.exe";
                if(!QFile::exists(pdfShellPath))
                {
                    pdfShellPath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + QDir::separator() + ".." + QDir::separator()) + "MindReportSln/ref/pdfreader/MindPDF.exe";
                    QDir dir(pdfShellPath);
                    pdfShellPath = dir.absolutePath();
//                        QMessageBox::information(this, pdffile, pdfShellPath);
                    if(!QFile::exists(pdfShellPath))
                        return "can not find MindPDF, please contact Mindteco LLC.";
                }
                QString pdfcmd = QString("\"%1\" -reuse-instance -restrict -presentation -page 1 -view \"continuous single page\" \"%2\"")
                        .arg(pdfShellPath).arg(pdffile);
                init(pdfcmd);
            }
            RS_DEBUG->print("ProcDockerWidget::executeCommand(%s, %s): Exit\n", cmdType.toUtf8().data(), cmd.toUtf8().data());

        }
    }
    // TODO: other cmdType
    else {
        init(cmd);
        // cmdType not recognized
        RS_DEBUG->print("ProcDockerWidget::executeCommand(%1, %2): cmdType is not recognized\n", cmdType.toUtf8().data(), cmd.toUtf8().data());

    }
    return "";
}


/*!
\section 处理命令反馈
*/
/*
服务器 home 目录不可用；
用户没有读写权限
*/
#include <QTextCodec>
#include <QDir>
void ProcDockerWidget::onStandardOutput()
{
    QString output = QString(m_getProcMainWnd.readAllStandardOutput());
    //QMessageBox::information(this,"output",output);
    unsigned long tmp = 0;
    bool ok = false;
    tmp = output.toULong(&ok);
    if(ok)
        m_mainWnd = tmp;
}
//QString ProcDockerWidget::standardOutputToUnicode(const QByteArray& rawBytes)
//{
//    QTextCodec* textCodec = QTextCodec::codecForName("System");
//    if(!textCodec)
//        return QString(rawBytes);

//    // 转换为 UTF8 格式，否则，cmd.exe 输出的汉字是乱码！
//    QString output = textCodec->toUnicode(rawBytes);
//    if(false == is_output_from_cmd_exe(output))
//        output = QString(rawBytes); // git.exe 的输出，本身就是 UTF8 格式
//    return output;
//}
//bool ProcDockerWidget::is_output_from_cmd_exe(const QString& output)
//{
//    return output.contains(QDir::toNativeSeparators("Git\\cmd\\git.exe")) ||
//            output.contains(m_para.m_cmdEndEcho) ||
//            output.contains("Microsoft Windows");
//}

void ProcDockerWidget::onStandardError()
{
    QString error = QString(m_getProcMainWnd.readAllStandardError());
    //QMessageBox::information(this,"error",error);
}

void ProcDockerWidget::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{

}


void ProcDockerWidget::onMainProcFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    m_mainWnd = 0;
}


void ProcDockerWidget::onMainProcErrorOccurred(QProcess::ProcessError error)
{
    if(error == QProcess::ProcessError::Crashed)
    {
        m_mainWnd = 0;
    }
}

void ProcDockerWidget::onMainProcStateChanged(QProcess::ProcessState state)
{

}
