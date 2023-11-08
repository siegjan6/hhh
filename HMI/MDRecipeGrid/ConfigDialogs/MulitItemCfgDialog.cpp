#include "MulitItemCfgDialog.h"
#include "ui_MulitItemCfgDialog.h"
#include "ShareTool.h"
#include "GridItems/GridItemDef.h"

#include "ValueMap/ValueMapManager.h"

CMulitItemCfgDialog::CMulitItemCfgDialog(CRecipeGridModel *model, const QModelIndexList &selectList, IDrawEnvrionment* environment, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMulitItemCfgDialog)
{
    ui->setupUi(this);

    m_environment = environment;
    m_selectList = &selectList;
    m_model = model;

    CGridBaseItem* item = m_model->getItem(m_selectList->at(0));
    m_itemCopy = item ? item->myClone(false) : new CGridBaseItem;

    //! 当选择为checkbox模式时候,此lineEdit用于填写该checkbox的label.在dialog上替换掉同一位置的combobox
    m_lineeditCheckBoxLabel = new QLineEdit(ui->groupboxShowMode);
    if( itemType::checkBox == m_itemCopy->type() )
        m_lineeditCheckBoxLabel->setText( ((CCheckBoxItem*)m_itemCopy)->m_label );
    m_lineeditCheckBoxLabel->setGeometry(ui->comboboxValueTable->geometry());
    m_lineeditCheckBoxLabel->setHidden(true);
    m_replaceIndex = ui->horlayoutValueMap->indexOf(ui->comboboxValueTable);
    m_stretch = ui->horlayoutValueMap->stretch(m_replaceIndex);

    //! 检查数据范围group
    QRegExpValidator* realNumber = new QRegExpValidator(QRegExp(RegExp_RealNumber), this);
    ui->lineeditMin->setText(QString::number(m_itemCopy->m_minValue));
    ui->lineeditMin->setValidator(realNumber);
    ui->lineeditMax->setText(QString::number(m_itemCopy->m_maxValue));
    ui->lineeditMax->setValidator(realNumber);
    ui->lineeditOutRangeTip->setText(m_itemCopy->m_strOutRange);
    ui->checkboxTooltip->setChecked(m_itemCopy->m_showOutRange);
    on_checkboxTooltip_toggled(m_itemCopy->m_showOutRange);
    ui->groupboxRangeCheck->setChecked(m_itemCopy->m_valueRangeCheckable);

    //! 显示模式
    CShareTool::initItemTypeComboBox(ui->comboboxType, (itemType)m_itemCopy->type());
    CShareTool::initValueMapsComboBox(ui->comboboxValueTable, m_environment, m_itemCopy->m_valueMapName);
    ui->comboboxType->removeItem(ui->comboboxType->findText(tr("图像")));
    ui->checkboxEditable->setChecked(!m_itemCopy->m_editable);

    //! 数据源group
    ui->lineeditRecipeName->setText(m_itemCopy->m_recipeName);
    ui->lineeditDecimal->setText( QString::number(m_itemCopy->m_decimalCount) );
    ui->lineeditDecimal->setValidator(new QIntValidator(1,6,this));
    ui->lineeditRecipeName->setEnabled(false);
    CShareTool::initDataSourceTypeComboBox(ui->comboboxDataSource, m_itemCopy->dataSource());

    CShareTool::installDialogTranslator(this);

}

CMulitItemCfgDialog::~CMulitItemCfgDialog()
{
    delete m_itemCopy;
    delete ui;
}

void CMulitItemCfgDialog::on_pushbuttonOk_clicked()
{
    saveData();
    setMulitItems();

    accept();
}

void CMulitItemCfgDialog::on_pushbuttonCancel_clicked()
{
    reject();
}

void CMulitItemCfgDialog::on_checkboxTooltip_toggled(bool checked)
{
    ui->lineeditOutRangeTip->setEnabled(checked);
}

void CMulitItemCfgDialog::on_comboboxDataSource_currentIndexChanged(int)
{
    dataSourceType currentSource = (dataSourceType)ui->comboboxDataSource->currentData().toInt();
    if( dataSourceType::normal == currentSource )
    {
        ui->pushbuttonSelectVar->setEnabled(false);
        ui->lineeditDecimal->setEnabled(false);
        ui->groupboxRangeCheck->setEnabled(false);
        ui->groupboxShowMode->setEnabled(false);
    }
    else if( dataSourceType::realTimeVariable == currentSource )
    {
        ui->pushbuttonSelectVar->setEnabled(false);
        ui->lineeditDecimal->setEnabled(true);
        ui->groupboxRangeCheck->setEnabled(true);
        ui->groupboxShowMode->setEnabled(true);
    }
    else
    {
        ui->pushbuttonSelectVar->setEnabled(true);
        ui->lineeditDecimal->setEnabled(true);
        ui->groupboxRangeCheck->setEnabled(true);
        ui->groupboxShowMode->setEnabled(true);
    }
}

void CMulitItemCfgDialog::on_comboboxType_currentIndexChanged(int)
{
    itemType newType = (itemType)ui->comboboxType->currentData().toInt();

    if( itemType::checkBox == newType )
    {
        ui->labelValueMap->setText(tr("文本"));
        replaceWidget(ui->comboboxValueTable, m_lineeditCheckBoxLabel);
    }
    else
    {
        ui->labelValueMap->setText(tr("数值映射表"));
        replaceWidget(m_lineeditCheckBoxLabel, ui->comboboxValueTable);
    }

    if( itemType::baseItem == newType )
        ui->comboboxValueTable->setEnabled(false);
    else
        ui->comboboxValueTable->setEnabled(true);
}

void CMulitItemCfgDialog::replaceWidget(QWidget *srcWidget, QWidget *destWidget)
{
    if( !destWidget->isHidden() )
        return;

    destWidget->setHidden(false);
    destWidget->setGeometry(srcWidget->geometry());
    ui->horlayoutValueMap->insertWidget(m_replaceIndex, destWidget, m_stretch);
    ui->horlayoutValueMap->removeWidget(srcWidget);
    srcWidget->setHidden(true);
}

void CMulitItemCfgDialog::saveData()
{
    itemType newType = (itemType)m_itemCopy->type();
    dataSourceType newSource = (dataSourceType)ui->comboboxDataSource->currentData().toInt();
    if( dataSourceType::realTimeVariable == newSource || dataSourceType::recipeVariable == newSource )
        newType = (itemType)ui->comboboxType->currentData().toInt();

    //! 如果类型发生变化,重新创建item实例
    CGridBaseItem* oldItem = m_itemCopy;
    bool needDelete = false;
    if( m_itemCopy->type() != newType )
    {
        needDelete = true;
        m_itemCopy = NULL;
        m_itemCopy = CGridBaseItem::createItem(newType);
        Q_ASSERT(m_itemCopy);
    }

    //! 更新item的各种变量
    bool ok=false;
    qint32 tmpInt;
    qreal tmpReal;
    //! 数据源group----------------------------------------------------------
    m_itemCopy->m_dataSource = newSource;
    //! 配方名称
    if( dataSourceType::recipeVariable == newSource)
        m_itemCopy->m_recipeName = ui->lineeditRecipeName->text();
    //! 小数位数
    tmpInt = ui->lineeditDecimal->text().toInt(&ok);
    m_itemCopy->m_decimalCount = ok ? tmpInt : oldItem->m_decimalCount;
    //!----------------------------------------------------------------------
    //! 检查数据范围group-----------------------------------------------------
    m_itemCopy->m_valueRangeCheckable = ui->groupboxRangeCheck->isChecked();
    m_itemCopy->m_strOutRange = ui->lineeditOutRangeTip->text();
    m_itemCopy->m_showOutRange = ui->checkboxTooltip->isChecked();
    tmpReal = QVariant(ui->lineeditMin->text()).toReal(&ok);
    if(ok)
        m_itemCopy->m_minValue = tmpReal;
    tmpReal = QVariant(ui->lineeditMax->text()).toReal(&ok);
    if(ok)
        m_itemCopy->m_maxValue = tmpReal;
    //! 显示模式--------------------------------------------------------------
    if( itemType::checkBox == newType )
        ((CCheckBoxItem*)m_itemCopy)->m_label = m_lineeditCheckBoxLabel->text();

    m_itemCopy->m_valueMapName = ui->comboboxValueTable->currentText();
    m_itemCopy->m_editable = !ui->checkboxEditable->isChecked();
    //!----------------------------------------------------------------------
    if(needDelete)
        delete oldItem;
}

bool CMulitItemCfgDialog::setDataDifferent(CGridBaseItem *destitem, CGridBaseItem *srcitem)
{
    bool isDifferent = false;
     //! 数据源group----------------------------------------------------------
     if(destitem->m_dataSource != srcitem->m_dataSource)
     {
         isDifferent = true;
         destitem->m_dataSource = srcitem->m_dataSource;
     }
     if(destitem->m_recipeName != srcitem->m_recipeName)
     {
         isDifferent = true;
         destitem->m_recipeName = srcitem->m_recipeName;
     }
     if(destitem->m_decimalCount != srcitem->m_decimalCount)
     {
         isDifferent = true;
         destitem->m_decimalCount = srcitem->m_decimalCount;
     }
     //! 检查数据范围group-----------------------------------------------------
     if(destitem->m_valueRangeCheckable != srcitem->m_valueRangeCheckable)
     {
         isDifferent = true;
         destitem->m_valueRangeCheckable = srcitem->m_valueRangeCheckable;
     }
     if(destitem->m_showOutRange != srcitem->m_showOutRange)
     {
         isDifferent = true;
         destitem->m_showOutRange = srcitem->m_showOutRange;
     }
     if(destitem->m_strOutRange != srcitem->m_strOutRange)
     {
         isDifferent = true;
         destitem->m_strOutRange = srcitem->m_strOutRange;
     }
     if(destitem->m_minValue != srcitem->m_minValue)
     {
         isDifferent = true;
         destitem->m_minValue = srcitem->m_minValue;
     }
     if(destitem->m_maxValue != srcitem->m_maxValue)
     {
         isDifferent = true;
         destitem->m_maxValue = srcitem->m_maxValue;
     }
     //! 显示模式--------------------------------------------------------------
     if(destitem->m_editable != srcitem->m_editable)
     {
         isDifferent = true;
         destitem->m_editable = srcitem->m_editable;
     }
     if(destitem->m_valueMapName != srcitem->m_valueMapName)
     {
         isDifferent = true;
         destitem->m_valueMapName = srcitem->m_valueMapName;
     }
     if( destitem->type() != srcitem->type() )
         isDifferent = true;

     if( itemType::checkBox == destitem->type() && itemType::checkBox == srcitem->type() )
     {
         if( ((CCheckBoxItem*)destitem)->m_label.compare( ((CCheckBoxItem*)srcitem)->m_label ) )
         {
             isDifferent = true;
             ((CCheckBoxItem*)destitem)->m_label = ((CCheckBoxItem*)srcitem)->m_label;
         }
     }

     return isDifferent;
}

void CMulitItemCfgDialog::setMulitItems()
{
    CGridBaseItem* prototype = CGridBaseItem::createItem((itemType)m_itemCopy->type());
    bool diffFromProtoType = setDataDifferent(prototype, m_itemCopy);

    foreach(QModelIndex index, *m_selectList)
    {
        CGridBaseItem* item = m_model->getItem(index);
        //! 单元格没有数据item,且数据有改动.则clone一份新的item.
        if(NULL == item)
        {
            if(diffFromProtoType)
                m_model->setItem(index.row(), index.column(), prototype->clone());
        }
        //! 如果单元格有item,并且数据和itemCopy不一样,则clone一份新的item.
        else
        {
            if( m_itemCopy->type() != item->type() )
            {
                CGridBaseItem* newItem = CGridBaseItem::createItem((itemType)m_itemCopy->type());
                newItem->copyMembers(item,false);
                setDataDifferent(newItem, m_itemCopy);
                m_model->setItem(index.row(), index.column(), newItem);
            }
            else
                setDataDifferent(item, m_itemCopy);
        }

    }

    delete prototype;
}
