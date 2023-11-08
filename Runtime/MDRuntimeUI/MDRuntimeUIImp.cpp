#include "MDRuntimeUIImp.h"
#include "MDRuntimeParaMainDialog.h"
#include "MDStringResource.h"

CMDRuntimeUIImp::CMDRuntimeUIImp(QObject *parent) :
    QObject(parent)
{
    m_studioInterface = NULL;
    m_rootInStudio = NULL;
}

CMDRuntimeUIImp::~CMDRuntimeUIImp()
{

}

/*!
\section 初始化
*/

void CMDRuntimeUIImp::load()
{
    buildTree(treeRoot());
}

void CMDRuntimeUIImp::unLoad()
{

}

/*!
\section 与开发器交互；通过树控件管理对象
*/
void CMDRuntimeUIImp::onMouseRClick(QTreeWidgetItem *pItem ,const QPoint &pos)
{

}

void CMDRuntimeUIImp::onMouseLClick(QTreeWidgetItem *pItem,int column)
{

}

void CMDRuntimeUIImp::onMouseDbClick(QTreeWidgetItem *pItem,int column)
{
    if(!m_studioInterface)
        return;

    if(!m_studioInterface->projectConfiger())
        return;

    QVariant d = pItem->data(0, Qt::UserRole);
     if(((qulonglong)this) == d.toULongLong())
     {
         CMDRuntimeParaMainDialog dlg;
         dlg.setUIImp(this);
         dlg.initial(this, m_studioInterface->projectConfiger(), (qint16)0);
         dlg.exec();
     }
}

void CMDRuntimeUIImp::buildTree(QTreeWidgetItem *pTreeRoot)
{
    connect(treeWidget(), &QTreeWidget::itemDoubleClicked, this, &CMDRuntimeUIImp::onMouseDbClick);

    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList()<<STRANS(STRING_CONFIG), RUNTIMEUI_TREEITEM_TYPE);
    item->setIcon(0,QIcon(":/images/file.png"));
    item->setCheckState(1, Qt::Checked);
    item->setData(0, Qt::UserRole, (qulonglong)this);
    pTreeRoot->addChild(item);
}

QTreeWidget *CMDRuntimeUIImp::treeWidget()
{
    return (m_studioInterface ? m_studioInterface->treeWidget() : NULL);
}

QTreeWidgetItem *CMDRuntimeUIImp::treeRoot()
{
    return m_rootInStudio;
}

QTreeWidgetItem *CMDRuntimeUIImp::selectedTreeItem()
{
    QTreeWidget *treeW = treeWidget();
    return (treeW ? treeW->currentItem() : NULL);
}

/*!
\section 多语言
*/
void CMDRuntimeUIImp::changeLanguage()
{
    int count = (m_rootInStudio ? m_rootInStudio->childCount() : 0);

    for(int i = 0; i < count; i++)
    {
        QTreeWidgetItem* myItem = m_rootInStudio->child(i);
        if(myItem->type() == RUNTIMEUI_TREEITEM_TYPE)
        {
            QString translated = STRANS(STRING_CONFIG);
            myItem->setText(0, translated);
        }
    }
}
