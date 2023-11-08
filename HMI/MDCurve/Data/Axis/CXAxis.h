/*!

\author eHVr

\section 功能
    X轴，又分为 数值轴和时间轴
\section 公共部分
    \a valueType
    \a valueLength
    \a totalLength
    \a startScale
    \a endScale
    \a unit
    \a drawArea
    \a divNum
    \a isCurveMoving
    \a xHeight

\section 时间轴
    \a endTime
    \a timeMark
    \a originalEndTime
    \a timeFormat
\section 数值轴
    \a  maxValue
    \a  minValue
    \a  originMax
    \a  originMin

    建议重构，将数值轴和时间轴分开处理
*/
#ifndef CXAXIS_H
#define CXAXIS_H

#include "CAbstractAxis.h"
#include "CDrawMode.h"

#include <QDateTime>
#include <QObject>
#include <QTimer>
#include <QPainter>

class CXAxis : public CAbstractAxis
{
public:
    CXAxis();
    virtual ~CXAxis();
/*!
\section 公共部分
*/
public:
    //! X轴值类型
    enum class ValueType{
        Time = 0,
        Value
    };

    ValueType valueType() const;
    void setValueType(const ValueType &valueType);
public:
    ValueType m_valueType;

public:
    //! 值长度
    qint64 valueLength() const;

    //! 数值总长度
    qint64 totalLength() const;
    void setTotalLength(const qint64 &totalLength);

    void setTotalLength(const int day, const int hour, const int minute, const int sec){
        qint64 time = day *24*60*60+hour*60*60+minute*60+sec;
        setTotalLength(time);
    }

    //! 单位
    QString unit() const;
    void setUnit(const QString &unit);
private:
    qint64    m_totalLength; //! 总长度
    QString   m_unit;

public:
    //! 绘制区域
    QRectF drawArea() const;
    void setDrawArea(const QRectF &drawArea);

    //! 等分线数
    int divNum() const;
    void setDivNum(int divNum);

    //! X轴高度
    double xHeight() const;
    void setXHeight(double xHeight);
private:
    QRectF          m_drawArea;
    qint32          m_divNum;
    double          m_xHeight;

/*!
\section 时间相关
*/
public:
    QString timeFormat() const;
    void setTimeFormat(const QString &timeFormat);

    QDateTime endTime() const;
    void setEndTime(const QDateTime &maxValue );

    //!  时间戳
    QDateTime timeMark() const;
    void setTimeMark(const QDateTime &timeMark);

    void setFinalEnd(const QDateTime &xEndTime, bool isStudioMode = true);
private:
    QDateTime   m_timeMark;         //! 时间戳
    QDateTime   m_endTime;
    QDateTime   m_originalEndTime;  //! 原始结束时间
    QString     m_timeFormat;       //! 时间格式
public:
    void setOriginalEndTime(const QDateTime &dateTime);
    QDateTime originEndTime() const;

public:
    void setOriginalLength(const int &length);
    int originLength() const;
private:
    int m_originalLength;


/*!
\section 数值相关
*/
public:
    //! 最大值，最小值
    double maxValue() const;
    void setMaxValue(double maxValue);

    double minValue() const;
    void setMinValue(double minValue);

    //! 原始最大值，最小值
    double originMax() const;
    void setOriginMax(double originMax);

    double originMin() const;
    void setOriginMin(double originMin);
private:
    double          m_maxValue ;
    double          m_minValue ;
    double          m_originMax ;
    double          m_originMin;

/*!
\section 计算，绘制
*/
public:
    void calculate();
    void draw(QPainter *p);
    void drawAxisLine(QPainter *);
private:
    void calculateTime();
    void calculateValue();
private:
    QList<double>   m_textXPointList;    //! 绘制的X位置链表
    QStringList     m_valueStringList;   //! 数值的字符链表
/*!
\section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

public:
    CXAxis &operator =(const CXAxis &other);
};

#endif // CXAXIS_H
