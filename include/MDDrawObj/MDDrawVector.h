/*!
\author aGRw 2014.05.16
\section 矢量控件
*/
#ifndef MDDRAWVECTOR_H
#define MDDRAWVECTOR_H

#include <QtGui>
#include "MDDrawObj.h"
#include "MDFill.h"
#include "MDColorManager/MDColorManager.h"
#include "MDHMIActionManager.h"


class CDrawVectorData;

class MDDRAWOBJSHARED_EXPORT CMDDrawVector : public CMDDrawObj
{
public:
    CMDDrawVector();
    virtual ~CMDDrawVector();
    virtual void initCreate();

private:
    CDrawVectorData* m_vector;

/*!
\section property
*/
public:
    QPainterPath* matrixPath();
    QPainterPath* path();
    CMDPenManager* pen();
    CMDPenManager* factPen();
    CMDBrushManager* brush();
    CMDBrushManager* factBrush();
	void setRotateAngle(qreal value);
    qreal rotateAngle();
	void setShear(qreal value);
    qreal shear();
	void setRotatePoint(const QPointF& value);
    QPointF rotatePoint();
	void setRotatePointPos(const QPointF& value);
    QPointF rotatePointPos();
	void setScalePoint(const QPointF& value);
    QPointF scalePoint();
	void setXScale(qreal value);
    qreal xScale();
	void setYScale(qreal value);
    qreal yScale();
    virtual void setEditMode(EditMode value);
    EditMode editMode();
    void setFillValue(qreal value);
    void setFill(CMDFill* value);
    CMDFill* fill();
    IDataVector* dataVector();
    CMDHMIActionManager* mouseDownActions();
    CMDHMIActionManager* mouseUpActions();
    CMDHMIActionManager* rightMouseDownActions();
    CMDHMIActionManager* rightMouseUpActions();
    void setInputVariable(const QString& value);
    QString inputVariable();
    void setDynamicBrush(CDynamicColorManager* value);
    CDynamicColorManager* dynamicBrush();
    void setDynamicPen(CDynamicColorManager* value);
    CDynamicColorManager* dynamicPen();
private:
	void setFactShear(qreal value);
    void setFactRotatePoint(const QPointF& value);
	void setFactXScale(qreal value);
	void setFactYScale(qreal value);
/*!
\section virtual property
*/
public:
    bool isVector() ;
    bool isWidget() ;
    qreal widenWidth() ;
/*!
\section public function
*/
public:
    void mouseFrameMove(const QPointF& point, int pos) ;
    bool canSelect(const QRectF& rect) ;
    void draw(QPainter* painter, bool isPrinting = false) ;
    void boundMove(qreal xOff, qreal yOff, qreal wOff, qreal hOff) ;
    bool isVisible(const QPointF& point) ;
    QRectF baseMultiRotate(qreal angle, const QPointF &center);
    virtual void multiRotate(qreal angle, const QPointF& center);
    void backupData() ;
    CMDPropertyDrawObj *createPropertyDrawObj() ;
    virtual void getBrushes(QList<CMDBrushManager*>& list);
    virtual void getPens(QList<CMDPenManager*>& list);
    void showToolTip();
/*!
\section protected function
*/
protected:
    void generateBound() ;
    //!获取矩阵转换后的点
    QPointF vectorPoint(const QPointF& point);
    //!边框移动，参数均是偏移量
    void frameMove(qreal xOff, qreal yOff, qreal wOff, qreal hOff) ;
	QRectF baseBoundMove(qreal xOff, qreal yOff, qreal wOff, qreal hOff);
/*!
\section event
*/
public:
    void loadInitalizationEvent() ;
    //!生成Brush和Pen
    void generateContent() ;
protected:
    void onMouseDown(Qt::MouseButton button, qreal x, qreal y);
    void onMouseUp(Qt::MouseButton button, qreal x, qreal y);
    void onMouseMove(Qt::MouseButton button, qreal x, qreal y);
    void loadMouseEvent(Qt::MouseButton button, const QPointF& location, MouseType type);
    void onMouseEnter();
    void onMouseLeave();
    //!初始化Brush和Pen
    void initContent() ;
/*!
\section calculate matrix
*/
public:
    void generateMatrix() ;
	virtual QTransform* matrix(); 

/*!
\section claculate mouse operation
*/
public:
    void mouseRotatePoint(const QPointF& point);
    void mouseRotate(const QPointF& point, bool isOrtho);
    void mouseShear(const QPointF& point, int pos, bool isOrtho);

/*!
\section calculate offset
*/
protected:
    //!设置一个固定点，转换矩阵变化后，计算出偏移值，使得固定点的值保持不变
    QRectF calculateOffset();
/*!
\section common
*/
public:
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
    CMDDrawObj* clone() ;

/*!
\section
*/
public:
    //解组操作
    void unMultiOper();
/*!
\section paint
*/
protected:
    void onPaint(QPainter* p) ;

/*!
\section flip
*/
public:
    void setIsFlipX(bool value);
    bool isFlipX();
    void setIsFlipY(bool value);
    bool isFlipY();
private:
    void setFlip(bool value, bool isX);
/*!
\section var
*/
public:
    static QList<QStringList> expNames();
    void loadDataChangedEvent(CMDExpInfo *exp) ;
/*!
\section undo
*/
protected:
    IUndoDataObj* createUndoData() ;
public:
    bool revertUndoData(IUndoDataObj* value) ;
    IUndoDataObj *fillUndoData(bool isOld = true) ;
    bool undoEqual() ;
public:
    void undoBrush(CMDBrushManager* value);
    void undoPen(CMDPenManager* value);
    void undoAction(int id, CMDHMIActionManager *value);
/*!
\section child changed
*/
public:
    void setDynamicRotateAngle(qreal value);
    void setDynamicShear(qreal value);
/*!
\section batch edit
*/
public:
     virtual CMDPropertyDrawObj* createPropertyDrawObj(const QString& className);
protected:
    //初始化类信息
    virtual void initClassInfo(QStringList &classInfo);

};

#endif // MDDRAWVECTOR_H


