#include "MDPropertyDrawSpinBox.h"
#include "MDDrawSpinBox.h"
/*!
\section 属性函数
*/
void CMDPropertyDrawSpinBox::setSingleStep(int value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchSingleStep(value);
    else
        widget()->setSingleStep(value);

    setModify(true);
}

int CMDPropertyDrawSpinBox::singleStep()
{
    if(widget())
        return widget()->singleStep();
    return 0;
}

void CMDPropertyDrawSpinBox::setMaximum(int value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchMaximum(value);
    else
        widget()->setMaximum(value);

    setModify(true);
}

int CMDPropertyDrawSpinBox::maximum()
{
    if(widget())
        return widget()->maximum();
    return 0;
}

void CMDPropertyDrawSpinBox::setMinimum(int value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchMinimum(value);
    else
        widget()->setMinimum(value);

    setModify(true);
}

int CMDPropertyDrawSpinBox::minimum()
{
    if(widget())
        return widget()->minimum();
    return 0;
}

void CMDPropertyDrawSpinBox::setInitialValue(int value)
{
    if(!widget())
        return;

    if(isBatch())
        setBatchInitialValue(value);
    else
        widget()->setInitialValue(value);

    setModify(true);
}

int CMDPropertyDrawSpinBox::initialValue()
{
    if(widget())
        return widget()->initialValue();
    return 10;
}

CMDPropertyDrawSpinBox::CMDPropertyDrawSpinBox(CMDDrawObj *parent):
    CMDPropertyDrawBasicWidget(parent)
{
    insertTranslateMap("singleStep",tr("单步值"));
    insertTranslateMap("maximum",tr("最大值"));
    insertTranslateMap("minimum",tr("最小值"));
    insertTranslateMap("initialValue",tr("初始值"));
}

CMDDrawSpinBox *CMDPropertyDrawSpinBox::widget()
{
    return dynamic_cast<CMDDrawSpinBox*>(m_object);
}
/*!
\section 设置属性值
*/
void CMDPropertyDrawSpinBox::resetAttribute()
{
    CMDPropertyDrawObj::resetAttribute();
}

//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawSpinBox::setBatchSingleStep(int value)
{
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawSpinBox* v = dynamic_cast<CMDDrawSpinBox*>(o);
        v->setSingleStep(value);
    }
}

void CMDPropertyDrawSpinBox::setBatchMaximum(int value)
{
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawSpinBox* v = dynamic_cast<CMDDrawSpinBox*>(o);
        v->setMaximum(value);
    }
}

void CMDPropertyDrawSpinBox::setBatchMinimum(int value)
{
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawSpinBox* v = dynamic_cast<CMDDrawSpinBox*>(o);
        v->setMinimum(value);
    }
}

void CMDPropertyDrawSpinBox::setBatchInitialValue(int value)
{
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawSpinBox* v = dynamic_cast<CMDDrawSpinBox*>(o);
        v->setInitialValue(value);
    }
}
//////////////////////////////////////////////////////////////////////////
