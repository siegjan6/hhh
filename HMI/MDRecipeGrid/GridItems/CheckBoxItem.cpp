#include "CheckBoxItem.h"
#include <QCheckBox>
#include "ShareTool.h"
#include "MDMultiLanguage.h"
#include "RecipeGridDelegate.h"
#include "GridCheckBox.h"

CCheckBoxItem::CCheckBoxItem()
{
}

QWidget *CCheckBoxItem::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const CRecipeGridDelegate *delegate)
{
    Q_UNUSED(option);

    CGridCheckBox* checkBox = new CGridCheckBox(this, parent);
    checkBox->setText(m_label);
    checkBox->setEnabled(isEditable());

    void (CGridCheckBox:: *signal)(int) = &CGridCheckBox::stateChanged;
    QObject::connect(checkBox, signal, delegate, &CRecipeGridDelegate::commitDataByCheckBox);

    return checkBox;
}

void CCheckBoxItem::onInitialization(IDrawEnvrionment *environment)
{
    CGridBaseItem::onInitialization(environment);
    if( !CShareTool::m_isStudioMode )
    {
        setData("", Qt::DisplayRole);
        setData("", Qt::EditRole);
    }

}

CCheckBoxItem *CCheckBoxItem::clone() const
{
    return new CCheckBoxItem(*this);
}

void CCheckBoxItem::serialize(QJsonObject &json, CRecipeGridModel *mainModel)
{
    CGridBaseItem::serialize(json, mainModel);

    json.insert("CheckBox_label", m_label);
}

void CCheckBoxItem::deserialize(const QJsonObject &json)
{
    CGridBaseItem::deserialize(json);
    m_label = json.value("CheckBox_label").toString();
}

void CCheckBoxItem::updateValidUIState()
{
    if( !isValidInRuntime() )
    {
        QString show = m_label + QObject::tr("(InValid)");
        setText(show);
        setForeground(QBrush(Qt::gray));
        setEditable(false);
    }
    else
    {
        setForeground(QBrush(Qt::black));
        setText(m_label);
        setEditable(m_editable);
    }
}

bool CCheckBoxItem::updateShowString(QVariant value, QWidget *editor)
{
    QCheckBox* checkbox = qobject_cast<QCheckBox*>(editor);
    if( !checkbox )
        return false;

    bool ok =false;
    double number = value.toDouble(&ok);
    if(ok)
        checkbox->setChecked(number ? true : false);


    return false;
}

