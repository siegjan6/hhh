/*!
\author eHVr

\section 功能

Y轴，现在仅能够显示数值。
*/
#ifndef CYAXIS_H
#define CYAXIS_H
#include "CAbstractAxis.h"
#include <QPainter>

class CYAxis : public CAbstractAxis
{
public:
    CYAxis();
    virtual ~CYAxis();
/*!
\section 基本属性
*/
public:

    //! y轴宽度设置
    double yWidth() const;
    void setYWidth(double yWidth);

    //! 是否在左侧
    bool isLeft() const;
    void setIsLeft(bool isLeft);
private:
    double m_yWidth;
    bool   m_isLeft ;
/*!
\section 数值
*/
public:
    //! 是否为数值型
    bool isNumeric() const;
    void setIsNumeric(bool isNumeric);

    double maxValue() const;
    void setMaxValue(double maxValue);

    double minValue() const;
    void setMinValue(double minValue);

    //! 工程百分比
    void setMaxPercent(const double &maxPercent);
    double maxPercent() const;

    void setMinPercent(const double &minPercent);
    double minPercent() const;

    //! 原始最大值，最小值
    double originMax() const;
    void setOriginMax(double originMax);

    double originMin() const;
    void setOriginMin(double originMin);

private:
    double          m_maxValue ;
    double          m_minValue ;
    bool            m_isNumeric;
    //
    double          m_maxPercent;
    double          m_minPercent;
    double          m_originMax ;
    double          m_originMin;
/*!
\section 计算，绘制
*/
public:

    //!绘制区域大小
    QRectF drawArea() const;
    void setDrawArea(const QRectF &drawArea);

    void draw(QPainter *p);
    void drawAxisLine(QPainter *p);

    /*!
     \brief calculate
        计算参数,LabelHeight为标签高度,由Manager获取
     \param lebelHeight
    */
    void calculate(double labelHeight, int divNum);
    void calculateCommentRect();
private:
    QRectF          m_drawArea;         //! 绘制区域
    QStringList     m_valueTextList;    //! 字符数据,绘制位置
    QList<QRectF>   m_valueRectF;
    double          m_axisLineLength;   //! 轴线长
    QRectF          m_commentRectF;     //! 说明文本的绘制位置
/*!
\section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

public:
    CYAxis &operator = (const CYAxis &other);
};

#endif // CYAXIS_H
