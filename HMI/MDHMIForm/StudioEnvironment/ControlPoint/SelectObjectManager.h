/*!
\author aGRw 2014.09.12
\section 控件选择管理类
*/
#ifndef SELECTOBJECTMANAGER_H
#define SELECTOBJECTMANAGER_H

#include <QVariant>
#include <QUndoStack>
#include "DrawObjDefine.h"
#include "IDrawData.h"
#include "ICustomEdit.h"
#include "INodeEdit.h"

class CControlPointContainer;
class CMDDrawObj;
class CMDDrawVector;
class CPropertySelectObject;

class CSelectObjectManager
{
public:
    CSelectObjectManager(CControlPointContainer* controlPoint);
    ~CSelectObjectManager();
/*!
\section field
*/
private:
    CControlPointContainer* m_controlPoint;
    ICalcData* m_dataBk;
/*!
\section property
*/
//edit setting
public:
    bool isVector(){return m_isVector;}
    //!是否是单个控件
    bool isSingle();
    //!是否是单个矢量控件
    bool isSingleVector();
    //!是否为空
    bool isEmpty();
    CMDDrawObj *obj();
    CMDDrawVector *vector();
    CMDDrawObj* lastSelectedObj(){return m_lastSelectedObj;}
    void setLastSelectedObj(CMDDrawObj* value){m_lastSelectedObj = value;}
    //!编辑模式
    EditMode studioMode();
    bool isCutomEdit();
    ICustomEdit* custom();
    bool existGroup();
    bool canCombine();
    bool existCombine();
    INodeEdit* node();
    bool isNodeEdit();
    bool isAddNode();
    bool isDeleteNode();
private:
    bool m_isVector; //!是否是矢量控件
    CMDDrawObj* m_lastSelectedObj; //!选择多个控件时，最后一个被选中的控件
//obj setting
public:
    QRectF rect();
    void setRect(const QRectF& rf);
    qreal shear(){return m_shear;}
    void setRotateAngle(qreal value);
    qreal rotateAngle(){return m_rotateAngle;}
    void setRotatePoint(const QPointF& value);
    QPointF rotatePoint(){return m_rotatePoint;}
    QPointF rotatePointPos();
    QTransform *matrix();
    QList<CMDDrawObj*> *list(){return &m_list;}
private:
    QRectF m_rect;
    qreal m_shear;
    qreal m_rotateAngle;
    QPointF m_rotatePoint;
    QPointF m_rotatePointPos;
    QTransform m_matrix;
    QList<CMDDrawObj*> m_list;
/*!
\section private function
*/
private:
    void setVectorSign();
    void setRotatePoint();
    void setRotatePointPos();
    void setListRect();
    void initProperty();
public:
    void resetProperty();
    void invalidate();
private:
    void resetLastSelectedObject(QList<CMDDrawObj*> *objList);
    void backupData();
/*!
\section public function
*/
public:
    void reset(QList<CMDDrawObj*> *objList);
    bool isVisible(const QPointF& point);
    void calculateProperty();
/*!
\section mouse operation
*/
public:
    void mouseFrameMove(const QPointF& point, int pos, bool isOrtho);
    void mouseRotatePoint(const QPointF& point);
    void mouseRotate(const QPointF& point, bool isOrtho);
    void mouseShear(const QPointF& point, int pos, bool isOrtho);
    void mouseCustom(const QPointF& point, int pos);
    void moveNode(const QPointF& point, int pos);
    void addNode(const QPointF& point);
    void deleteNode(int pos);
    void mouseDown(const QPointF& point, ControlState state);
    void mouseMove(const QPointF& point);
    void mouseUp(const QPointF& point);
/*!
\section flip
*/
public:
    void flipX();
    void flipY();
/*!
\section property
*/
public:
    QObject *propertyDrawObj();
private:
    CPropertySelectObject* m_propertyObject;
/*!
\section undo
*/
private:
    void undoOperation();
private:
    QVariant m_oldValue;
    QVariant m_newValue;

};

#endif // SELECTOBJECTMANAGER_H




