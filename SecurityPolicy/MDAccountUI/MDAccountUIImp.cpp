#include "MDAccountUIImp.h"
#include "../../include/MDAccountConfiger/MDAccountConfiger.h"
#include "MDAccountProperty_Config.h"
#include "MDAccountTreeWindow.h"
#include "../../include/MDCommonHelper/MDCommonConstant.h"
#include "MDSecurityPolicyConfiger.h"
#include "MDProjectConfiger.h"
#include "MDStringResource.h"

CMDAccountUIImp::CMDAccountUIImp(QObject *parent) :
    QObject(parent)
{
    m_studioInterface = NULL;
    m_rootInStudio = NULL;

    m_accountTreeWindow = NULL;
}

CMDAccountUIImp::~CMDAccountUIImp()
{

}

/*!
\section 初始化
*/
void CMDAccountUIImp::load()
{
    buildAccountTree(treeRoot());
}

void CMDAccountUIImp::unLoad()
{
    if(m_accountTreeWindow)
    {
        m_accountTreeWindow->close();
        delete m_accountTreeWindow;
        m_accountTreeWindow = NULL;
    }
}

/*!
\section 与开发器交互；通过树控件管理对象
*/
void CMDAccountUIImp::onMouseRClick(QTreeWidgetItem *pItem ,const QPoint &pos)
{

}

void CMDAccountUIImp::onMouseLClick(QTreeWidgetItem *pItem,int column)
{

}

void CMDAccountUIImp::onMouseDbClick(QTreeWidgetItem *pItem,int column)
{
    if(!m_studioInterface)
        return;

    QVariant d = pItem->data(0, Qt::UserRole);
     if(((qulonglong)this) == d.toULongLong())
     {
         if (!m_accountTreeWindow)
         {
             m_accountTreeWindow = new CMDAccountTreeWindow();
             m_accountTreeWindow->init(this, m_studioInterface->projectConfiger()->securityConfiger()->accountConfiger());
             m_studioInterface->openMDIWindow(m_accountTreeWindow);
             connect(m_accountTreeWindow, SIGNAL(destroyed(QObject*)),this, SLOT(on_accountTreeWindow_destroyed(QObject*)));
         }
         else m_studioInterface->setActiveForm(m_accountTreeWindow);
     }
}

void CMDAccountUIImp::buildAccountTree(QTreeWidgetItem *pTreeRoot)
{
    connect(treeWidget(), &QTreeWidget::itemDoubleClicked, this, &CMDAccountUIImp::onMouseDbClick);

    QTreeWidgetItem* item= new QTreeWidgetItem(QStringList()<<STRANS(STRING_ACCOUNT_TEXT), ACCOUNT_TREEITEM_TYPE);
    item->setIcon(0,QIcon(":/images/file.png"));
    item->setCheckState(1, Qt::Checked);
    item->setData(0, Qt::UserRole, (qulonglong)this);
    pTreeRoot->addChild(item);
}

void CMDAccountUIImp::on_accountTreeWindow_destroyed(QObject* obj)
{
    if(m_accountTreeWindow)
    {
        m_accountTreeWindow->deleteLater();
        m_accountTreeWindow = NULL;
    }
}

QTreeWidget *CMDAccountUIImp::treeWidget()
{
    return (m_studioInterface ? m_studioInterface->treeWidget() : NULL);
}

QTreeWidgetItem *CMDAccountUIImp::treeRoot()
{
    return m_rootInStudio;
}

QTreeWidgetItem *CMDAccountUIImp::selectedTreeItem()
{
    QTreeWidget *treeW = treeWidget();
    return (treeW ? treeW->currentItem() : NULL);
}

/*!
\section 显示对象选择界面
*/
bool CMDAccountUIImp::selectAccounts(const QStringList& sourceList, QStringList& destList)
{
    CMDAccountTreeWindow win;
    win.init(this, m_studioInterface->projectConfiger()->securityConfiger()->accountConfiger());
    return win.doModalSelectObjectUI(STRANS(STRING_SELECT_ACCOUNT), (qint16)MDObjectType::account, sourceList, destList);
}

/*!
\section 创建对象树管理界面，目前用于运行时管理帐户
*/
QWidget* CMDAccountUIImp::createMgrWidget()
{
    CMDAccountTreeWindow* treeWindow = new CMDAccountTreeWindow();
    treeWindow->init(this, m_studioInterface->projectConfiger()->securityConfiger()->accountConfiger());
    return treeWindow;
}

/*!
\section 多语言
*/
void CMDAccountUIImp::changeLanguage()
{
    int count = (m_rootInStudio ? m_rootInStudio->childCount() : 0);

    for(int i = 0; i < count; i++)
    {
        QTreeWidgetItem* myItem = m_rootInStudio->child(i);
        if(myItem->type() == ACCOUNT_TREEITEM_TYPE)
        {
            QString translated = STRANS(STRING_ACCOUNT_TEXT);
            myItem->setText(0, translated);
        }
    }

    if(m_accountTreeWindow)
        m_accountTreeWindow->changeLanguage();
}

void CMDAccountUIImp::collectWords()
{
    QList<CMDAccountProperty_Config*> accountList;
    m_studioInterface->projectConfiger()->securityConfiger()->accountConfiger()->configedList(accountList);
    foreach (const CMDAccountProperty_Config* cp, accountList)
    {
        CMDMultiLanguage::instance()->addWord(cp->comment(), false);
    }
}
