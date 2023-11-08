/*!

\author eHVr

\section 功能

    曲线绘制区域,x,y方向分别具有，主等分线，次等分线属性，基本属性，
    \a borderColor

*/
#ifndef CGRIDREGION_H
#define CGRIDREGION_H
#include <QColor>
#include <QPainter>

#include "CBisectrixLine.h"

class CGridRegion
{
public:
    CGridRegion();
    ~CGridRegion();

public:
    //! 边框色
    QColor borderColor() const;
    void setBorderColor(const QColor &borderColor);
    //! 曲线区域
    QRectF gridRegionArea() const;
    void setGridRegionArea(const QRectF &gridRegionArea);
private:
    QRectF  m_gridRegionArea;
    QColor  m_borderColor;
/*!
\section x 方向等分线
*/
public:
    int xMainBisectrixNum() const;
    void setXMainBisectrixNum(int xMainBisectrixNum);

    int xSecondaryBisectrixNum() const;
    void setXSecondaryBisectrixNum(int xSecondaryBisectrixNum);

    CBisectrixLine *xMainBisectrixLine();
    CBisectrixLine *xSecondaryBisectrixLine();
private:
    double          m_xMainBisectrixWidth;       //! 垂直方向主等分线间距
    double          m_xSecondaryBisectrixWidth;  //! 垂直方向次等分线间距
    qint32          m_xMainBisectrixNum;         //! 垂直方向主等分线个数
    qint32          m_xSecondaryBisectrixNum;    //! 垂直方向次等分线个数
    CBisectrixLine  *m_xMainBisectrixLine;       //! 垂直方向主等分线
    CBisectrixLine  *m_xSecondaryBisectrixLine;  //! 垂直方向次等分线
/*!
\section y 方向等分线
*/
public:
    int yMainBisectrixNum() const;
    void setYMainBisectrixNum(int yMainBisectrixNum);

    int ySecondaryBisectrixNum() const;
    void setYSecondaryBisectrixNum(int ySecondaryBisectrixNum);

    CBisectrixLine *yMainBisectrixLine();
    CBisectrixLine *ySecondaryBisectrixLine();
private:
    double          m_yMainBisectrixHeight;        //! 水平方向主等分线间距
    double          m_ySecondaryBisectrixHeight;   //! 水平方向次等分线间距
    qint32          m_yMainBisectrixNum;           //! 水平方向主等分线个数
    qint32          m_ySecondaryBisectrixNum;      //! 水平方向次等分线个数
    CBisectrixLine  *m_yMainBisectrixLine;         //! 水平方向主等分线
    CBisectrixLine  *m_ySecondaryBisectrixLine;    //! 水平方向次等分线

/*!
\section 计算，绘制
*/
public:
    void draw(QPainter*p);
    void calculate();
private:
    void drawXBisectrix(QPainter *p);
    void drawYBisectrix(QPainter *p);

public:
    CGridRegion &operator =(const CGridRegion &other);
/*!
\section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // CGRIDREGION_H
