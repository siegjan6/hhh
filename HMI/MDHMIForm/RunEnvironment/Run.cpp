#include <QList>

#include "Run.h"
#include "MDDrawVector.h"
#include "MDHMIForm.h"
#include "MDExpInfo.h"
#include "MDParameter.h"
#include "DrawCreator.h"
#include "MDProjectExpert/IMDProjectExpert.h"
#include "HMIFormWrapper.h"

#include "MDDrawGroup.h"
#include "MDDrawCalendarWidget.h"
#include "MDDrawCheckBox.h"
#include "MDDrawComboBox.h"
#include "MDDrawDateTimeEdit.h"
#include "MDDrawList.h"
#include "MDDrawRadioButton.h"
#include "MDDrawSpinBox.h"
#include "MDDrawTextEdit.h"

//////////////////////////////////////////////////////////////////////////
CRun::CRun(IHMIForm *container) : CEnvironment(container)
{
    m_flags = 0;
    m_mouseChildObj = NULL;//!鼠标实际经过的子控件
    m_mouseEventObj = NULL;//!会触发事件的控件
    m_frameObj = NULL;//!显示选择框控件
    m_hasInitScript = false;
}
CRun::~CRun()
{
    releaseScript();
}
//////////////////////////////////////////////////////////////////////////
void CRun::enterFullScreen()
{
    if (formType() != FormType::Normal || !fullScreen())
        return;

    QWidget* w = window()->mdiSubWindow();
    w->setWindowFlags(Qt::FramelessWindowHint);
    w->showMaximized();
}
void CRun::exitFullScreen()
{
    if (formType() != FormType::Normal || !fullScreen())
        return;

    QWidget* w = window()->mdiSubWindow();
    w->setWindowFlags(m_flags);
    w->showNormal();
}
//////////////////////////////////////////////////////////////////////////
void CRun::initForm()
{
    QWidget* w = window();

    switch (formType())
    {
    case FormType::Modal:
        w->setWindowModality(Qt::WindowModal);
        w->setWindowFlags(Qt::Tool);
        break;
    case FormType::TopMost:
        w->setWindowFlags(Qt::WindowStaysOnTopHint);
        break;
    default:
        break;
    }

    if(window()->mdiSubWindow() != NULL)
        w = window()->mdiSubWindow();
    m_flags = w->windowFlags();

    //frame
    if (!hasFrame())
        w->setWindowFlags(m_flags | Qt::FramelessWindowHint);
    m_flags = w->windowFlags();

    QRect r = getScrollAndScaleRect(rect()).toRect();
    w->setGeometry(r);

    //fullScreen
    if (container()->framework()->isFullScreen())
        enterFullScreen();
}
void CRun::drawFrame(QPainter* p, const QRectF &rect)
{
    QRectF rf = rect;
    rf.adjust(-2, -2, 2, 2);
    p->setPen(Qt::black);
    p->drawRect(rf);

    rf.adjust(-1, -1, 1, 1);
    p->setPen(Qt::white);
    p->drawRect(rf);

    rf.adjust(-1, -1, 1, 1);
    p->setPen(Qt::black);
    p->drawRect(rf);
}
//////////////////////////////////////////////////////////////////////////
void CRun::initParameters()
{
    //form
    analyseExpression();

    for(int i = 0; i < objs()->count(); i++)
    {
        CMDDrawObj* obj = objs()->at(i);
        obj->replaceCustomProperty();
        obj->analyseExpression();
    }
}
void CRun::onDataChanged(const QString &name, const QVariant &value)
{
    QMap<QString, CMDParameter*>* dict = CMDExpConfig::paraDict();
    if (dict->contains(name))
    {
        CMDParameter* p = (*dict)[name];
        p->setValue(value);
        for(int i = 0; i < p->list()->count(); i++)
            p->list()->at(i)->onDataChanged();
    }
}
void CRun::analyseExpression()
{
    expManager()->setContainer(this);
    CMDDrawObj::analyseExternalExpression(expManager(), container()->framework());
}
void CRun::loadDataChangedEvent(CMDExpInfo *exp)
{
    switch (exp->index())
    {
    case 0:     //x
    {
        QRect r = rect();
        r.moveLeft(exp->decimalValue());
        setRect(r);
    }
        break;
    case 1:   //y
    {
        QRect r = rect();
        r.moveTop(exp->decimalValue());
        setRect(r);
    }
        break;
    case 2:   //width
    {
        QRect r = rect();
        r.setWidth(exp->decimalValue());
        setRect(r);
    }
        break;
    case 3:   //height
    {
        QRect r = rect();
        r.setHeight(exp->decimalValue());
        setRect(r);
    }
        break;
    case 4:   //visible
        if (exp->decimalValue() != 0)
            dynamic_cast<QWidget*>(window()->parent())->show();
        else
            dynamic_cast<QWidget*>(window()->parent())->hide();
        break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    {
        int i = exp->index() - 5;
        (*visibleLayers())[i] = exp->decimalValue();
        updateVisible();
    }
        break;
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    {
        int i = exp->index() - 15;
        (*lockedLayers())[i] = exp->decimalValue();
        updateLocked();
    }
        break;
    default:
        break;
    }
}
//////////////////////////////////////////////////////////////////////////
void CRun::initalization()
{
    initForm();

    CEnvironment::initalization();

    initParameters();
    statisticsDrawObj();

    initScript();
    runScript();

    openActions()->execute();
    onLoad();
}
void CRun::invalidateObject(CMDDrawObj *obj)
{
    int w = 5 + obj->widenWidth();
    QRectF rf = getScrollAndScaleRect(obj->bound());
    rf.adjust(-w, -w, w, w);
    setRectInvalidate(rf.united(rectInvalidate()));
}
void CRun::mouseDown(Qt::MouseButton button, const QPointF &location)
{
    QPointF pf = getRevertScrollAndScalePointF(location);
    CEnvironment::mouseDown(button, pf);

    if (button == Qt::LeftButton)
    {
        mouseDownActions()->execute();
        onMouseDown(pf.toPoint());
    }
    else if (button = Qt::RightButton)
        rightMouseDownActions()->execute();
}
void CRun::mouseUp(Qt::MouseButton button, const QPointF &location)
{
    QPointF pf = getRevertScrollAndScalePointF(location);
    CEnvironment::mouseUp(button, pf);

    if (button == Qt::LeftButton)
    {
        mouseUpActions()->execute();
        onMouseUp(pf.toPoint());
    }
    else if (button = Qt::RightButton)
        rightMouseUpActions()->execute();
}
void CRun::mouseMove(Qt::MouseButton button, const QPointF &location)
{
    QPointF pf = getRevertScrollAndScalePointF(location);
    CEnvironment::mouseMove(button, pf);

    onMouseMove(pf.toPoint());
}
void CRun::paint(QPainter *p, const QRect &rect)
{
    qreal scaleW = formScale().x();
    qreal scaleH = formScale().y();
    if(scaleW != 1 || scaleH != 1)
        p->scale(scaleW, scaleH);

    CEnvironment::paint(p, rect);

    CMDDrawObj* obj = frameObj();
    if (obj != NULL && obj->showSelectedFrame())
        drawFrame(p, obj->bound());
}
void CRun::setRect(const QRect &value)
{
    CEnvironment::setRect(value);

    QWidget* w = window();
    if(window()->mdiSubWindow() != NULL)
        w = window()->mdiSubWindow();
    QRect r = getScrollAndScaleRect(rect()).toRect();
    w->setGeometry(r);
}
//////////////////////////////////////////////////////////////////////////
void CRun::initScript()
{
    IMDProjectExpert* expert = container()->framework()->projectExpert();
    m_scriptEngine.init(false, expert ? expert->PID() : "");

    m_wrapperList.append(new CHMIFormWrapper(window(), "form"));
    foreach(CMDDrawObj* o, *objs())
        fillScriptObject(o);
    m_scriptEngine.addObjects(m_wrapperList);

    if (expert != NULL)
    {
        QList<CMDObjectWrapper*> wrappers;
        expert->projectScriptWrappers(wrappers);
        m_scriptEngine.addObjects(wrappers);
    }
}
void CRun::runScript()
{
    m_scriptEngine.run(scriptCode(),fullName());
    m_hasInitScript = true;
}
void CRun::releaseScript()
{
    m_scriptEngine.release();

    for(int i = 0; i < m_wrapperList.count(); i++)
        delete m_wrapperList.at(i);
    m_wrapperList.clear();
}

void CRun::fireEvent(const QString &objName, const QString &eventName, QList<QVariant> &paras)
{
    if (!m_hasInitScript)
        return;

    m_scriptEngine.fireEvent(objName, eventName, paras);
}
void CRun::fillScriptObject(CMDDrawObj *obj)
{
    m_wrapperList.append(CDrawCreator::createObjectWrapper(obj));
    if(obj->type() == DrawType::Group)
    {
        CMDDrawGroup* g = dynamic_cast<CMDDrawGroup*>(obj);
        foreach(CMDDrawObj* o, *g->objList())
            fillScriptObject(o);
    }
}
//////////////////////////////////////////////////////////////////////////
void CRun::loadWidgetEnterEvent(CMDDrawObj *obj)
{
    changeChildObject(obj);
}
void CRun::loadWidgetLeaveEvent(CMDDrawObj *obj)
{
    changeChildObject(NULL);
}
void CRun::loadFormLeaveEvent()
{
    changeChildObject(NULL);
    onLeave();
}
void CRun::loadMouseEvent(Qt::MouseButton button, const QPointF &location, MouseType type)
{
    CMDDrawObj* draw = NULL;
    int count = objs()->count();
    for (int i = count - 1; i >= 0; i--)
    {
        if (objs()->at(i)->canSelect(location))
        {
            draw = objs()->at(i);
            if (draw->type() == DrawType::Group)
                draw = static_cast<CMDDrawGroup*>(draw)->visibleChild(location);
            break;
        }
    }

    changeChildObject(draw);
    if (m_mouseEventObj != NULL)
        m_mouseEventObj->loadMouseEvent(button, location, type);
}
void CRun::changeChildObject(CMDDrawObj *obj)
{
    if (obj == m_mouseChildObj)
        return;

    //获取显示选择框控件
    CMDDrawObj* oldFrameObj = m_mouseChildObj;
    while (oldFrameObj != NULL)
    {
        if (oldFrameObj->showSelectedFrame())
            break;
        else
            oldFrameObj = oldFrameObj->groupParent();
    }
    CMDDrawObj* newFrameObj = obj;
    while (newFrameObj != NULL)
    {
        if (newFrameObj->showSelectedFrame())
            break;
        else
            newFrameObj = newFrameObj->groupParent();
    }

    if ((oldFrameObj != newFrameObj))
    {
        if (oldFrameObj != NULL && oldFrameObj->showSelectedFrame())
            oldFrameObj->invalidate();
        if (newFrameObj != NULL  && newFrameObj->showSelectedFrame())
            newFrameObj->invalidate();
    }

    m_frameObj = newFrameObj;

    //获取触发事件之控件
    CMDDrawObj* oldEventObj = m_mouseChildObj;
    while((oldEventObj != NULL) && oldEventObj->supportGroupEvent())
    {
        CMDDrawObj* group = oldEventObj->groupParent();
        if(group == NULL)
            break;
        else
            oldEventObj = group;
    }

    CMDDrawObj* newEventObj = obj;
    while((newEventObj != NULL) && newEventObj->supportGroupEvent())
    {
        CMDDrawObj* group = newEventObj->groupParent();
        if(group == NULL)
            break;
        else
            newEventObj = group;
    }

    if (oldEventObj != newEventObj)
    {
        if (oldEventObj != NULL)
            oldEventObj->loadMouseLeaveEvent();
        if (newEventObj != NULL)
            newEventObj->loadMouseEnterEvent();
    }

    //获取显示提示信息之控件
    CMDDrawObj* tipObj = obj;
    while((tipObj != NULL) && (tipObj->toolTip().isEmpty()))
        tipObj = tipObj->groupParent();

    if(tipObj != NULL)
    {
        //widget控件自动显示，不用管理
        if(tipObj->isVector() || tipObj->type() == DrawType::Group)
            static_cast<CMDDrawVector*>(tipObj)->showToolTip();
    }

    m_mouseChildObj = obj;
    m_mouseEventObj = newEventObj;
}
//////////////////////////////////////////////////////////////////////////
void CRun::timerEvent()
{
}
void CRun::closeEvent()
{
    closeActions()->execute();
    onUnload();
}
//////////////////////////////////////////////////////////////////////////
void CRun::notify(NotifyType type)
{
    switch (type)
    {
    case NotifyType::Alarm:
        for(int i = 0; i < m_alarms.count(); i++)
            m_alarms.at(i)->notify(type);
        break;
    case NotifyType::Log:
        for(int i = 0; i < m_logs.count(); i++)
            m_logs.at(i)->notify(type);
        break;
    default:
        break;
    }
}
void CRun::statisticsDrawObj()
{
    m_alarms.clear();
    m_logs.clear();

    for(int i = 0; i < objs()->count(); i++)
    {
        CMDDrawObj* obj = objs()->at(i);
        if (obj->type() == DrawType::Alarm)
            m_alarms.append(obj);
        else if (obj->type() == DrawType::Log)
            m_logs.append(obj);
    }
}
//////////////////////////////////////////////////////////////////////////
bool CRun::checkSecurity(MDActionType type)
{
    bool result = false;

    switch (type)
    {
    case MDActionType::window_Open:
    {
        CMDActionSPSession s(*openSecurity());
        result = container()->framework()->projectExpert()->checkSP_WithUI(&s);
    }
        break;
    case MDActionType::window_Close:
    {
        CMDActionSPSession s(*closeSecurity());
        result = container()->framework()->projectExpert()->checkSP_WithUI(&s);
    }
        break;
    default:
        break;
    }

    return result;
}
void CRun::onLoad()
{
    QList<QVariant> paras;
    fireEvent("form", "onLoad", paras);
}
void CRun::onUnload()
{
    QList<QVariant> paras;
    fireEvent("form", "onUnload", paras);
}
void CRun::onMouseUp(const QPoint &point)
{
    QList<QVariant> paras;
    paras.append(point.x());
    paras.append(point.y());
    fireEvent("form", "onMouseUp", paras);
}
void CRun::onMouseDown(const QPoint &point)
{
    QList<QVariant> paras;
    paras.append(point.x());
    paras.append(point.y());
    fireEvent("form", "onMouseDown", paras);
}
void CRun::onMouseMove(const QPoint &point)
{
    QList<QVariant> paras;
    paras.append(point.x());
    paras.append(point.y());
    fireEvent("form", "onMouseMove", paras);
}
void CRun::onEnter()
{
    QList<QVariant> paras;
    fireEvent("form", "onEnter", paras);
}
void CRun::onLeave()
{
    QList<QVariant> paras;
    fireEvent("form", "onLeave", paras);
}
void CRun::onGetFocuse()
{
    QList<QVariant> paras;
    fireEvent("form", "onGetFocus", paras);
}
void CRun::onLostFocus()
{
    QList<QVariant> paras;
    fireEvent("form", "onLostFocus", paras);
}
void CRun::onShow()
{
    QList<QVariant> paras;
    fireEvent("form", "onShow", paras);
}
void CRun::onHide()
{
    QList<QVariant> paras;
    fireEvent("form", "onHide", paras);
}
void CRun::callJsFunc(const QString funcName, QList<QVariant>& paras)
{
    if (!m_hasInitScript)
        return;

    m_scriptEngine.callFunc(funcName, paras);
}
bool CRun::runDynamically(const QString& scriptCode)
{
    if (!m_hasInitScript)
        return false;

    return m_scriptEngine.runDynamically(scriptCode);
}
//////////////////////////////////////////////////////////////////////////
void CRun::changeLanguage()
{
    CEnvironment::changeLanguage();

    QString title = CMDMultiLanguage::instance()->value(text());
    if (title.isEmpty())
        title = QFileInfo(fullName()).baseName();
    window()->setWindowTitle(title);
}
//////////////////////////////////////////////////////////////////////////
QPointF CRun::formScale()
{
    return window()->framework()->formScale();
}
QRectF CRun::calculactPaintSize(const QRectF &value)
{
    return getRevertScrollAndScaleRect(value);
}
//////////////////////////////////////////////////////////////////////////
bool CRun::createObject(const QString &origName, const QString &newName)
{
    if(nameManager()->containsName(newName))
        return false;
    CMDDrawObj* obj = findObject(origName);
    if(obj == NULL)
        return false;

    CMDDrawObj* newObj = cloneDrawObj(obj);

    newObj->setName(newName);
    if(newObj->type() == DrawType::Group || newObj->type() == DrawType::Combine)
    {
        CMDDrawMulti* m = dynamic_cast<CMDDrawMulti*>(newObj);
        CObjName* n = dynamic_cast<CObjName*>(nameManager());
        for(int i = 0; i < m->objList()->count(); i++)
            n->createName(m->objList()->at(i), true);
    }

    objs()->append(newObj);
    newObj->undoAdd();

    //var
    newObj->replaceCustomProperty();
    newObj->analyseExpression();

    //script
    CMDObjectWrapper* wrapper = CDrawCreator::createObjectWrapper(newObj);
    m_scriptEngine.addObject(wrapper);
    m_wrapperList.append(wrapper);

    return true;
}
CMDDrawObj *CRun::cloneDrawObj(CMDDrawObj *orig)
{
    CMDDrawObj* newObj = orig->clone();
    newObj->setParent(container());
    newObj->loadInitalizationEvent();
    newObj->changeLanguage();
    newObj->invalidate();

    return newObj;
}
//////////////////////////////////////////////////////////////////////////

