/*!
\author eHVr

\section 功能

游标。可 固定 或 浮动 显示曲线的当前值，伴有和曲线颜色一致的色块。
*/
#ifndef CVERNIER_H
#define CVERNIER_H

#include <QWidget>
#include <QGuiApplication>
#include "CCurveObject.h"
#include "CReferenceCurve.h"

class CLabelItem;
class CVernier
{
public:
    CVernier();
    ~CVernier();

/*!
\section 基本属性
*/
public:
    bool display() const;
    void setDisplay(bool display);

    QFont font() const;
    void setFont(const QFont &font);

    QString timeFormat() const;
    void setTimeFormat(const QString &timeFormat);
private:
    bool    m_display;
    QFont   m_font;
    QString m_timeFormat;
/*!
\section 固定设置
*/
public:
    bool isFixed() const;
    void setIsFixed(bool isFixed);

    //! 固定显示点的位置坐标
    QPoint fixedPoint() const;
    void setFixedPoint(const QPoint &fixedPoint);
private:
    bool     m_isFixed;
    QPoint   m_fixedPoint;

/*!
\section 布局
*/
public:
    //! 各项间距
    int interval() const;
    void setInterval(int interval);

    //! 是否为竖排
    bool isVertical() const;
    void setIsVertical(bool isVertical);
private:
    qint32  m_interval;
    bool    m_isVertical;

/*!
\section 超时
*/
public:
    //! 超时长度
    int timeLength() const;
    void setTimeLength(int timeLength);

    QString customString() const;
    void setCustomString(const QString &customString);

    //! 就近原则
    bool isChoseNearer() const;
    void setIsChoseNearer(bool isChoseNearer);
private:
    qint32  m_timeLength ;   //! 当前点与最近点之间的时间间隔
    bool    m_isChoseNearer; //! 是否显示最近的数据点
    QString m_customString ; //! 自定义内容

public:
    //! 鼠标当前点的X坐标
    double moursePressX() const;
    void setMoursePressX(double moursePressX);

    //! 是否为编辑版
    bool isStudioMode() const;
    void setIsStudioMode(bool isStudioMode);

    QList<CCurveObject *> list() const;
    void setList(const QList<CCurveObject *> &list);

    QList<CReferenceCurve*> referenceList() const;
    void setReferenceList(const QList<CReferenceCurve *> &reList);

    QList<CLabelItem *> labelList() const;
    void setLabelList(const QList<CLabelItem *> &labelList);
private:
    double          m_moursePressX ;
    bool            m_isStudioMode ;

    QList<CCurveObject *>    m_list;
    QList<CReferenceCurve *> m_referenceList;
    QList<CLabelItem *>      m_labelList;


/*!
\section  计算
*/
public:
    QSizeF widgetSize() const;
    void setWidgetSize(const QSizeF &widgetSize);

    void calculate();
private:
    double calculateTimeValue(double xVisble, CCurveObject *curve, int &timeFlag, QString &timeValue);
    double calculateDoubleValue(double xVisble, CCurveObject *curve);

    double calculateTimeValue(double xVisble, CReferenceCurve *reCurve, int &timeFlag, QString &timeValue);
    double calculateDoubleValue(double xVisble,CReferenceCurve *reCurve);
private:
     QSizeF  m_widgetSize;//! 窗体尺寸

private:
     void clearItemList();//! 清空链表
/*!
\section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
private:
    bool     m_isCalculated;
public:
     CVernier &operator =(const CVernier &other);
};

#endif // CFLOATTAG_H
