/*!
\author emhhbmdsZQ==
\section  绘图的数据类。
*/

#ifndef DRAWDATA_H
#define DRAWDATA_H
#include <QColor>
#include <QRectF>
#include <QPainter>
#include <QtMath>
#include <QFontDatabase>
#include <QDataStream>
#include "DrawDefine.h"

/*!
\section  基类数据类。
*/
class CData
{
public:
    virtual drawType getDrawType() = 0;
    virtual void operator=(const CData& data) = 0;
public:
    virtual void serialize(QJsonObject& json) = 0;
    virtual void deserialize(const QJsonObject& json) = 0;
    virtual ~CData(){}  //! 不加此函数，会导致子类中的QString字段内存泄漏
};

/*!
\section  背景色数据类。
*/
class CBackColor
{
public:
    CBackColor();
    void operator=(const CBackColor& data);
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
public:
    QColor m_color1; //! 渐变色1
    QColor m_color2; //! 渐变色2
    gradientStyle m_gradientStyle;  //! 渐变类型

};
/////////////////////////////////////////////////////////////////
/*!
\section   边缘线数据类。
*/
class CEdgeLine
{
public:
    CEdgeLine();
    void operator=(const CEdgeLine& data);
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
//! 设置线宽，范围1-10
public:
    void setWidth(qint32 width);
    qint32 getWidth();
private:
    qint32 m_width;
public:
    QColor m_color; //! 线的颜色
    lineStyle m_lineStyle; //! 线类型
};
//////////////////////////////////////////////////////////////////////
/*!
\section
控制层数据类，控制仪表盘绘制的位置以及仪表上的显示数据。
*/
class CDataLayer:public CData
{
public:
    CDataLayer();
    ~CDataLayer();
    virtual drawType getDrawType();
    void operator=(const CDataLayer& data);
    virtual void operator=(const CData& data);

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
//! 设置绘图区域
public:
    void setRectF(const QRectF& rect);
    QRectF getRectF();
private:
    QRectF m_rect;
//! 设置仪表指针的起始角度，取值范围为0-360
public:
    void setStartAngle(qint32 startAngle);
    qint32 getStartAngle();
private:
    qint32 m_startAngle;
 //! 设置仪表指针的旋转角度范围，取值范围为0-360
public:
    void setAngleRange(qint32 angleRange);
    qint32 getAngleRange();
private:
    qint32 m_angleRange;
//! 设置仪表的最小刻度值
public:
    void setMinValue(double minValue);
    double getMinValue();
private:
    double m_minValue;
//! 设置仪表刻度值范围 ,取值>0
public:
    void setValueRange(double valueRange);
    double getValueRange();
private:
    double m_valueRange;
//! 设置刻度数，取值范围为2-16
public:
    void setScaleCount(qint32 scaleCount);
    qint32 getScaleCount();
private:
    qint32 m_scaleCount;
public:
    QList <CData*> m_item;  //! 绘图项列表
    bool m_isHori;  //! 直表是否水平模式
public:
    void setIndex(const int &index);
    int getIndex() const;
private:
    qint32 m_index;  //! 编号

public:
    /*!
        根据绘图项生成相应的数据类。
        \param 绘图项枚举类型。
        \returns 数据类基类指针。
        */
    CData* createData(drawType type);   

};
/////////////////////////////////////////////////////////////////////////////////////
/*!
\section  数据类值。
*/
class CDataValue:public CData
{
public:
    virtual drawType getDrawType() = 0;
    virtual void operator=(const CData& data) = 0;
//    virtual void serialize(QDataStream& stream, bool isOrigVersion = false) = 0;
//    virtual void deserialize(QDataStream& stream) = 0;
//    virtual void serialize_1(QDataStream& stream) = 0;
public:
    virtual void serialize(QJsonObject& json) = 0;
    virtual void deserialize(const QJsonObject& json) = 0;
//!  设置阻尼系数，取值范围为0-10
    virtual void setStep(qint32 step) = 0;
    virtual qint32 getStep() = 0;

public:
    qint32 m_index;    //! 绘图项的编号
    QString m_expression;    //! 仪表表盘绑定的表达式;
    double m_value;   //! 仪表指针指示的当前值，不需要序列化
    double m_currentValue;   //! 中间值，用于动态移动。不序列化
    qint32 m_currentStep;   //! 当前步数，不序列化
};
///////////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项中心点的数据类。
*/
class CDataCap:public CData
{
public:
    CDataCap();
    drawType getDrawType();
    void operator=(const CData& data);

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
//! 设置圆表中心点半径，取值范围为1.0-36.0
public:
    void setRadius(qreal radius);
    qreal getRadius();
private:
    qreal m_radius;
public:
    qint32 m_style;    //! 中心点风格
    CEdgeLine m_edgeLine;
    CBackColor m_backColor;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项指针的数据类。
*/
class CDataNeedle:public CDataValue
{
public:
    CDataNeedle();
    drawType getDrawType();
    void operator=(const CData& data);

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
//! 设置指针的宽度，取值范围为1.0-25.0
public:
    void setWidth(qreal width);
    qreal getWidth();
private:
    qreal m_width;
//! 设置指针的长度，取值范围为1.0-100.0
public:
    void setLen(qreal len);
    qreal getLen();
private:
    qreal m_len;
//! 设置指针的半径，取值范围为-55.0-55.0
public:
    void setRadius(qreal radius);
    qreal getRadius();
private:
    qreal m_radius;
//! 设置阻尼系数，取值范围为0-10
public:
    void setStep(qint32 step);
    qint32 getStep();
private:
    qint32 m_step;
public:
    qint32 m_style; //! 指针的风格
    CEdgeLine m_edgeLine;
    CBackColor m_backColor;
};
///////////////////////////////////////////////////////////
/*!
\section 绘图项进度条的数据类。
*/
class CDataRange:public CData
{
public:
    CDataRange();
    drawType getDrawType();
    void operator=(const CData& data);
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
//! 设置进度条的半径，取值范围为1.0-50.0
public:
    void setRadius(qreal radius);
    qreal getRadius();
private:
    qreal m_radius;
//! 设置进度条的宽度，取值范围为1.0-25.0
public:
    void setWidth(qreal width);
    qreal getWidth();
private:
    qreal m_width;
//! 设置进度条指示进度的最小值
public:
    void setMinValue(double minValue);
    double getMinValue();
private:
    double m_minValue;
//! 设置进度条指示进度的范围，取值>0
public:
    void setValueRange(double valueRange);
    double getValueRange();
private:
    double m_valueRange;

 public:
    qint32 m_arrangement; //! 进度条的布局
    CEdgeLine m_edgeLine;
    CBackColor m_backColor;

};
/////////////////////////////////////////////////////////////
/*!
\section 绘图项刻度的数据类。
*/
class CDataScale:public CData
{
public:
    CDataScale();
    drawType getDrawType();
    void operator=(const CData& data);

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

//!  设置刻度宽度，取值范围为1.0-25.0
public:
    void setWidth(qreal width);
    qreal getWidth();
private:
    qreal m_width;
//!  设置刻度长度，取值为1.0-36.0
public:
    void setLen(qreal len);
    qreal getLen();
private:
    qreal m_len;
//!  设置刻度半径，取值范围为10.0-80.0
public:
    void setRadius(qreal radius);
    qreal getRadius();
private:
    qreal m_radius;
//!  设置值刻度数，取值范围为1-10
public:
    void setSubCount(qint32 subCount);
    qint32 getSubCount();
private:
    qint32 m_subCount;
public:
    qint32 m_style;    //! 刻度风格
    qint32 m_arrangement;   //!  刻度布局
    bool m_isSubScale;   //!  是否是子刻度
    CEdgeLine m_edgeLine;
    CBackColor m_backColor;

};
////////////////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项刻度背景的数据类。
*/
class CDataScaleBack:public CData
{
public:
    CDataScaleBack();
    drawType getDrawType();
    void operator=(const CData& data);

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
//! 设置刻度背景的宽，取值范围为1.0-25.0
public:
    void setWidth(qreal width);
    qreal getWidth();
private:
    qreal m_width;
//! 设置刻度背景的半径，取值范围为1.0-50.0
public:
    void setRadius(qreal radius);
    qreal getRadius();
private:
    qreal m_radius;
public:
    qint32 m_arrangement;  //!  刻度背景的风格
    CEdgeLine m_edgeLine;
    CBackColor m_backColor;
};
////////////////////////////////////////////////////////////////////
/*!
\section 绘图项刻度文本的数据类。
*/
class CDataScaleText:public CData
{
public:
    CDataScaleText();
    drawType getDrawType();
    void operator=(const CData& data);

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
//! 设置刻度文本的精度，取值范围为0-5
public:
    void setDecimals(qint32 decimals);
    qint32 getDecimals();
private:
    qint32 m_decimals;
//!  设置刻度文本的半径，取值范围为1.0-80.0
public:
    void setRadius(qreal radius);
    qreal getRadius();
private:
    qreal m_radius;
//!  设置刻度文本的大小，取值范围为5.0-20.0
public:
    void setSize(qreal size);
    qreal getSize();
private:
    qreal m_size;
public:
    qint32 m_style;   //! 刻度文本的风格
    QString m_name;  //! 字体名称
    QColor  m_color;  //!  刻度文本颜色
    bool m_isBold;   //! 是否为粗体
};
////////////////////////////////////////////////////////////////////
/*!
\section 绘图项仪表背景的数据类。
*/
class CDataBack:public CData
{
public:
    CDataBack();
    virtual drawType getDrawType();
    virtual void operator=(const CData& data);

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
//!  设置仪表背景的边缘宽度，取值范围为1.0-50.0
public:
    void setEdgeWidth(qreal edgeWidth);
    qreal getEdgeWidth();
private:
    qreal m_edgeWidth;
public:
    qint32 m_style;   //!  背景的风格
    CEdgeLine m_edgeLine;
    CBackColor m_backColor;
    CBackColor m_edgeColor;

};
//////////////////////////////////////////////////////////////////
/*!
\section 绘图项半圆背景的数据类。
*/
class CDataHalfBack:public CDataBack
{
public:
    CDataHalfBack();
    virtual drawType getDrawType();
    virtual void operator=(const CData& data);

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
public:
    qint32 m_arrangement;  //!  半圆背景布局
};
///////////////////////////////////////////////////////////////////
/*!
\section 绘图项标签的数据类。
*/
class CDataLabel:public CData
{
public:
    CDataLabel();
    drawType getDrawType();
    void operator=(const CData& data);

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
//!  设置标签字体的大小，取值范围为5.0-20.0
public:
    void setSize(qreal size);
    qreal getSize();
private:
    qreal m_size;
//!  设置标签上显示的字符串
public:
    void setString(QString str);
    QString getString();
    QString m_text;
    QString m_showText;
public:
    QPointF  m_pos;  //!  位置
    QString m_name;   //!  字体名称
    QColor m_color;    //! 字体颜色
    bool m_isBold;   //!  粗体

};
//////////////////////////////////////////////////////////////////
/*!
\section 绘图项LED数字的数据类。
*/
class CDataLed:public CDataValue
{
public:
    CDataLed();
    drawType getDrawType();
    void operator=(const CData& data);

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
 //!  设置显示数字的大小，取值范围为5.0-20.0
public:
    void setSize(qreal size);
    qreal getSize();
private:
    qreal m_size;
//!  设置显示数字的精度，取值范围为0-6
public:
    void setDecimals(qint32 decimals);
    qint32 getDecimals();
private:
    qint32 m_decimals;
//!  设置阻尼系数，取值范围为0-10
public:
    void setStep(qint32 step);
    qint32 getStep();
public:
    QPointF m_pos;   //!  LED数字显示的位置
    QColor m_color;  //! 显示数字的颜色

};
//////////////////////////////////////////////////////////////////
/*!
\section 绘图项温度计的数据类。
*/
class CDataTemperature:public CDataValue
{
public:
    CDataTemperature();
    drawType getDrawType();
    void operator=(const CData& data);

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
//! 设置温度计上部的宽度，取值范围为1.0-50.0
public:
    void setWidth1(double width);
    double getWidth1();
private:
    double m_width1;
//! 设置温度计下部圆的宽度，取值范围为1.0-50.0
public:
    void setWidth2(double width);
    double getWidth2();
private:
     double m_width2;
//! 设置温度计上部的长度，取值范围为0-30.0
public:
    void setHeight1(double height);
    double getHeight1();
private:
    double m_height1;
//!  设置温度计下部的长度，取值范围为0-30.0
public:
    void setHeight2(double height);
    double getHeight2();
private:
    double m_height2;
//!  设置温度计的左右偏移量，取值范围为-55.0-55.0
public:
    void setOffset(double offset);
    double getOffset();
private:
    double m_offset;
//!  设置温度计的阻尼系数，取值范围为0-10
public:
    void setStep(qint32 step);
    qint32 getStep();
private:
     qint32 m_step;

public:
    qint32 m_arrangement;   //!   温度计的风格
    CEdgeLine m_edgeLine;
    CBackColor m_backColor;
    CBackColor m_temColor;

};
///////////////////////////////////////////////////////////////////////////////////////
//! 以下是直表数据类
//////////////////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项直表刻度背景的数据类。
*/
class CRulerScaleBack: public CDataScaleBack
{
public:
    CRulerScaleBack();

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
    drawType getDrawType();
    void operator=(const CData& data);
//!  设置直表刻度背景的左右偏移量，取值范围为-55.0-55.0
public:
    void setOffset(double offset);
    double getOffset();
private:
    double m_offset;
};
/////////////////////////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项直表刻度的数据类。
*/
class CRulerScale: public CDataScale
{
public:
    CRulerScale();

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
    drawType getDrawType();
    void operator=(const CData& data);
//!  设置直表刻度的左右偏移量，取值范围为-55.0-55.0
public:
    void setOffset(double offset);
    double getOffset();
private:
    double m_offset;

};
////////////////////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项直表刻度文字的数据类。
*/
class CRulerScaleText: public CDataScaleText
{
public:
    CRulerScaleText();

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
    drawType getDrawType();
    void operator=(const CData& data);

//!  设置直表刻度文字的左右偏移量，取值范围为-55.0-55.0
public:
    void setOffset(double offset);
    double getOffset();
private:
    double m_offset;
public:
     qint32 m_arrangement;    //!   刻度文字的布局
};
/////////////////////////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项直表临界区的数据类。
*/
class CRulerRange: public CDataRange
{
public:
    CRulerRange();

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
    drawType getDrawType();
    void operator=(const CData& data);
//!  设置直表临界区的左右偏移量，取值范围为-55.0-55.0
public:
    void setOffset(double offset);
    double getOffset();
private:
     double m_offset;
//!  设置直表临界区的开始百分比，取值范围为0.0-100.0
public:
    void setMinPercent(double min);
    double getMinPercent();
private:
    double m_minPercent;
//!  设置直表临界区的百分比量程，取值范围为0.0-100.0
public:
    void setPercentRange(double range);
    double getPercentRange();
private:
    double m_percentRange;
public:
    qint32  m_style;
};
///////////////////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项直表指针的数据类。
*/
class CRulerNeedle: public CDataNeedle
{
public:
    CRulerNeedle();

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
    drawType getDrawType();
    void operator=(const CData& data);
//!  设置直表指针的左右偏移量，取值范围为-55.0-55.0
public:
    void setOffset(double offset);
    double getOffset();  
private:
    double m_offset;
public:
     qint32 m_arrangement;  //!  直表指针的布局
};
//////////////////////////////////////////////////////////////////////////
/*!
\section 绘图项直表指示块的数据类。
*/
class CRulerMarker: public CRulerNeedle
{
public:
    CRulerMarker();
    drawType getDrawType();
};

//////////////////////////////////////////////////////////////////////////

#endif // DRAWDATA_H
