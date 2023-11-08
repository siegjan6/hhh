/*!
\author aGRw 2015.05.04
\section 节点编辑接口
*/
#ifndef INODEEDIT_H
#define INODEEDIT_H

#include <QList>
#include <QPointF>
#include <QPainter>
#include "DrawObjDefine.h"

class INodeEdit
{
public:
    virtual ~INodeEdit(){}
/*!
\section property
*/
public:
    //!编辑状态
    virtual void setNodeState(NodeState value) = 0;
    virtual NodeState nodeState() = 0;
    //!是否是在用鼠标以节点方式创建创建
    virtual void setIsNodeCreating(bool value) = 0;
    virtual bool isNodeCreating() = 0;
    //!是否是使用鼠标编辑节点
    virtual bool isNodeOperate() = 0;
    //!节点控件是否创建成功
    virtual bool createSuccess() = 0;
    //!节点控件是否创建完成。比如直线，只需要创建两个点，便自动完成
    virtual bool createFinish() = 0;
    //!节点控件包含的最小节点数
    virtual int minCount() = 0;
    //!节点数据
    virtual QList<QPointF>* nodeDatas() = 0;
/*!
\section function
*/
public:
    virtual void moveNode(const QPointF& point, int pos) = 0;
    virtual void addNode(const QPointF& point) = 0;
    virtual void deleteNode(int pos) = 0;
    //!鼠标创建控件时绘制图形
    virtual void creatingPaint(QPainter* p) = 0;
};


#endif // INODEEDIT_H



