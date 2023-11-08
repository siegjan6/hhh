/*!
\author aGRw 2014.11.05
\section 窗体管理类
*/
#ifndef FORMOPERATION_H
#define FORMOPERATION_H

#include <QString>
#include <QObject>
#include <QMainWindow>
#include <QProcess>

#include "IHMIFramework.h"
#include "IFormOperation.h"

#ifdef Q_OS_WIN
    #include <windows.h>
#elif defined (Q_OS_LINUX)
    //TODO: linux os windocker
#elif defined (Q_OS_MAC)
    //TODO: mac os windocker
#else
    //TODO: other os windocker
#endif

class CMDHMIForm;

class CFormOperation : public QObject, public IFormOperation
{
    Q_OBJECT
public:
    CFormOperation(IHMIFramework *framework);
/*!
\section field
*/
private:
    IHMIFramework* m_framework;
/*!
\section property
*/
public:
    //!打开的窗体列表
    QList<CMDHMIForm*>* openedList(){return &m_openedList;}
private:
    QList<CMDHMIForm*> m_openedList;
/*!
\section private function
*/
private:
    void formClosed(QObject *obj);
    void initalization(CMDHMIForm *f, const QString& fullName);
    void show(CMDHMIForm *form);
    CMDHMIForm *findOpened(const QString& fullName);
    bool closeForm(CMDHMIForm* f, bool notSave = false);
    bool hideForm(CMDHMIForm* f);

#ifdef Q_OS_WIN
    static bool IsMainWindow(HWND handle, HWND parent_handle);
    static int CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam);
#elif defined(Q_OS_LINUX)
    //TODO: linux os windocker
#elif defined(Q_OS_MAC)
    //TODO: mac os windocker
#else
    //TODO: other os windocker
#endif

    unsigned long FindMainWindow(unsigned long process_id, unsigned long parent_hwnd = 0);
    unsigned long m_pid;
    unsigned long m_mainWnd;
    unsigned long m_usercustomWnd;

    QProcess m_getProcMainWnd;

private slots:
    void onStandardOutput();

/*!
\section public function
*/
public:
    QMainWindow *create(const QString& fullName);
    bool save(QMainWindow* form);
    void saveAll();
    QMainWindow *open(const QString& fullName, int pwid = 0, int x = 0, int y = 0, int w = 0, int h = 0);
    bool isOpened(const QString& fullName);
    bool close(const QString& fullName, bool notSave = false);
    bool hide(const QString &fullName);
    bool runFormScriptDynamically(const QString &fullName, const QString &script);
    bool showByPid(int pid, int phwnd = 0);
    bool hideByPid(int pid, int phwnd = 0);
    bool closeByPid(int pid, int phwnd = 0);
    bool closeAll();
    bool rename(const QString& newFullName, const QString& oldFullName);
    //!隐蔽打开文件
    CMDHMIForm* openBackgroundForm(const QString& fullName, bool &hasOpened);
/*!
\section signals
*/
signals:
    void formOpenedEvent(const QString& fullName);
    void formClosedEvent(const QString& fullName);
};

#endif // FORMOPERATION_H




