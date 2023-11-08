/*!
\author eHVr

\section 功能

Y轴管理类
*/
#ifndef CYAXISMANAGER_H
#define CYAXISMANAGER_H

#include <QList>
#include <QRectF>

#include "CYAxis.h"

class CYAxisManager
{
public:
    CYAxisManager();
    ~CYAxisManager();
/*!
\section    基本属性
*/
public:
    //! Y轴链表
    QList<CYAxis *> &list() ;
    void setList(const QList<CYAxis *> &list);

    //! 初始化比例
    int scale() const;
    void setScale(int scale);

     //! 自动隐藏轴(无曲线绑定,或绑定曲线不可见)
    bool autoHideCurve() const;
    void setAutoHideCurve(bool autoHideCurve);

    void setYBoundVisible(QList<bool>boundVisibleList);
    QList<bool>yBoundVisble();
private:
    QList<CYAxis *> m_list;               //! Y轴链表
    qint32          m_scale;              //! 初始化比例
    bool            m_autoHideCurve;      //! 自动隐藏
    QList<bool>     m_yBoundVisbleList;   //!是否绑定曲线以及曲线是否可见
/*!
\section
*/
public:
    void appendCurve(CYAxis *y);
    //! 根据轴线名称，查找对应的轴。
    CYAxis *findAxis(const QString &name);
/*!
\section 计算，绘制
*/
public:
    //! 左侧,右侧 y轴区域
    void setRect(const QRectF& left,const QRectF &right);

    //! 获取一侧的Y轴总宽度
    double width(bool isLeft);

    //!  标签栏高度
    double labelHeight() const;
    void setLabelHeight(double labelHeight);

    //! 等分线数
    int divNum() const;
    void setDivNum(int divNum);

    void calculate();
    void draw(QPainter *p);
private:
    QRectF   m_leftRect;       //! 左侧Y轴区域
    QRectF   m_rightRect;      //! 右侧Y轴区域
    double   m_labelHeight;    //! 标签高度
    qint32   m_divNum;         //! 等分线数
/*!
\section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

public:
    CYAxisManager &operator = (const CYAxisManager &other);
private:
    void clearItemList();      //! 清空链表
};

#endif // CYAXISMANAGER_H
