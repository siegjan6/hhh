/*!
\author aGRw 2015.01.07
\section 运行版环境
*/
#ifndef RUN_H
#define RUN_H

#include <QMap>

#include "Environment.h"
#include "IRun.h"
#include "MDHMIActionManager.h"
#include "MDDrawAlarm.h"
#include "MDDrawLog.h"
#include "MDScriptEngine.h"


class CRun : public CEnvironment, public IRun, public IVarObject
{
public:
    CRun(IHMIForm* container);
    ~CRun();
/*!
\section fullScreen
*/
public:
    void enterFullScreen();
    void exitFullScreen();
private:
    Qt::WindowFlags m_flags;
/*!
\section private function
*/
private:
    void initForm();
    void drawFrame(QPainter *p, const QRectF& rect);
/*!
\section var
*/
public:
    //!初始化变量列表
    void initParameters();
    void onDataChanged(const QString& name, const QVariant& value);
    void analyseExpression();
    //!变量变化通知
    void loadDataChangedEvent(CMDExpInfo *exp);
/*!
\section virtual function
*/
public:
    void initalization() ;
    void invalidateObject(CMDDrawObj* obj) ;
    void mouseDown(Qt::MouseButton button, const QPointF& location) ;
    void mouseUp(Qt::MouseButton button, const QPointF& location) ;
    void mouseMove(Qt::MouseButton button, const QPointF& location) ;
    void paint(QPainter* p, const QRect& rect) ;
    void setRect(const QRect& value) ;
/*!
\section script
*/
public:

private:
    CMDScriptEngine m_scriptEngine;//!脚本引擎
    QList<CMDObjectWrapper*> m_wrapperList;//!封装对象列表
public:
    void initScript();
    void runScript();
    void releaseScript();
    void fireEvent(const QString& objName, const QString& eventName, QList<QVariant>& paras);
    bool m_hasInitScript;
private:
    void fillScriptObject(CMDDrawObj *obj);
/*!
\section mouse event
事件传递逻辑:
如果子控件supportGroupEvent=false,子控件触发事件;
如果子控件supportGroupEvent=true,递归寻找父控件触发事件。
显示选择框`提示信息逻辑同上。
by aGRw 2016.04.15
*/
public:
    void loadWidgetEnterEvent(CMDDrawObj* obj);
    void loadWidgetLeaveEvent(CMDDrawObj* obj);
    void loadFormLeaveEvent();
    void loadMouseEvent(Qt::MouseButton button, const QPointF& location, MouseType type);
private:
    void changeChildObject(CMDDrawObj* obj);
public:
    CMDDrawObj* frameObj(){return m_frameObj;}
private:
    CMDDrawObj* m_mouseChildObj;//!鼠标实际经过的子控件
    CMDDrawObj* m_mouseEventObj;//!会触发事件的控件
    CMDDrawObj* m_frameObj;//!显示选择框控件
/*!
\section action
*/
public:
    void timerEvent();
    void closeEvent();
/*!
\section notify
*/
private:
    QList<CMDDrawObj*> m_alarms;
    QList<CMDDrawObj*> m_logs;
public:
    void notify(NotifyType type);
private:
    void statisticsDrawObj();
/*!
\section security
*/
public:
    bool checkSecurity(MDActionType type);
//script event
public:
    void onLoad();
    void onUnload();
    void onMouseUp(const QPoint& point);
    void onMouseDown(const QPoint& point);
    void onMouseMove(const QPoint& point);
    void onEnter();
    void onLeave();
    void onGetFocuse();
    void onLostFocus();
    void onShow();
    void onHide();
public:
    void callJsFunc(const QString funcName, QList<QVariant>& paras);
    bool runDynamically(const QString& scriptCode);
/*!
\section language
*/
public:
    virtual void changeLanguage();
/*!
\section form scroll and scale
*/
public:
    virtual QPointF formScale();
    virtual QRectF calculactPaintSize(const QRectF& value);
/*!
\section create object
*/
public:
    //!动态创建对象
    bool createObject(const QString& origName, const QString& newName);
private:
    CMDDrawObj* cloneDrawObj(CMDDrawObj *orig);
};

#endif // RUN_H




