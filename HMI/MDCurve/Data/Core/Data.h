/*!
\author eHVr

\section 功能

原始数据，转换后数据相关类，包含原始数据点，转换后数据点
原始数据点管理类，转换后数据点管理类等
*/
#ifndef DATA_H
#define DATA_H

#include <QDateTime>
#include <QList>
#include <QFont>
#include <QColor>
#include <QPointF>

enum class XAxisValueType
{
    Double,
    Time,
    None
};

/*!
 \section CBasePoint
    数据点抽象基类
*/
class CBasePoint
{
public:
    CBasePoint(){}
    virtual ~CBasePoint(){}

    virtual double xDouble() const {return 0;}
    virtual void setXDouble(double value)   {Q_UNUSED(value)}

    virtual QDateTime xTime()const  {return QDateTime();}
    virtual void setXTime(const QDateTime &xValue)  {Q_UNUSED(xValue)}

    virtual double y() const {return 0;}
    virtual void setY(double){}
};


/*!
 \section CTimeValuePoint
    时间值数据点
*/
class CTimeValuePoint:public CBasePoint
{
public:
    CTimeValuePoint():m_value(0){}
    virtual ~CTimeValuePoint(){}

    QDateTime xTime() const{return m_time;}
    void setXTime(const QDateTime &xValue){m_time = xValue;}

    virtual double y() const{return m_value;}
    virtual void setY(double yValue){m_value = yValue;}
public:
    inline bool operator ==(const CTimeValuePoint& v1)
    { return {(xTime() == v1.xTime()) && (y() == v1.y())};}

    inline bool operator !=(const CTimeValuePoint& v1)
    { return {(xTime() != v1.xTime()) || (y() != v1.y())};}

private:
    QDateTime m_time;   //! 时间
    double    m_value;
};

/*!
 \section CPureValuePoint
    X轴数据为double
*/
class CPureValuePoint:public CBasePoint
{
public:
    CPureValuePoint (){}
    virtual ~CPureValuePoint(){}
public:
    virtual double xDouble() const {return m_pointValue.x();}
    virtual void setXDouble(double value){m_pointValue.setX(value);}

    virtual double y() const{return m_pointValue.y();}
    virtual void setY(double yValue){m_pointValue.setY(yValue);}
private:
    QPointF m_pointValue; //! 纯数据值
};

/*!
 \section CCurveData
    曲线数据
*/
class CCurveData
{
public:
    CCurveData():m_valueType(XAxisValueType::None){}
    ~CCurveData(){}

    QList<CPureValuePoint> &valueList();
    void setValueList(const QList<CPureValuePoint> &valueList);

    XAxisValueType valueType() const;
    void setValueType(XAxisValueType type);

    QString name() const;
    void setName(const QString &name);

    QList<CTimeValuePoint> &timeList();
    void setTimeList(const QList<CTimeValuePoint> &timeList);

    inline bool isEmpty()
    { return {m_timeList.isEmpty() && m_valueList.isEmpty()};}
private:

    QString                 m_name;
    QList<CPureValuePoint>  m_valueList;    //!曲线值
    QList<CTimeValuePoint>  m_timeList;
    XAxisValueType          m_valueType;
};
/*!
 \section CCurveDataManager
   曲线数据管理,曲线数据组
*/
typedef QList<CCurveData> CCurveDataManager ;

/*!
 \section CCurveVisible
    曲线屏幕坐标
*/
typedef QList<QPoint> CCurveVisible;
typedef QList<CCurveVisible> CVisibleDataManager;

#endif // DATA_H



