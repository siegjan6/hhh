#ifndef CREFERENCECURVE_H
#define CREFERENCECURVE_H

#include <QObject>
#include <QPoint>
#include <QRectF>
#include <QColor>
#include <QPainter>
#include <QVariant>
#include "Data.h"
#include "CFind.h"

class CXAxis;
class CYAxis;
class CPoint;

class CReferenceCurve
{
public:
    CReferenceCurve();
    ~CReferenceCurve();

    /*!
    \section 绑定的坐标轴
    */
public:
    CXAxis *xAxis() const;
    void setXAxis(CXAxis *xAxis);

    QString xName() const;
    void setXName(const QString &xName);

    CYAxis *yAxis();
    void setYAxis(CYAxis *yAxis);

    QString yName() const;
    void setYName(const QString &yName);
private:
    CXAxis  *m_xAxis ; //! 绑定的X轴
    CYAxis  *m_yAxis ; //! 绑定的Y轴
    QString  m_xName;
    QString  m_yName;

    /*!
    \section 数据
    */
public:
    CCurveData &rawData();
    void setRawData(const CCurveData &rawData);

    CCurveVisible &totalVisblePoint();


private:
    CCurveData          m_rawData;
    CCurveVisible       m_visiblePoint;//! 可视数据
    CCurveVisible       m_totalVPoint;
public:
    void clearDataForZoom();

    /*!
         \section 基准曲线属性:
              线宽,线色,线型,是否显示,名称,描述,绘制区域
    */
public:
    QString name() const;
    void setName(const QString &name);

    QString comment() const;
    void setComment(const QString &comment);

    QString variantDepict() const;
    void setVariantDepict(const QString &variantDepict);

    int curveWidth() const;
    void setCurveWidth(int curveWidth);

    QColor reCurveColor() const;
    void setReCurveColor(const QColor &curveColor);

    void  setCurveStyle (int style);
    qint32 curveStyle() const;

    QRectF curveArea() const;
    void setCurveArea(const QRectF &curveArea);

    bool curveVisible() const;
    void setCurveVisible(bool curveVisible);

    bool reCurveVernierVisible() const;
    void setReCurveVernierVisible(bool vernierVisible);

private:
    QString             m_reName;          //! 名称
    QString             m_reComment;       //! 说明
    QColor              m_reCurveColor;    //! 颜色
    QString             m_variantDepict;   //! 变量说明
    qint32              m_reCurveWidth;    //! 曲线线宽
    qint32              m_reCurveStyle;    //! 线型
    QRectF              m_reCurveArea;     //! 曲线区域

    bool                m_reCurveVisible;  //! 是否显示
    CPoint              *m_normalPoint;
    bool                m_isCalculated;

    QDateTime           m_preEndTime;
    QPoint              m_lastPoint;
    bool                m_reVernierVisible;

public:
    CPoint *normalPoint();
    /*!
    \section 计算,绘制
    */
public:
    void draw(QPainter *p);
    void calculate();
private:
    void calculatePoint(const QPointF &p); //! 计算数据点
    bool calculateCheck();                 //! 计算前数据检测
    void calVisiblePoint();
    void clearOldData();
    void calculateTimeType();
    void calculateValueType();

    /*!
    \section 序列化和反序列化
    */
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

    CReferenceCurve & operator =(const CReferenceCurve &other);
    /*!
    \section 基准曲线起始参考值
    */
public:
    void setStartValue(const QVariant &value);
    QVariant startValue() const;
public:
    void setStartTime(const QVariant &startTime);
    QVariant startTime() const;
private:
    QVariant m_startValue;                      //! 设置起始数值
    QVariant m_startTime;                       //! 设置起始时间

};

#endif // CREFERENCECURVE_H
