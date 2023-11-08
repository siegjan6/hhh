#include <QPainter>
#include <QMessageBox>
#include <QMainWindow>

#include "DesignDialog.h"
#include "ui_DesignDialog.h"
#include "DrawItem.h"
#include "DrawGauge.h"
#include "ModuleSelectDialog.h"
#include "LayerDialog.h"
#include "CapDialog.h"
#include "NeedleDialog.h"
#include "RangeDialog.h"
#include "ScaleTextDialog.h"
#include "ScaleDialog.h"
#include "ScaleBackDialog.h"
#include "BackDialog.h"
#include "HalfBackDialog.h"
#include "LabelDialog.h"
#include "LedDialog.h"
#include "TemperatureDialog.h"
#include "RulerMarkerDialog.h"
#include "RulerNeedleDialog.h"
#include "RulerRangeDialog.h"
#include "RulerScaleBackDialog.h"
#include "RulerScaleDialog.h"
#include "RulerScaleTextDialog.h"
#include "MDMultiLanguage.h"

#include <QDebug>

/*!
\section 对控制层列表进行拷贝
*/
void clone(QList<CDataLayer*>& source, QList<CDataLayer*>& target)
{
    if (!target.isEmpty())
    {
        for (qint32 i = 0; i < target.count(); i++)
            delete target.at(i);
        target.clear();
    }

    CDataLayer* pLayer = NULL;
    CDataLayer* newLayer = NULL;
    for (qint32 i = 0; i < source.count(); i++)
    {
        pLayer = source.at(i);
        newLayer = new CDataLayer();
        *newLayer = *pLayer;
        target.append(newLayer);
    }
}
CDesignDialog::CDesignDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDesignDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("属性表"));

    m_gauge = NULL;
    m_gaugeRect = QRect(580,252,200,232);
    m_itemRect = QRect(580,15,200,232);
    m_itemData = NULL;
    m_itemDialog = NULL;

    ui->treeWidget->setDragEnabled(false);
    connect(ui->treeWidget, &QTreeWidget::currentItemChanged, this, &CDesignDialog::getDataItem);
    m_names<<tr("中心点")<<tr("指针")<<tr("临界区")<<tr("环绕文字")<<tr("刻度")
          <<tr("刻度背景")<<tr("背景")<<tr("半圆背景")<<tr("标签")<<tr("显示值")
         <<tr("控制层")<<tr("直表指针")<<tr("直表临界区")<<tr("直表刻度文字")<<tr("直表刻度")
        <<tr("直表刻度背景")<<tr("直表显示块")<<tr("温度计");

    CMDMultiLanguage::instance()->translateWidget(this);
}

CDesignDialog::~CDesignDialog()
{
    for (qint32 i = 0; i < m_layers.count(); i++)
        delete m_layers.at(i);
    m_layers.clear();
    delete  m_itemDialog;
    delete ui;
}
/*!
\section 对控制层列表进行拷贝，并将控制层列表显示在树上
*/
void CDesignDialog::onInitDialog()
{
    clone(m_gauge->m_layer, m_layers);

    for (qint32 i = 0; i < m_layers.count(); i++)
    {
        CDataLayer* layer = m_layers.at(i);
        QTreeWidgetItem *layerItem  = createTreeItem(layer);

        for (qint32 j = 0; j < layer->m_item.count(); j++)
            layerItem->addChild(createTreeItem(layer->m_item.at(j)));

        ui->treeWidget->addTopLevelItem(layerItem);
        if(i == 0)
            ui->treeWidget->setCurrentItem(layerItem,0);

        m_layerIndexList.append(m_layers.at(i)->getIndex());
    }

    ui->treeWidget->expandAll();
}

QTreeWidgetItem *CDesignDialog::createTreeItem(CData *value)
{
    qint32 index = (qint32)value->getDrawType();
    QString strIndex = QString::number(index);
    QString name = m_names.at(index);

    if (value->getDrawType() == dtLayer)
    {
        CDataLayer* layer = dynamic_cast<CDataLayer*>(value);
        QString num =QString::number(layer->getIndex());
        QString str = CMDMultiLanguage::instance()->value("控制层");
        name = str+" "+num;

    }

    QTreeWidgetItem* item= new QTreeWidgetItem();
    item->setText(0,CMDMultiLanguage::instance()->value(name));
    item->setIcon(0,QIcon(":/picture/image/tree" + strIndex + ".bmp"));
    item->setData(0,Qt::UserRole,QVariant((qlonglong)value));
    return item;
    delete item;
}

CData *CDesignDialog::createData(drawType type)
{
    CData* data = NULL;

    switch (type) {
    case dtCap:
        data = new CDataCap;
        break;
    case dtNeedle:
        data = new CDataNeedle;
        break;
    case dtRange:
        data = new CDataRange;
        break;
    case dtScaleText:
        data = new CDataScaleText;
        break;
    case dtScale:
        data = new CDataScale;
        break;
    case dtScaleBack:
        data = new CDataScaleBack;
        break;
    case dtBack:
        data = new CDataBack;
        break;
    case dtHalfBack:
        data = new CDataHalfBack;
        break;
    case dtLabel:
        data = new CDataLabel;
        break;
    case dtLed:
        data = new CDataLed;
        break;
    case dtLayer:
        data = new CDataLayer;
        break;
    case dtRNeedle:
        data = new CRulerNeedle;
        break;
    case dtRRange:
        data = new CRulerRange;
        break;
    case dtRScaleText:
        data = new CRulerScaleText;
        break;
    case dtRScale:
        data = new CRulerScale;
         break;
    case dtRScaleBack:
        data = new CRulerScaleBack;
         break;
    case dtRMarker:
        data = new CRulerMarker;
         break;
    case dtTemperature:
        data = new CDataTemperature;
         break;
    default:
        break;
    }

    return data;

    delete data;

}

void CDesignDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(QPen(Qt::black,1));
    painter.drawRect(m_itemRect);
    painter.drawRect(m_gaugeRect);

    if(m_itemData == NULL)
    {
        painter.fillRect(m_itemRect,Qt::NoBrush);
        painter.fillRect(m_gaugeRect,Qt::NoBrush);
        return;
    }

    m_gauge->draw(&painter, m_gaugeRect, &m_layers);
    m_gauge->draw(&painter, m_itemRect, m_itemData,  &m_layers);
}

void CDesignDialog::getDataItem(QTreeWidgetItem *item)
{
    if(m_itemDialog != NULL)
    {
        delete m_itemDialog;
        m_itemDialog = NULL;
    }

    if (item == NULL)
    {
        m_itemData = NULL;
        return;
    }
    m_itemData = reinterpret_cast<CData*>(item->data(0, Qt::UserRole).toLongLong());

    switch(m_itemData->getDrawType())
    {
    case dtCap:
    {
        m_itemDialog = new CCapDialog(this);
        ((CCapDialog*)m_itemDialog)->setData(m_itemData);
        break;
    }
    case dtLayer:
    {
       m_itemDialog = new CLayerDialog(this);
       ((CLayerDialog*)m_itemDialog)->setData(m_itemData);
        break;
    }
    case dtNeedle:
        m_itemDialog = new CNeedleDialog(this);
        ((CNeedleDialog*)m_itemDialog)->setEnvrionment(m_gauge->m_environment);
        ((CNeedleDialog*)m_itemDialog)->setData(m_itemData);
        break;
    case dtRange:
        m_itemDialog = new CRangeDialog(this);
        ((CRangeDialog*)m_itemDialog)->setData(m_itemData);
        break;
    case dtScaleText:
        m_itemDialog = new CScaleTextDialog(this);
        ((CScaleTextDialog*)m_itemDialog)->setData(m_itemData);
        break;
    case dtScale:
        m_itemDialog = new CScaleDialog(this);
        ((CScaleDialog*)m_itemDialog)->setData(m_itemData);
        break;
    case dtScaleBack:
        m_itemDialog = new CScaleBackDialog(this);
        ((CScaleBackDialog*)m_itemDialog)->setData(m_itemData);
        break;
    case dtBack:
        m_itemDialog = new CBackDialog(this);
        ((CBackDialog*)m_itemDialog)->setData(m_itemData);
        break;
    case dtHalfBack:
        m_itemDialog = new CHalfBackDialog(this);
        ((CHalfBackDialog*)m_itemDialog)->setData(m_itemData);
        break;
    case dtLabel:
        m_itemDialog = new CLabelDialog(this);
        ((CLabelDialog*)m_itemDialog)->setData(m_itemData);
        break;
    case dtLed:
        m_itemDialog = new CLedDialog(this);
        ((CLedDialog*)m_itemDialog)->setEnvironment(m_gauge->m_environment);
        ((CLedDialog*)m_itemDialog)->setData(m_itemData);
        break;
    case dtRNeedle:
        m_itemDialog = new CRulerNeedleDialog(this);
        ((CRulerNeedleDialog*)m_itemDialog)->setEnvrionment(m_gauge->m_environment);
        ((CRulerNeedleDialog*)m_itemDialog)->setData(m_itemData);
        break;
    case dtRRange:
        m_itemDialog = new CRulerRangeDialog(this);
        ((CRulerRangeDialog*)m_itemDialog)->setData(m_itemData);
        break;
    case dtRScaleText:
        m_itemDialog = new CRulerScaleTextDialog(this);
        ((CRulerScaleTextDialog*)m_itemDialog)->setData(m_itemData);
        break;
    case dtRScale:
        m_itemDialog = new CRulerScaleDialog(this);
        ((CRulerScaleDialog*)m_itemDialog)->setData(m_itemData);
        break;
    case dtRScaleBack:
        m_itemDialog = new CRulerScaleBackDialog(this);
        ((CRulerScaleBackDialog*)m_itemDialog)->setData(m_itemData);
        break;
    case dtRMarker:
        m_itemDialog = new CRulerMarkerDialog(this);
        ((CRulerMarkerDialog*)m_itemDialog)->setEnvrionment(m_gauge->m_environment);
        ((CRulerMarkerDialog*)m_itemDialog)->setData(m_itemData);
        break;
    case dtTemperature:
        m_itemDialog = new CTemperatureDialog(this);
        ((CTemperatureDialog*)m_itemDialog)->setData(m_itemData);
        ((CTemperatureDialog*)m_itemDialog)->setEnvrionment(m_gauge->m_environment);
        break;
    default:
        return;
    }
    m_itemDialog->show();
    update();
}

void CDesignDialog::on_addBtn_clicked()
{
    CModuleSelectDialog module(this);

    if (module.exec() == QDialog::Accepted)
    {
        drawType type = (drawType)module.m_index;
        addItem(type);
    }

}

void CDesignDialog::addItem(drawType type)
{
    CData* newData = createData(type);
    QTreeWidget* tree = ui->treeWidget;
    QTreeWidgetItem* currentItem = tree->currentItem();
    CData* currentData = NULL;
    QTreeWidgetItem* layerItem = NULL;
    CDataLayer* layerData = NULL;
    if (currentItem != NULL)
    {
        currentData = reinterpret_cast<CData*>(currentItem->data(0, Qt::UserRole).toLongLong());
        layerItem = currentItem;
        if (layerItem->parent() != NULL)
            layerItem = layerItem->parent();
        layerData = reinterpret_cast<CDataLayer*>(layerItem->data(0, Qt::UserRole).toLongLong());
    }

    qint32 index = 0;
    if (type == dtLayer)
    {
//        m_layers.insert(index, static_cast<CDataLayer*>(newData));
        CDataLayer* newLayer = dynamic_cast<CDataLayer*>(newData);
        int createIndex(const QList<int>&);
        int layerIndex = createIndex(m_layerIndexList) -1;
        newLayer->setIndex(layerIndex+1);
        m_layers.insert(layerIndex, newLayer);
        m_layerIndexList.append(newLayer->getIndex());

        QTreeWidgetItem* newItem = createTreeItem(newData);
        tree->insertTopLevelItem(layerIndex, newItem);
        tree->setCurrentItem(newItem);
    }
    else
    {
        QTreeWidgetItem* newItem = createTreeItem(newData);

//!  无数据，需要添加控制层
        if (currentItem == NULL)
        {
            layerData = static_cast<CDataLayer*>(createData(drawType::dtLayer));
            layerItem = createTreeItem(layerData);

            m_layerIndexList.append(layerData->getIndex());

            m_layers.append(layerData);
            tree->addTopLevelItem(layerItem);

            currentData = layerData;
            currentItem = layerItem;
        }

        if (currentData->getDrawType() != dtLayer)
            index = layerData->m_item.indexOf(currentData) + 1;

        layerData->m_item.insert(index, newData);
        layerItem->insertChild(index, newItem);
        tree->setCurrentItem(newItem);
    }

}

void CDesignDialog::on_deleteBtn_clicked()
{
    QTreeWidget* tree = ui->treeWidget;
    QTreeWidgetItem* currentItem = tree->currentItem();
    if (currentItem == NULL)
        return;
    QTreeWidgetItem* aboveItem = tree->itemAbove(currentItem);
    QTreeWidgetItem* belowItem = tree->itemBelow(currentItem);

    CData* currentData = reinterpret_cast<CData*>(currentItem->data(0, Qt::UserRole).toLongLong());
    QTreeWidgetItem* layerItem = currentItem;
    if (layerItem->parent() != NULL)
        layerItem = layerItem->parent();
    CDataLayer* layerData = reinterpret_cast<CDataLayer*>(layerItem->data(0, Qt::UserRole).toLongLong());

    if (currentData->getDrawType() == dtLayer)
    {
        QMessageBox box ;
        box.setIcon(QMessageBox::Information);
        box.setText(tr("确认要删除控制层？"));
        box.setWindowTitle(tr("删除控制层"));
        box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);

        CMDMultiLanguage::instance()->translateWidget(&box);
        if(box.exec() == QMessageBox::Yes)
        {
            delete tree->takeTopLevelItem(tree->currentIndex().row());
            m_layers.removeOne(layerData);

            m_layerIndexList.removeOne(layerData->getIndex());

            delete layerData;
        }
        else
            return;
    }
    else
    {
        layerItem->removeChild(currentItem);
        delete currentItem;
        layerData->m_item.removeOne(currentData);
        delete currentData;
    }
    if (aboveItem != NULL)
        tree->setCurrentItem(aboveItem);
    else
        tree->setCurrentItem(belowItem);
}

void CDesignDialog::on_onOkBtn_clicked()
{
    clone(m_layers, m_gauge->m_layer);
    m_gauge->m_environment->update(m_gauge->regionRect());
    accept();
}

void CDesignDialog::on_cancleBtn_clicked()
{
    close();
}
void CDesignDialog::on_upBtn_clicked()
{
    QTreeWidgetItem* item=ui->treeWidget->currentItem();
    if(NULL == item)
        return;
    QTreeWidgetItem* parent=item->parent();
    if(parent == NULL)
    {
       QTreeWidgetItem * item= NULL;
       QTreeWidgetItem * itemAbove= NULL;
       qint32 pNum = ui->treeWidget->currentIndex().row();

       if(pNum != 0)
       {
           if(pNum - 1 >= 0)
           {
               itemAbove =ui->treeWidget->itemAbove(item);
               item = ui->treeWidget->takeTopLevelItem(pNum);
               ui->treeWidget->insertTopLevelItem(pNum -1,item);
               ui->treeWidget->insertTopLevelItem(pNum + 1,itemAbove);
           }

           if(pNum - 1 >= 0)
           {
               m_layers.swap(pNum - 1,pNum);
           }
       }
    }
    else
    {
        qint32 i = parent->indexOfChild(item);
        if(i-1 >= 0)
        {
            QTreeWidgetItem * item= NULL;
            QTreeWidgetItem * itemAbove= NULL;
            itemAbove =ui->treeWidget->itemAbove(item);
            item = parent->takeChild(i);
            parent->insertChild(i-1,item);
            parent->insertChild(i+1,itemAbove);
        }

        qint32 curNum = ui->treeWidget->indexOfTopLevelItem(parent);
        if(i-1 < 0 && curNum > 0)
        {
            parent->takeChild(i);
            CDataLayer* layer = NULL;
            long p = 0;
            p = parent->data(0, Qt::UserRole).toLongLong();
            m_itemData = reinterpret_cast<CData*>(p);
            layer =(CDataLayer*)m_itemData;
            layer->m_item.removeAt(i);

            parent = ui->treeWidget->itemAbove(parent);
            parent = parent->parent();
            parent->addChild(item);
            CDataLayer* uplayer = NULL;
            long pp = 0;
            pp = parent->data(0, Qt::UserRole).toLongLong();
            m_itemData = reinterpret_cast<CData*>(pp);
            uplayer =(CDataLayer*)m_itemData;
            long qq = item->data(0, Qt::UserRole).toLongLong();
            m_itemData = reinterpret_cast<CData*>(qq);
            uplayer->m_item.append(m_itemData);

        }

        ui->treeWidget->setCurrentItem(item);
        CDataLayer* layer = NULL;
        long p = 0;
        p = parent->data(0, Qt::UserRole).toLongLong();
        m_itemData = reinterpret_cast<CData*>(p);
        layer =(CDataLayer*)m_itemData;

        long q = item->data(0, Qt::UserRole).toLongLong();
        m_itemData = reinterpret_cast<CData*>(q);

        qint32 num = layer->m_item.indexOf(m_itemData);
        if(num - 1 >= 0)
        {
           layer->m_item.swap(num-1,num);
        }
    }
    ui->treeWidget->setCurrentItem(item);
    this->update();
}

void CDesignDialog::on_downBtn_clicked()
{
    QTreeWidgetItem* item=ui->treeWidget->currentItem();
    if(NULL == item)
        return;
    QTreeWidgetItem* parent=item->parent();
    if(parent == NULL)
    {
       QTreeWidgetItem * item= NULL;
       QTreeWidgetItem * itemBelow= NULL;
       qint32 pNum = ui->treeWidget->currentIndex().row();
       qint32 count = m_layers.count();
       if(pNum != count - 1)
       {
           if(pNum < count -1)
             {
               itemBelow =ui->treeWidget->itemBelow(item);
               item = ui->treeWidget->takeTopLevelItem(pNum);
               ui->treeWidget->insertTopLevelItem(pNum + 1,item);
               ui->treeWidget->insertTopLevelItem(pNum - 1,itemBelow);
             }

               ui->treeWidget->setCurrentItem(item);

           if(pNum < count -1)
             {
                m_layers.swap(pNum,pNum + 1);
             }
       }
    }
    else
    {
        qint32 count = parent->childCount();
        qint32 i = parent->indexOfChild(item);
        if(i < count - 1)
        {
            QTreeWidgetItem * item= NULL;
            QTreeWidgetItem * itemBelow= NULL;
            itemBelow =ui->treeWidget->itemBelow(item);
            item = parent->takeChild(i);
            parent->insertChild(i+1,item);
            parent->insertChild(i-1,itemBelow);
        }

         CDataLayer* layer = NULL;
         long p = 0;
         p = parent->data(0, Qt::UserRole).toLongLong();
         m_itemData = reinterpret_cast<CData*>(p);
         layer =(CDataLayer*)m_itemData;
         long q = item->data(0, Qt::UserRole).toLongLong();
         m_itemData = reinterpret_cast<CData*>(q);
         qint32 subNum = layer->m_item.count();
         qint32 num = layer->m_item.indexOf(m_itemData);
         if(num < subNum -1)
         {
            layer->m_item.swap(num,num + 1);
         }

         qint32 parentCount = ui->treeWidget->topLevelItemCount();
         qint32 curNum = ui->treeWidget->indexOfTopLevelItem(parent);
         if(i == count - 1 && curNum < parentCount - 1)
         {
             QTreeWidgetItem *curItem = NULL;
             curItem = parent->takeChild(i);

             CDataLayer* layer = NULL;
             long p = 0;
             p = parent->data(0, Qt::UserRole).toLongLong();
             m_itemData = reinterpret_cast<CData*>(p);
             layer =(CDataLayer*)m_itemData;
             layer->m_item.removeAt(i);

             QTreeWidgetItem *parentNext = NULL;
             parentNext = ui->treeWidget->topLevelItem(curNum+1);
             parentNext->insertChild(0,curItem);
             CDataLayer* uplayer = NULL;
             long pp = 0;
             pp = parentNext->data(0, Qt::UserRole).toLongLong();
             m_itemData = reinterpret_cast<CData*>(pp);
             uplayer =(CDataLayer*)m_itemData;
             long qq = item->data(0, Qt::UserRole).toLongLong();
             m_itemData = reinterpret_cast<CData*>(qq);
             uplayer->m_item.insert(0,m_itemData);
         }
    }
    ui->treeWidget->setCurrentItem(item);
    this->update();
}

void CDesignDialog::on_applyBtn_clicked()
{
    clone(m_layers, m_gauge->m_layer);
    m_gauge->m_environment->update(m_gauge->regionRect());
}

int createIndex(const QList<int>&layerIndexList)
{
    int count = layerIndexList.count();
    for(int i= 0; i < count;i++)
    {
        int index = i+1;
        if(!layerIndexList.contains(index))
            return index;
    }
    return (count+1);
}


