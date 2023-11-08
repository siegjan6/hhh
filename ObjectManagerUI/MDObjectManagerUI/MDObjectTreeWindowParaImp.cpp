#include "MDObjectTreeWindowParaImp.h"
#include "MDObjectTreeItem.h"
#include "MDObjectTreeModel.h"
#include <QItemSelectionModel>
#include <QTreeView>
#include <QJsonArray>
#include "MDMultiLanguage.h"
#include "ITranslateWidgetTool.h"
#include "MDObjectTreeWindow.h"

CMDObjectTreeWindowParaImp::CMDObjectTreeWindowParaImp()
{
    m_treeWindow = NULL;
    m_objectTreeModel = NULL;
    m_objectConfiger = NULL;
    m_objectUI = NULL;

    m_treeView = NULL;

    m_menu = NULL;
    m_subMenuForAdd = NULL;
    m_actAdd = NULL;
    m_actConfig = NULL;
    m_actDelete = NULL;
    m_actCopy = NULL;
    m_actPaste = NULL;
    m_actRename = NULL;
    m_actBatchAdd = NULL;

    m_translateTool = NULL;

    m_addObjOnSelect = true;
}

CMDObjectTreeWindowParaImp::~CMDObjectTreeWindowParaImp()
{

}

void CMDObjectTreeWindowParaImp::release()
{
    clearCopyList();

    if(m_objectTreeModel)
        delete m_objectTreeModel;

    if(m_translateTool)
        delete m_translateTool;
}

void CMDObjectTreeWindowParaImp::resetModelData()
{
    if(m_objectTreeModel)
        m_objectTreeModel->resetModelData();
}

CMDObjectTreeItem* CMDObjectTreeWindowParaImp::toItem(const QModelIndex& index)
{
    if(!index.isValid())
        return NULL;

    CMDObjectTreeItem *item = static_cast<CMDObjectTreeItem*>(index.internalPointer());
    return item;
}

void CMDObjectTreeWindowParaImp::selectedItems(QList<CMDObjectTreeItem*>& items)
{
    QItemSelectionModel *selections = m_treeView->selectionModel();
    QModelIndexList indexs = selections->selectedIndexes();
    foreach (const QModelIndex& index, indexs)
        items.append(toItem(index));
}

void CMDObjectTreeWindowParaImp::selectedTopItems(QList<CMDObjectTreeItem*>& topItems)
{
    QItemSelectionModel *selections = m_treeView->selectionModel();
    QModelIndexList indexs = selections->selectedIndexes();
    foreach (const QModelIndex& index, indexs)
    {
        CMDObjectTreeItem* item = toItem(index);
        if(m_treeWindow->isTopObject(item->type()) &&
                0 == index.column())
        {
            topItems.append(item);
        }
    }
}

void CMDObjectTreeWindowParaImp::initTranslateTool(CMDObjectTreeWindow* treeWindow)
{
    m_translateTool = CMDMultiLanguage::createWidgetTool();
    m_translateTool->init((QObject*)treeWindow);
}

void CMDObjectTreeWindowParaImp::changeLanguage()
{
    if(m_translateTool)
        m_translateTool->changeLanguage();
}

void CMDObjectTreeWindowParaImp::clearCopyList()
{
    m_copyList.clear();
}

void CMDObjectTreeWindowParaImp::prepareCopy(QList<CMDObjectTreeItem*>& copyList)
{
    m_copyList.clear();

    int count = copyList.count();
    for(int i = 0; i < count; i++)
    {
        m_copyList.append(copyList.at(i)->ptr());
    }
}

/*!
\section 批量增加
*/
void CMDObjectTreeWindowParaImp::showBatchAddDialog(const QJsonArray& ja)
{
    QJsonObject js;
    js.insert("BatchAdd", ja);
    m_batchAdd.setRefJson(js);
}

QJsonObject CMDObjectTreeWindowParaImp::propToJson(const QString& propID, const QString& propComment,
                                                     const QString& refValue, const QString& formatValue, int start, int step, int base)
{
    QJsonObject jsProp;
    jsProp.insert("PropID", propID);
    jsProp.insert("PropComment", propComment);
    jsProp.insert("RefValue", refValue);
    jsProp.insert("FormatValue", formatValue);
    jsProp.insert("Start", start);
    jsProp.insert("Step", step);
    jsProp.insert("Base", base);
    return jsProp;
}

void CMDObjectTreeWindowParaImp::addOneOnBatch(const QJsonObject &json)
{
    QJsonArray ja = json.value("BatchAdd").toArray();

    QString resultMsg;
    if(!m_treeWindow->onBatchAddOneObject(ja, resultMsg))
        m_batchAdd.setErrorInfor(QObject::tr("添加失败！- ") + resultMsg);

    resetModelData();
}
