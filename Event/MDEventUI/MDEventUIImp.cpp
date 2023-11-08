#include "MDEventUIImp.h"
#include "../../include/MDProjectConfiger/MDProjectConfiger.h"
#include "MDEventTreeWindow.h"
#include "MDStringResource.h"

CMDEventUIImp::CMDEventUIImp(QObject *parent) :
    QObject(parent)
{
    m_studioInterface = NULL;
    m_rootInStudio = NULL;

    m_contacter = NULL;
    m_evenTreeWindow = NULL;
}

CMDEventUIImp::~CMDEventUIImp()
{

}

/*!
\section 初始化
*/

void CMDEventUIImp::load()
{
    buildEventTree(treeRoot());
}

void CMDEventUIImp::unLoad()
{
    if(m_evenTreeWindow)
    {
        m_evenTreeWindow->close();
        delete m_evenTreeWindow;
        m_evenTreeWindow = NULL;
    }
}

/*!
\section 与外界联系的接口
*/
void CMDEventUIImp::setContacter(CMDEventUIContacter* contacter)
{
    m_contacter = contacter;
}

CMDEventUIContacter* CMDEventUIImp::contacter()
{
    return m_contacter;
}

/*!
\section 与开发器交互；通过树控件管理对象
*/
void CMDEventUIImp::onMouseRClick(QTreeWidgetItem *pItem ,const QPoint &pos)
{

}

void CMDEventUIImp::onMouseLClick(QTreeWidgetItem *pItem,int column)
{

}

void CMDEventUIImp::onMouseDbClick(QTreeWidgetItem *pItem,int column)
{
    if(!m_studioInterface)
        return;

    CMDEventConfiger* configer = m_studioInterface->projectConfiger()->eventConfiger();
    if(!configer)
        return;

    QVariant d = pItem->data(0, Qt::UserRole);
     if(((qulonglong)this) == d.toULongLong())
     {
         if (!m_evenTreeWindow)
         {
             m_evenTreeWindow = new CMDEventTreeWindow();
             m_evenTreeWindow->init(this, configer);
             m_studioInterface->openMDIWindow(m_evenTreeWindow);
             connect(m_evenTreeWindow, SIGNAL(destroyed(QObject*)),this, SLOT(on_eventTreeWindow_destroyed(QObject*)));
         }
         else m_studioInterface->setActiveForm(m_evenTreeWindow);
     }
}

void CMDEventUIImp::buildEventTree(QTreeWidgetItem *pTreeRoot)
{
    connect(treeWidget(), &QTreeWidget::itemDoubleClicked, this, &CMDEventUIImp::onMouseDbClick);

    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList()<<STRANS(STRING_EVENT_TEXT), EVENT_TREEITEM_TYPE);
    item->setIcon(0,QIcon(":/images/file.png"));
    item->setCheckState(1, Qt::Checked);
    item->setData(0, Qt::UserRole, (qulonglong)this);
    pTreeRoot->addChild(item);
}

void CMDEventUIImp::on_eventTreeWindow_destroyed(QObject* obj)
{
    if(m_evenTreeWindow)
    {
        m_evenTreeWindow->deleteLater();
        m_evenTreeWindow = NULL;
    }
}

QTreeWidget *CMDEventUIImp::treeWidget()
{
    return (m_studioInterface ? m_studioInterface->treeWidget() : NULL);
}

QTreeWidgetItem *CMDEventUIImp::treeRoot()
{
    return m_rootInStudio;
}

QTreeWidgetItem *CMDEventUIImp::selectedTreeItem()
{
    QTreeWidget *treeW = treeWidget();
    return (treeW ? treeW->currentItem() : NULL);
}

/*!
\section 多语言
*/
void CMDEventUIImp::changeLanguage()
{
    int count = (m_rootInStudio ? m_rootInStudio->childCount() : 0);

    for(int i = 0; i < count; i++)
    {
        QTreeWidgetItem* myItem = m_rootInStudio->child(i);
        if(myItem->type() == EVENT_TREEITEM_TYPE)
        {
            QString translated = STRANS(STRING_EVENT_TEXT);
            myItem->setText(0, translated);
        }
    }

    if(m_evenTreeWindow)
        m_evenTreeWindow->changeLanguage();
}

void CMDEventUIImp::collectWords()
{
    CMDEventConfiger* configer = m_studioInterface->projectConfiger()->eventConfiger();
    if(!configer)
        return;

    QList<CMDEventProperty_Config*> eventList;
    configer->configedEventList(eventList);
    foreach (const CMDEventProperty_Config* epc, eventList)
    {
        CMDMultiLanguage::instance()->addWord(epc->comment(), false);
    }
}
