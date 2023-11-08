/*!
\author aGRw 2014.05.14
\section 控件数据类
*/
#ifndef DRAWDATA_H
#define DRAWDATA_H

#include <QJsonArray>
#include <QJsonObject>

#include "IDrawData.h"

/*!
\section 基本控件的数据参数
*/
class CDataObj : public IDataObj
{
public:
    CDataObj();
    virtual ~CDataObj(){}
public:
    void setRect(const QRectF& value){m_rect = value;}
    QRectF rect(){return m_rect;}
private:
	QRectF m_rect;
public:
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
    bool equal(CDataObj* other);
};

/*!
\section 矢量控件的数据参数
*/
class CDataVector : public IDataVector
{
public:
    CDataVector();
    virtual ~CDataVector(){}
public:
    void setRotateAngle(qreal value){m_rotateAngle = value;}
    qreal rotateAngle(){return m_rotateAngle;}
    void setShear(qreal value){m_shear = value;}
    qreal shear(){return m_shear;}
    void setRotatePointPos(const QPointF& value){m_rotatePointPos = value;}
    QPointF rotatePointPos(){return m_rotatePointPos;}
    void setRotatePoint(const QPointF& value){m_rotatePoint = value;}
    QPointF rotatePoint(){return m_rotatePoint;}
    void setXScale(qreal value){m_xScale = value;}
    qreal xScale(){return m_xScale;}
    void setYScale(qreal value){m_yScale = value;}
    qreal yScale(){return m_yScale;}
    void setScalePoint(const QPointF& value){m_scalePoint = value;}
    QPointF scalePoint(){return m_scalePoint;}
    void setIsFlipX(bool value){m_isFlipX = value;}
    bool isFlipX(){return m_isFlipX;}
    void setIsFlipY(bool value){m_isFlipY = value;}
    bool isFlipY(){return m_isFlipY;}
private:
    qreal m_rotateAngle;
    qreal m_shear;
	QPointF m_rotatePointPos;
    QPointF m_rotatePoint;
    qreal m_xScale;
    qreal m_yScale;
    QPointF m_scalePoint;
    bool m_isFlipX;
    bool m_isFlipY;
public:
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
    bool equal(CDataVector* other);
};

/*!
\section 控件计算时需要的备份数据
*/
class CCalcData : public ICalcData
{
public:
    CCalcData();
    virtual ~CCalcData(){}
public:
    IDataObj* data(){return &m_data;}
    IDataVector *dataVector(){return &m_dataVector;}
    void setMousePos(const QPointF& value){m_mousePos = value;}
    QPointF mousePos(){return m_mousePos;}
    void setBound(const QRectF& value){m_bound = value;}
    QRectF bound(){return m_bound;}
    void setMatrixBound(const QRectF& value){m_matrixBound = value;}
    QRectF matrixBound(){return m_matrixBound;}
    void setFixRate(const QPointF& value){m_fixRate = value;}
    QPointF fixRate(){return m_fixRate;}
    void setOffset(const QPointF& value){m_offset = value;}
    QPointF offset(){return m_offset;}
    void setMatrix(QTransform* value){m_matrix = *value;}
    QTransform* matrix(){return &m_matrix;}
    void setState(ControlState value);
    ControlState state(){return m_state;}
private:
    CDataObj m_data;
    CDataVector m_dataVector;
    QPointF m_mousePos;			//!鼠标坐标
    QRectF m_bound;					//!合并成组矩阵后的最终尺寸
    QRectF m_matrixBound;			//!应用控件矩阵后的尺寸
    QPointF m_fixRate;				//!特定点，比例系数
    QPointF m_offset;				//!偏移值
    QTransform m_matrix;
    ControlState m_state;
};






#endif // DRAWDATA_H



