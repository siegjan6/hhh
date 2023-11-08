#ifndef PROCDOCKERWIDGET_H
#define PROCDOCKERWIDGET_H

#include <QWidget>
#include <QProcess>
#include <QTimer>
#include <QLabel>
#include "IDrawEnvironment.h"

#ifdef Q_OS_WIN
    #include <windows.h>
#elif defined (Q_OS_LINUX)
    //TODO: linux os windocker
#elif defined (Q_OS_MAC)
    //TODO: mac os windocker
#else
    //TODO: other os windocker
#endif

namespace Ui {
class ProcDockerWidget;
}

class ProcDockerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProcDockerWidget(QWidget *parent = 0);
    ~ProcDockerWidget();

public:



    void setDrawEnvrionment(IDrawEnvrionment *value);
    /**
     * execute cmd
     */
    QString executeCommand(const QString& cmdType, const QString& cmd);

private:
#ifdef Q_OS_WIN
    static bool IsMainWindow(HWND handle);
    static int CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam);
#elif defined(Q_OS_LINUX)
    //TODO: linux os windocker
#elif defined(Q_OS_MAC)
    //TODO: mac os windocker
#else
    //TODO: other os windocker
#endif


    QProcess process;
    QTimer timer;
    QLabel label_tip;
    int count = 0;
    void init(QString cmd);
    void replaceWidget(QWidget *pluginWidget);
    unsigned long FindMainWindow(unsigned long process_id);

    bool isProcExistByPid(unsigned long pid);

    IDrawEnvrionment* m_pEnv;

    QProcess m_getProcMainWnd;

    QTimer timerDemonmainProc;

//    QString standardOutputToUnicode(const QByteArray& rawBytes);
//    bool is_output_from_cmd_exe(const QString& output);

    unsigned long m_pid;
    unsigned long m_mainWnd;
    unsigned long m_usercustomWnd;

    QString m_lastcmd;
    unsigned long m_lastpid;

private:
    Ui::ProcDockerWidget *ui;

private slots:
    void started();
    void checkProcMainWnd();
    void onStandardOutput();
    void onStandardError();
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void demonMainProc();

    void onMainProcErrorOccurred(QProcess::ProcessError error);
    void onMainProcStateChanged(QProcess::ProcessState state);
    void onMainProcFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif // PROCDOCKERWIDGET_H
