#include "MDActionUIImp.h"
#include "../../include/MDProjectConfiger/MDProjectConfiger.h"
#include "MDActionTreeWindow.h"
#include "MDStringResource.h"

CMDActionUIImp::CMDActionUIImp(QObject *parent) :
    QObject(parent)
{
    m_studioInterface = NULL;
    m_rootInStudio = NULL;

    m_contacter = NULL;
    m_actionTreeWindow = NULL;
}

CMDActionUIImp::~CMDActionUIImp()
{

}

/*!
\section 初始化
*/

void CMDActionUIImp::load()
{
    buildActionTree(treeRoot());
}

void CMDActionUIImp::unLoad()
{
    if(m_actionTreeWindow)
    {
        m_actionTreeWindow->close();
        delete m_actionTreeWindow;
        m_actionTreeWindow = NULL;
    }
}

/*!
\section 与外界联系的接口
*/
void CMDActionUIImp::setContacter(CMDActionUIContacter* contacter)
{
    m_contacter = contacter;
}

CMDActionUIContacter* CMDActionUIImp::contacter()
{
    return m_contacter;
}

/*!
\section 与开发器交互；通过树控件管理对象
*/
void CMDActionUIImp::onMouseRClick(QTreeWidgetItem *pItem ,const QPoint &pos)
{

}

void CMDActionUIImp::onMouseLClick(QTreeWidgetItem *pItem,int column)
{

}

void CMDActionUIImp::onMouseDbClick(QTreeWidgetItem *pItem,int column)
{
    if(!m_studioInterface)
        return;

    CMDActionConfiger* configer = m_studioInterface->projectConfiger()->actionConfiger();
    if(!configer)
        return;

    QVariant d = pItem->data(0, Qt::UserRole);
     if(((qulonglong)this) == d.toULongLong())
     {
         if (!m_actionTreeWindow)
         {
             m_actionTreeWindow = new CMDActionTreeWindow();
             m_actionTreeWindow->init(this, configer);
             m_studioInterface->openMDIWindow(m_actionTreeWindow);
             connect(m_actionTreeWindow, SIGNAL(destroyed(QObject*)),this, SLOT(on_actionTreeWindow_destroyed(QObject*)));
         }
         else m_studioInterface->setActiveForm(m_actionTreeWindow);
     }
}

void CMDActionUIImp::buildActionTree(QTreeWidgetItem *pTreeRoot)
{
    connect(treeWidget(), &QTreeWidget::itemDoubleClicked, this, &CMDActionUIImp::onMouseDbClick);

    QTreeWidgetItem* actionItem = new QTreeWidgetItem(QStringList()<<STRANS(STRING_ACTION_TEXT), ACTION_TREEITEM_TYPE);
    actionItem->setIcon(0,QIcon(":/images/file.png"));
    actionItem->setCheckState(1, Qt::Checked);
    actionItem->setData(0, Qt::UserRole, (qulonglong)this);
    pTreeRoot->addChild(actionItem);
}

void CMDActionUIImp::on_actionTreeWindow_destroyed(QObject* obj)
{
    if(m_actionTreeWindow)
    {
        m_actionTreeWindow->deleteLater();
        m_actionTreeWindow = NULL;
    }
}

QTreeWidget *CMDActionUIImp::treeWidget()
{
    return (m_studioInterface ? m_studioInterface->treeWidget() : NULL);
}

QTreeWidgetItem *CMDActionUIImp::treeRoot()
{
    return m_rootInStudio;
}

QTreeWidgetItem *CMDActionUIImp::selectedTreeItem()
{
    QTreeWidget *treeW = treeWidget();
    return (treeW ? treeW->currentItem() : NULL);
}

/*!
\section 显示对象选择界面
*/
bool CMDActionUIImp::selectActions(const QStringList& sourceList, QStringList& destList)
{
    CMDActionTreeWindow win;
    win.init(this, m_studioInterface->projectConfiger()->actionConfiger());
    return win.doModalSelectObjectUI(STRANS(STRING_SELECT_ACTION), (qint16)MDObjectType::action, sourceList, destList);
}

/*!
\section 多语言
*/
void CMDActionUIImp::changeLanguage()
{
    int count = (m_rootInStudio ? m_rootInStudio->childCount() : 0);

    for(int i = 0; i < count; i++)
    {
        QTreeWidgetItem* myItem = m_rootInStudio->child(i);
        if(myItem->type() == ACTION_TREEITEM_TYPE)
        {
            QString translated = STRANS(STRING_ACTION_TEXT);
            myItem->setText(0, translated);
        }
    }

    if(m_actionTreeWindow)
        m_actionTreeWindow->changeLanguage();
}

void CMDActionUIImp::collectWords()
{
    CMDActionConfiger* configer = m_studioInterface->projectConfiger()->actionConfiger();
    if(!configer)
        return;

    QList<CMDActionProperty_Config*> actionList;
    configer->configedActionList(actionList);
    foreach (const CMDActionProperty_Config* apc, actionList)
    {
        CMDMultiLanguage::instance()->addWord(apc->comment(), false);
    }
}
