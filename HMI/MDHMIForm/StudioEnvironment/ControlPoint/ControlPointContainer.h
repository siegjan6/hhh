/*!
\author aGRw 2014.09.18
\section 控制点管理
** 控制点索引说明
** 0--1--2
** 3-- --4
** 5--6--7
*/
#ifndef CONTROLPOINTCONTAINER_H
#define CONTROLPOINTCONTAINER_H

#include <QtCore>
#include <QtGui>

#include "DrawObjDefine.h"

class IHMIForm;
class CFramePoint;
class CCustomPoint;
class CFormPoint;
class CSelectObjectManager;
class CMDDrawObj;
class CNodePoint;


class CControlPointContainer
{
public:
    CControlPointContainer(IHMIForm* container);
    ~CControlPointContainer();
/*!
\section const
*/
public:
    static int PointSize;
/*!
\section field
*/
private:
    //!边框点
    CFramePoint* m_frame;
    //!自定义点
    CCustomPoint* m_custom;
    //!窗体
    CFormPoint* m_form;
    //!节点
    CNodePoint* m_node;
/*!
\section property
*/
public:
    CSelectObjectManager* selectObjs(){return m_selectObjs;}
    IHMIForm* container(){return m_container;}
    QPointF formScale();
private:
    //!选中的控件
    CSelectObjectManager* m_selectObjs;
    IHMIForm* m_container;
/*!
\section private function
*/
private:
    //!获取网格模式下鼠标指针值
    QPointF getGridPointF(ControlState state, const QPointF& point);
    void calculate();
    void mouseOperation(const QPointF& point);
    void mouseDownOperation(const QPointF& point);
    void getState(const QPointF& point, ControlState& state, int& index);
/*!
\section public function
*/
public:
    static QPointF transFromData(const QTransform &m, qreal scale, const QPointF& data);
/*!
\section control point
*/
public:
    bool isSelected();
    QRectF invalidateRect(){return m_invalidateRect;}
    QRectF rotateCenterRect(){return m_rotateCenterRect;}
private:
    QRectF m_invalidateRect;
    QRectF m_rotateCenterRect;

public:
    bool contains(CMDDrawObj* obj);
    //!设置当前选中控件
    void changeSelectObj(QList<CMDDrawObj*>* list);
    void draw(QPainter* p);
    void invalidate();
    //!计算后控制点刷新
    void calculateAndInvalidate();

/*!
\section mouse operation
*/
public:
    bool mouseDown(const QPointF& point);
    void mouseUp(const QPointF& point);
    void mouseMove(const QPointF& point, ControlState& state, int& pos);
private:
    ControlState m_state;
    int m_pos;
    //!是否在鼠标操作中
    bool m_mouseOperation;

//////////////////////////////////////////////////////////////////////////
};

#endif // CONTROLPOINTCONTAINER_H
