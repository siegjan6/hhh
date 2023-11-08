#include <QListWidgetItem>
#include "MDPropertyDrawComboBox.h"
#include "MDDrawComboBox.h"
#include "DrawSubitemDialog.h"
#include "undo/DrawComboboxCommand.h"

/*!
\section 属性函数
*/
void CMDPropertyDrawComboBox::setDropDownStyle(propertyItemStyle value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchDropDownStyle(value);
    else
    {
        int factValue = 0;
        switch (value) {
        case propertyItemStyle::DropItem:
            factValue = 0;
            break;
        case propertyItemStyle::DropListItem:
            factValue = 1;
            break;
        default:
            break;
        }
        undo()->push(new DrawComboboxCommand(
                         widget(),widget()->itemStyle(),factValue,(int)DrawComboboxId::ItemStyle));
    }
    setModify(true);
}

CMDPropertyDrawComboBox::propertyItemStyle CMDPropertyDrawComboBox::dropDownStyle()
{
    if(widget())
    {
        int Itemstyle = widget()->itemStyle();
        return (propertyItemStyle)Itemstyle;
    }
    return propertyItemStyle::DropItem;
}

void CMDPropertyDrawComboBox::setDILItem(const QString &value)
{
    Q_UNUSED(value)


        CDrawSubitemDialog dialog(dynamic_cast<QWidget*>(QObject::parent()));
        dialog.setItems(widget()->itemAdd());
        if(dialog.exec() == QDialog::Accepted)
        {
            QStringList items = dialog.items().split(breakString(),QString::SkipEmptyParts);
            for(int i = 0; i < items.count();i++)
            {
                if(items.at(i) == breakString())
                    items.removeAt(i);
            }
            if(isBatch())
                setBatchDILItem(items);
            else
                widget()->setItemAdd(items);
        }

    setModify(true);
}

QString CMDPropertyDrawComboBox::DILitem()
{
    QString itemIn;
    if(widget()->itemAdd().count())
        itemIn.append(widget()->itemAdd().at(0));
    return itemIn;
}

void CMDPropertyDrawComboBox::setLayoutDirection(bool value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchLayoutDirection(value);
    else
        undo()->push(new DrawComboboxCommand(
                         widget(),widget()->layoutDirection(),value,(int)DrawComboboxId::LayoutDirection));

    setModify(true);
}

bool CMDPropertyDrawComboBox::layoutDirection()
{
    if(widget())
        return widget()->layoutDirection();
    return false;
}

void CMDPropertyDrawComboBox::setSorted(bool value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchSorted(value);
    else
        undo()->push(new DrawComboboxCommand(
                         widget(),widget()->Sorted(),value,(int)DrawComboboxId::Sorted));

    setModify(true);
}

bool CMDPropertyDrawComboBox::Sorted()
{
    if(widget())
        return widget()->Sorted();
    return false;
}

CMDPropertyDrawComboBox::CMDPropertyDrawComboBox(CMDDrawObj *parent):
    CMDPropertyDrawBasicWidget(parent)
{
    insertTranslateMap("layoutDirection",tr("滚动条居左"));
    insertTranslateMap("dropDownStyle",tr("列表类型"));
    insertTranslateMap("item",tr("子项"));
    insertTranslateMap("Sorted",tr("子项排序"));
    insertTranslateMap("DropItem",tr("下拉式"));
    insertTranslateMap("DropListItem",tr("下拉列表式"));
}

CMDDrawComboBox *CMDPropertyDrawComboBox::widget()
{
    return dynamic_cast<CMDDrawComboBox*>(m_object);
}

/*!
\section 设置属性值
*/
void CMDPropertyDrawComboBox::resetAttribute()
{
    CMDPropertyDrawObj::resetAttribute();
}


//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawComboBox::setBatchDropDownStyle(CMDPropertyDrawComboBox::propertyItemStyle value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawComboBox* v = dynamic_cast<CMDDrawComboBox*>(o);
        int factValue = 0;
        switch (value) {
        case propertyItemStyle::DropItem:
            factValue = 0;
            break;
        case propertyItemStyle::DropListItem:
            factValue = 1;
            break;
        default:
            break;
        }
        undo()->push(new DrawComboboxCommand(
                         v,v->itemStyle(),factValue,(int)DrawComboboxId::ItemStyle));
    }

    undo()->endMacro();
}

void CMDPropertyDrawComboBox::setBatchDILItem(QStringList list)
{
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawComboBox* v = dynamic_cast<CMDDrawComboBox*>(o);
        v->setItemAdd(list);
    }
}

void CMDPropertyDrawComboBox::setBatchLayoutDirection(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawComboBox* v = dynamic_cast<CMDDrawComboBox*>(o);
        undo()->push(new DrawComboboxCommand(
                         v,v->layoutDirection(),value,(int)DrawComboboxId::LayoutDirection));
    }

    undo()->endMacro();
}

void CMDPropertyDrawComboBox::setBatchSorted(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawComboBox* v = dynamic_cast<CMDDrawComboBox*>(o);
        undo()->push(new DrawComboboxCommand(
                         v,v->Sorted(),value,(int)DrawComboboxId::Sorted));
    }

    undo()->endMacro();
}
//////////////////////////////////////////////////////////////////////////
