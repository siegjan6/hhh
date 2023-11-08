/*!

\author eHVr

\section 功能

*/
#ifndef CXAXISMANAGER_H
#define CXAXISMANAGER_H
#include <QList>
#include <QSizeF>
#include "CXAxis.h"
#include "CDrawMode.h"


/*!
 \brief CXAxisManager
    x轴管理类
*/
class CXAxisManager
{
public:
    CXAxisManager();
    ~CXAxisManager();

/*!
\section 基本属性
*/
public:
    QList<CXAxis *> &list() ;
    void setList(const QList<CXAxis *> &list);

    CDrawMode::Orientation orientation() const;
    void setOrientation(const CDrawMode::Orientation &orientation);

    double startScale() const;
    double endScale() const;
    void setBeginEnd(double startScale,double endScale);

     //! 自动隐藏轴(无曲线绑定,或绑定曲线不可见)
    bool autoHideCurve() const;
    void setAutoHideCurve(bool autoHideCurve);
    void setXBoundVisible(QList<bool>boundVisibleList);
    QList<bool>xBoundVisble();
private:
    QList<CXAxis *> m_list;               //! 轴链表
    CDrawMode::Orientation m_orientation; //! 绘制方向
    double          m_startScale;
    double          m_endScale;
    bool            m_autoHideCurve;
    QList<bool>     m_xBoundVisbleList;
/*!
\section 工具函数
*/
public:
    double height();    //! 总高度
    void setEndTime(const QDateTime &time, bool isStudioMode);
    void appendAxis(CXAxis *x);
    CXAxis *findAxis(const QString &name);
    void setOrientationOnRun();
/*!
\section 内部工具函数
*/
private:
    void clearItemList();
    void setXOrientation(CXAxis *, bool isStudioMode = true);
/*!
\section 计算，绘制
*/
public:
    QRectF drawArea() const;
    void setDrawArea(const QRectF &drawArea);

    int divNum() const;
    void setDivNum(int divNum);

    void draw(QPainter *p);
    void calculate();
private:
    QRectF  m_drawArea;//! 绘制区域
    qint32  m_divNum;  //! 主等分线数
/*!
\section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

public:
    CXAxisManager &operator =(const CXAxisManager &other);
};

#endif // CXAXISMANAGER_H
