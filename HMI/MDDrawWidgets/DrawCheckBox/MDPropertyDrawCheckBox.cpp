#include "MDPropertyDrawCheckBox.h"
#include "MDDrawCheckBox.h"
#include "undo/DrawCheckBoxCommand.h"
/*!
\section 属性函数
*/
void CMDPropertyDrawCheckBox::setText(const QString &value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchText(value);
    else
        undo()->push(new DrawCheckBoxCommand(
                         widget(),widget()->text(),value,(int)DrawCheckBoxId::Text));

    setModify(true);
}

QString CMDPropertyDrawCheckBox::text()
{
    if(widget())
        return widget()->text();
    return QString();
}

void CMDPropertyDrawCheckBox::setChecked(bool value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchChecked(value);
    else
        undo()->push(new DrawCheckBoxCommand(
                         widget(),widget()->checked(),value,(int)DrawCheckBoxId::Checked));

    setModify(true);
}

bool CMDPropertyDrawCheckBox::checked()
{
    if(widget())
        return widget()->checked();
    return false;
}

CMDPropertyDrawCheckBox::CMDPropertyDrawCheckBox(CMDDrawObj *parent):
    CMDPropertyDrawBasicWidget(parent)
{
    insertTranslateMap("checked",tr("选中状态"));
    insertTranslateMap("text",tr("文本"));
}

CMDDrawCheckBox *CMDPropertyDrawCheckBox::widget()
{
    return dynamic_cast<CMDDrawCheckBox*>(m_object);
}

/*!
\section 设置属性值
*/
void CMDPropertyDrawCheckBox::resetAttribute()
{
    CMDPropertyDrawObj::resetAttribute();
}

//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawCheckBox::setBatchText(const QString &value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawCheckBox* v = dynamic_cast<CMDDrawCheckBox*>(o);
        undo()->push(new DrawCheckBoxCommand(
                         v,v->text(),value,(int)DrawCheckBoxId::Text));
        v->setText(value);
    }

    undo()->endMacro();
}

void CMDPropertyDrawCheckBox::setBatchChecked(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawCheckBox* v = dynamic_cast<CMDDrawCheckBox*>(o);
        undo()->push(new DrawCheckBoxCommand(
                         v,v->checked(),value,(int)DrawCheckBoxId::Checked));

    }

    undo()->endMacro();
}
//////////////////////////////////////////////////////////////////////////
