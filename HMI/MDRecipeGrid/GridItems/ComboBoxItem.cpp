#include "ComboBoxItem.h"
#include "ShareTool.h"
#include <QApplication>
#include "RecipeGridModel.h"
#include "RecipeGridDelegate.h"
#include "GridComboBox.h"

CComboBoxItem::CComboBoxItem()
{
}

QWidget *CComboBoxItem::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const CRecipeGridDelegate *delegate)
{
    Q_UNUSED(option);

    CGridComboBox* comboBox = new CGridComboBox(this, parent);
    //! 加入一个空的item，如果数据未匹配则选中这个空的item
    comboBox->addItem("", -1);
    qint32 cnter = 0;
    foreach(const CValueMapManager::ValuePair& pair, m_values)
    {
        comboBox->addItem(pair.second, cnter++);
    }


    comboBox->setEnabled(isEditable());

    void (CGridComboBox:: *signal1)(int) = &CGridComboBox::currentIndexChanged;
    QObject::connect(comboBox, signal1, delegate, &CRecipeGridDelegate::commitDataByComboBox);

    return comboBox;
}

void CComboBoxItem::onInitialization(IDrawEnvrionment *environment)
{
    CGridBaseItem::onInitialization(environment);
    if( !CShareTool::m_isStudioMode )
    {
        setData("", Qt::DisplayRole);
        setData("", Qt::EditRole);
    } 
}

CComboBoxItem *CComboBoxItem::clone() const
{
    return new CComboBoxItem(*this);
}

bool CComboBoxItem::needCustomSetModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)
{
    CGridComboBox* combobox = qobject_cast<CGridComboBox*>(editor);
    if( !combobox )
        return false;

    qint32 valueindex = combobox->currentData().toInt();
    if( -1 != valueindex && valueindex < m_values.count() )
        model->setData(index, m_values.at(valueindex).first, Qt::EditRole);
    else
        model->setData(index, valueindex, Qt::EditRole);

    //! getCellText脚本函数会用到
    model->setData(index, combobox->currentText(), Qt::DisplayRole);

    return true;
}

void CComboBoxItem::updateValidUIState()
{
    CRecipeGridModel* myModel = static_cast<CRecipeGridModel*>(model());

    if( !isValidInRuntime() )
    {
        QString show = m_varName + QObject::tr("(InValid)");
        setText(show);
        setForeground(QBrush(Qt::gray));
        setEditable(false);
        myModel->showEditorByItem(index(), false);
    }
    else
    {
        setForeground(QBrush(Qt::black));
        setEditable(m_editable);
        myModel->showEditorByItem(index(), true);
    }
}

bool CComboBoxItem::updateShowString(QVariant value, QWidget *editor)
{
    CGridComboBox* combobox = qobject_cast<CGridComboBox*>(editor);
    if( !combobox )
        return false;

    qint32 index = indexOfValue(value);

    index += 1;
    if( index>= 0 && index < combobox->count() )
        combobox->setCurrentIndex(index);

    return false;
}

