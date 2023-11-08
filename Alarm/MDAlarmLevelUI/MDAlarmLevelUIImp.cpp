#include "MDAlarmLevelUIImp.h"
#include "../../include/MDAlarmLevelConfiger/MDAlarmLevelConfiger.h"
#include "MDProjectConfiger/MDProjectConfiger.h"
#include "MDAlarmConfiger.h"
#include "MDAlarmLevel.h"
#include "MDAlarmLevelTreeWindow.h"
#include "../../include/MDCommonHelper/MDCommonConstant.h"

#include "MDStringResource.h"

CMDAlarmLevelUIImp::CMDAlarmLevelUIImp(QObject *parent) :
    QObject(parent)
{
    m_studioInterface = NULL;
    m_rootInStudio = NULL;

    m_treeWindow = NULL;
}

CMDAlarmLevelUIImp::~CMDAlarmLevelUIImp()
{

}

/*!
\section 初始化
*/
void CMDAlarmLevelUIImp::load()
{
    buildTree(treeRoot());
}

void CMDAlarmLevelUIImp::unLoad()
{
    if(m_treeWindow)
    {
        m_treeWindow->close();
        delete m_treeWindow;
        m_treeWindow = NULL;
    }
}

/*!
\section 与开发器交互；通过树控件管理对象
*/
void CMDAlarmLevelUIImp::onMouseRClick(QTreeWidgetItem *pItem ,const QPoint &pos)
{

}

void CMDAlarmLevelUIImp::onMouseLClick(QTreeWidgetItem *pItem,int column)
{

}

void CMDAlarmLevelUIImp::onMouseDbClick(QTreeWidgetItem *pItem,int column)
{
    if(!m_studioInterface)
        return;

    QVariant d = pItem->data(0, Qt::UserRole);
     if(((qulonglong)this) == d.toULongLong())
     {
         if (!m_treeWindow)
         {
             m_treeWindow = new CMDAlarmLevelTreeWindow();
             m_treeWindow->init(this, m_studioInterface->projectConfiger()->alarmConfiger());
             m_studioInterface->openMDIWindow(m_treeWindow);
             connect(m_treeWindow, SIGNAL(destroyed(QObject*)),this, SLOT(on_treeWindow_destroyed(QObject*)));
         }
         else m_studioInterface->setActiveForm(m_treeWindow);
     }
}

void CMDAlarmLevelUIImp::buildTree(QTreeWidgetItem *pTreeRoot)
{
    connect(treeWidget(), &QTreeWidget::itemDoubleClicked, this, &CMDAlarmLevelUIImp::onMouseDbClick);

    // 多语言：此处为原始串，翻译由 changeLanguage() 完成
    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList()<<STRING_LEVEL_TEXT, ALARM_LEVEL_TREEITEM_TYPE);
    item->setIcon(0,QIcon(":/images/file.png"));
    item->setCheckState(1, Qt::Checked);
    item->setData(0, Qt::UserRole, (qulonglong)this);
    pTreeRoot->addChild(item);
}

void CMDAlarmLevelUIImp::on_treeWindow_destroyed(QObject* obj)
{
    if(m_treeWindow)
    {
        m_treeWindow->deleteLater();
        m_treeWindow = NULL;
    }
}

QTreeWidget *CMDAlarmLevelUIImp::treeWidget()
{
    return (m_studioInterface ? m_studioInterface->treeWidget() : NULL);
}

QTreeWidgetItem *CMDAlarmLevelUIImp::treeRoot()
{
    return m_rootInStudio;
}

QTreeWidgetItem *CMDAlarmLevelUIImp::selectedTreeItem()
{
    QTreeWidget *treeW = treeWidget();
    return (treeW ? treeW->currentItem() : NULL);
}

/*!
\section 显示对象选择界面
*/
bool CMDAlarmLevelUIImp::select(const QString& source, QString& dest)
{
    QStringList sourceList;
    sourceList.append(source);
    QStringList destList;

    CMDAlarmLevelTreeWindow win;
    win.init(this, m_studioInterface->projectConfiger()->alarmConfiger());
    bool result = win.doModalSelectObjectUI(STRANS(STRING_SELECT_LEVEL), (qint16)MDObjectType::alarmLevel, sourceList, destList);
    if(!destList.isEmpty())
        dest = destList.first();
    return result;
}

/*!
\section 多语言
*/
void CMDAlarmLevelUIImp::changeLanguage()
{
    int count = (m_rootInStudio ? m_rootInStudio->childCount() : 0);

    for(int i = 0; i < count; i++)
    {
        QTreeWidgetItem* myItem = m_rootInStudio->child(i);
        if(myItem->type() == ALARM_LEVEL_TREEITEM_TYPE)
        {
            QString translated = STRANS(STRING_LEVEL_TEXT);
            myItem->setText(0, translated);
        }
    }

    if(m_treeWindow)
        m_treeWindow->changeLanguage();
}

void CMDAlarmLevelUIImp::collectWords()
{
    QList<CMDAlarmLevel*> levelList;
    m_studioInterface->projectConfiger()->alarmLevelConfiger()->configedList(levelList);
    foreach (const CMDAlarmLevel* al, levelList)
    {
        CMDMultiLanguage::instance()->addWord(al->comment(), false);
    }
}
