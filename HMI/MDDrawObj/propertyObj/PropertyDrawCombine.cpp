#include "PropertyDrawCombine.h"
//////////////////////////////////////////////////////////////////////////
CPropertyDrawCombine::CPropertyDrawCombine(CMDDrawObj *parent) :
    CMDPropertyDrawVector(parent)
{
    insertTranslateMap("connectLine", tr("线连接"));
    insertTranslateMap("fillMode", tr("填充模式"));
    insertTranslateMap("OddEvenFill", tr("奇偶填充"));
    insertTranslateMap("WindingFill", tr("完整填充"));
}
//////////////////////////////////////////////////////////////////////////
void CPropertyDrawCombine::setConnectLine(bool value)
{
    if(isBatch())
        setBatchConnectLine(value);
    else
        combine()->setConnectLine(value);
    setModify(true);
}
bool CPropertyDrawCombine::connectLine()
{
    return combine()->connectLine();
}
void CPropertyDrawCombine::setFillMode(Qt::FillRule value)
{
    if(isBatch())
        setBatchFillMode(value);
    else
        combine()->setFillMode(value);
    setModify(true);
}
Qt::FillRule CPropertyDrawCombine::fillMode()
{
    return combine()->fillMode();
}
//////////////////////////////////////////////////////////////////////////
CMDDrawCombine *CPropertyDrawCombine::combine()
{
    return dynamic_cast<CMDDrawCombine*>(m_object);
}
//////////////////////////////////////////////////////////////////////////
void CPropertyDrawCombine::setBatchConnectLine(bool value)
{
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawCombine* c = dynamic_cast<CMDDrawCombine*>(o);
        c->setConnectLine(value);
    }
}
void CPropertyDrawCombine::setBatchFillMode(Qt::FillRule value)
{
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawCombine* c = dynamic_cast<CMDDrawCombine*>(o);
        c->setFillMode(value);
    }
}
//////////////////////////////////////////////////////////////////////////





