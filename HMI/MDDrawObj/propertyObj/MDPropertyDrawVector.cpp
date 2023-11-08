#include "MDPropertyDrawVector.h"
#include "MDRTDBManagerUI/IMDRTDBManagerUI.h"
#include "FillDialog.h"
#include "DynamicColor.h"
#include "DynamicColorDialog.h"
#include "MDActionUI.h"

CMDPropertyDrawVector::CMDPropertyDrawVector(CMDDrawObj *parent) :
    CMDPropertyDrawObj(parent)
{
    insertTranslateMap("GUID_ActionEvent", tr("事件动作"));

    insertTranslateMap("rotateAngle", tr("旋转角度"));
    insertTranslateMap("rotatePoint", tr("旋转中心点"));
    insertTranslateMap("rotatePointPos", tr("旋转中心点坐标"));
    insertTranslateMap("shear", tr("倾斜"));
    insertTranslateMap("brush", tr("背景"));
    insertTranslateMap("line", tr("线条"));
    insertTranslateMap("dynamicBrush", tr("背景切换"));
    insertTranslateMap("dynamicLine", tr("线条切换"));
    insertTranslateMap("fill", tr("填充"));
    insertTranslateMap("mouseDown", tr("鼠标按下事件"));
    insertTranslateMap("mouseUp", tr("鼠标弹起事件"));
    insertTranslateMap("rightMouseDown", tr("鼠标右键按下事件"));
    insertTranslateMap("rightMouseUp", tr("鼠标右键弹起事件"));
    insertTranslateMap("inputVariable", tr("输入变量"));
    insertTranslateMap("supportGroupEvent", tr("事件传递到父控件"));
    insertTranslateMap("supportHidedEvent", tr("隐藏时仍触发事件"));
}
//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawVector::setRotateAngle(qreal value)
{
    if(isBatch())
        setBatchRotateAngle(value);
    else
        undo()->push(new CMDDrawVectorCommand(vector(), vector()->rotateAngle(), value, (int)DrawVectorId::Angle));
    setModify(true);
}
qreal CMDPropertyDrawVector::rotateAngle()
{
    return vector()->rotateAngle();
}
void CMDPropertyDrawVector::setRotatePoint(const QPointF &value)
{
    if(isBatch())
        setBatchRotatePoint(value);
    else
        undo()->push(new CMDDrawVectorCommand(
            vector(), vector()->rotatePoint(), value, (int)DrawVectorId::RotatePoint));
    setModify(true);
}
QPointF CMDPropertyDrawVector::rotatePoint()
{
    return vector()->rotatePoint();
}
void CMDPropertyDrawVector::setRotatePointPos(const QPointF &value)
{
    if(isBatch())
        setBatchRotatePointPos(value);
    else
        undo()->push(new CMDDrawVectorCommand(
            vector(), vector()->rotatePointPos(), value, (int)DrawVectorId::RotatePointPos));
    setModify(true);
}
QPointF CMDPropertyDrawVector::rotatePointPos()
{
    return vector()->rotatePointPos();
}
void CMDPropertyDrawVector::setShear(qreal value)
{
    if(isBatch())
        setBatchShear(value);
    else
        undo()->push(new CMDDrawVectorCommand(vector(), vector()->shear(), value, (int)DrawVectorId::Shear));
    setModify(true);
}
qreal CMDPropertyDrawVector::shear()
{
    return vector()->shear();
}
void CMDPropertyDrawVector::setBrush(const QString &/*value*/)
{
    CMDBrushManager* oldValue = new CMDBrushManager;
    *oldValue = *vector()->brush();

    if (vector()->brush()->showDialog())
    {          
        CMDBrushManager* newValue = new CMDBrushManager;
        *newValue = *vector()->brush();

        if(isBatch())
        {
            *vector()->brush() = *oldValue;
            setBatchBrush(newValue);
            delete oldValue;
            delete newValue;
        }
        else
        {
            undo()->push(new CMDDrawVectorCommand(
                vector(), (qulonglong)oldValue, (qulonglong)newValue, (int)DrawVectorId::Brush));
            vector()->invalidate();
        }

        setModify(true);
    }
    else
        delete oldValue;
}
QString CMDPropertyDrawVector::brush()
{
    return vector()->brush()->text();
}
void CMDPropertyDrawVector::setLine(const QString &/*value*/)
{
    CMDPenManager* oldValue = new CMDPenManager;
    *oldValue = *vector()->pen();

    if (vector()->pen()->showDialog())
    {
        CMDPenManager* newValue = new CMDPenManager;
        *newValue = *vector()->pen();

        if(isBatch())
        {
            *vector()->pen() = *oldValue;
            setBatchLine(newValue);
            delete oldValue;
            delete newValue;
        }
        else
        {
            undo()->push(new CMDDrawVectorCommand(
                    vector(), (qulonglong)oldValue, (qulonglong)newValue, (int)DrawVectorId::Pen));
            vector()->invalidate();
        }

        setModify(true);
    }
    else
        delete oldValue;
}
QString CMDPropertyDrawVector::line()
{
    return vector()->pen()->text();
}
void CMDPropertyDrawVector::setDynamicBrush(const QString &value)
{
    CDynamicColorManager* oldValue = new CDynamicColorManager(DynamicType::Brush);
    *oldValue = *vector()->dynamicBrush();

    CDynamicColorDialog dlg(container());
    dlg.init(vector()->dynamicBrush());
    if (dlg.exec() == QDialog::Accepted)
    {
        CDynamicColorManager* newValue = new CDynamicColorManager(DynamicType::Brush);
        *newValue = *dlg.colors();

        if(isBatch())
        {
            setBatchDynamicBrush(newValue);
            delete oldValue;
            delete newValue;
        }
        else
            undo()->push(new CMDDrawVectorCommand(
                vector(), (qulonglong)oldValue, (qulonglong)newValue, (int)DrawVectorId::DynamicBrush));

        setModify(true);
    }
    else
        delete oldValue;
}
QString CMDPropertyDrawVector::dynamicBrush()
{
    return QString::number(vector()->dynamicBrush()->list()->count());
}
void CMDPropertyDrawVector::setDynamicLine(const QString &value)
{
    CDynamicColorManager* oldValue = new CDynamicColorManager(DynamicType::Pen);
    *oldValue = *vector()->dynamicPen();

    CDynamicColorDialog dlg(container());
    dlg.init(vector()->dynamicPen());
    if (dlg.exec() == QDialog::Accepted)
    {
        CDynamicColorManager* newValue = new CDynamicColorManager(DynamicType::Pen);
        *newValue = *dlg.colors();

        if(isBatch())
        {
            setBatchDynamicBrush(newValue);
            delete oldValue;
            delete newValue;
        }
        else
            undo()->push(new CMDDrawVectorCommand(
                vector(), (qulonglong)oldValue, (qulonglong)newValue, (int)DrawVectorId::DynamicPen));

        setModify(true);
    }
    else
        delete oldValue;
}
QString CMDPropertyDrawVector::dynamicLine()
{
    return QString::number(vector()->dynamicPen()->list()->count());
}
void CMDPropertyDrawVector::setFill(const QString &/*value*/)
{
    CMDFill* oldValue = new CMDFill;
    *oldValue = *vector()->fill();

    CFillDialog dlg(container());
    dlg.init(vector()->fill());
    if (dlg.exec() == QDialog::Accepted)
    {
        CMDFill* newValue = new CMDFill;
        *newValue = *dlg.fill();

        if(isBatch())
        {
            setBatchFill(newValue);
            delete oldValue;
            delete newValue;
        }
        else
            undo()->push(new CMDDrawVectorCommand(
                vector(), (qulonglong)oldValue, (qulonglong)newValue, (int)DrawVectorId::Fill));

        setModify(true);
    }
    else
        delete oldValue;
}
QString CMDPropertyDrawVector::fill()
{
    return vector()->fill()->text();
}
void CMDPropertyDrawVector::setMouseDownAction(const QString &value)
{
    changeActionsId((int)DrawVectorId::MouseDownAction);
}
QString CMDPropertyDrawVector::mouseDownAction()
{
    QString str;
    str.setNum(vector()->mouseDownActions()->list()->count());
    return str;
}
void CMDPropertyDrawVector::setMouseUpAction(const QString &value)
{
    changeActionsId((int)DrawVectorId::MouseUpAction);
}
QString CMDPropertyDrawVector::mouseUpAction()
{
    QString str;
    str.setNum(vector()->mouseUpActions()->list()->count());
    return str;
}
void CMDPropertyDrawVector::setRightMouseDownAction(const QString &value)
{
    changeActionsId((int)DrawVectorId::RightMouseDownAction);
}
QString CMDPropertyDrawVector::rightMouseDownAction()
{
    QString str;
    str.setNum(vector()->rightMouseDownActions()->list()->count());
    return str;
}
void CMDPropertyDrawVector::setRightMouseUpAction(const QString &value)
{
    changeActionsId((int)DrawVectorId::RightMouseUpAction);
}
QString CMDPropertyDrawVector::rightMouseUpAction()
{
    QString str;
    str.setNum(vector()->rightMouseUpActions()->list()->count());
    return str;
}
void CMDPropertyDrawVector::setInputVariable(const QString &value)
{
    IMDRTDBManagerUI* varUI = vector()->environment()->studioProject()->rtdbManagerUI();
    QStringList varNames;
    bool result = varUI->selectVariables(varNames);

    if (result)
    {
        QString var;
        if (varNames.count() > 0)
            var = varNames.at(0);

        undo()->push(new CMDDrawVectorCommand(
                vector(), vector()->inputVariable(), var, (int)DrawVectorId::InputVariable));
        setModify(true);
    }
}
QString CMDPropertyDrawVector::inputVariable()
{
    return vector()->inputVariable();
}
void CMDPropertyDrawVector::setSupportGroupEvent(bool value)
{
    if(isBatch())
        setBatchSupportGroupEvent(value);
    else
        undo()->push(new CMDDrawObjCommand(vector(), vector()->supportGroupEvent(), value, (int)DrawObjId::SupportGroupEvent));
    setModify(true);
}
bool CMDPropertyDrawVector::supportGroupEvent()
{
    return vector()->supportGroupEvent();
}
void CMDPropertyDrawVector::setSupportHidedEvent(bool value)
{
    if(isBatch())
        setBatchSupportHidedEvent(value);
    else
        vector()->setSupportHidedEvent(value);
    setModify(true);
}
bool CMDPropertyDrawVector::supportHidedEvent()
{
    return vector()->supportHidedEvent();
}

//////////////////////////////////////////////////////////////////////////
CMDDrawVector *CMDPropertyDrawVector::vector()
{
    return dynamic_cast<CMDDrawVector*>(m_object);
}
//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawVector::resetAttribute()
{
    QVariant v(3);
    propertyGrid()->setPropertyAttribute("rotatePoint", "decimals", v);
}
void CMDPropertyDrawVector::changeActionsId(int id)
{
    CMDHMIActionManager* manager = getAction(vector(), id);

    CMDHMIActionManager* oldValue = new CMDHMIActionManager;
    *oldValue = *manager;

    CMDActionUI action;
    action.setStudioInterface(vector()->parent()->framework()->studioProject());

    QStringList source(*manager->list());
    QStringList target;
    if (action.selectActions(source, target))
    {
        CMDHMIActionManager* newValue = new CMDHMIActionManager;
        newValue->list()->append(target);

        if(isBatch())
        {
            batchChangeActionsId(newValue, id);
            delete oldValue;
            delete newValue;
        }
        else
            undo()->push(new CMDDrawVectorCommand(
                vector(), (qulonglong)oldValue, (qulonglong)newValue, id));

        setModify(true);
    }
    else
        delete oldValue;
}
CMDHMIActionManager *CMDPropertyDrawVector::getAction(CMDDrawVector* vector, int id)
{
    switch (id)
    {
    case (int)DrawVectorId::MouseDownAction:
        return vector->mouseDownActions();
    case (int)DrawVectorId::MouseUpAction:
        return vector->mouseUpActions();
    case (int)DrawVectorId::RightMouseDownAction:
        return vector->rightMouseDownActions();
    case (int)DrawVectorId::RightMouseUpAction:
        return vector->rightMouseUpActions();
    default:
        break;
    }

    return NULL;
}
//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawVector::resetBatchProperty(CPropertyBrowser *browser)
{
    if(!isBatch())
        return;

    CMDPropertyDrawObj::resetBatchProperty(browser);

    //隐藏属性，不应该放这里的;但是放子类不好处理
    bool hideGroup = false;
    bool hideNode = false;
    foreach(CMDDrawObj* obj, *batchObjs())
    {
        switch (obj->type())
        {
        case DrawType::Group:
        case DrawType::Gauge:
            hideGroup = true;
            break;
        case DrawType::StraightLine:
        case DrawType::Pipe:
            hideNode = true;
            break;
        default:
            break;
        }
    }

    if(hideGroup)
    {
        browser->setSinglePropertyHide("DILbrush", true);
        browser->setSinglePropertyHide("DILline", true);
        browser->setSinglePropertyHide("DILdynamicBrush", true);
        browser->setSinglePropertyHide("DILdynamicLine", true);
        browser->setSinglePropertyHide("DILfill", true);
    }
    if(hideNode)
    {
        browser->setSinglePropertyHide("DILbrush", true);
        browser->setSinglePropertyHide("DILdynamicBrush", true);
        browser->setSinglePropertyHide("DILfill", true);
    }
}
void CMDPropertyDrawVector::setBatchRotateAngle(qreal value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawVector* v = dynamic_cast<CMDDrawVector*>(o);
        undo()->push(new CMDDrawVectorCommand(v, v->rotateAngle(), value, (int)DrawVectorId::Angle));
    }

    undo()->endMacro();
    invalidateBatchControlPoint();
}
void CMDPropertyDrawVector::setBatchRotatePoint(const QPointF &value)
{
    undo()->beginMacro("");

    PointType type = getPointChange(value, vector()->rotatePoint());
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawVector* v = dynamic_cast<CMDDrawVector*>(o);
        QPointF pf = v->rotatePoint();
        switch(type)
        {
        case PointType::X:
            pf.setX(value.x());
            break;
        case PointType::Y:
            pf.setY(value.y());
            break;
        default:
            break;
        }

        undo()->push(new CMDDrawVectorCommand(
                    v, v->rotatePoint(), pf, (int)DrawVectorId::RotatePoint));
    }

    undo()->endMacro();
}
void CMDPropertyDrawVector::setBatchRotatePointPos(const QPointF &value)
{
    undo()->beginMacro("");

    PointType type = getPointChange(value, vector()->rotatePointPos());
    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawVector* v = dynamic_cast<CMDDrawVector*>(o);
        QPointF pf = v->rotatePointPos();
        switch(type)
        {
        case PointType::X:
            pf.setX(value.x());
            break;
        case PointType::Y:
            pf.setY(value.y());
            break;
        default:
            break;
        }
        undo()->push(new CMDDrawVectorCommand(
                    v, v->rotatePointPos(), pf, (int)DrawVectorId::RotatePointPos));
    }

    undo()->endMacro();
}
void CMDPropertyDrawVector::setBatchShear(qreal value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawVector* v = dynamic_cast<CMDDrawVector*>(o);
        undo()->push(new CMDDrawVectorCommand(v, v->shear(), value, (int)DrawVectorId::Shear));
    }

    undo()->endMacro();
    invalidateBatchControlPoint();
}
void CMDPropertyDrawVector::setBatchBrush(CMDBrushManager *value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawVector* v = dynamic_cast<CMDDrawVector*>(o);

        CMDBrushManager* oldValue = new CMDBrushManager;
        *oldValue = *v->brush();
        CMDBrushManager* newValue = new CMDBrushManager;
        *newValue =*value;

        undo()->push(new CMDDrawVectorCommand(
            v, (qulonglong)oldValue, (qulonglong)newValue, (int)DrawVectorId::Brush));
        v->invalidate();
    }

    undo()->endMacro();
}
void CMDPropertyDrawVector::setBatchLine(CMDPenManager *value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawVector* v = dynamic_cast<CMDDrawVector*>(o);

        CMDPenManager* oldValue = new CMDPenManager;
        *oldValue = *v->pen();
        CMDPenManager* newValue = new CMDPenManager;
        *newValue =*value;

        undo()->push(new CMDDrawVectorCommand(
                v, (qulonglong)oldValue, (qulonglong)newValue, (int)DrawVectorId::Pen));
        v->invalidate();
    }

    undo()->endMacro();
}
void CMDPropertyDrawVector::setBatchDynamicBrush(CDynamicColorManager *value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawVector* v = dynamic_cast<CMDDrawVector*>(o);

        CDynamicColorManager* oldValue = new CDynamicColorManager(DynamicType::Brush);
        *oldValue = *v->dynamicBrush();
        CDynamicColorManager* newValue = new CDynamicColorManager(DynamicType::Brush);
        *newValue =*value;

        undo()->push(new CMDDrawVectorCommand(
            v, (qulonglong)oldValue, (qulonglong)newValue, (int)DrawVectorId::DynamicBrush));
        v->invalidate();
    }

    undo()->endMacro();
}
void CMDPropertyDrawVector::setBatchDynamicLine(CDynamicColorManager *value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawVector* v = dynamic_cast<CMDDrawVector*>(o);

        CDynamicColorManager* oldValue = new CDynamicColorManager(DynamicType::Pen);
        *oldValue = *v->dynamicPen();
        CDynamicColorManager* newValue = new CDynamicColorManager(DynamicType::Pen);
        *newValue =*value;

        undo()->push(new CMDDrawVectorCommand(
            v, (qulonglong)oldValue, (qulonglong)newValue, (int)DrawVectorId::DynamicPen));
        v->invalidate();
    }

    undo()->endMacro();
}
void CMDPropertyDrawVector::setBatchFill(CMDFill* value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawVector* v = dynamic_cast<CMDDrawVector*>(o);

        CMDFill* oldValue = new CMDFill;
        *oldValue = *v->fill();
        CMDFill* newValue = new CMDFill;
        *newValue =*value;

        undo()->push(new CMDDrawVectorCommand(
            v, (qulonglong)oldValue, (qulonglong)newValue, (int)DrawVectorId::Fill));
    }

    undo()->endMacro();
}
void CMDPropertyDrawVector::batchChangeActionsId(CMDHMIActionManager *value, int id)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
    {
        CMDDrawVector* v = dynamic_cast<CMDDrawVector*>(o);

        CMDHMIActionManager* oldValue = new CMDHMIActionManager;
        *oldValue = *getAction(v, id);
        CMDHMIActionManager* newValue = new CMDHMIActionManager;
        *newValue = *value;

        undo()->push(new CMDDrawVectorCommand(
            v, (qulonglong)oldValue, (qulonglong)newValue, id));
    }

    undo()->endMacro();
}
void CMDPropertyDrawVector::setBatchSupportGroupEvent(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, *batchObjs())
        undo()->push(new CMDDrawObjCommand(o, o->supportGroupEvent(), value, (int)DrawObjId::SupportGroupEvent));

    undo()->endMacro();
}
void CMDPropertyDrawVector::setBatchSupportHidedEvent(bool value)
{
    foreach(CMDDrawObj* o, *batchObjs())
        o->setSupportHidedEvent(value);
}
//////////////////////////////////////////////////////////////////////////


