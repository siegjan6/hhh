/*!

\author bWVpeGw=

\section 功能

管理【线和管道控件属性管理类.
    用来配置线和管道相关属性】。

*/

#ifndef MDPROPERTYDRAWNODE_H
#define MDPROPERTYDRAWNODE_H
#include "MDPropertyDrawVector.h"
#include "MDDrawNode.h"
class CMDPropertyDrawNode :public CMDPropertyDrawVector
{
    //属性Start
    Q_OBJECT
    Q_ENUMS(NodeState1)
    Q_ENUMS(PropertyEditMode)
    Q_ENUMS(PropertyDirection)

    Q_PROPERTY(QString GUID_Action)

    Q_PROPERTY(NodeState1 nodeState READ nodeState WRITE setNodeState)
    Q_PROPERTY(PropertyEditMode editMode READ editMode WRITE setEditMode)


    Q_PROPERTY(QString GUID_Stream)
    Q_PROPERTY(bool pipeBackGroundVisible READ pipeBackGroundVisible WRITE setPipeBackGroundVisible)
    Q_PROPERTY(qreal width READ width WRITE setWidth)
    Q_PROPERTY(QColor baseColor READ baseColor WRITE setBaseColor)
    Q_PROPERTY(QColor highColor READ highColor WRITE setHighColor)

    Q_PROPERTY(bool streamLineVisible READ streamLineVisible WRITE setStreamLineVisible)
    Q_PROPERTY(bool lineStreamEnable READ lineStreamEnable WRITE setLineStreamEnable)
    Q_PROPERTY(QString DILstreamPen READ DILstream WRITE setDILstream)
    Q_PROPERTY(PropertyDirection streamDirection READ streamDirection WRITE setStreamDirection)
    Q_PROPERTY(int streamSpeed READ streamSpeed WRITE setStreamSpeed)

/*!
\section enum
*/
public:
    enum class PropertyEditMode{ Normal, Node};
    enum class NodeState1{ Move, Add, Delete };
    enum class PropertyDirection { forward ,reverse};
    enum class ColorType{ Red ,Green, Blue, Alpha, All};

public:
    CMDPropertyDrawNode(CMDDrawObj* parent);
    virtual ~CMDPropertyDrawNode();

public:
    NodeState1 nodeState();
    void setNodeState(NodeState1 state);

    PropertyEditMode editMode();
    void setEditMode(PropertyEditMode value);

    bool pipeBackGroundVisible();
    void setPipeBackGroundVisible(bool visible);

    qreal width();
    void setWidth(qreal width);

    QColor baseColor();
    void setBaseColor(QColor clr);

    QColor highColor();
    void setHighColor(QColor clr);

    bool streamLineVisible();
    void setStreamLineVisible(bool visible);

    bool lineStreamEnable();
    void setLineStreamEnable(bool enable);

    QString DILstream();
    void setDILstream(const QString &stream);

    PropertyDirection streamDirection();
    void setStreamDirection(PropertyDirection value);

    int streamSpeed();
    void setStreamSpeed(int value);

    CMDDrawNode *object();

/*!
\section batch edit
*/
private:
    void setBatchNodeState(NodeState1 value);
    void setBatchEditMode(PropertyEditMode value);
    void setBatchPipeBackGroundVisible(bool value);
    void setBatchWidth(qreal value);
    void setBatchBaseColor(const QColor &value);
    void setBatchHighColor(QColor value);
    void setBatchStreamLineVisible(bool value);
    void setBatchLineStreamEnable(bool value);
    void setBatchDILstream(CMDPenManager *value);
    void setBatchStreamDirection(PropertyDirection value);
    void setBatchStreamSpeed(int value);
    CMDPropertyDrawNode::ColorType getColorChanged(const QColor &c1, const QColor &c2);
    QColor getChangedColor(QColor value, QColor c, ColorType type);
};

class CMDPropertyDrawLine :public CMDPropertyDrawNode
{
    Q_OBJECT

    Q_PROPERTY(QString $DILbrush)
    Q_PROPERTY(QString $DILfill)
    Q_PROPERTY(QString $DILdynamicBrush)
public:
    CMDPropertyDrawLine(CMDDrawObj *parent);
    virtual ~CMDPropertyDrawLine(){}
};


class CMDPropertyDrawPipe :public CMDPropertyDrawNode
{
    Q_OBJECT

    Q_PROPERTY(QString $DILbrush)
    Q_PROPERTY(QString $DILfill)
    Q_PROPERTY(QString $DILdynamicBrush)
public:
    CMDPropertyDrawPipe(CMDDrawObj *parent);
    virtual ~CMDPropertyDrawPipe(){}
};

#endif // MDPROPERTYDRAWNODE_H
