/*!
\section 统计图数据类
*/
#ifndef CDRAWDATA_H
#define CDRAWDATA_H

#include <QFont>
#include <QColor>
class CDrawData
{
public:
    CDrawData();
    ~CDrawData();

public:
    //! 背景色
    QColor m_backColor;
    //! 内部颜色
    QColor m_interColor;
    //! 统计图类型
    qint32 m_diagramStyle;
    //! 图表字体
    QFont m_diagramFont;
    //! 图例绘制位置（上、下、左、右、隐藏）
    qint32 m_typePosition;
    //! 图例字体
    QFont m_typeValueFont;
    //！显示工具栏位置
    qint32 m_toolPosition;
    //！描述的位置(上、下)
    qint32 m_stringPosition;
    //! 描述的排布（左、中、右）
    qint32 m_stringArrange;
    //！描述的字体
    QFont m_stringFont;
    //! 描述的字体的颜色
    QColor m_stringColor;
    //! 描述内容
    QString m_stringText;
    //! 是否显示客户边缘
    qint32 m_isSetEdge;

    //! 绘制类型
    qint32 m_category;
    //! 图表数值小数位
    qint32 m_decimals;
    //! 手动设置Y轴
    qint32 m_setYValue;
    //! Y轴最大值最小值
    qreal m_maxValue;
    qreal m_minValue;
    //! 主刻度数
    qint32 m_mainScaleNum;
    //! 次级刻度数
    qint32 m_subScaleNum;
    //! 轴线颜色
    QColor m_lineColor;

    //! setString
    QString m_string1;
    QString m_string2;
    QString m_string3;
    //! setValue
    QString m_valueString;
/*!
 \section 序列化
*/
public:
    CDrawData &operator =(const CDrawData &other);
public:
    virtual void serialize(QJsonObject& json);
    virtual void deserialize(const QJsonObject& json);

};

#endif // CDRAWDATA_H
