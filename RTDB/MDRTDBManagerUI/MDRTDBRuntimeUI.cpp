#include "MDRTDBRuntimeUI.h"
#include "MDRTDBRuntimeDialog.h"
CMDRTDBRuntimeUI::CMDRTDBRuntimeUI()
{
        m_mdProjectConfiger = NULL;
        m_rtdbDialog = NULL;
}
CMDRTDBRuntimeUI::~CMDRTDBRuntimeUI()
{
    closeRTDBWindow();
}
void CMDRTDBRuntimeUI::setProjectConfiger(CMDProjectConfiger *projectConfiger)
{
        m_mdProjectConfiger = projectConfiger;
}
/*!
\section 打开/关闭实时数据库对话框
*/
void CMDRTDBRuntimeUI::openRTDBWindow(QWidget *parent)
{
    if(m_mdProjectConfiger ==NULL)
    {
        return;
    }
    if(m_rtdbDialog==NULL)
    {
        m_rtdbDialog = new CMDRTDBRuntimeDialog(parent);
        m_rtdbDialog->setProjectConfiger(m_mdProjectConfiger);
        //m_rtdbDialog->setWindowFlags(Qt::Tool |Qt::WindowStaysOnTopHint);//Y2hw
        if(parent)
        {
            m_rtdbDialog->resize(QSize(parent->size().width()*0.75,parent->size().height()*0.75));
        }
        m_rtdbDialog->show();
        if(parent)
        {

            m_rtdbDialog->move(parent->pos().x()+parent->size().width()*0.125,parent->pos().y()+parent->size().height()*0.125);
        }
         connect(m_rtdbDialog, SIGNAL(accepted()),this, SLOT(on_Closed()));
         connect(m_rtdbDialog, SIGNAL(rejected()),this, SLOT(on_Closed()));
    }
    else
    {
        m_rtdbDialog->activateWindow();
    }
}

void CMDRTDBRuntimeUI::closeRTDBWindow()
{
    if(m_rtdbDialog!=NULL)
    {
        m_rtdbDialog->close();
        m_rtdbDialog->deleteLater();
        m_rtdbDialog = NULL;
    }
}

void CMDRTDBRuntimeUI::on_Closed()
{
    if(m_rtdbDialog!=NULL)
    {
        m_rtdbDialog->deleteLater();
        m_rtdbDialog = NULL;
    }
}

void CMDRTDBRuntimeUI::changeLanguage()
{
    
}
