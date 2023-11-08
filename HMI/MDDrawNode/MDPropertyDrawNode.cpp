#include "MDPropertyDrawNode.h"
#include "MDNodeStream.h"
#include "MDDrawNode.h"
#include "undo/DrawPipeCommand.h"

CMDPropertyDrawNode::CMDPropertyDrawNode(CMDDrawObj *parent) :
    CMDPropertyDrawVector(parent)
{
    insertTranslateMap("nodeState", tr("节点状态"));
    insertTranslateMap("editMode", tr("编辑模式"));

    insertTranslateMap("Move", tr("移动"));
    insertTranslateMap("Add", tr("增加"));
    insertTranslateMap("Delete", tr("删除"));

    insertTranslateMap("Normal", tr("常规"));
    insertTranslateMap("Node", tr("节点"));

    insertTranslateMap("GUID_Stream", tr("流动"));
    insertTranslateMap("pipeBackGroundVisible", tr("显示管道"));
    insertTranslateMap("width", tr("管道宽度"));
    insertTranslateMap("baseColor", tr("管道背景色"));
    insertTranslateMap("highColor", tr("管道前景色"));
    insertTranslateMap("streamLineVisible", tr("显示流动线条"));
    insertTranslateMap("lineStreamEnable", tr("线条流动"));
    insertTranslateMap("streamPen", tr("流动线条"));
    insertTranslateMap("streamDirection", tr("流动方向"));
    insertTranslateMap("streamSpeed", tr("流动速度（0-2）"));

    insertTranslateMap("forward", tr("正向"));
    insertTranslateMap("reverse", tr("反向"));
}

CMDPropertyDrawNode::~CMDPropertyDrawNode(){}

CMDDrawNode *CMDPropertyDrawNode::object()
{
    return dynamic_cast<CMDDrawNode*>(m_object);
}

CMDPropertyDrawNode::NodeState1 CMDPropertyDrawNode::nodeState()
{
    return (CMDPropertyDrawNode::NodeState1)object()->nodeState();
}

void CMDPropertyDrawNode::setNodeState(CMDPropertyDrawNode::NodeState1 state)
{
    if(isBatch())
        setBatchNodeState(state);
    else
        object()->setNodeState((NodeState)state);

    setModify(true);
}

CMDPropertyDrawNode::PropertyEditMode CMDPropertyDrawNode::editMode()
{
    return (PropertyEditMode)object()->editMode();
}

void CMDPropertyDrawNode::setEditMode(PropertyEditMode value)
{
    if(isBatch())
        setBatchEditMode(value);
    else
        object()->setEditMode((EditMode)value);

    setModify(true);
}

bool CMDPropertyDrawNode::pipeBackGroundVisible()
{
    return object()->pipeBackGroundVisible();
}

void CMDPropertyDrawNode::setPipeBackGroundVisible(bool visible)
{
    if(isBatch())
        setBatchPipeBackGroundVisible(visible);
    else
    {
        undo()->push(new CDrawPipeCommand(
                         object(), object()->pipeBackGroundVisible(), visible, (int)DrawPipeId::PipeVisible));
        object()->invalidate();
    }

    setModify(true);
}

qreal CMDPropertyDrawNode::width()
{
    return object()->pipewidth();
}

void CMDPropertyDrawNode::setWidth(qreal width)
{
    if(isBatch())
        setBatchWidth(width);
    else
    {
        object()->invalidate();
        undo()->push(new CDrawPipeCommand(
                         object(), object()->pipewidth(), width, (int)DrawPipeId::PipeWidth));
        object()->invalidate();
    }

    setModify(true);
}

QColor CMDPropertyDrawNode::baseColor()
{
    return object()->pipeClr2();
}

void CMDPropertyDrawNode::setBaseColor(QColor clr)
{
    if(isBatch())
        setBatchBaseColor(clr);
    else
    {
        undo()->push(new CDrawPipeCommand(
                         object(), object()->pipeClr2(), clr, (int)DrawPipeId::PipeBackColor));
        object()->invalidate();
    }

    setModify(true);
}

QColor CMDPropertyDrawNode::highColor()
{
    return object()->pipeClr1();
}

void CMDPropertyDrawNode::setHighColor(QColor clr)
{
    if(isBatch())
        setBatchHighColor(clr);
    else
    {
        undo()->push(new CDrawPipeCommand(
                         object(), object()->pipeClr1(), clr, (int)DrawPipeId::PipeHighColor));
    }

    setModify(true);
}

bool CMDPropertyDrawNode::streamLineVisible()
{
    return object()->pLineStream()->streamLineVisible();
}

void CMDPropertyDrawNode::setStreamLineVisible(bool visible)
{
    if(isBatch())
        setBatchStreamLineVisible(visible);
    else
    {
        undo()->push(new CDrawPipeCommand(
                         object(), streamLineVisible(), visible, (int)DrawPipeId::StreamLineVisible));
        object()->invalidate();
    }

    setModify(true);
}

bool CMDPropertyDrawNode::lineStreamEnable()
{
    return object()->pLineStream()->streamLineFlow();
}

void CMDPropertyDrawNode::setLineStreamEnable(bool enable)
{
    if(isBatch())
        setBatchLineStreamEnable(enable);
    else
    {
        undo()->push(new CDrawPipeCommand(
                         object(), lineStreamEnable(), enable, (int)DrawPipeId::StreamLineFlow));
        object()->invalidate();
    }
    setModify(true);
}

QString CMDPropertyDrawNode::DILstream()
{
    return object()->pLineStream()->m_penManager.text();
}

void CMDPropertyDrawNode::setDILstream(const QString &stream)
{
    Q_UNUSED(stream);
    CMDPenManager* oldValue = new CMDPenManager;
    *oldValue = object()->pLineStream()->m_penManager;

    if (object()->pLineStream()->m_penManager.showDialog())
    {
        CMDPenManager* newValue = new CMDPenManager;
        *newValue = object()->pLineStream()->m_penManager;

        if(isBatch())
        {
            object()->pLineStream()->m_penManager = *oldValue;
            setBatchDILstream(newValue);
            delete oldValue;
            delete newValue;
        }
        else
        {
            undo()->push(new CDrawPipeCommand(
                             object(), (qulonglong)oldValue, (qulonglong)newValue, (int)DrawPipeId::StreamPen));
            object()->invalidate();
        }

        setModify(true);
    }
    else
        delete oldValue;

}

CMDPropertyDrawNode::PropertyDirection CMDPropertyDrawNode::streamDirection()
{
    return (CMDPropertyDrawNode::PropertyDirection)(object()->pLineStream()->direction());
}

void CMDPropertyDrawNode::setStreamDirection(CMDPropertyDrawNode::PropertyDirection value)
{
    if(isBatch())
        setBatchStreamDirection(value);
    else
    {
        object()->pLineStream()->setDirection((CMDNodeStream::Direction)value);
        object()->invalidate();
    }
    setModify(true);
}

int CMDPropertyDrawNode::streamSpeed()
{
    int interval = object()->pLineStream()->interval();

    switch (interval) {
    case 500:
        return 0;
    case 300:
        return 1;
    case 100:
        return 2;
    default:
        break;
    }

    return 0;
}

void CMDPropertyDrawNode::setStreamSpeed(int value)
{
    value = (value > 2) ? 2 : value;
    value = (value < 0) ? 0 : value;
    int interval = 500;

    switch (value) {
    case 0:
        interval = 500;
        break;
    case 1:
        interval = 300;
        break;
    case 2:
        interval = 100;
        break;
    default:
        break;
    }

    if(isBatch())
        setBatchStreamSpeed(interval);
    else
    {
        object()->pLineStream()->setInterval(interval);
        object()->invalidate();
    }

    setModify(true);
}


//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawNode::setBatchNodeState(CMDPropertyDrawNode::NodeState1 value)
{
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawNode* v = dynamic_cast<CMDDrawNode*>(o);
        v->setNodeState((NodeState)value);
    }
}

void CMDPropertyDrawNode::setBatchEditMode(CMDPropertyDrawNode::PropertyEditMode value)
{
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawNode* v = dynamic_cast<CMDDrawNode*>(o);
        v->setEditMode((EditMode)value);
    }
}

void CMDPropertyDrawNode::setBatchPipeBackGroundVisible(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawNode* v = dynamic_cast<CMDDrawNode*>(o);
        undo()->push(new CDrawPipeCommand(
                         v, v->pipeBackGroundVisible(), value, (int)DrawPipeId::PipeVisible));
        v->invalidate();
    }

    undo()->endMacro();
}

void CMDPropertyDrawNode::setBatchWidth(qreal value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawNode* v = dynamic_cast<CMDDrawNode*>(o);
        v->invalidate();
        undo()->push(new CDrawPipeCommand(
                         v, v->pipewidth(), value, (int)DrawPipeId::PipeWidth));
        v->invalidate();
    }

    undo()->endMacro();
}

void CMDPropertyDrawNode::setBatchBaseColor(const QColor &value)
{
    undo()->beginMacro("");

    ColorType type = getColorChanged(value, object()->pipeClr2());
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawNode* v = dynamic_cast<CMDDrawNode*>(o);
        QColor c = v->pipeClr2();

        undo()->push(new CDrawPipeCommand(
                         v, v->pipeClr2(), getChangedColor(value,c,type), (int)DrawPipeId::PipeBackColor));
        v->invalidate();
    }

    undo()->endMacro();
}

void CMDPropertyDrawNode::setBatchHighColor(QColor value)
{
    undo()->beginMacro("");

    ColorType type = getColorChanged(value, object()->pipeClr1());
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawNode* v = dynamic_cast<CMDDrawNode*>(o);
        QColor c = v->pipeClr1();

        undo()->push(new CDrawPipeCommand(
                         v, v->pipeClr1(), getChangedColor(value,c,type), (int)DrawPipeId::PipeHighColor));
        v->invalidate();
    }

    undo()->endMacro();
}

void CMDPropertyDrawNode::setBatchStreamLineVisible(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawNode* v = dynamic_cast<CMDDrawNode*>(o);
        undo()->push(new CDrawPipeCommand(
                         v, v->pLineStream()->streamLineVisible(), value, (int)DrawPipeId::StreamLineVisible));
        v->invalidate();
    }

    undo()->endMacro();
}

void CMDPropertyDrawNode::setBatchLineStreamEnable(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawNode* v = dynamic_cast<CMDDrawNode*>(o);
        undo()->push(new CDrawPipeCommand(
                         v, v->pLineStream()->streamLineFlow(), value, (int)DrawPipeId::StreamLineFlow));
        v->invalidate();
    }

    undo()->endMacro();
}

void CMDPropertyDrawNode::setBatchDILstream(CMDPenManager *value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawNode* v = dynamic_cast<CMDDrawNode*>(o);

        CMDPenManager* oldValue = new CMDPenManager;
        *oldValue = v->pLineStream()->m_penManager;
        CMDPenManager* newValue = new CMDPenManager;
        *newValue = *value;

        undo()->push(new CDrawPipeCommand(
                         v, (qulonglong)oldValue, (qulonglong)newValue, (int)DrawPipeId::StreamPen));
        v->invalidate();
    }

    undo()->endMacro();

}

void CMDPropertyDrawNode::setBatchStreamDirection(CMDPropertyDrawNode::PropertyDirection value)
{
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawNode* v = dynamic_cast<CMDDrawNode*>(o);
        v->pLineStream()->setDirection((CMDNodeStream::Direction)value);
        v->invalidate();
    }
}

void CMDPropertyDrawNode::setBatchStreamSpeed(int value)
{
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawNode* v = dynamic_cast<CMDDrawNode*>(o);
        v->pLineStream()->setInterval(value);
        v->invalidate();
    }
}
//////////////////////////////////////////////////////////////////////////


CMDPropertyDrawLine::CMDPropertyDrawLine(CMDDrawObj *parent):
    CMDPropertyDrawNode(parent)
{

}
CMDPropertyDrawPipe::CMDPropertyDrawPipe(CMDDrawObj *parent):
    CMDPropertyDrawNode(parent)
{

}

CMDPropertyDrawNode::ColorType CMDPropertyDrawNode::getColorChanged(const QColor &c1, const QColor &c2)
{
    int changeCount = 0;
    ColorType type = ColorType::Red;

    if(c1.red() != c2.red())
    {
        type = ColorType::Red;
        changeCount++;
    }
    if(c1.green() != c2.green())
    {
        type = ColorType::Green;
        changeCount++;
    }
    if(c1.blue() != c2.blue())
    {
        type = ColorType::Blue;
        changeCount++;
    }
    if(c1.alpha() != c2.alpha())
    {
        type = ColorType::Alpha;
        changeCount++;
    }

    if(changeCount > 1)
        return ColorType::All;

    return type;
}

QColor CMDPropertyDrawNode::getChangedColor(QColor value, QColor c, CMDPropertyDrawNode::ColorType type)
{
    switch(type)
    {
    case ColorType::Red:
        c.setRed(value.red());
        break;
    case ColorType::Green:
        c.setGreen(value.green());
        break;
    case ColorType::Blue:
        c.setBlue(value.blue());
        break;
    case ColorType::Alpha:
        c.setAlpha(value.alpha());
        break;
    case ColorType::All:
        c = value;
        break;
    default:
        break;
    }
    return c;
}
