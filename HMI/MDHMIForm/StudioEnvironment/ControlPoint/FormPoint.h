/*!
\author aGRw 2015.03.06
\section 窗体控制点类
*/
#ifndef FORMPOINT_H
#define FORMPOINT_H

#include <QPointF>
#include <QRect>
#include "DrawObjDefine.h"

class CMDHMIForm;

class CFormPoint
{
public:
    CFormPoint(CMDHMIForm* container);
/*!
\section field
*/
private:
    CMDHMIForm* m_container;
    QPointF m_downMousePos;
    QRect m_downRect;
/*!
\section public function
*/
public:
    bool canOperate(const QPointF& point, ControlState& state, int& index);
    void mouseDown(const QPointF& point);
    void changeFormSize(const QPointF& point, ControlState state);
};


#endif // FORMPOINT_H


