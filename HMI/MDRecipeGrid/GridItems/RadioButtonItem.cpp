#include "RadioButtonItem.h"
#include <QApplication>
#include "RecipeGridDelegate.h"
#include "ShareTool.h"



CRadioButtonItem::CRadioButtonItem()
{
}

QWidget *CRadioButtonItem::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const CRecipeGridDelegate *delegate)
{
    Q_UNUSED(option);

    CGridRadioGroup *groupBox = new CGridRadioGroup(this, delegate, parent);
    groupBox->enableEdit(isEditable());


    return groupBox;
}

CRadioButtonItem *CRadioButtonItem::clone() const
{
    return new CRadioButtonItem(*this);
}

bool CRadioButtonItem::needCustomSetModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)
{
    qint32 selIndex = editor->property("index").toInt();

    QString desc = "";
    if( selIndex >= 0 && selIndex < m_values.size() )
        desc = m_values.at(selIndex).second;

    model->setData(index, selIndex, Qt::EditRole);
    //! 将当前选中的radiobutton的Title，也就是数值映射表对应的描述字符串给到Qt::DisplayRole,脚本getCellText会用到
    model->setData(index, desc, Qt::DisplayRole);
    return true;
}

void CRadioButtonItem::onInitialization(IDrawEnvrionment *environment)
{
    CGridBaseItem::onInitialization(environment);
    if( !CShareTool::m_isStudioMode )
    {
        setData("", Qt::DisplayRole);
        setData("", Qt::EditRole);
    }
}

void CRadioButtonItem::updateValidUIState()
{
    if( !isValidInRuntime() )
    {
        QString show = m_varName + QObject::tr("(InValid)");
        setText(show);
        setForeground(QBrush(Qt::gray));
        setEditable(false);
    }
    else
    {
        setForeground(QBrush(Qt::black));
        setEditable(m_editable);
    }
}

bool CRadioButtonItem::updateShowString(QVariant value, QWidget *editor)
{
    CGridRadioGroup* groupbox = qobject_cast<CGridRadioGroup*>(editor);
    if( !groupbox )
        return false;

    qint32 index = indexOfValue(value);
    groupbox->setCheckState(index);

    return false;
}

void CRadioButtonItem::enableEdit(bool edit, QWidget *editor)
{
    setEditable(edit);
    CGridRadioGroup* groupBox = qobject_cast<CGridRadioGroup*>(editor);
    if( groupBox )
        groupBox->enableEdit(edit);
}

QButtonGroup *CRadioButtonItem::buttonGroup(QGroupBox *groupbox)
{
    foreach(QObject* obj, groupbox->children())
    {
        QString className = obj->metaObject()->className();
        if( 0 == className.compare("QButtonGroup") )
            return qobject_cast<QButtonGroup*>(obj);
    }

    return NULL;
}

