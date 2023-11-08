#include "CFeatureTagSettingForm.h"
#include "ui_CFeatureTagSettingForm.h"
#include "CTagItemSettingDialog.h"
#include "CFeatureTagPropertySettingDialog.h"

#include "CHistoryTagManager.h"
#include "CRealTimeTagManager.h"
#include "CRealFTag.h"
#include "CRealTagSettingDialog.h"

#include "CFeatureTag.h"
#include "CFeatureTagItem.h"
#include "CXAxisManager.h"
#include "CXAxis.h"

#include "CFeatureTagValueItem.h"
#include "CFeatureTagCommentItem.h"
#include "CFeatureTagTimeItem.h"

#include "CDataProperty.h"

#include "IMDRTDBManagerUI.h"
#include "IMDDataArchives.h"
#include "IMDDataSourceManager.h"
#include "IMDDataSource.h"
#include "IMDDataTable.h"
#include "CVariantDialog.h"

#include "CFeatureTagConfigDialog.h"
#include "CFormToolFunc.h"
#include "../include/MDDBQueryUI/MDDBQueryUI.h"
#include "CViewFieldDialog.h"

#include <QDebug>

using namespace FormToolFunc;
CFeatureTagSettingForm::CFeatureTagSettingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFeatureTagSettingForm),
    m_isModified(false),
    m_historyTag(0),
    m_realTimeTag(0),
    m_tag(0),
    m_xs(0),
    m_variantForm(0),
    m_dataArchives(0),
    m_dataManager(0),
    m_isRealTime(false),
    m_settingForm(new CFeatureTagPropertySettingDialog),
    m_configDialog(new CFeatureTagConfigDialog(this))

{
    ui->setupUi(this);
}

CFeatureTagSettingForm::~CFeatureTagSettingForm()
{        
    delete ui;
}

void CFeatureTagSettingForm::setData(CHistoryTagManager *tag, CXAxisManager *xs)
{
    if(tag && xs)
    {
        m_historyTag =  tag;
        m_xs = xs;
        m_itemPosMap = tag->itemPosMap();
        initialize();
    }
}

void CFeatureTagSettingForm::setData(CRealTimeTagManager *tag, CXAxisManager *xs)
{
    if(tag && xs)
    {
        m_realTimeTag = tag;
        m_itemPosMap = tag->itemPosMap();
        m_xs = xs;
        initialize();
    }
}

void CFeatureTagSettingForm::setXList(const QList<CXAxis *> &list)
{
    m_xs->setList(list);
}

void CFeatureTagSettingForm::setVariantForm(IMDRTDBManagerUI *variantForm)
{
    m_variantForm = variantForm;
}

void CFeatureTagSettingForm::setDataArchives(IMDDataArchives *dataArchives)
{
    m_dataArchives = dataArchives;
}

void CFeatureTagSettingForm::setIsRealTime(bool isOrNot)
{
    m_isRealTime = isOrNot;
}

void CFeatureTagSettingForm::setDataManager(CDataProperty *data)
{
    m_dataManager = data;
}

/*!
\section
*/

void CFeatureTagSettingForm::applyChanges()
{
    if(m_isRealTime)
    {
        applyRealTimeTagChanges();
    }
    else
    {
        applyHistoryTagChanges();
    }
    initializeUI();
}

void CFeatureTagSettingForm::applyHistoryTagChanges()
{
    if(m_historyTag)
    {
        m_historyTag->setTagList(m_historyFeatureTagList);
        m_historyTag->setItemPosMap(m_itemPosMap);

        for(int i = 0 ; i < m_historyFeatureTagList.count(); i++)
        {
            CFeatureTag *featureTag = new CFeatureTag;
            *featureTag = *m_historyFeatureTagList.at(i);
            m_prehHistoryFeatureTagList.append(featureTag);
        }

        QStringList stringList;
        for(int i = 0; i<m_historyFeatureTagList.count(); i++)
        {
            CFeatureTag *tag = m_historyFeatureTagList.at(i);
            for(int j = 0; j < tag->itemList().count();j++)
            {
                CFeatureTagItem *item = tag->itemList().at(j);
                if(item->type() == CFeatureTagItem::ItemType::Value)
                {
                    stringList.append(item->itemName());
                }
            }
        }
    }
}

void CFeatureTagSettingForm::applyRealTimeTagChanges()
{
    if(m_realTimeTag)
    {
        m_realTimeTag->setItems(m_realTagList);
        m_realTimeTag->setItemPosMap(m_itemPosMap);

        for(int i = 0 ; i < m_realTagList.count(); i++)
        {
            CRealFTag *realTag = new CRealFTag;
            *realTag = *m_realTagList.at(i);
            m_preRealTagList.append(realTag);
        }
    }
}

/*!
\section
*/
void CFeatureTagSettingForm::resetModified()
{
    m_isModified = false;
}
void CFeatureTagSettingForm::slotHandleModified()
{
    if(!m_isModified)
    {
        m_isModified = true;
        emit itemModified();
    }
}
/*!
\section
*/

void CFeatureTagSettingForm::initialize()
{
    initializeData();
    initializeUI();
}

void CFeatureTagSettingForm::initializeUI()
{
    breakConnections();
    ui->addButton->setVisible(false);

    ui->tagTableWidget->clear();
    ui->tagTableWidget->setColumnCount(3);
    QStringList labels;
    labels<<"名称"<<"显示"<<"描述";
    ui->tagTableWidget->setHorizontalHeaderLabels(labels);

    ui->tagTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    parseItemsInformation();
    if(m_isRealTime)
    {
        if(!m_realTagList.isEmpty())
            ui->tagTableWidget->selectRow(0);
    }
    else
    {
        if(!m_historyFeatureTagList.isEmpty())
            ui->tagTableWidget->selectRow(0);
    }
    createConnections();
    createMenu();
}

void CFeatureTagSettingForm::initializeData()
{
    if(!m_isRealTime)
    {
        if(m_historyTag)
        {
            m_prehHistoryFeatureTagList = m_historyTag->tagList();
            for(int i = 0; i< m_prehHistoryFeatureTagList.count();i++)
            {
                CFeatureTag *featureTag = new CFeatureTag;
                *featureTag = *m_prehHistoryFeatureTagList.at(i);
                m_historyFeatureTagList.append(featureTag);
            }
        }
    }
    else
    {
        if(m_realTimeTag)
        {
            m_preRealTagList = m_realTimeTag->items();
            for(int i = 0; i < m_preRealTagList.count(); i++)
            {
                CRealFTag *realFTag = new CRealFTag;
                *realFTag = *m_preRealTagList.at(i);
                m_realTagList.append(realFTag);
            }
        }
    } 
}          

void CFeatureTagSettingForm::createMenu()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slotCustomMenu(QPoint)));// 响应右键菜单

    m_menu = new QMenu(ui->tagTableWidget);
    QAction* addAction = new QAction(tr("增加"), ui->tagTableWidget);
    m_menu->addAction(addAction);
    QAction* deleteAction = new QAction(tr("删除"), ui->tagTableWidget);
    m_menu->addAction(deleteAction);
    QAction *upAction = new QAction(tr("上移"),ui->tagTableWidget);
    m_menu->addAction(upAction);
    QAction *downAction = new QAction(tr("下移"),ui->tagTableWidget);
    m_menu->addAction(downAction);
    QAction *propertyAction = new QAction(tr("属性"),ui->tagTableWidget);
    m_menu->addAction(propertyAction);

    connect (addAction,&QAction::triggered,this,&CFeatureTagSettingForm::slotAddDozenItem);
    connect (deleteAction,&QAction::triggered,this,&CFeatureTagSettingForm::slotDeleteItem);
    connect (upAction,&QAction::triggered,this,&CFeatureTagSettingForm::slotMoveUpRow);
    connect (downAction,&QAction::triggered,this,&CFeatureTagSettingForm::slotMoveDownRow);
    connect (propertyAction,&QAction::triggered,this,&CFeatureTagSettingForm::slotSetProperty);
}

/*!
\section
*/
void CFeatureTagSettingForm::createConnections()
{
    connect(ui->tagTableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this,SLOT(slotSetItemProperty(QTableWidgetItem*)));
    connect(ui->tagTableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),
            this,SLOT(slotSetItemVisible(QTableWidgetItem*)));

    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(slotAddItem()));
    connect(ui->addDozenButton,SIGNAL(clicked()),this,SLOT(slotAddDozenItem()));
    connect(ui->deleteButton,SIGNAL(clicked()),this,SLOT(slotDeleteItem()));
    connect(ui->moveUpButton,SIGNAL(clicked()),this,SLOT(slotMoveUpRow()));
    connect(ui->moveDownButton,SIGNAL(clicked()),this,SLOT(slotMoveDownRow()));
    connect(ui->propertyButton,SIGNAL(clicked()),this,SLOT(slotSetProperty()));

}

void CFeatureTagSettingForm::breakConnections()
{
    disconnect(ui->tagTableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(slotSetItemProperty(QTableWidgetItem*)));
    disconnect(ui->tagTableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(slotSetItemVisible(QTableWidgetItem*)));

    disconnect(ui->addButton,SIGNAL(clicked()),this,SLOT(slotAddItem()));
    disconnect(ui->addDozenButton,SIGNAL(clicked()),this,SLOT(slotAddDozenItem()));
    disconnect(ui->deleteButton,SIGNAL(clicked()),this,SLOT(slotDeleteItem()));
    disconnect(ui->moveUpButton,SIGNAL(clicked()),this,SLOT(slotMoveUpRow()));
    disconnect(ui->moveDownButton,SIGNAL(clicked()),this,SLOT(slotMoveDownRow()));
    disconnect(ui->propertyButton,SIGNAL(clicked()),this,SLOT(slotSetProperty()));

}

void CFeatureTagSettingForm::slotAddItem()
{
    if(m_isRealTime)
    {
        addRealTimeItem();
    }
    else
    {
        addHistoryItem();
    }
}

void CFeatureTagSettingForm::slotAddDozenItem()
{
    QString createTagName(const QStringList &);
    if(m_isRealTime)
    {
        if(m_variantForm)
        {
            QStringList variantList;
            m_variantForm->selectVariables(variantList);
            if(!variantList.isEmpty())
            {
                for(int j = 0; j < variantList.count(); j++)
                {
                    bool isRepeat = isTagRepeat(variantList.at(j));
                    if(isRepeat)
                    {
                        continue;
                    }
                    CRealFTag *tagItem = new CRealFTag;
                    QStringList nameList;
                    for(int i = 0; i < m_realTagList.count();i++)
                        nameList.append(m_realTagList.at(i)->name());
                    tagItem->setName(createTagName(nameList));
                    CFeatureTagItem *valueItem = tagItem->itemList().at(2);
                    valueItem->setItemName(variantList.at(j));

                    if(!tagItem->name().isEmpty())
                    {
                        tagItem->setXAxis(m_xs->list().at(0));
                        tagItem->setXName(m_xs->list().at(0)->name());
                        m_realTagList.append(tagItem);
                        m_itemPosMap.insert(tagItem->name(),CTagPos());
                        parseItemsInformation();
                        slotHandleModified();
                    }
                    else
                    {
                        delete tagItem;
                        tagItem = NULL;
                    }
                }
            }
        }
    }
    else
    {
        QString variantName;
        bool ok = false;
        QStringList viewList;
        QStringList list;
        QMap<QString,QList<DBQ::ViewField *>>map;
        DBQ::View_GetAllViewNameFromDir(m_historyTag->projectPath()+"/DBQuery",list);
        for(int i = 0; i<list.count();i++)
        {
            QString str;
            QString view = list.at(i);
            QList<DBQ::ViewField *>viewField;
            viewField = DBQ::View_GetAllViewFieldsInViewFromDir(m_historyTag->projectPath()+"/DBQuery"
                                                      ,view,str);
            map.insert(view,viewField);
        }
        viewList = CViewFieldDialog::getField(&ok,list,map,this);
//        variantName = viewList.count()>0 ? viewList.first() : variantName;
//        if(!variantName.isEmpty())
//        {
//            int index = variantName.indexOf(".");
//            QString viewName = variantName.left(index);
//            tag->setViewName(viewName);
//        }

        for(int j = 0; j<viewList.count();j++)
        {
            CFeatureTag *tag = new CFeatureTag;
            tag->setProjectPath(m_historyTag->projectPath());
            if(!viewList.isEmpty())
            {
                variantName = viewList.at(j);
                int index = variantName.indexOf(".");
                QString viewName = variantName.left(index);
                tag->setViewName(viewName);

                QList<DBQ::ViewField *>viewField = map.value(viewName);
                for(int j = 0; j<viewField.count();j++)
                {
                    DBQ::ViewField *view = viewField.at(j);
                    if(view->type == DBQ::VIEW_FIELD_TYPE::VIEW_FIELD_TYPE_DATETIME)
                    {
                        tag->setTimeVariant(viewField.at(j)->fieldName);
                        break;
                    }
                }

                bool isRepeat = isTagRepeat(variantName);
                if(isRepeat)
                {
//                    FormToolFunc::showMessage(tr("特征标签已存在，请重新设置！"),this);
                    delete tag;
                    tag = NULL;
                    continue;
                }
                QStringList nameList;
                for(int i = 0; i < m_historyFeatureTagList.count();i++)
                    nameList.append(m_historyFeatureTagList.at(i)->name());
                tag->setName(createTagName(nameList));
            }
            CFeatureTagItem *valueItem = tag->itemList().at(2);
            valueItem->setItemName(variantName);

            if(!tag->name().isEmpty())
            {
                tag->setXAxis(m_xs->list().at(0));
                tag->setXName(m_xs->list().at(0)->name());
                m_historyFeatureTagList.append(tag);
                m_itemPosMap.insert(tag->name(),CTagPos());
                parseItemsInformation();
                slotHandleModified();
            }
            else
            {
                delete tag;
                tag = NULL;
            }
        }
    }
}

void CFeatureTagSettingForm::slotDeleteItem()
{
    if(!ui->tagTableWidget->rowCount())
    {
        showMessage(tr("没有特征标签!"));
    }
    else
    {
        QList<QTableWidgetItem*> selectedTableItems = ui->tagTableWidget->selectedItems();
        if(!selectedTableItems.isEmpty())
        {
            QStringList itemNames;
            QString names;
            for(int i = 0; i < selectedTableItems.count();i++)
            {
                if(selectedTableItems.at(i)->data(Qt::UserRole).toString() == "tagName")
                {
                    itemNames.append(selectedTableItems.at(i)->text());
                    names += selectedTableItems.at(i)->text() + QString(",");
                }
            }
            names = names.left(names.length()-1); // remove the last spilter ,

            int ret = showMessage(tr("确定删除 ")+names + "?",
                                  QMessageBox::Ok|QMessageBox::Cancel);

            if(ret == QMessageBox::Ok)
            {
                for(int j=0; j< itemNames.count();j++)
                {
                    int itemIndex =findItemByName(itemNames.at(j));
                    if(m_isRealTime)
                    {
                        CRealFTag *tag = m_realTagList.at(itemIndex);
                        QString tagName = tag->name();
                        m_itemPosMap.remove(tagName);
                        delete m_realTagList.takeAt(itemIndex);
                    }
                    else
                    {
                        CFeatureTag *tag = m_historyFeatureTagList.at(itemIndex);
                        QString tagName = tag->name();
                        m_itemPosMap.remove(tagName);
                        delete m_historyFeatureTagList.takeAt(itemIndex);
                    }
                }
                parseItemsInformation();
                slotHandleModified();
            }
            else
            {
                // To do
            }
        }
        else
        {
            showMessage(tr("请选择一个特征标签！"));
        }
    }
}

void CFeatureTagSettingForm::slotSetProperty()
{
    if(m_isRealTime && m_realTimeTag)
    {
        setRealTimeTagProperty();
    }
    else if(!m_isRealTime && m_historyTag)
    {
        setHistoryTagPorperty();
    }
}

void CFeatureTagSettingForm::slotSetItemProperty(QTableWidgetItem *item)
{
    if(item)
    {
        int rowCount = item->row();
        if(m_isRealTime)
        {
            if(!m_realTagList.isEmpty())
            {
                CRealFTag *item = m_realTagList[rowCount];
                setItemProperty(item);
            }
        }
        else
        {
            if(!m_historyFeatureTagList.isEmpty())
            {
                CFeatureTag *itemInitial = m_historyFeatureTagList[rowCount];
                setItemProperty(itemInitial);
            }
        }
    }
}

void CFeatureTagSettingForm::slotSetItemVisible(QTableWidgetItem *item)
{
    if(item && Qt::ItemIsUserCheckable == item->flags())
    {
        if(Qt::Checked == item->checkState())
        {
            item->setCheckState(Qt::Unchecked);
            if(m_isRealTime)
                m_realTagList.at(item->row())->setVisible(false);
            else
                m_historyFeatureTagList.at(item->row())->setVisible(false);
            slotHandleModified();
        }
        else if(Qt::Unchecked == item->checkState())
        {
            item->setCheckState(Qt::Checked);
            if(m_isRealTime)
                m_realTagList.at(item->row())->setVisible(true);
            else
                m_historyFeatureTagList.at(item->row())->setVisible(true);
            slotHandleModified();
        }
    }
}

void CFeatureTagSettingForm::slotMoveUpRow()
{
    int currentIndex = ui->tagTableWidget->currentRow();
    if(currentIndex != -1)
    {
        int preIndex = currentIndex -1;
        if(preIndex != -1) //! not the first one
            swapTableItem(preIndex,currentIndex);
        slotHandleModified();
    }
    else
    {
        showMessage(tr("请选中一个标签"));
    }
}

void CFeatureTagSettingForm::slotMoveDownRow()
{
    int currentIndex = ui->tagTableWidget->currentRow();
    int maxIndex = ui->tagTableWidget->rowCount()-1;
    if(currentIndex != -1) {
        int nextIndex = currentIndex +1;
        if(nextIndex <= maxIndex) //!
            swapTableItem(nextIndex,currentIndex);
        slotHandleModified();
    } else {
        showMessage(tr("请选中一个标签"));
    }
}

void CFeatureTagSettingForm::slotCustomMenu(const QPoint &p)
{
    QPoint point =  mapToGlobal(p);
    m_menu->exec(point);
}
/*!
\section
*/

void CFeatureTagSettingForm::setItemProperty(CFeatureTag *itemInitial)
{
    m_configDialog->setTitle(itemInitial->name()+"属性");
    m_configDialog->setData(itemInitial,m_xs->list());
    m_configDialog->setFields(m_fields);

    CFeatureTag *tag =  itemInitial;
    CTagPos pos = m_historyTag->itemPosMap().value(tag->name());
    m_configDialog->setData(tag,pos);

    int ret = m_configDialog->exec();
    if(QDialog::Accepted == ret)
    {
        m_configDialog->applyChanges();
        m_itemPosMap[tag->name()] = m_configDialog->tagPos();
        parseItemsInformation();
        slotHandleModified();
    }
}

void CFeatureTagSettingForm::setItemProperty(CRealFTag *itemInitial)
{
    m_configDialog->setTitle(itemInitial->name()+"属性");
    m_configDialog->setData(itemInitial,m_xs->list());
    m_configDialog->setVariantForm(m_variantForm);

    CRealFTag *tag =  itemInitial;
    CTagPos pos = m_realTimeTag->itemPosMap().value(tag->name());
    m_configDialog->setData(tag,pos);

    int ret = m_configDialog->exec();
    if(QDialog::Accepted == ret)
    {
        m_configDialog->applyChanges();
        m_itemPosMap[tag->name()] = m_configDialog->tagPos();
        parseItemsInformation();
        slotHandleModified();
    }

}

void CFeatureTagSettingForm::parseItemsInformation()
{
    if(m_isRealTime)
    {
        int count = m_realTagList.count();
        ui->tagTableWidget->setRowCount(count);
        for(int i = 0; i < count;i++)
        {
            CRealFTag *item = m_realTagList.at(i);
            insertTableItem(i,0,createNameItem(item->name()));
            insertTableItem(i,1,createVisibleItem(item->visible()));
            insertTableItem(i,2,createCommentItem(item->comment()));
        }
    }
    else
    {
        int count = m_historyFeatureTagList.count();
        ui->tagTableWidget->setRowCount(count);
        for(int i = 0; i < count;i++)
        {
            CFeatureTag *item = m_historyFeatureTagList.at(i);
            insertTableItem(i,0,createNameItem(item->name()));
            insertTableItem(i,1,createVisibleItem(item->visible()));
            insertTableItem(i,2,createCommentItem(item->comment()));
        }
    }
}

int CFeatureTagSettingForm::showMessage(const QString &comment, QMessageBox::StandardButtons buttons)
{
    return QMessageBox::warning(this,tr("警告"),comment,buttons);
}

void CFeatureTagSettingForm::swapTableItem(int preIndex, int currentIndex)
{
    QString currentName = ui->tagTableWidget->item(currentIndex,0)->text();
    Qt::CheckState currentState = ui->tagTableWidget->item(currentIndex,1)->checkState();
    QString currentComment = ui->tagTableWidget->item(currentIndex,2)->text();

    QString preName = ui->tagTableWidget->item(preIndex,0)->text();
    Qt::CheckState preState = ui->tagTableWidget->item(preIndex,1)->checkState();
    QString preComment = ui->tagTableWidget->item(preIndex,2)->text();

    ui->tagTableWidget->item(currentIndex,0)->setText(preName);
    ui->tagTableWidget->item(currentIndex,1)->setCheckState(preState);
    ui->tagTableWidget->item(currentIndex,2)->setText(preComment);

    ui->tagTableWidget->item(preIndex,0)->setText(currentName);
    ui->tagTableWidget->item(preIndex,1)->setCheckState(currentState);
    ui->tagTableWidget->item(preIndex,2)->setText(currentComment);

    ui->tagTableWidget->selectRow(preIndex);

    if(m_isRealTime)
    {
        m_realTagList.swap(preIndex,currentIndex);
    }
    else
    {
        m_historyFeatureTagList.swap(preIndex,currentIndex);
    }
}

QTableWidgetItem *CFeatureTagSettingForm::createNameItem(const QString &name)
{
    QTableWidgetItem *nameItem = new QTableWidgetItem;
    nameItem->setTextAlignment(Qt::AlignCenter);
    nameItem->setData(Qt::UserRole,"tagName");
    nameItem->setText(name);
    return nameItem;
}

QTableWidgetItem *CFeatureTagSettingForm::createVisibleItem(bool visible)
{
    QTableWidgetItem *visibleItem = new QTableWidgetItem;
    visibleItem->setTextAlignment(Qt::AlignCenter);
    visibleItem->setFlags(Qt::ItemIsUserCheckable);
    if(visible)
        visibleItem->setCheckState(Qt::Checked);
    else
        visibleItem->setCheckState(Qt::Unchecked);

    return visibleItem;
}

QTableWidgetItem *CFeatureTagSettingForm::createCommentItem(const QString &comment)
{
    QTableWidgetItem *commentItem = new QTableWidgetItem;
    commentItem->setTextAlignment(Qt::AlignCenter);
    commentItem->setText(comment);

    return commentItem;
}

QTableWidgetItem *CFeatureTagSettingForm::createFormatItem(const QString &format)
{
    QTableWidgetItem *formatItem = new QTableWidgetItem;
    formatItem->setText(format);
    return formatItem;
}

void CFeatureTagSettingForm::insertTableItem(int row, int column, QTableWidgetItem *item)
{
    ui->tagTableWidget->setItem(row,column,item);
}

void CFeatureTagSettingForm::deleteHistoryTagItem(int index)
{
    CFeatureTag *tag = m_historyFeatureTagList.at(index);
    QString tagName = tag->name();
    int ret = showMessage(tr("是否删除标签 ")+tagName +"?",
                          QMessageBox::Ok|QMessageBox::Cancel);
    if(QMessageBox::Ok == ret)
    {
        m_itemPosMap.remove(tagName);
        delete m_historyFeatureTagList.takeAt(index);
        parseItemsInformation();
    }
}

void CFeatureTagSettingForm::deleteRealTimeTagItem(int index)
{
    CRealFTag *tag = m_realTagList.at(index);
    QString tagName = tag->name();
    int ret = showMessage(tr("是否删除标签 ")+tagName +"?",
                          QMessageBox::Ok|QMessageBox::Cancel);
    if(QMessageBox::Ok == ret)
    {
        m_itemPosMap.remove(tagName);
        delete m_realTagList.takeAt(index);
        parseItemsInformation();
    }
}

void CFeatureTagSettingForm::setRealTimeTagProperty()
{
    int currentIndex = ui->tagTableWidget->currentRow();
    if(currentIndex != -1)
    {
        CRealFTag *tag =  m_realTagList.at(currentIndex);
        CTagPos pos = m_realTimeTag->itemPosMap().value(tag->name());

        m_configDialog->setData(tag,pos);
        m_configDialog->setData(tag,m_xs->list());
        m_configDialog->setTitle(tag->name()+"属性");

        m_configDialog->show();
        int ret = m_configDialog->exec();
        if(QDialog::Accepted == ret)
        {
            m_configDialog->applyChanges();
            m_itemPosMap[tag->name()] = m_configDialog->tagPos();
            slotHandleModified();
        }
        else
        {
            m_configDialog->tagSettingInitialize();
        }
    }
    else
    {
        showMessage(tr("请选择一个标签！"));
    }
}

void CFeatureTagSettingForm::setHistoryTagPorperty()
{
    int currentIndex = ui->tagTableWidget->currentRow();
    if(currentIndex != -1)
    {
        CFeatureTag *tag =  m_historyFeatureTagList.at(currentIndex);
        CTagPos pos = m_historyTag->itemPosMap().value(tag->name());
        m_configDialog->setData(tag,pos);

        m_configDialog->setData(tag,m_xs->list());
        m_configDialog->setFields(m_fields);
        m_configDialog->setTitle(tag->name()+"属性");
        m_configDialog->show();

        int ret = m_configDialog->exec();
        if(QDialog::Accepted == ret)
        {
            m_configDialog->applyChanges();
            m_itemPosMap[tag->name()] = m_configDialog->tagPos();
            slotHandleModified();
            parseItemsInformation();
        }
        else
        {
            m_configDialog->tagSettingInitialize();
        }
    }
    else
    {
        showMessage(tr("请选择一个标签！"));
    }
}

void CFeatureTagSettingForm::addHistoryItem()
{
    QString createTagName(const QStringList &);
    CFeatureTag *tag = new CFeatureTag;
    tag->setProjectPath(m_historyTag->projectPath());
    QStringList viewList;
    if(!m_fields.isEmpty())
    {
        QString variantName;
        bool ok = false;
//        QStringList name = CVariantDialog::getField(&ok,variantName,m_fields,this);
//        if(ok)
//        {
//            variantName = name.count() > 0 ? name.first() : variantName;
//        }
        QStringList list;
        QMap<QString,QList<DBQ::ViewField *>>map;
        DBQ::View_GetAllViewNameFromDir(m_historyTag->projectPath()+"/DBQuery",list);
        for(int i = 0; i<list.count();i++)
        {
            QString str;
            QString view = list.at(i);
            QList<DBQ::ViewField *>viewField;
            viewField = DBQ::View_GetAllViewFieldsInViewFromDir(m_historyTag->projectPath()+"/DBQuery"
                                                      ,view,str);
            map.insert(view,viewField);
        }
        viewList = CViewFieldDialog::getField(&ok,list,map,this);
        variantName = viewList.count()>0 ? viewList.first() : variantName;
        if(!variantName.isEmpty())
        {
            int index = variantName.indexOf(".");
            QString viewName = variantName.left(index);
            tag->setViewName(viewName);
        }

//        bool isRepeat = isTagRepeat(variantName);
//        if(isRepeat)
//        {
//            FormToolFunc::showMessage(tr("特征标签已存在，请重新设置！"),this);
//            delete tag;
//            tag = NULL;
//            return;
//        }
        if(!variantName.isEmpty())
        {
            QStringList nameList;
            for(int i = 0; i < m_historyFeatureTagList.count();i++)
                nameList.append(m_historyFeatureTagList.at(i)->name());
            tag->setName(createTagName(nameList));
        }
        CFeatureTagItem *valueItem = tag->itemList().at(2);
        valueItem->setItemName(variantName);
    }
    if(!tag->name().isEmpty())
    {
        tag->setXAxis(m_xs->list().at(0));
        tag->setXName(m_xs->list().at(0)->name());
        m_historyFeatureTagList.append(tag);
        m_itemPosMap.insert(tag->name(),CTagPos());
        parseItemsInformation();
        slotHandleModified();
    }
    else
    {
        delete tag;
        tag = NULL;
    }
}

void CFeatureTagSettingForm::addRealTimeItem()
{
    CRealFTag *tagItem = new CRealFTag;

    QString createTagName(const QStringList &);
    if(m_variantForm)
    {
        QStringList variantList;
        m_variantForm->selectVariables(variantList);
        if(!variantList.isEmpty())
        {
//            bool isRepeat = isTagRepeat(variantList.at(0));
//            if(isRepeat)
//            {
//                FormToolFunc::showMessage(tr("特征标签已存在，请重新设置！"),this);
//                delete tagItem;
//                tagItem = NULL;
//                return;
//            }
            QStringList nameList;
            for(int i = 0; i < m_realTagList.count();i++)
                nameList.append(m_realTagList.at(i)->name());
            tagItem->setName(createTagName(nameList));
            CFeatureTagItem *valueItem = tagItem->itemList().at(2);
            valueItem->setItemName(variantList.first());
        }
    }

    if(!tagItem->name().isEmpty())
    {
        tagItem->setXAxis(m_xs->list().at(0));
        tagItem->setXName(m_xs->list().at(0)->name());
        m_realTagList.append(tagItem);
        m_itemPosMap.insert(tagItem->name(),CTagPos());
        parseItemsInformation();
        slotHandleModified();
    }
    else
    {
        delete tagItem;
        tagItem = NULL;
    }
}

bool CFeatureTagSettingForm::isTagRepeat(const QString &str)
{
    if(!m_isRealTime)
    {
        for(int i = 0; i<m_historyFeatureTagList.count(); i++)
        {
            CFeatureTag *historyTag = m_historyFeatureTagList.at(i);
            for(int j = 0; j <historyTag->itemList().count(); j++)
            {
                if(historyTag->itemList().at(j)->type() == CFeatureTagItem::ItemType::Value)
                {
                    if(historyTag->itemList().at(j)->itemName() == str)
                        return true;
                }
            }
        }
        return false;
    }
    else
    {
        for(int i=0; i< m_realTagList.count(); i++)
        {
            CRealFTag *realTag = m_realTagList.at(i);
            for(int j=0; j<realTag->itemList().count(); j++)
            {
                if(realTag->itemList().at(j)->type() == CFeatureTagItem::ItemType::Value)
                {
                    if(realTag->itemList().at(j)->itemName() == str)
                        return true;
                }
            }
        }
        return false;
    }
    return false;
}

int CFeatureTagSettingForm::findItemByName(const QString &name)
{
    if(m_isRealTime)
    {
        for(int i = 0; i< m_realTagList.count();i++)
        {
            if(m_realTagList.at(i)->name() == name)
                return i;
        }
    }
    else
    {
        for(int i = 0; i< m_historyFeatureTagList.count();i++)
        {
            if(m_historyFeatureTagList.at(i)->name() == name)
                return i;
        }
    }

    return -1;
}

void CFeatureTagSettingForm::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        slotDeleteItem();
    }
    else if(event->key() == Qt::Key_Enter)
    {
        slotSetProperty();
    }
}

QString createTagName(const QStringList &nameList)
{
    int count = nameList.count();
    for(int i= 0; i < count;i++)
    {
        QString name = QString("Tag%1").arg(QString::number(i+1));
        if(!nameList.contains(name))
            return name;
    }
    return QString("Tag%1").arg(QString::number(count+1));
}


