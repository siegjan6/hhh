#include "MDAlarmConditionUIImp.h"
#include "MDProjectConfiger.h"
#include "MDAlarmConditionConfiger.h"
#include "MDAlarmCondition.h"
#include "MDAlarmConditionTreeWindow.h"
#include "../../include/MDCommonHelper/MDCommonConstant.h"
#include "MDStringResource.h"

CMDAlarmConditionUIImp::CMDAlarmConditionUIImp(QObject *parent) :
    QObject(parent)
{
    m_studioInterface = NULL;
    m_rootInStudio = NULL;

    m_contacter = NULL;
    m_treeWindow = NULL;
}

CMDAlarmConditionUIImp::~CMDAlarmConditionUIImp()
{

}

/*!
\section 初始化
*/

void CMDAlarmConditionUIImp::load()
{
    buildTree(treeRoot());
}

void CMDAlarmConditionUIImp::unLoad()
{
    if(m_treeWindow)
    {
        m_treeWindow->close();
        delete m_treeWindow;
        m_treeWindow = NULL;
    }
}

/*!
\section 与外界联系的接口
*/
void CMDAlarmConditionUIImp::setContacter(CMDAlarmConditionUIContacter* contacter)
{
    m_contacter = contacter;
}

CMDAlarmConditionUIContacter* CMDAlarmConditionUIImp::contacter()
{
    return m_contacter;
}

/*!
\section 与开发器交互；通过树控件管理对象
*/
void CMDAlarmConditionUIImp::onMouseRClick(QTreeWidgetItem *pItem ,const QPoint &pos)
{

}

void CMDAlarmConditionUIImp::onMouseLClick(QTreeWidgetItem *pItem,int column)
{

}

void CMDAlarmConditionUIImp::onMouseDbClick(QTreeWidgetItem *pItem,int column)
{
    if(!m_studioInterface)
        return;

    QVariant d = pItem->data(0, Qt::UserRole);
     if(((qulonglong)this) == d.toULongLong())
     {
         if (!m_treeWindow)
         {
             m_treeWindow = new CMDAlarmConditionTreeWindow();
             m_treeWindow->init(this, m_studioInterface->projectConfiger()->conditionConfiger());
             m_studioInterface->openMDIWindow(m_treeWindow);
             connect(m_treeWindow, SIGNAL(destroyed(QObject*)),this, SLOT(on_treeWindow_destroyed(QObject*)));
         }
         else m_studioInterface->setActiveForm(m_treeWindow);
     }
}

void CMDAlarmConditionUIImp::buildTree(QTreeWidgetItem *pTreeRoot)
{
    connect(treeWidget(), &QTreeWidget::itemDoubleClicked, this, &CMDAlarmConditionUIImp::onMouseDbClick);

    QTreeWidgetItem* actionItem = new QTreeWidgetItem(QStringList()<<STRANS(STRING_COND_TEXT), ALARM_COND_TREEITEM_TYPE);
    actionItem->setIcon(0,QIcon(":/images/file.png"));
    actionItem->setCheckState(1, Qt::Checked);
    actionItem->setData(0, Qt::UserRole, (qulonglong)this);
    pTreeRoot->addChild(actionItem);
}

void CMDAlarmConditionUIImp::on_treeWindow_destroyed(QObject* obj)
{
    if(m_treeWindow)
    {
        m_treeWindow->deleteLater();
        m_treeWindow = NULL;
    }
}

QTreeWidget *CMDAlarmConditionUIImp::treeWidget()
{
    return (m_studioInterface ? m_studioInterface->treeWidget() : NULL);
}

QTreeWidgetItem *CMDAlarmConditionUIImp::treeRoot()
{
    return m_rootInStudio;
}

QTreeWidgetItem *CMDAlarmConditionUIImp::selectedTreeItem()
{
    QTreeWidget *treeW = treeWidget();
    return (treeW ? treeW->currentItem() : NULL);
}

/*!
\section 显示对象选择界面
*/
bool CMDAlarmConditionUIImp::select(const QStringList& sourceList, QStringList& destList)
{
    CMDAlarmConditionTreeWindow win;
    win.init(this, m_studioInterface->projectConfiger()->conditionConfiger());
    return win.doModalSelectObjectUI(STRANS(STRING_SELECT_COND), (qint16)MDObjectType::alarmCondition, sourceList, destList);
}

/*!
\section 多语言
*/
void CMDAlarmConditionUIImp::changeLanguage()
{
    int count = (m_rootInStudio ? m_rootInStudio->childCount() : 0);

    for(int i = 0; i < count; i++)
    {
        QTreeWidgetItem* myItem = m_rootInStudio->child(i);
        if(myItem->type() == ALARM_COND_TREEITEM_TYPE)
        {
            QString translated = STRANS(STRING_COND_TEXT);
            myItem->setText(0, translated);
        }
    }

    if(m_treeWindow)
        m_treeWindow->changeLanguage();
}

void CMDAlarmConditionUIImp::collectWords()
{
    QList<CMDAlarmCondition*> condList;
    m_studioInterface->projectConfiger()->conditionConfiger()->configedList(condList);
    foreach (const CMDAlarmCondition* ac, condList)
    {
        CMDMultiLanguage::instance()->addWord(ac->comment(), false);
    }
}
