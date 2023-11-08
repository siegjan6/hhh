//by aGRw 2015.08.19
#ifndef MDDRAWCURVEWRAPPER_H
#define MDDRAWCURVEWRAPPER_H

#include "MDDrawObjWrapper.h"

class CMDDrawHistory;
class CMDDrawCurve;

class CDrawCurveWrapper : public CMDDrawObjWrapper
{
    Q_OBJECT
public:
    CDrawCurveWrapper(void* object, const QString& name);
    virtual ~CDrawCurveWrapper(){}
//////////////////////////////////////////////////////////////////////////
public Q_SLOTS:
    void setStartTime(const QString &curveName,const QVariant &startTime);
    void setReferenceCurveData(const QString &curveName, const QVariant &xValue, const QVariant &yValue);

};

class CDrawHisCurveWrapper : public CDrawCurveWrapper
{
    Q_OBJECT
public:
    CDrawHisCurveWrapper(void* object, const QString& name);
    virtual ~CDrawHisCurveWrapper(){}

public Q_SLOTS:
    void setDataSourceParameter(int,const QString &);
    void setFilter(const QString& args);
    void setTagFilter(const QString& args);
    void query();

};

class CDrawRealCurveWrapper : public CDrawCurveWrapper
{
    Q_OBJECT
public:
    CDrawRealCurveWrapper(void* object, const QString& name);
    virtual ~CDrawRealCurveWrapper(){}
};


#endif // MDDRAWCURVEWRAPPER_H





