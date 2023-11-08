#include "ItemCfgDialog.h"
#include "ui_ItemCfgDialog.h"
#include "ShareTool.h"
#include "GridItems/GridItemDef.h"

#include "ValueMap/ValueMapManager.h"

#include <QFileDialog>
#include <QMessageBox>

CItemCfgDialog::CItemCfgDialog(CRecipeGridModel *model, const QModelIndex& index, IDrawEnvrionment *environment, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CItemCfgDialog)
{
    ui->setupUi(this);

    m_environment = environment;
    m_model = model;
    m_index = index;

    CGridBaseItem* item = m_model->getItem(m_index);
    m_itemCopy = item ? item->myClone(false) : new CGridBaseItem;

    m_realNumber = new QRegExpValidator(QRegExp(RegExp_RealNumber), this);

    //! 当选择为checkbox模式时候,此lineEdit用于填写该checkbox的label.在dialog上替换掉同一位置的combobox
    m_lineeditCheckBoxLabel = new QLineEdit(ui->groupboxShowMode);
    if( itemType::checkBox == m_itemCopy->type() )
        m_lineeditCheckBoxLabel->setText( ((CCheckBoxItem*)m_itemCopy)->m_label );
    m_lineeditCheckBoxLabel->setGeometry(ui->comboboxValueTable->geometry());
    m_lineeditCheckBoxLabel->setHidden(true);
    m_replaceIndex = ui->horlayoutValueMap->indexOf(ui->comboboxValueTable);
    m_stretch = ui->horlayoutValueMap->stretch(m_replaceIndex);

    //! 检查数据范围group
    m_minVar = m_itemCopy->m_minVariable;
    m_maxVar = m_itemCopy->m_maxVariable;
    m_minValue = QString::number(m_itemCopy->m_minValue);
    m_maxValue = QString::number(m_itemCopy->m_maxValue);
    ui->checkboxVarBind->setChecked(m_itemCopy->m_variableBind);
    on_checkboxVarBind_toggled(m_itemCopy->m_variableBind);
    ui->lineeditOutRangeTip->setText(m_itemCopy->m_strOutRange);
    ui->checkboxTooltip->setChecked(m_itemCopy->m_showOutRange);
    on_checkboxTooltip_toggled(m_itemCopy->m_showOutRange);
    ui->groupboxRangeCheck->setChecked(m_itemCopy->m_valueRangeCheckable);
    //! 显示模式
    CShareTool::initItemTypeComboBox(ui->comboboxType, (itemType)m_itemCopy->type());
    CShareTool::initValueMapsComboBox(ui->comboboxValueTable, m_environment, m_itemCopy->m_valueMapName);
    ui->checkboxEditable->setChecked(!m_itemCopy->m_editable);
    ui->checkboxMultiLanguage->setChecked(m_itemCopy->m_multiLanguage);

    //! 数据源group
    itemType type = (itemType)m_itemCopy->type();
    m_recipeName = m_itemCopy->m_recipeName;

    if( itemType::image == type )
         m_imageName = ((CImageItem*)m_itemCopy)->imageName();
    ui->lineeditVarName->setText(m_itemCopy->text());

    ui->lineeditDecimal->setText( QString::number(m_itemCopy->m_decimalCount) );
    ui->lineeditDecimal->setValidator(new QIntValidator(1,6,this));
    ui->lineeditDateFormat->setText( m_itemCopy->m_dateFormat );
    //ui->lineeditRecipeName->setEnabled(false);
    CShareTool::initDataSourceTypeComboBox(ui->comboboxDataSource, m_itemCopy->dataSource());

    CShareTool::installDialogTranslator(this);

    qint32 itemIdx = ui->comboboxType->findData((int)itemType::image);
    if( itemIdx != -1 )
        m_imageItemTitle = ui->comboboxType->itemText(itemIdx);
    else
        m_imageItemTitle = tr("图像");
}

CItemCfgDialog::~CItemCfgDialog()
{
    delete m_itemCopy;
    delete ui;
}

void CItemCfgDialog::on_pushbuttonOk_clicked()
{
    itemType newType = (itemType)m_itemCopy->type();
    dataSourceType newSource = (dataSourceType)ui->comboboxDataSource->currentData().toInt();
    if( dataSourceType::normal == newSource)
       newType = ui->lineeditRecipeName->text().isEmpty() ? itemType::baseItem : itemType::image;

    else
        newType = (itemType)ui->comboboxType->currentData().toInt();

    //! 如果类型发生变化,重新创建item实例
    CGridBaseItem* oldItem = m_itemCopy;
    bool needDelete = false;
    if( m_itemCopy->type() != newType )
    {
        needDelete = true;
        m_itemCopy = NULL;
        m_itemCopy = CGridBaseItem::createItem(newType);
        //! 这里要保留成组信息
        m_itemCopy->copyMembers(oldItem, false);
        Q_ASSERT(m_itemCopy);
    }

    //! 更新item的各种变量
    bool ok=false;
    qint32 tmpInt;
    qreal tmpReal;
    //! 数据源group----------------------------------------------------------
    m_itemCopy->m_dataSource = newSource;
    //! 变量名称/常规字符串/图片名称 
    if( dataSourceType::normal == newSource)
    {
        if( itemType::image == newType )
        {
            ((CImageItem*)m_itemCopy)->setImageName(ui->lineeditRecipeName->text());
            m_itemCopy->setText(((CImageItem*)m_itemCopy)->imageName());
        }
        else
            m_itemCopy->setText(ui->lineeditVarName->text());
    }
    else
    {
        m_itemCopy->setText(ui->lineeditVarName->text());
        m_itemCopy->m_recipeName = ui->lineeditRecipeName->text();
    }
    //! 小数位数
    tmpInt = ui->lineeditDecimal->text().toInt(&ok);
    m_itemCopy->m_decimalCount = ok ? tmpInt : oldItem->m_decimalCount;
    m_itemCopy->m_dateFormat = ui->lineeditDateFormat->text();
    //!----------------------------------------------------------------------
    //! 检查数据范围group-----------------------------------------------------
    m_itemCopy->m_valueRangeCheckable = ui->groupboxRangeCheck->isChecked();
    m_itemCopy->m_variableBind = ui->checkboxVarBind->isChecked();
    m_itemCopy->m_strOutRange = ui->lineeditOutRangeTip->text();
    m_itemCopy->m_showOutRange = ui->checkboxTooltip->isChecked();
    if( ui->checkboxVarBind->isChecked() )
    {
        m_itemCopy->m_minVariable = ui->lineeditMin->text();
        m_itemCopy->m_maxVariable = ui->lineeditMax->text();
    }
    else
    {
        tmpReal = QVariant(ui->lineeditMin->text()).toReal(&ok);
        if(ok)
            m_itemCopy->m_minValue = tmpReal;
        tmpReal = QVariant(ui->lineeditMax->text()).toReal(&ok);
        if(ok)
            m_itemCopy->m_maxValue = tmpReal;
    }
    //! 显示模式--------------------------------------------------------------
    if( itemType::checkBox == newType )
        ((CCheckBoxItem*)m_itemCopy)->m_label = m_lineeditCheckBoxLabel->text();
    m_itemCopy->m_valueMapName = ui->comboboxValueTable->currentText();
    m_itemCopy->m_editable = !ui->checkboxEditable->isChecked();
    m_itemCopy->m_multiLanguage = ui->checkboxMultiLanguage->isChecked();

    //! 设置最新的item到model上去
    m_model->setItem(m_index.row(), m_index.column(), m_itemCopy->myClone(false));

    if(needDelete)
        delete oldItem;

    accept();
}

void CItemCfgDialog::on_pushbuttonCancel_clicked()
{
    reject();
}

void CItemCfgDialog::on_pushbuttonSelectVar_clicked()
{
    dataSourceType currentSource = (dataSourceType)ui->comboboxDataSource->currentData().toInt();
    if( dataSourceType::normal == currentSource )
    {
        QString fileName = CShareTool::setlectImageUI(m_environment, this);
        if( fileName.isEmpty() )
            return;
        m_imageName = fileName;
        ui->lineeditRecipeName->setText(fileName);
        ui->lineeditVarName->setText(fileName);
    }
    else
    {
        QString varName;
        if( !CShareTool::selectVariableUI(m_environment, varName) )
            return;

        //!Y2hvdWps null
        //! 配方变量也在这里赋值,等待接口开放再补充
        ui->lineeditVarName->setText(varName);
    }
}

void CItemCfgDialog::on_checkboxVarBind_toggled(bool checked)
{
    ui->pushbuttonSelectMin->setEnabled(checked);
    ui->pushbuttonSelectMax->setEnabled(checked);

    if( checked )//! 显示变量
    {
        ui->lineeditMin->setText( m_minVar );
        ui->lineeditMax->setText( m_maxVar );
        ui->lineeditMin->setValidator(0);
        ui->lineeditMax->setValidator(0);
    }
    else//! 直接输入数值
    {

        ui->lineeditMin->setText( m_minValue );
        ui->lineeditMax->setText( m_maxValue );
        ui->lineeditMin->setValidator(m_realNumber);
        ui->lineeditMax->setValidator(m_realNumber);

    }
}

void CItemCfgDialog::on_pushbuttonSelectMin_clicked()
{
    QString varName;
    if( !CShareTool::selectVariableUI(m_environment, varName) )
        return;

    m_minVar = varName;
    if( ui->checkboxVarBind )
        ui->lineeditMin->setText(m_minVar);
}
void CItemCfgDialog::on_pushbuttonSelectMax_clicked()
{
    QString varName;
    if( !CShareTool::selectVariableUI(m_environment, varName) )
        return;

    m_maxVar = varName;
    if( ui->checkboxVarBind )
        ui->lineeditMax->setText(m_maxVar);
}

void CItemCfgDialog::on_checkboxTooltip_toggled(bool checked)
{
    ui->lineeditOutRangeTip->setEnabled(checked);
}

void CItemCfgDialog::on_pushbuttonValueMapEdit_clicked()
{
    QString valueMapName = ui->comboboxValueTable->currentText();
    valueMapName = CValueMapManager::editValueMapUI(m_environment, valueMapName, this);

    //! 由于可能新添加数值映射表,这里要刷新一下combobox
    ui->comboboxValueTable->clear();
    CShareTool::initValueMapsComboBox(ui->comboboxValueTable, m_environment, valueMapName);
}

void CItemCfgDialog::on_comboboxDataSource_currentIndexChanged(int)
{
    //! UI上的变化-----------------------------------------------
    dataSourceType currentSource = (dataSourceType)ui->comboboxDataSource->currentData().toInt();
    if( dataSourceType::normal == currentSource )
    {
        ui->pushbuttonSelectVar->setText(tr("插图..."));
        ui->labelRecipe->setText(tr("图片名:"));
        ui->labelVariable->setText(tr("文本"));
        ui->lineeditDecimal->setEnabled(false);
        ui->lineeditDateFormat->setEnabled(false);
        ui->groupboxRangeCheck->setEnabled(false);
        ui->groupboxShowMode->setEnabled(false);
        ui->checkboxMultiLanguage->setEnabled(true);
    }
    else
    {
        ui->pushbuttonSelectVar->setText(tr("选择..."));
        ui->labelRecipe->setText(tr("配方名:"));
        ui->labelVariable->setText(tr("变量名:"));
        ui->lineeditDecimal->setEnabled(true);
        ui->lineeditDateFormat->setEnabled(true);
        ui->groupboxRangeCheck->setEnabled(true);
        ui->groupboxShowMode->setEnabled(true);
        ui->checkboxMultiLanguage->setEnabled(false);
    }
    if(dataSourceType::realTimeVariable == currentSource)
    {
        qint32 index = ui->comboboxType->findData((int)itemType::image);
        if( index == -1 )
            ui->comboboxType->addItem(m_imageItemTitle, itemType::image);
    }
    else if(dataSourceType::recipeVariable == currentSource)
    {
        qint32 index = ui->comboboxType->findData((int)itemType::image);
        if( index != -1 )
            ui->comboboxType->removeItem(index);
    }

    //! 数据上的变化----------------------------------------------
    dataSourceType itemSource = m_itemCopy->dataSource();
    if( dataSourceType::normal == itemSource )
    {
        ui->lineeditRecipeName->setText(m_imageName);
        //ui->lineeditDateFormat->setEnabled(false);
    }
    else if(dataSourceType::realTimeVariable == itemSource)
    {
        ui->lineeditRecipeName->setText("");
        //! Y2hvdWps null
        //! 当实时变量为时间型变量时,使能.等待接口开放后更新
        //if( 0 )
        //ui->lineeditDateFormat->setEnabled(true);
    }
    else if(dataSourceType::recipeVariable == itemSource)
    {
        ui->lineeditRecipeName->setText(m_recipeName);
        //ui->lineeditDateFormat->setEnabled(false);
    }

}

void CItemCfgDialog::on_comboboxType_currentIndexChanged(int)
{
    itemType newType = (itemType)ui->comboboxType->currentData().toInt();

    if( itemType::checkBox == newType )
    {
        ui->labelValueMap->setText(tr("文本"));
        replaceWidget(ui->comboboxValueTable, m_lineeditCheckBoxLabel);
        ui->pushbuttonValueMapEdit->setEnabled(false);
    }
    else
    {
        ui->labelValueMap->setText(tr("数值映射表"));
        replaceWidget(m_lineeditCheckBoxLabel, ui->comboboxValueTable);
        ui->pushbuttonValueMapEdit->setEnabled(true);
    }

    if( itemType::baseItem == newType )
    {
        ui->comboboxValueTable->setEnabled(false);
        ui->pushbuttonValueMapEdit->setEnabled(false);
    }
    else
    {
        ui->comboboxValueTable->setEnabled(true);
        ui->pushbuttonValueMapEdit->setEnabled(true);
    }

}

void CItemCfgDialog::replaceWidget(QWidget *srcWidget, QWidget *destWidget)
{
    if( !destWidget->isHidden() )
        return;

    destWidget->setHidden(false);
    destWidget->setGeometry(srcWidget->geometry());
    ui->horlayoutValueMap->insertWidget(m_replaceIndex, destWidget, m_stretch);
    ui->horlayoutValueMap->removeWidget(srcWidget);
    srcWidget->setHidden(true);
}


