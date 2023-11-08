#include "MDWindow.h"
#include <QMessageBox>
#include "MDUiWrappers.h"

// CMDWindow
CMDWindow::CMDWindow()
{
    m_wrapper = NULL;
}

CMDWindow::CMDWindow(const QString& name)
{
    m_name = name;
    m_wrapper = NULL;
}

CMDWindow::~CMDWindow()
{
    if(m_wrapper)
        delete m_wrapper;
}

void CMDWindow::setCode(const QString& code)
{
    m_code = code;
}

QString CMDWindow::getCode() const
{
    return m_code;
}

void CMDWindow::showMsg(const QString& msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}

void CMDWindow::initScript(const CMDObjectWrapperMgr& userWM, const QString& PID)
{
    m_scriptEngine.init(false, PID);

    // 加入窗口自身
    m_wrapper = new CMDWindowWrapper(this, m_name);
    if(m_wrapper)
        m_scriptEngine.addObject(m_wrapper);

    // 加入控件
    // ----------------------------------------
    // ..................

    // 加入用户管理器
    m_scriptEngine.addObjects(userWM);
}

void CMDWindow::runScript()
{
    QString source = getCode();
    m_scriptEngine.run(source);
}

void CMDWindow::releaseScript()
{
    m_scriptEngine.release();
    if(m_wrapper)
    {
        delete m_wrapper;
        m_wrapper = NULL;
    }
}

void CMDWindow::fireEvent_SizeChanged(int x, int y)
{
    QList<QVariant> paras;
    paras.append(QVariant(x));
    paras.append(QVariant(y));
    m_scriptEngine.fireEvent(m_name, "onSizeChanged", paras);
}
