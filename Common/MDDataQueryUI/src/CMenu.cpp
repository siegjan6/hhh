#include "CMenu.h"
#include <QAction>
#include "CDataSourceDialog.h"
#include "CFilterDialog.h"
#include "CExportDialog.h"
#include "CContants.h"
#include "MDMultiLanguage.h"

CMenu::CMenu(QWidget *parent, bool isReport) :
    QWidget(parent),
    m_menu(new QMenu),
    m_parent(parent),
    m_filterDialog(new CFilterDialog(parent)),
    m_exportDialog(new CExportDialog(parent)),
    m_dataSourceDialog(new CDataSourceDialog(parent,isReport)),
    m_filterManager(NULL)
{

}

CMenu::~CMenu()
{
    if(m_menu){
        delete m_menu;
        m_menu = NULL;
    }
    if(m_dataSourceDialog){
        delete m_dataSourceDialog;
        m_dataSourceDialog = NULL;
    }
    if(m_filterDialog){
        delete m_filterDialog;
        m_filterDialog = NULL;
    }
    if(m_exportDialog){
        delete m_exportDialog;
        m_exportDialog = NULL;
    }
}

void CMenu::setDataSourceInfor(const QMap<QString, QStringList> &dataSourceInfor)
{
    m_dataInfor = dataSourceInfor;
}

QMap<QString, QStringList> CMenu::dataSourceInfor() const
{
    return m_dataInfor;
}
QString CMenu::hostName() const
{
    return m_dataSourceDialog->hostName();
}
QString CMenu::driver() const
{
    return m_dataSourceDialog->driver();
}
QString CMenu::IP() const
{
    return m_dataSourceDialog->IP();
}
QString CMenu::port() const
{
    return m_dataSourceDialog->port();
}
QString CMenu::userName() const
{
    return m_dataSourceDialog->userName();
}
QString CMenu::password() const
{
    return m_dataSourceDialog->password();
}

bool CMenu::isDatabaseEnabled() const
{
    return m_dataSourceDialog->isDatabaseEnabled();
}

int CMenu::exportType() const
{
    return m_exportDialog->exportType();
}

int CMenu::exec(const QPoint &p)
{    
    QAction *action = m_menu->exec(p);
    if(!action){
        return (int)ReturnType::Rejected;
    }
    switch (ActionType(action->data().toInt())) {
    case ActionType::Data:
    {
        m_dataSourceDialog->setDataSourceInfor(m_dataInfor);
        if(QDialog::Accepted == m_dataSourceDialog->exec()){
            m_dataInfor = m_dataSourceDialog->dataSourceInfor();
            return (int)ReturnType::DataSource;
        }
        return (int)ReturnType::Rejected;
    }
    case ActionType::Filter:
    {
        m_filterDialog->setFilterPluginManager(m_filterManager);
        m_filterDialog->setDataInfor(m_dataInfor);        
        if(QDialog::Accepted == m_filterDialog->exec()){
            m_dataInfor = m_filterDialog->dataInfor();
            return (int)ReturnType::Filter;
        }
        return (int)ReturnType::Rejected;
    }
    case ActionType::Export:
    {
        if(QDialog::Accepted == m_exportDialog->exec()){
            return (int)ReturnType::Export;
        }
        return (int)ReturnType::Rejected;
    }
    default:
        break;
    }

    return (int)ReturnType::Rejected;
}

void CMenu::setFilterPluginManager(CMDFilterTreeDataManager *manager)
{
    m_filterManager = manager;
}

void CMenu::createActionFlag(int num)
{
    CMDMultiLanguage::instance()->translateWidget(m_menu);
//    CMDMultiLanguage::instance()->translateWidget(m_filterDialog);
//    CMDMultiLanguage::instance()->translateWidget(m_dataSourceDialog);

    ITranslateWidgetTool *tool = CMDMultiLanguage::instance()->createWidgetTool();
    tool->init(this);

    switch (num) {
    case (int)ActionType::Data:
        createAction(tr("数据源配置"),ActionType::Data);
        m_menu->addSeparator();
        break;
    case (int)ActionType::Filter:
        createAction(tr("查询"),ActionType::Filter);
        m_menu->addSeparator();
        break;
    case (int)ActionType::Export:
        createAction(tr("导出"),ActionType::Export);
        break;
    default:
        break;
    }
}

void CMenu::createAction(const QString &name, CMenu::ActionType type)
{
    QAction * action = m_menu->addAction(name);
    action->setData((int)(type));
}
