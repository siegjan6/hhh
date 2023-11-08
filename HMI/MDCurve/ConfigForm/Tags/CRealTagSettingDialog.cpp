#include "CRealTagSettingDialog.h"
#include "ui_CRealTagSettingDialog.h"

#include "CRealFTag.h"
#include "CFeatureTag.h"
#include "CTagItemSettingDialog.h"
#include "IMDRTDBManagerUI.h"
#include <QTableWidgetItem>
#include "CTranslator.h"
#include "CXAxis.h"
#include "CFeatureTagValueItem.h"
#include "CVariantDialog.h"
#include "CFormToolFunc.h"
#include "MDDBQueryUI.h"
//#include "MDDBQueryUI\MDDBQueryUI.h"
#include <QDebug>

using namespace Translator;
using namespace FormToolFunc;

CRealTagSettingDialog::CRealTagSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRealTagSettingDialog),
    m_tag(0),
    m_historyTag(0),
    m_variantForm(0),
    m_transTool(0)
{
    ui->setupUi(this);
    installDialogTranslator(m_transTool,this);

    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(4);
    QStringList labels;
    labels<<"名称"<<"显示"<<"描述"<<"显示格式";
    ui->tableWidget->setHorizontalHeaderLabels(labels);

    this->move(135,0);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
}

CRealTagSettingDialog::~CRealTagSettingDialog()
{
    delete ui;
}

void CRealTagSettingDialog::setData(CFeatureTag *tag, const QList<CXAxis *> &xList)
{
    if(tag && !xList.isEmpty())
    {
        m_historyTag = tag;
        m_xList = xList;
        m_itemList = m_historyTag->itemList();
        initialize();
    }
}

void CRealTagSettingDialog::setData(CRealFTag *tag,const QList<CXAxis *> &xList)
{
    if(tag && !xList.isEmpty())
    {
        m_tag = tag;
        m_xList = xList;
        m_itemList = m_tag->itemList();
        initialize();
    }
}

void CRealTagSettingDialog::setVariantForm(IMDRTDBManagerUI *variantForm)
{
    m_variantForm = variantForm;
}

void CRealTagSettingDialog::setFields(QStringList fields)
{
    m_fields = fields;
}

void CRealTagSettingDialog::initialize()
{
    breakConnections();
    initializeAxisComboBox(m_xList,ui->xsComboBox);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    if(m_tag)
    {
        ui->visibleCheckBox->setChecked(m_tag->visible());
        ui->internalSpinBox->setValue(m_tag->internal());
        ui->timeLabel->setVisible(false);
        ui->timePushButton->setVisible(false);

        if(!m_tag->xName().isEmpty())
        {
            for(int i = 0; i< m_xList.count();i++)
            {
                if(m_tag->xName() == m_xList.at(i)->name())
                {
                    ui->xsComboBox->setCurrentIndex(i);
                }
            }
        }
    }
    if(m_historyTag)
    {
        ui->visibleCheckBox->setChecked(m_historyTag->visible());
        ui->internalSpinBox->setValue(m_historyTag->internal());
        ui->timePushButton->setText(m_historyTag->timeVariant());

        if(!m_historyTag->xName().isEmpty())
        {
            for(int i = 0; i< m_xList.count();i++)
            {
                if(m_historyTag->xName() == m_xList.at(i)->name())
                {
                    ui->xsComboBox->setCurrentIndex(i);
                }
            }
        }
    }

    parseItemsInformation();
    createConnections();
    createMenu();
}

void CRealTagSettingDialog::applyChanges()
{
    if(m_tag && !m_xList.isEmpty())
    {
        m_tag->setItemList(m_itemList);
        m_tag->setVisible(ui->visibleCheckBox->isChecked());
        m_tag->setInternal(ui->internalSpinBox->value());
        m_tag->setXName(m_xList.at(ui->xsComboBox->currentIndex())->name());
    }

    if(m_historyTag && !m_xList.isEmpty())
    {
        m_historyTag->setItemList(m_itemList);
        m_historyTag->setVisible(ui->visibleCheckBox->isChecked());
        m_historyTag->setInternal(ui->internalSpinBox->value());
        m_historyTag->setXName(m_xList.at(ui->xsComboBox->currentIndex())->name());
        m_historyTag->setTimeVariant(ui->timePushButton->text());
    }
}

void CRealTagSettingDialog::createConnections()
{
    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(slotAddItem()));
    connect(ui->deleteButton,SIGNAL(clicked()),this,SLOT(slotDeleteItem()));

    connect(ui->moveUpButton,SIGNAL(clicked()),this,SLOT(slotMoveUpRow()));
    connect(ui->moveDownButton,SIGNAL(clicked()),this,SLOT(slotMoveDownRow()));

    connect(ui->tableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this,SLOT(slotSetItemProperty(QTableWidgetItem*)));

    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),
            this,SLOT(slotSetItemVisible(QTableWidgetItem*)));
    connect(ui->timePushButton,SIGNAL(clicked()),this,SLOT(slotSetTimeVariant()));
}

void CRealTagSettingDialog::breakConnections()
{
    disconnect(ui->addButton,SIGNAL(clicked()),this,SLOT(slotAddItem()));
    disconnect(ui->deleteButton,SIGNAL(clicked()),this,SLOT(slotDeleteItem()));

    disconnect(ui->moveUpButton,SIGNAL(clicked()),this,SLOT(slotMoveUpRow()));
    disconnect(ui->moveDownButton,SIGNAL(clicked()),this,SLOT(slotMoveDownRow()));

    disconnect(ui->tableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
               this,SLOT(slotSetItemProperty(QTableWidgetItem*)));
    disconnect(ui->timePushButton,SIGNAL(clicked()),this,SLOT(slotSetTimeVariant()));

}

void CRealTagSettingDialog::slotAddItem()
{
    if(m_variantForm)
    {
        QStringList variantList;
        m_variantForm->selectVariables(variantList);
        if(!variantList.isEmpty())
            for(int i = 0; i<variantList.count();i++)
            {
                CFeatureTagItem *itemInitial = new  CFeatureTagValueItem;
                itemInitial->setItemName(variantList.at(i));
                bool hasRepeat = isTagRepeat(itemInitial->itemName());
                if(hasRepeat)
                {
                    FormToolFunc::showMessage(tr("特征标签")+itemInitial->itemName()+tr("已存在!"),this);
                    delete itemInitial;
                    itemInitial = NULL;
                    continue;
                }

                if(!itemInitial->itemName().isEmpty())
                {
                    m_itemList.append(itemInitial);
                    parseItemsInformation();
                }
                else
                {
                    delete itemInitial;
                    itemInitial = NULL;
                }
            }
    }
    else
    {
        QString variantName;
        QList<DBQ::ViewField *>viewFiled;
        viewFiled = DBQUI::GetFieldsFromViewSelected(m_historyTag->projectPath()+"/DBQuery",m_historyTag->viewName(),DBQUI::VIEW_FIELD_RETURN_TYPE_VARIABLE);
        if(viewFiled.count() > 0)
        {
            for(int i = 0; i<viewFiled.count();i++)
            {
                CFeatureTagItem *itemInitial = new  CFeatureTagValueItem;
                variantName = m_historyTag->viewName()+ "." +viewFiled.at(i)->fieldName;
                itemInitial->setItemName(variantName);
                bool hasRepeat = isTagRepeat(itemInitial->itemName());
                if(hasRepeat)
                {
                    FormToolFunc::showMessage(tr("特征标签")+variantName+tr("已存在!"),this);
                    delete itemInitial;
                    itemInitial = NULL;
                    continue;
                }

                if(!itemInitial->itemName().isEmpty())
                {
                    m_itemList.append(itemInitial);
                    parseItemsInformation();
                }
                else
                {
                    delete itemInitial;
                    itemInitial = NULL;
                }
            }
        }
    }
}

void CRealTagSettingDialog::slotDeleteItem()
{
    int currentIndex = ui->tableWidget->currentRow();
    if( currentIndex != -1)
    {
        CFeatureTagItem *item = m_itemList.at(currentIndex);
        int ret = showMessage(tr("是否删除标签 ")+item->itemName()+"?",
                              QMessageBox::Ok|QMessageBox::Cancel);
        if(QMessageBox::Ok == ret)
        {
            delete m_itemList.takeAt(currentIndex);
            parseItemsInformation();
        }
    }
    else
    {
        showMessage(tr("请选择一个变量"));
    }
}

void CRealTagSettingDialog::slotMoveUpRow()
{
    int currentIndex = ui->tableWidget->currentRow();
    if(currentIndex != -1) {
        int preIndex = currentIndex -1;
        if(preIndex != -1) //! not the first one
            swapTableItem(preIndex,currentIndex);
    }
    else
    {
        showMessage(tr("请选中一个标签"));
    }
}

void CRealTagSettingDialog::slotMoveDownRow()
{
    int currentIndex = ui->tableWidget->currentRow();
    int maxIndex = ui->tableWidget->rowCount()-1;
    if(currentIndex != -1)
    {
        int nextIndex = currentIndex +1;
        if(nextIndex <= maxIndex) //!
            swapTableItem(nextIndex,currentIndex);
    }
    else
    {
        showMessage(tr("请选中一个标签"));
    }
}

void CRealTagSettingDialog::slotSetItemProperty(QTableWidgetItem *item)
{
    if(item)
    {
        int rowCount = item->row();
        CFeatureTagItem *itemInitial = m_itemList.at(rowCount);

        bool ok = false;
        CFeatureTagItem *item = CTagItemSettingDialog::getItem(&ok,itemInitial,this);
        if(item)
        {
            if(ok)
            {
                parseItemsInformation();
            }
        }
    }
}

void CRealTagSettingDialog::slotSetItemVisible(QTableWidgetItem *item)
{
    if(item && Qt::ItemIsUserCheckable == item->flags())
    {
        if(Qt::Checked == item->checkState())
        {
            item->setCheckState(Qt::Unchecked);
            m_itemList.at(item->row())->setVisible(false);
        }
        else if(Qt::Unchecked == item->checkState())
        {
            item->setCheckState(Qt::Checked);
            m_itemList.at(item->row())->setVisible(true);
        }
    }
}

void CRealTagSettingDialog::slotSetTimeVariant()
{
    if(!m_historyTag)
        return;
    QString timeVariantName = m_historyTag->timeVariant();

    QList<DBQ::ViewField *>viewFiled;
    viewFiled = DBQUI::GetFieldsFromViewSelected(m_historyTag->projectPath()+"/DBQuery",m_historyTag->viewName(),DBQUI::VIEW_FIELD_RETURN_TYPE_DATETIME);
    if(viewFiled.count() > 0)
    {
        timeVariantName = viewFiled.first()->fieldName;
    }
    ui->timePushButton->setText(timeVariantName);
}

void CRealTagSettingDialog::slotCustomMenu(const QPoint &p)
{
    QPoint point = mapToGlobal(p);
    m_menu->exec(point);
}

void CRealTagSettingDialog::parseItemsInformation()
{
    int count = m_itemList.count();
    ui->tableWidget->setRowCount(count);
    for(int i = 0; i < count;i++)
    {
        CFeatureTagItem *item = m_itemList.at(i);
        QString itemName;
        switch (item->type())
        {
        case CFeatureTagItem::ItemType::Value:
            itemName = item->itemName();
            break;
        case CFeatureTagItem::ItemType::Comment:
            itemName = tr("标签说明");
            break;
        case CFeatureTagItem::ItemType::Time:
            itemName = tr("时间");
            break;
        default:
            break;
        }

        insertTableItem(i,0,createNameItem(itemName));
        insertTableItem(i,1,createVisibleItem(item->visible()));
        insertTableItem(i,2,createCommentItem(item->comment()));
        insertTableItem(i,3,createFormatItem(item->format()));
    }
}

void CRealTagSettingDialog::swapTableItem(int preIndex, int currentIndex)
{
    QString currentName = ui->tableWidget->item(currentIndex,0)->text();
    Qt::CheckState currentState = ui->tableWidget->item(currentIndex,1)->checkState();
    QString currentComment = ui->tableWidget->item(currentIndex,2)->text();
    QString currentFormat = ui->tableWidget->item(currentIndex,3)->text();

    QString preName = ui->tableWidget->item(preIndex,0)->text();
    Qt::CheckState preState = ui->tableWidget->item(preIndex,1)->checkState();
    QString preComment = ui->tableWidget->item(preIndex,2)->text();
    QString preFormat = ui->tableWidget->item(preIndex,3)->text();

    ui->tableWidget->item(currentIndex,0)->setText(preName);
    ui->tableWidget->item(currentIndex,1)->setCheckState(preState);
    ui->tableWidget->item(currentIndex,2)->setText(preComment);
    ui->tableWidget->item(currentIndex,3)->setText(preFormat);

    ui->tableWidget->item(preIndex,0)->setText(currentName);
    ui->tableWidget->item(preIndex,1)->setCheckState(currentState);
    ui->tableWidget->item(preIndex,2)->setText(currentComment);
    ui->tableWidget->item(preIndex,3)->setText(currentFormat);

    ui->tableWidget->selectRow(preIndex);

    m_itemList.swap(preIndex,currentIndex);
}
QTableWidgetItem *CRealTagSettingDialog::createNameItem(const QString &name)
{
    QTableWidgetItem *nameItem = new QTableWidgetItem;
    nameItem->setTextAlignment(Qt::AlignCenter);
    nameItem->setText(name);
    return nameItem;
}

QTableWidgetItem *CRealTagSettingDialog::createVisibleItem(bool visible)
{
    QTableWidgetItem *nameItem = new QTableWidgetItem;
    nameItem->setTextAlignment(Qt::AlignCenter);
    nameItem->setFlags(Qt::ItemIsUserCheckable);
    if(visible)
        nameItem->setCheckState(Qt::Checked);
    else
        nameItem->setCheckState(Qt::Unchecked);

    return nameItem;
}

QTableWidgetItem *CRealTagSettingDialog::createCommentItem(const QString &comment)
{
    QTableWidgetItem *commentItem = new QTableWidgetItem;
    commentItem->setTextAlignment(Qt::AlignCenter);
    commentItem->setText(comment);

    return commentItem;
}

QTableWidgetItem *CRealTagSettingDialog::createFormatItem(const QString &format)
{
    QTableWidgetItem *formatItem = new QTableWidgetItem;
    formatItem->setText(format);
    return formatItem;
}

void CRealTagSettingDialog::insertTableItem(int row, int column, QTableWidgetItem *item)
{
    ui->tableWidget->setItem(row,column,item);
}

int CRealTagSettingDialog::showMessage(const QString &comment, QMessageBox::StandardButtons buttons)
{
    return QMessageBox::warning(this,tr("警告"),comment,buttons);
}

void CRealTagSettingDialog::initializeAxisComboBox(QList<CXAxis *> axisList, QComboBox *comboBox)
{
    if(!axisList.isEmpty() && comboBox)
    {
        comboBox->clear();
        for(int i  = 0; i < axisList.count();i++)
        {
            CXAxis *axis = axisList.at(i);
            comboBox->addItem(QString("%1--%2").arg(axis->name())
                                           .arg(axis->comment()));
        }
    }
}

void CRealTagSettingDialog::createMenu()
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slotCustomMenu(QPoint)));// 响应右键菜单

    m_menu = new QMenu(ui->tableWidget);
    QAction *addAction = new QAction(tr("增加"), ui->tableWidget);
    m_menu->addAction(addAction);
    QAction* deleteAction = new QAction(tr("删除"), ui->tableWidget);
    m_menu->addAction(deleteAction);
    QAction *upAction = new QAction(tr("上移"),ui->tableWidget);
    m_menu->addAction(upAction);
    QAction *downAction = new QAction(tr("下移"),ui->tableWidget);
    m_menu->addAction(downAction);

    connect (addAction,&QAction::triggered,this,&CRealTagSettingDialog::slotAddItem);
    connect (deleteAction,&QAction::triggered,this,&CRealTagSettingDialog::slotDeleteItem);
    connect (upAction,&QAction::triggered,this,&CRealTagSettingDialog::slotMoveUpRow);
    connect (downAction,&QAction::triggered,this,&CRealTagSettingDialog::slotMoveDownRow);

}

bool CRealTagSettingDialog::isTagRepeat(const QString &str)
{
    if(m_variantForm)
    {
        for(int i = 0; i< m_itemList.count();i++)
        {
            CFeatureTagItem *item = m_itemList.at(i);
            if(item->type() == CFeatureTagItem::ItemType::Value)
            {
                if(item->itemName() == str)
                    return true;
            }
        }
    }
    else
    {
        for(int i = 0; i< m_historyTag->itemList().count();i++)
        {
            CFeatureTagItem *item = m_historyTag->itemList().at(i);
            if(item->type() == CFeatureTagItem::ItemType::Value)
            {
                if(item->itemName() == str)
                    return true;
            }
        }
    }
    return false;

}

