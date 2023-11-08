/*!
\author aGRw 2015.03.02
\section 控件的数据参数接口类
*/
#ifndef IDRAWDATA_H
#define IDRAWDATA_H

#include <QTransform>
#include "DrawObjDefine.h"

class CMDDrawObj;

/*!
\section 基本控件的数据参数
*/
class IDataObj
{
public:
    virtual ~IDataObj(){}
public:
    virtual void setRect(const QRectF& value) = 0;
    virtual QRectF rect() = 0;
};

/*!
\section 矢量控件的数据参数
*/
class IDataVector
{
public:
    virtual ~IDataVector(){}
public:
    virtual void setRotateAngle(qreal value) = 0;
    virtual qreal rotateAngle() = 0;
    virtual void setShear(qreal value) = 0;
    virtual qreal shear() = 0;
    virtual void setRotatePointPos(const QPointF& value) = 0;
    virtual QPointF rotatePointPos() = 0;
    virtual void setRotatePoint(const QPointF& value) = 0;
    virtual QPointF rotatePoint() = 0;
    virtual void setXScale(qreal value) = 0;
    virtual qreal xScale() = 0;
    virtual void setYScale(qreal value) = 0;
    virtual qreal yScale() = 0;
    virtual void setScalePoint(const QPointF& value) = 0;
    virtual QPointF scalePoint() = 0;
    virtual void setIsFlipX(bool value) = 0;
    virtual bool isFlipX() = 0;
    virtual void setIsFlipY(bool value) = 0;
    virtual bool isFlipY() = 0;
};

/*!
\section 控件计算时需要的备份数据
*/
class ICalcData
{
public:
    virtual ~ICalcData(){}
public:
    virtual IDataObj* data() = 0;
    virtual IDataVector* dataVector() = 0;
    virtual void setMousePos(const QPointF& value) = 0;
    virtual QPointF mousePos() = 0;
    virtual void setBound(const QRectF& value) = 0;
    virtual QRectF bound() = 0;
    virtual void setMatrixBound(const QRectF& value) = 0;
    virtual QRectF matrixBound() = 0;
    virtual void setFixRate(const QPointF& value) = 0;
    virtual QPointF fixRate() = 0;
    virtual void setOffset(const QPointF& value) = 0;
    virtual QPointF offset() = 0;
    virtual void setMatrix(QTransform* value) = 0;
    virtual QTransform* matrix() = 0;
    virtual void setState(ControlState value) = 0;
    virtual ControlState state() = 0;
};

/*!
\section redo操作时需要的基本数据
*/
class IUndoDataObj
{
public:
    virtual ~IUndoDataObj(){}
public:
    virtual CMDDrawObj* obj() = 0;
};

#endif // IDRAWDATA_H




