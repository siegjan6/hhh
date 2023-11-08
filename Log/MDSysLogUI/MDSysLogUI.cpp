#include "MDSysLogUI.h"
#include "MDSysLogUIImp.h"

CMDSysLogUI::CMDSysLogUI()
{
    m_imp = new CMDSysLogUIImp;
}

CMDSysLogUI::~CMDSysLogUI()
{
    if(m_imp)
        delete m_imp;
}

QDialog* CMDSysLogUI::createModaless(CMDSysLogger* logger, const QString& title, QWidget *parent)
{
    return (m_imp ? m_imp->createModaless(logger, title, parent) : NULL);
}

bool CMDSysLogUI::destroyModaless(QDialog* loggerDialog)
{
    return (m_imp ? m_imp->destroyModaless(loggerDialog) : false);
}

void CMDSysLogUI::showModal(CMDSysLogger* logger, const QString& title, QWidget *parent)
{
    if(m_imp)
        m_imp->showModal(logger, title, parent);
}

void CMDSysLogUI::showModaless(CMDSysLogger* logger, const QString& title, QWidget *parent)
{
    if(m_imp)
        m_imp->showModaless(logger, title, parent);
}

void CMDSysLogUI::closeAll()
{
    if(m_imp)
        m_imp->closeAll();
}
