/*!
\author aGRw 2015.05.05
\section 控件创建管理类
*/
#ifndef CREATEDRAWOBJECT_H
#define CREATEDRAWOBJECT_H

#include "MDDrawVector.h"

class CStudio;

class CCreateDrawObject
{
public:
    CCreateDrawObject(CStudio* container);
/*!
\section enum
*/
public:
    enum class CreateState
    {
        NotCreate,			//!未创建
        BeginCreate,		//!预备创建
        HasCreate			//!已创建
    };
/*!
\section field
*/
private:
    //!控件创建过程状态
    CreateState m_state;
    //!创建控件类型
    DrawType m_type;
    //!节点控件,默认为null
    CMDDrawVector* m_nodeObject;
    CStudio* m_studio;
    //!是否是以节点控件方式创建
    bool m_isNodeCreate;
/*!
\section ortho
*/
private:
    //!正交模式状态
    OrthoMode m_orthoState;
private:
    //!返回正交模式下正方形模式的点位置
    QPointF orthoSquarePoint(const QPointF& begin, const QPointF& end);
    //!返回正交模式下水平垂直模式的点位置
    QPointF orthoHoriorVert(const QPointF& begin, const QPointF& end);
/*!
\section public function
*/
public:
    //!开始创建控件
    void begin(DrawType type);
    //!创建控件终止
    void end();
    bool mouseDown(Qt::MouseButton button, const QPointF& location, const QPointF& revertPoint);
    bool mouseUp(Qt::MouseButton button, const QPointF& location, const QPointF& revertPoint, const QPointF& revertDownPoint);
    bool mouseMove(Qt::MouseButton button, const QPointF& location, const QPointF& revertPoint, const QPointF& downPoint);

};

#endif // CREATEDRAWOBJECT_H



