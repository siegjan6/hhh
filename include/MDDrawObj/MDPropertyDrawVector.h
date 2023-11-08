/*!
\author aGRw
\section 矢量控件属性类
*/
#ifndef MDPROPERTYDRAWVECTOR_H
#define MDPROPERTYDRAWVECTOR_H

#include "MDPropertyDrawObj.h"
#include "MDDrawVector.h"

class MDDRAWOBJSHARED_EXPORT CMDPropertyDrawVector : public CMDPropertyDrawObj
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_Layout)
    Q_PROPERTY(qreal rotateAngle READ rotateAngle WRITE setRotateAngle)
    Q_PROPERTY(QPointF rotatePoint READ rotatePoint WRITE setRotatePoint)
    Q_PROPERTY(QPointF rotatePointPos READ rotatePointPos WRITE setRotatePointPos)
    Q_PROPERTY(qreal shear READ shear WRITE setShear)

    Q_PROPERTY(QString GUID_Appearance)
    Q_PROPERTY(QString DILbrush READ brush WRITE setBrush)
    Q_PROPERTY(QString DILline READ line WRITE setLine)
    Q_PROPERTY(QString DILdynamicBrush READ dynamicBrush WRITE setDynamicBrush)
    Q_PROPERTY(QString DILdynamicLine READ dynamicLine WRITE setDynamicLine)
    Q_PROPERTY(QString DILfill READ fill WRITE setFill)

    Q_PROPERTY(QString GUID_ActionEvent)
    Q_PROPERTY(QString DILmouseDown READ mouseDownAction WRITE setMouseDownAction)
    Q_PROPERTY(QString DILmouseUp READ mouseUpAction WRITE setMouseUpAction)
    Q_PROPERTY(QString DILrightMouseDown READ rightMouseDownAction WRITE setRightMouseDownAction)
    Q_PROPERTY(QString DILrightMouseUp READ rightMouseUpAction WRITE setRightMouseUpAction)

    Q_PROPERTY(QString GUID_Variable)
    Q_PROPERTY(QString DILinputVariable READ inputVariable WRITE setInputVariable)

    Q_PROPERTY(QString GUID_Action)
    Q_PROPERTY(bool supportGroupEvent READ supportGroupEvent WRITE setSupportGroupEvent)
    Q_PROPERTY(bool supportHidedEvent READ supportHidedEvent WRITE setSupportHidedEvent)
//////////////////////////////////////////////////////////////////////////
public:
    CMDPropertyDrawVector(CMDDrawObj* parent);
    virtual ~CMDPropertyDrawVector(){}
//////////////////////////////////////////////////////////////////////////
/*!
\section property
*/
public:
    void setRotateAngle(qreal value);
    qreal rotateAngle();
    void setRotatePoint(const QPointF& value);
    QPointF rotatePoint();
    void setRotatePointPos(const QPointF& value);
    QPointF rotatePointPos();
    void setShear(qreal value);
    qreal shear();

    void setBrush(const QString& value);
    QString brush();
    void setLine(const QString& value);
    QString line();
    void setDynamicBrush(const QString& value);
    QString dynamicBrush();
    void setDynamicLine(const QString& value);
    QString dynamicLine();

    void setFill(const QString& value);
    QString fill();

    void setMouseDownAction(const QString& value);
    QString mouseDownAction();
    void setMouseUpAction(const QString& value);
    QString mouseUpAction();
    void setRightMouseDownAction(const QString& value);
    QString rightMouseDownAction();
    void setRightMouseUpAction(const QString& value);
    QString rightMouseUpAction();

    void setInputVariable(const QString& value);
    QString inputVariable();

    void setSupportGroupEvent(bool value);
    bool supportGroupEvent();
    void setSupportHidedEvent(bool value);
    bool supportHidedEvent();
//////////////////////////////////////////////////////////////////////////
protected:
    CMDDrawVector *vector();

/*!
\section virtual function
*/
public:
    void resetAttribute() ;
protected:
    void changeActionsId(int id);
    CMDHMIActionManager* getAction(CMDDrawVector *vector, int id);
/*!
\section batch edit
*/
protected:
    virtual void resetBatchProperty(CPropertyBrowser* browser);
private:
    void setBatchRotateAngle(qreal value);
    void setBatchRotatePoint(const QPointF& value);
    void setBatchRotatePointPos(const QPointF& value);
    void setBatchShear(qreal value);

    void setBatchBrush(CMDBrushManager *value);
    void setBatchLine(CMDPenManager* value);
    void setBatchDynamicBrush(CDynamicColorManager* value);
    void setBatchDynamicLine(CDynamicColorManager* value);
    void setBatchFill(CMDFill* value);

    void batchChangeActionsId(CMDHMIActionManager *value, int id);

    void setBatchSupportGroupEvent(bool value);
    void setBatchSupportHidedEvent(bool value);
};

#endif // MDPROPERTYDRAWVECTOR_H
