#include "MDPropertyDrawBasicArc.h"
#include "DrawBasicArcCommand.h"

//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawBasicArc::CMDPropertyDrawBasicArc(CMDDrawObj *parent) :
    CMDPropertyDrawVector(parent)
{
    insertTranslateMap("startAngle", tr("开始角度"));
    insertTranslateMap("spanAngle", tr("量程"));
}
//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawBasicArc::setStartAngle(qreal value)
{
    if(isBatch())
        setBatchStartAngle(value);
    else
        undo()->push(new CDrawBasicArcCommand(
            object(), object()->startAngle(), value, (int)DrawBasicArcId::StartAngle));
    setModify(true);
}
qreal CMDPropertyDrawBasicArc::startAngle()
{
    return object()->startAngle();
}
void CMDPropertyDrawBasicArc::setSpanAngle(qreal value)
{
    if(isBatch())
        setBatchSpanAngle(value);
    else
        undo()->push(new CDrawBasicArcCommand(
            object(), object()->spanAngle(), value, (int)DrawBasicArcId::SpanAngle));
    setModify(true);
}
qreal CMDPropertyDrawBasicArc::spanAngle()
{
    return object()->spanAngle();
}
//////////////////////////////////////////////////////////////////////////
CMDDrawBasicArc *CMDPropertyDrawBasicArc::object()
{
    return dynamic_cast<CMDDrawBasicArc*>(m_object);
}
//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawBasicArc::setBatchStartAngle(qreal value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawBasicArc* v = dynamic_cast<CMDDrawBasicArc*>(o);
        undo()->push(new CDrawBasicArcCommand(
            v, v->startAngle(), value, (int)DrawBasicArcId::StartAngle));
    }

    undo()->endMacro();
}
void CMDPropertyDrawBasicArc::setBatchSpanAngle(qreal value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawBasicArc* v = dynamic_cast<CMDDrawBasicArc*>(o);
        undo()->push(new CDrawBasicArcCommand(
            v, v->spanAngle(), value, (int)DrawBasicArcId::SpanAngle));
    }

    undo()->endMacro();
}
//////////////////////////////////////////////////////////////////////////
