#ifndef DRAWCURVEDATA_H
#define DRAWCURVEDATA_H

#include "../include/MDCurve/MDCurve.h"
//#include "MDCurve\MDCurve.h"

//曲线控件,数据类
class CDrawCurveData
{
public:
    CDrawCurveData();
    ~CDrawCurveData(){}
public:
    void setIsRealTime(bool isRealTime = true);
    void setDataSourceParameter(int type, const QString &parameter); //! 设置数据库参数
    void setFilter(const QString &filter);
    void setTagFilter(const QString &filter);
    void query();
public:
    CMDCurve m_curve;
//////////////////////////////////////////////////////////////////////////
//common
public:
    CDrawCurveData& operator=(const CDrawCurveData& other);

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
public:
    void changeLanguage();
    void collectWords();
};

#endif // DRAWCURVEDATA_H
