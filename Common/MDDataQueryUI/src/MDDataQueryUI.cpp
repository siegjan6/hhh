#include "MDDataQueryUI.h"
#include "CMenu.h"

MDDataQueryUI::MDDataQueryUI(QWidget *parent,bool isReport):
    m_menu(new CMenu(parent,isReport))
{
}

MDDataQueryUI::~MDDataQueryUI()
{
    if(m_menu){
        delete m_menu;
        m_menu = NULL;
    }
}

void MDDataQueryUI::setDataSourceInfor(const QMap<QString, QStringList> &dataSourceInfor)
{
    m_menu->setDataSourceInfor(dataSourceInfor);
}

QMap<QString, QStringList> MDDataQueryUI::dataSourceInfor() const
{
    return m_menu->dataSourceInfor();
}
QString MDDataQueryUI::hostName() const
{
    return m_menu->hostName();
}
QString MDDataQueryUI::driver() const
{
    return m_menu->driver();
}
QString MDDataQueryUI::IP() const
{
    return m_menu->IP();
}
QString MDDataQueryUI::port() const
{
    return m_menu->port();
}
QString MDDataQueryUI::userName() const
{
    return m_menu->userName();
}
QString MDDataQueryUI::password() const
{
    return m_menu->password();
}

bool MDDataQueryUI::isDatabaseEnabled() const
{
    return m_menu->isDatabaseEnabled();
}

int MDDataQueryUI::exec(const QPoint &p) const
{
    return m_menu->exec(p);
}

int MDDataQueryUI::exportType() const
{
    return m_menu->exportType();
}

void MDDataQueryUI::createActionFlag(int num)
{
    m_menu->createActionFlag(num);
}

void MDDataQueryUI::setFilterPluginManager(CMDFilterTreeDataManager *manager)
{
    m_menu->setFilterPluginManager(manager);
}
