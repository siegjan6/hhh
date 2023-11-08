#include "DrawCurveWrapper.h"
#include "MDDrawCurve.h"
#include <QDebug>

//////////////////////////////////////////////////////////////////////////
CDrawCurveWrapper::CDrawCurveWrapper(void *object, const QString &name)
    :CMDDrawObjWrapper(object, name)
{
}

void CDrawCurveWrapper::setStartTime(const QString &curveName, const QVariant &startTime)
{
    CMDDrawCurve *obj = (CMDDrawCurve*)(MDObject());
    obj->setStartTime(curveName,startTime);
}

void CDrawCurveWrapper::setReferenceCurveData(const QString &curveName, const QVariant &xValue, const QVariant &yValue)
{
    CMDDrawCurve *obj = (CMDDrawCurve*)(MDObject());
    obj->setReferenceCurveData(curveName,xValue,yValue);
}
//////////////////////////////////////////////////////////////////////////


CDrawHisCurveWrapper::CDrawHisCurveWrapper(void *object, const QString &name)
    :CDrawCurveWrapper(object, name)
{
}

void CDrawHisCurveWrapper::query()
{
    CMDDrawHistory *obj = (CMDDrawHistory*)(MDObject());
    obj->query();
}

void CDrawHisCurveWrapper::setDataSourceParameter(int type, const QString &para)
{
    CMDDrawHistory *obj = (CMDDrawHistory*)(MDObject());
    obj->setDataSourceParameter(type,para);
}

void CDrawHisCurveWrapper::setFilter(const QString &args)
{
     CMDDrawHistory *obj = (CMDDrawHistory*)(MDObject());
     obj->setFilter(args);
}

void CDrawHisCurveWrapper::setTagFilter(const QString &args)
{
     CMDDrawHistory *obj = (CMDDrawHistory*)(MDObject());
     obj->setTagFilter(args);
}

//////////////////////////////////////////////////////////////////////////
CDrawRealCurveWrapper::CDrawRealCurveWrapper(void *object, const QString &name)
    :CDrawCurveWrapper(object, name)
{
}


