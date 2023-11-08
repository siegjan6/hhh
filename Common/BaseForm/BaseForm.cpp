#include <QString>
#include <QCloseEvent>
#include <QMessageBox>
#include <QWindow>

#include "BaseForm.h"
#include "BaseFormData.h"
///////////////////////////////////////////////////////////////////////////
CBaseForm::CBaseForm(QWidget *parent) :
    QMainWindow(parent)
{
    m_data = new CBaseFormData();
    this->setWindowIcon(QIcon(":/images/ico.png"));
}
CBaseForm::~CBaseForm()
{
    delete m_data;
}
///////////////////////////////////////////////////////////////////////////
void CBaseForm::saveEvent()
{
    if(m_data->m_modify)
    {
        save();
        setModify(false);
    }
}
void CBaseForm::setModify(bool value)
{
    m_data->m_modify = value;
    resetText();
}
bool CBaseForm::isModify()
{
    return m_data->m_modify;
}
void CBaseForm::setPwid(int value)
{
    m_data->m_pwid = value;
}
int CBaseForm::pwid()
{
    return m_data->m_pwid;
}
void CBaseForm::setWindowText(const QString &text)
{
    setWindowTitle(text);
    resetText();
}
void CBaseForm::setActive(bool value)
{
    m_data->m_isActived = value;
}
bool CBaseForm::isActived()
{
    return m_data->m_isActived;
}
void CBaseForm::setNeedScroll(bool value)
{
    m_data->m_needScroll = value;
}
bool CBaseForm::needScroll()
{
    return m_data->m_needScroll;
}
void CBaseForm::setModifiedVisible(bool value)
{
    m_data->m_modifiedVisible = value;
}
bool CBaseForm::modifiedVisible()
{
    return m_data->m_modifiedVisible;
}
QString CBaseForm::factWindowTitle()
{
    QString title = windowTitle();
    if(title.endsWith("*"))
        title = title.left(title.count()-1);
    return title;
}
///////////////////////////////////////////////////////////////////////////
void CBaseForm::closeForm()
{
    QMdiSubWindow* sub = mdiSubWindow();
    //如果有父窗体，关闭父窗体
    if(sub)
    {
        sub->close();
        delete sub;
    }
    else
    {
        close();
        deleteLater();
        if(parentWidget())
            parentWidget()->close();
    }
}

void CBaseForm::hideForm()
{
    QMdiSubWindow* sub = mdiSubWindow();
    //如果有父窗体，隐藏父窗体
    if(sub)
    {
        sub->hide();
    }
    else
    {
        hide();
    }
    QWindow* w = windowHandle();
    if(w)
        w->hide();
}

void CBaseForm::showForm()
{
    QMdiSubWindow* sub = mdiSubWindow();
    //如果有父窗体，显示父窗体
    if(sub)
    {
        sub->show();
    }
    else
    {
        show();
    }
}
///////////////////////////////////////////////////////////////////////////
void CBaseForm::resetText()
{
    QString title = windowTitle();
    bool modified = m_data->m_modify;

    if(title.endsWith("*"))
        title = title.left(title.count()-1);

    title = (modified && modifiedVisible()) ? title+"*" : title;
    setWindowTitle(title);
}
QMdiSubWindow *CBaseForm::mdiSubWindow()
{
    QMdiSubWindow* sub = NULL;
    QObject* p = parent();
    while(p != NULL)
    {
        sub = dynamic_cast<QMdiSubWindow*>(p);
        if(sub != NULL)
            return sub;

        p = p->parent();
    }

    return sub;
}
