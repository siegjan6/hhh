#include <QMainWindow>
#include <QFileInfo>
#include <QException>
#include <QApplication>
#include <QWindow>
#include <QThread>
#include <QDebug>


#include "FormOperation.h"
#include "MDHMIForm.h"
#include "HMIFramework.h"


CFormOperation::CFormOperation(IHMIFramework *framework)
{
    Q_ASSERT(framework != NULL);
    m_framework = framework;
    m_mainWnd = 0;
    connect(&m_getProcMainWnd, SIGNAL(readyReadStandardOutput()), this, SLOT(onStandardOutput()));
}
//////////////////////////////////////////////////////////////////////////
void CFormOperation::formClosed(QObject* obj)
{
    bool result = m_openedList.removeOne(static_cast<CMDHMIForm*>(obj));
    if(result)
        emit formClosedEvent((dynamic_cast<CMDHMIForm*>(obj))->common()->fullName());
}
void CFormOperation::initalization(CMDHMIForm *f, const QString &fullName)
{
    f->common()->setFullName(fullName);
    m_openedList.append(f);
    emit formOpenedEvent(f->common()->fullName());

    connect(f, &CMDHMIForm::destroyed, this, &CFormOperation::formClosed);

    //studio
    IStudioInterface* project = static_cast<CHMIFramework*>(m_framework)->studioProject();
    if (project != NULL)
        project->openMDIWindow(f);

    f->common()->initalization();
    show(f);
}
void CFormOperation::show(CMDHMIForm *form)
{
    form->show();
    form->setFocus();
    form->showForm();
}

CMDHMIForm *CFormOperation::findOpened(const QString &fullName)
{
    for(int i = 0; i < m_openedList.count(); i++)
    {
        CMDHMIForm* f = m_openedList.at(i);
        if (fullName.compare(f->common()->fullName(), Qt::CaseSensitive) == 0)
            return f;
    }
    return NULL;
}
bool CFormOperation::closeForm(CMDHMIForm *f, bool notSave)
{
    if(f == NULL)
        return false;
    if(notSave)
        f->setModify(false);

    f->closeForm();
    return true;
}

bool CFormOperation::hideForm(CMDHMIForm *f)
{
    if(f == NULL)
        return false;

    f->hideForm();
//    if(f->parentWidget())
//        f->parentWidget()->hide();
    return true;
}
//////////////////////////////////////////////////////////////////////////
QMainWindow *CFormOperation::create(const QString &fullName)
{
    CMDHMIForm* f = new CMDHMIForm(m_framework);

    initalization(f, fullName);
    show(f);
    f->save();
    return f;
}
bool CFormOperation::save(QMainWindow *form)
{
    static_cast<CMDHMIForm*>(form)->save();
    return true;
}
void CFormOperation::saveAll()
{
    for(int i = 0; i < m_openedList.count(); i++)
    {
        CMDHMIForm* f = m_openedList.at(i);
        save(f);
    }
}
QMainWindow *CFormOperation::open(const QString &fullName, int pwid/* = 0*/, int x/* = 0*/, int y/* = 0*/, int w/* = 0*/, int h/* = 0*/)
{
    CHMIFramework* framework = static_cast<CHMIFramework*>(m_framework);
    //has opened
    CMDHMIForm* f = findOpened(fullName);
    if(pwid < 0 && f != NULL)
    {
        // 外部进程第一次调用打开窗口时，第一次打开的窗口pwid为负（等同于当前设置的父窗口与之前保存的父窗口不一致,先关闭再打开）
        closeForm(f);
        if(findOpened(fullName))
            formClosed(f);
        pwid = abs(pwid);
        goto CRASHOPEN;
    }
    if(pwid < 0)
        pwid = abs(pwid);
    if (f != NULL)
    {
        if (m_framework->isStudioMode())
        {
            IStudioInterface* project = framework->studioProject();
            if (project != NULL)
                project->setActiveForm(f);
        }
        else{
            if(pwid >0)
            {
                QWindow* parentWin = QWindow::fromWinId(pwid);
                f->setGeometry(x,y,w,h);
                //TODO: 告诉w它的本地父窗口是proc1Window_HWND,这步务必要做不然w上的一些控件会出问题，比如qcombobox的下拉框可能会错位
                //f->setProperty("_q_embedded_native_parent_handle",QVariant(pwid));
                f->winId();
#ifdef Q_OS_WIN
                Qt::WindowFlags flags = f->windowFlags();
                f->setWindowFlags(flags | Qt::CoverWindow | Qt::WindowStaysOnTopHint);
#elif defined (Q_OS_LINUX)
                f->setWindowFlags(Qt::CoverWindow | Qt::X11BypassWindowManagerHint);
                f->windowHandle()->setParent(parentWin);
#endif
//                f->setWindowFlag(Qt::Popup);


                //TODO: 将进程1的窗口设置为w的父窗口（w嵌入到proc1的窗口中）
                //parentWin = 0;
                //QEvent e(QEvent::EmbeddingControl);
                //QApplication::sendEvent(f, &e);
                //QWidget *m_widget = QWidget::createWindowContainer(parentWin, 0, Qt::Widget);
                //f->setParent(m_widget);
                //f->resize(250,250);
//                f->move(80,100);
//                f->resize(w,h);
//                f->move(x,y);

                f->update();
                f->show();
            }
            show(f);


        }



        return f;
    }

CRASHOPEN:
    f = new CMDHMIForm(m_framework);
    bool success = f->load(fullName);
    if (!success)
    {
        f->close();
        delete f;
        return NULL;
    }
    //security
    if (!m_framework->isStudioMode())
    {
        if (!f->run()->checkSecurity(MDActionType::window_Open))
        {
            f->close();
            delete f;
            return NULL;
        }
    }

    //field
    QMdiArea* mdiArea = framework->mdiArea();
    if (mdiArea != NULL)
    {
        FormType type = f->common()->formType();
        switch (type)
        {
        case FormType::Normal:
            mdiArea->addSubWindow(f);
            break;
        case FormType::Modal:
            f->setParent(mdiArea);
            break;
        default:
            break;
        }
    }

    if(pwid >0)
    {
        QWindow* parentWin = QWindow::fromWinId(pwid);
        f->setGeometry(x,y,w,h);
        f->setPwid(pwid);
        //f->setProperty("_q_embedded_native_parent_handle",QVariant(pwid));
        f->winId();
#ifdef Q_OS_WIN
//        HWND winHandle = (HWND)f->winId();
//        if(winHandle != NULL)
//        {
//            qDebug() << "winHandle =" << winHandle;

           //::ShowWindow(winHandle, SW_HIDE);

            // Remove the window border
            //LONG lStyle = GetWindowLong(winHandle, GWL_STYLE);
            //lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU | WS_POPUP | WS_CHILD);
            //::SetWindowLong(winHandle, GWL_STYLE, lStyle);


//            ::SetParent(winHandle, (HWND)(pwid));//set the server's widget to be the parent of the client win
//            ::SetWindowPos(winHandle, HWND_TOP, x, y, w, h , 0);//SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_NOSIZE | SWP_ASYNCWINDOWPOS);
//            //LONG lExStyle = GetWindowLong(winHandle, GWL_EXSTYLE);
//            //lExStyle |= WS_EX_TOPMOST;
//            //::SetWindowLong(winHandle, GWL_EXSTYLE, lExStyle);
//            //f->setWindowFlags(Qt::WindowStaysOnTopHint);
//             ::UpdateWindow(winHandle);
//             //::ShowWindow(winHandle, SW_SHOW);
//            //SetWindowLong(winHandle, GWL_STYLE, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
//            ::BringWindowToTop(winHandle);
//        }
        Qt::WindowFlags flags = f->windowFlags();
        f->setWindowFlags(flags | Qt::CoverWindow | Qt::WindowStaysOnTopHint);//| Qt::BypassWindowManagerHint);
        //f->setWindowFlags(Qt::ForeignWindow | Qt::BypassGraphicsProxyWidget);
#elif defined (Q_OS_LINUX)
        f->setWindowFlags(Qt::CoverWindow | Qt::X11BypassWindowManagerHint);
        f->windowHandle()->setParent(parentWin);
#endif
        //HWND h = static_cast<HWND>(QGuiApplication::platformNativeInterface() );//   nativeResourceForWindow("handle", window));
        //SetParent(winHandle,pwid);
        //f->windowHandle()->setTransientParent(parentWin);
        //QEvent e(QEvent::EmbeddingControl);
        //QApplication::sendEvent(f, &e);
//        f->resize(w,h);
//        f->move(x,y);

        f->update();
        f->show();
    }

    initalization(f, fullName);

    return f;
}
bool CFormOperation::isOpened(const QString &fullName)
{
    return findOpened(fullName) != NULL;
}
bool CFormOperation::close(const QString &fullName, bool notSave)
{
    return closeForm(findOpened(fullName), notSave);
}
bool CFormOperation::hide(const QString &fullName)
{
    return hideForm(findOpened(fullName));
}
bool CFormOperation::runFormScriptDynamically(const QString &fullName, const QString &script)
{
    //has opened
    CMDHMIForm* f = findOpened(fullName);
    if (f != NULL && f->run())
    {
        return f->run()->runDynamically(script);
    }
    return false;
}
bool CFormOperation::showByPid(int pid, int phwnd/* = 0*/)
{
    unsigned int mainWndId = FindMainWindow(pid, phwnd);
    if(mainWndId >0)
    {
        QWindow* parentWin = QWindow::fromWinId(mainWndId);
        if(parentWin)
        {
            parentWin->show();
#ifdef Q_OS_WIN
            ShowWindow((HWND)mainWndId, 1);
#endif
            return true;
        }
    }
    return false;
}
#include <QMessageBox>
bool CFormOperation::hideByPid(int pid, int phwnd/* = 0*/)
{
    unsigned int mainWndId = FindMainWindow(pid, phwnd);
    //QMessageBox::information(0,"mainWndId",QString("%1").arg(mainWndId));
    if(mainWndId >0)
    {
        QWindow* parentWin = QWindow::fromWinId(mainWndId);
        qDebug() << "mainWndId:" << mainWndId << "parentWin:" << parentWin->winId();
        if(parentWin)
        {
//            parentWin->setVisible(false);
            parentWin->hide();
//            parentWin->setVisibility(QWindow::Visibility::Hidden);
//            parentWin->showMinimized();

            // case 1:
//            WINDOWPLACEMENT   wp;
//            wp.length = sizeof(WINDOWPLACEMENT);
//            wp.flags = WPF_RESTORETOMAXIMIZED;
//            wp.showCmd = SW_HIDE;
//            ::SetWindowPlacement(HWND(mainWndId), &wp);

            // case 2:
#ifdef Q_OS_WIN
            ShowWindow((HWND)mainWndId, 0);
#endif

            // case 3:
//            ::SetWindowLong(HWND(mainWndId), GWL_STYLE, GetWindowLong(HWND(mainWndId), GWL_STYLE) &~WS_EX_TOPMOST);
//            ::SetWindowPos(HWND(mainWndId), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE);
            return true;
        }
    }
    return false;
}
bool CFormOperation::closeByPid(int pid, int phwnd/* = 0*/)
{
    unsigned int mainWndId =  FindMainWindow(pid, phwnd);
    if(mainWndId >0)
    {
        QWindow* parentWin = QWindow::fromWinId(mainWndId);
        if(parentWin)
        {
            parentWin->close();
#ifdef Q_OS_WIN
            CloseWindow((HWND)mainWndId);
            DestroyWindow((HWND)mainWndId);
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
            if (hProcess) {
                return TerminateProcess(hProcess, -1);
            }
#endif
        }
    }
    return false;
}
bool CFormOperation::closeAll()
{
    int count = m_openedList.count();
    for (int i = 0; i < count; i++ )
    {
        CMDHMIForm* f = m_openedList.at(count - i - 1);
        bool result = closeForm(f);
        if(!result)
            return false;
    }

    return true;
}
bool CFormOperation::rename(const QString &newFullName, const QString &oldFullName)
{
    CMDHMIForm* f = findOpened(oldFullName);
    if (f != NULL)
    {
        f->common()->setFullName(newFullName);
        return true;
    }

    return false;
}
CMDHMIForm *CFormOperation::openBackgroundForm(const QString &fullName, bool& hasOpened)
{
    CMDHMIForm* f = findOpened(fullName);
    hasOpened = (f != NULL);
    if(hasOpened)
        return f;

    f = new CMDHMIForm(m_framework);
    bool success = f->load(fullName, true);
    if (!success)
    {
        f->closeForm();
        delete f;
        return NULL;
    }

    f->common()->setFullName(fullName);
    f->common()->initalization();
    return f;
}
//////////////////////////////////////////////////////////////////////////
#ifdef Q_OS_WIN

struct handle_data {
    unsigned long process_id;
    HWND parent_handle;
    HWND best_handle;
};

bool CFormOperation::IsMainWindow(HWND handle, HWND parent_handle)
{
    qDebug() << "handle:" << handle << "GetParent:" << ::GetParent(handle) << "getOwner:" << ::GetWindow(handle, GW_OWNER) << "GetDesktopWindow:" << GetDesktopWindow();
    if(parent_handle == 0) {
        //case 1: topmost and owner not null
        return ::GetParent(handle) == (HWND)0 && ::GetWindow(handle, GW_OWNER) != (HWND)0;
    }
    else {
        // 内嵌式
        return ::GetParent(handle) != (HWND)0 && ::GetWindow(handle, GW_OWNER) == (HWND)0;
    }
    //return ::GetWindow(handle, GW_OWNER) == GetDesktopWindow();
    //return ::GetWindow(handle, GW_OWNER) == (HWND)0/* && ::IsWindowVisible(handle)*/;
}

int CFormOperation::EnumWindowsCallback(HWND handle, LPARAM lParam)
{
    handle_data& data = *(handle_data*)lParam;
    unsigned long process_id = 0;
    ::GetWindowThreadProcessId(handle, &process_id);
    qDebug() << "everyone: pid:" << process_id << "hwnd:" << handle;
    if(data.process_id == process_id && IsMainWindow(handle, data.parent_handle))
    {
        qDebug() << "result: pid:" << process_id << "hwnd:" << handle;
    }
    else
    {
        return TRUE;
    }
//    if (data.process_id != process_id || !IsMainWindow(handle)) {
//        return TRUE;
//    }
    data.best_handle = handle;
    return FALSE;
}
#endif

unsigned long CFormOperation::FindMainWindow(unsigned long process_id, unsigned long parent_hwnd)
{
    //if(m_usercustomWnd!=0)
    //    return m_usercustomWnd;
#ifdef Q_OS_WIN
    handle_data data;
    data.process_id = process_id;
    data.parent_handle = (HWND)parent_hwnd;
    data.best_handle = 0;
    if(parent_hwnd == 0)
        ::EnumWindows(EnumWindowsCallback, (LPARAM)&data);
    else
        ::EnumChildWindows((HWND)parent_hwnd, EnumWindowsCallback, (LPARAM)&data);
    return (unsigned long)data.best_handle;
#elif defined(Q_OS_LINUX)
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

#include <QTextCodec>
#include <QDir>
void CFormOperation::onStandardOutput()
{
    QString output = QString(m_getProcMainWnd.readAllStandardOutput());
    //QMessageBox::information(this,"output",output);
    unsigned long tmp = 0;
    bool ok = false;
    tmp = output.toULong(&ok);
    if(ok)
        m_mainWnd = tmp;
}
//////////////////////////////////////////////////////////////////////////
