/*!

\author eHVr

\section 功能

    标签管理类，曲线左上角显示标签。内有一个标签链表，对标签进行管理,
    基本属性：
    \a height
    \a font;
    \a selectedColor
    \a lableVisible
    \a movingValueVisible
    \a valuePrecision
*/
#ifndef CLABEL_H
#define CLABEL_H

#include <QPainter>
#include "CCurveObject.h"
#include "CReferenceCurve.h"
#include <QObject>
#include "CLabelItem.h"
#include <QSet>

class CLabelManager
{
public:
    CLabelManager();
    ~CLabelManager();
/*!
\section 基本属性
*/
public:

    //! 区域高度(根据内部字体高度计算)
    double height() const;
    void setHeight(double height);

    QFont font() const;
    void setFont(const QFont &font);

    //! 选中色(未实现)
    QColor selectedColor() const;
    void setSelectedColor(const QColor &selectedColor);

    bool lableVisible() const;
    void setLableVisible(bool lableVisible);

    //!  是否显示鼠标位置的曲线值
    bool movingValueVisible() const;
    void setMovingValueVisible(bool movingValueVisible);

    //!  数值精度
    int valuePrecision() const;
    void setValuePrecision(int valuePrecision);
private:
    double  m_height;

    QFont   m_font;

    QColor  m_selectedColor ;
    bool    m_lableVisible ;
    bool    m_movingValueVisible;

public:
    void addCurve(CCurveObject *curve);
    void removeCurve(CCurveObject *curve);

public:
    QList<CCurveObject *> curves() const;
    void setCurves(const QList<CCurveObject *> &curves);
private:
    QList<CCurveObject *> m_curves;

public:
    void addReCurve(CReferenceCurve *reCurves);

public:
    QList<CReferenceCurve*>reCurves() const;
    void setReCurves(const QList<CReferenceCurve*> &reCurves);
private:
    QList<CReferenceCurve *> m_reCurves;

/*!
\section 环境
*/
public:
    bool isStudioMode() const;
    void setIsStudioMode(bool isStudioMode);
private:
    bool  m_isStudioMode;

/*!
\section 计算，绘制
*/
public:

    //! 绘制区域
    QRectF drawArea() const;
    void setDrawArea(const QRectF &drawArea);

    //! 鼠标点击点的X坐标
    double moursePressX() const;
    void setMoursePressX(double moursePressX);

    void calculate();
    void draw(QPainter *p);
private:
    double calculateTimeValue(CCurveObject *curve);
    double calculateDoubleValue(CCurveObject *curve);
private:
    QRectF  m_drawArea;
    double  m_moursePressX;

/*!
\section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

public:
    CLabelManager &operator = (const CLabelManager &other);
private:
    QList<CLabelItem>  m_itemList;     //! 曲线项链表
    bool               m_isCalculated; //! 是否经过计算

};

#endif // CLABEL_H
