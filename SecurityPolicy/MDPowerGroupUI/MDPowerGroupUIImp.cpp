#include "MDPowerGroupUIImp.h"
#include "../../include/MDPowerGroupConfiger/MDPowerGroupConfiger.h"
#include "MDProjectConfiger.h"
#include "MDPowerGroup.h"
#include "MDPowerGroupTreeWindow.h"
#include "../../include/MDCommonHelper/MDCommonConstant.h"
#include "MDStringResource.h"

CMDPowerGroupUIImp::CMDPowerGroupUIImp(QObject *parent) :
    QObject(parent)
{
    m_studioInterface = NULL;
    m_rootInStudio = NULL;

    m_accountUI = NULL;
    m_powerGroupTreeWindow = NULL;
}

CMDPowerGroupUIImp::~CMDPowerGroupUIImp()
{

}

/*!
\section 初始化
*/
void CMDPowerGroupUIImp::setAccountUI(CMDAccountUI* accountUI)
{
    m_accountUI = accountUI;
}

void CMDPowerGroupUIImp::load()
{
    buildPowerGroupTree(treeRoot());
}

void CMDPowerGroupUIImp::unLoad()
{
    if(m_powerGroupTreeWindow)
    {
        m_powerGroupTreeWindow->close();
        delete m_powerGroupTreeWindow;
        m_powerGroupTreeWindow = NULL;
    }
}

/*!
\section 与开发器交互；通过树控件管理对象
*/
void CMDPowerGroupUIImp::onMouseRClick(QTreeWidgetItem *pItem ,const QPoint &pos)
{

}

void CMDPowerGroupUIImp::onMouseLClick(QTreeWidgetItem *pItem,int column)
{

}

void CMDPowerGroupUIImp::onMouseDbClick(QTreeWidgetItem *pItem,int column)
{
    if(!m_studioInterface)
        return;

    QVariant d = pItem->data(0, Qt::UserRole);
     if(((qulonglong)this) == d.toULongLong())
     {
         if (!m_powerGroupTreeWindow)
         {
             m_powerGroupTreeWindow = new CMDPowerGroupTreeWindow();
             m_powerGroupTreeWindow->init(this, m_studioInterface->projectConfiger()->powerGroupConfiger());
             m_studioInterface->openMDIWindow(m_powerGroupTreeWindow);
             connect(m_powerGroupTreeWindow, SIGNAL(destroyed(QObject*)),this, SLOT(on_powerGroupTreeWindow_destroyed(QObject*)));
         }
         else m_studioInterface->setActiveForm(m_powerGroupTreeWindow);
     }
}

void CMDPowerGroupUIImp::buildPowerGroupTree(QTreeWidgetItem *pTreeRoot)
{
    connect(treeWidget(), &QTreeWidget::itemDoubleClicked, this, &CMDPowerGroupUIImp::onMouseDbClick);

    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList()<<STRANS(STRING_POWER_GROUP_TEXT), POWER_GROUP_TREEITEM_TYPE);
    item->setIcon(0,QIcon(":/images/file.png"));
    item->setCheckState(1, Qt::Checked);
    item->setData(0, Qt::UserRole, (qulonglong)this);
    pTreeRoot->addChild(item);
}

void CMDPowerGroupUIImp::on_powerGroupTreeWindow_destroyed(QObject* obj)
{
    if(m_powerGroupTreeWindow)
    {
        m_powerGroupTreeWindow->deleteLater();
        m_powerGroupTreeWindow = NULL;
    }
}

QTreeWidget *CMDPowerGroupUIImp::treeWidget()
{
    return (m_studioInterface ? m_studioInterface->treeWidget() : NULL);
}

QTreeWidgetItem *CMDPowerGroupUIImp::treeRoot()
{
    return m_rootInStudio;
}

QTreeWidgetItem *CMDPowerGroupUIImp::selectedTreeItem()
{
    QTreeWidget *treeW = treeWidget();
    return (treeW ? treeW->currentItem() : NULL);
}

/*!
\section 显示对象选择界面
*/
bool CMDPowerGroupUIImp::selectPowerGroups(const QStringList& sourceList, QStringList& destList)
{
    CMDPowerGroupTreeWindow win;
    win.init(this, m_studioInterface->projectConfiger()->powerGroupConfiger());
    return win.doModalSelectObjectUI(STRANS(STRING_SELECT_POWER_GROUP), (qint16)MDObjectType::powerGroup, sourceList, destList);
}

/*!
\section 创建对象树管理界面，目前用于运行时管理权限组
*/
QWidget* CMDPowerGroupUIImp::createMgrWidget()
{
    CMDPowerGroupTreeWindow* treeWindow = new CMDPowerGroupTreeWindow();
    treeWindow->init(this, m_studioInterface->projectConfiger()->powerGroupConfiger());
    return treeWindow;
}

/*!
\section 多语言
*/
void CMDPowerGroupUIImp::changeLanguage()
{
    int count = (m_rootInStudio ? m_rootInStudio->childCount() : 0);

    for(int i = 0; i < count; i++)
    {
        QTreeWidgetItem* myItem = m_rootInStudio->child(i);
        if(myItem->type() == POWER_GROUP_TREEITEM_TYPE)
        {
            QString translated = STRANS(STRING_POWER_GROUP_TEXT);
            myItem->setText(0, translated);
        }
    }

    if(m_powerGroupTreeWindow)
        m_powerGroupTreeWindow->changeLanguage();
}

void CMDPowerGroupUIImp::collectWords()
{
    QList<CMDPowerGroup*> pgList;
    m_studioInterface->projectConfiger()->powerGroupConfiger()->configedList(pgList);
    foreach (const CMDPowerGroup* pg, pgList)
    {
        CMDMultiLanguage::instance()->addWord(pg->comment(), false);
    }
}
