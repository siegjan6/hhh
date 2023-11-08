#include "MDPropertyDrawRadioButton.h"
#include "MDDrawRadioButton.h"
#include "undo/DrawRadioButtonCommand.h"

/*!
\section 属性函数
*/
void CMDPropertyDrawRadioButton::setText(const QString &value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchText(value);
    else
        undo()->push(new DrawRadioButtonCommand(
                         widget(),widget()->text(),value,(int)DrawRadioButtonId::Text));

    setModify(true);
}

QString CMDPropertyDrawRadioButton::text()
{
    if(widget())
        return widget()->text();
    return QString();
}

//void CMDPropertyDrawRadioButton::setChecked(bool value)
//{
//    if(!widget())
//        return;

//    if(isBatch())
//        setBatchChecked(value);
//    else
//        undo()->push(new DrawRadioButtonCommand(
//                         widget(),widget()->checked(),value,(int)DrawRadioButtonId::Checked));

//    setModify(true);
//}

//bool CMDPropertyDrawRadioButton::checked()
//{
//    if(widget())
//        return widget()->checked();
//    return false;
//}

void CMDPropertyDrawRadioButton::setGroupId(int value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchGroupId(value);
    else
        widget()->setGroupId(value);

    setModify(true);
}

int CMDPropertyDrawRadioButton::groupId()
{
    if(widget())
        return widget()->groupId();
    return 0;
}

void CMDPropertyDrawRadioButton::setCheckValue(qreal value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchCheckValue(value);
    else
        widget()->setCheckValue(value);

    setModify(true);
}

qreal CMDPropertyDrawRadioButton::checkValue()
{
    return widget()->checkValue();
}

CMDPropertyDrawRadioButton::CMDPropertyDrawRadioButton(CMDDrawObj *parent):
    CMDPropertyDrawBasicWidget(parent)
{
    insertTranslateMap("checked",tr("选中状态"));
    insertTranslateMap("groupId",tr("分组"));
    insertTranslateMap("checkValue",tr("选中值"));
    insertTranslateMap("text",tr("文本"));
}

CMDDrawRadioButton *CMDPropertyDrawRadioButton::widget()
{
    return dynamic_cast<CMDDrawRadioButton*>(m_object);
}

/*!
\section 设置属性值
*/
void CMDPropertyDrawRadioButton::resetAttribute()
{
    CMDPropertyDrawObj::resetAttribute();
}

//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawRadioButton::setBatchText(const QString &value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawRadioButton* v = dynamic_cast<CMDDrawRadioButton*>(o);
        undo()->push(new DrawRadioButtonCommand(
                         v,v->text(),value,(int)DrawRadioButtonId::Text));
    }

    undo()->endMacro();
}

//void CMDPropertyDrawRadioButton::setBatchChecked(bool value)
//{
//    undo()->beginMacro("");

//    foreach(CMDDrawObj* o, *batchObjs())
//    {
//        CMDDrawRadioButton* v = dynamic_cast<CMDDrawRadioButton*>(o);
//        undo()->push(new DrawRadioButtonCommand(
//                        v,v->checked(),value,(int)DrawRadioButtonId::Checked));
//    }

//    undo()->endMacro();
//}

void CMDPropertyDrawRadioButton::setBatchGroupId(int value)
{
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawRadioButton* v = dynamic_cast<CMDDrawRadioButton*>(o);
        v->setGroupId(value);
    }
}

void CMDPropertyDrawRadioButton::setBatchCheckValue(qreal value)
{
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawRadioButton* v = dynamic_cast<CMDDrawRadioButton*>(o);
        v->setCheckValue(value);
    }
}
//////////////////////////////////////////////////////////////////////////
