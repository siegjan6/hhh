/*!
\author aGRw 2014.09.10
\section 运行环境
*/
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QtGui>
#include <QVector>
#include <QtPrintSupport/QPrinter>

#include "DrawObjDefine.h"
#include "IEnvironment.h"
#include "MDDrawObj.h"
#include "MDColorManager/MDBrushManager.h"
#include "IDrawEnvironment.h"
#include "MDHMIActionManager.h"
#include "MDFormDefine.h"
#include "MDActionSP.h"
#include "MDMultiLanguage.h"
#include "ObjName.h"
#include "MDDrawGroup.h"

class CMDHMIForm;

class CEnvironment : public IEnvironment,
        public IDrawEnvrionment
{
public:
    CEnvironment(IHMIForm* container);
    virtual ~CEnvironment();
/*!
\section public function
*/
public:
    bool save();
    bool load(const QString& fullName);
    void resizeEvent(const QSize &size);
    CMDDrawObj *findObject(const QString& name);
    //将widget控件的parent置空，防止删除时触发wiget事件
    void setWidgetParentNull();
/*!
\section virtual function
*/
public:
    virtual void mouseMove(Qt::MouseButton button, const QPointF& location);
    virtual void mouseDown(Qt::MouseButton button, const QPointF& location);
    virtual void mouseUp(Qt::MouseButton button, const QPointF& location);
    virtual void loadMouseEvent(Qt::MouseButton /*button*/, const QPointF& /*location*/, MouseType /*type*/){}
    virtual void keyPressEvent(QKeyEvent */*e*/){}
    virtual void initalization();
/*!
\section private function
*/
private:
    CMDDrawObj *findGroupObject(const QString& name, CMDDrawGroup* parent);
/*!
\section property
*/
public:
    QList<CMDDrawObj*>* objs(){return &m_objs;}
    IHMIForm* container(){return m_container;}
    virtual void setRect(const QRect& value);
    QRect rect(){return m_rect;}
    CMDHMIForm* window(){return m_window;}
    void setFormType(FormType value){m_formType = value;}
    FormType formType(){return m_formType;}
    void setHasFrame(bool value){m_hasFrame = value;}
    bool hasFrame(){return m_hasFrame;}
    void setFullScreen(bool value){m_fullScreen = value;}
    bool fullScreen(){return m_fullScreen;}
    virtual void setFullName(const QString& value);
    QString fullName(){return m_fullName;}
    void setText(const QString& value){m_text = value;}
    QString text(){return m_text;}
    IObjName* nameManager(){return &m_nameManager;}
private:
    QList<CMDDrawObj*> m_objs;  //!控件列表
    IHMIForm* m_container;  //!窗体
    QRect m_rect; //!设计尺寸,窗体最大化或全屏状态下实际尺寸和设计尺寸不一致。
    CMDHMIForm* m_window;
    FormType m_formType;//!窗体类型
    bool m_hasFrame; //!窗体边框
    bool m_fullScreen;//!全屏
    QString m_fullName;//!窗体全路径名称
    QString m_text;//!窗体文本
    QRectF m_paintSize;//!窗体实际尺寸
    CObjName m_nameManager;
/*!
\section paint
*/
public:
    virtual void paint(QPainter* p, const QRect& rect);
    virtual void drawBackground(QPainter* p);
protected:
    virtual bool isInvalidateObject(CMDDrawObj* obj, const QRect& invalidateRect);
private:
    void environmentDrawBackgound(QPainter* p);
    void environmentPaint(QPainter* p, const QRect &rect);
/*!
\section refresh
*/
public:
    virtual void invalidateObject(CMDDrawObj* /*obj*/){}
    virtual void timeInvalidate();
protected:
    void setRectInvalidate(const QRectF& value){m_rectInvalidate = value;}
    QRectF rectInvalidate(){return m_rectInvalidate;}
private:
    QRectF m_rectInvalidate;
/*!
\section common
*/
public:
    virtual void serialize(QJsonObject &json);
    virtual void deserialize(const QJsonObject &json);
private:
    void serializeHead(QJsonObject &json);
/*!
\section brush
*/
public:
    CMDBrushManager* brush(){return &m_brushManager;}
    void generateContent();
private:
    CMDBrushManager m_brushManager;//!画刷
    QPainterPath m_path;
private:
    void resetPath(QPainterPath& path);
/*!
\section layer
*/
public:
    virtual void objectCanSelectChanged(CMDDrawObj* obj);
public:
    void setDefaultLayer(int value);
    int defaultLayer(){return m_defaultLayer;}
    QVector<bool>* visibleLayers(){return &m_visibleLayers;}
    QVector<bool>* lockedLayers(){return &m_lockedLayers;}
    void updateVisible();
    void updateLocked();
private:
    int m_defaultLayer;//!当前层
    int m_layerCount;
    QVector<bool> m_visibleLayers;//!层的可见信息
    QVector<bool> m_lockedLayers;//!层的锁定信息
/*!
\section print
*/
public:
    //!打印边框参数
    qint32 m_printTop;
    qint32 m_printBottom;
    qint32 m_printLeft;
    qint32 m_printRight;
    qint32 m_printHeader;
    qint32 m_printFooter;
    QString m_printLeftHeader;
    QString m_printMiddleHeader;
    QString m_printRightHeader;
    QString m_printLeftFooter;
    QString m_printMiddleFooter;
    QString m_printRightFooter;
    QString m_printDateFormat;
    QString m_printTimeFormat;
private:
    QString m_showPrintLeftHeader;
    QString m_showPrintMiddleHeader;
    QString m_showPrintRightHeader;
    QString m_showPrintLeftFooter;
    QString m_showPrintMiddleFooter;
    QString m_showPrintRightFooter;
    QString m_showPrintDateFormat;
    QString m_showPrintTimeFormat;
public:
    void print(bool showPrintDialog);
    void directPrint(QPrinter* printer);
    void setPrintMargins(QPrinter* printer);
    void printRegion(const QRect& rect, bool showPrintDialog, bool isFullPage);
    void printObject(const QString& name, bool showPrintDialog, bool isFullPage);
    void printConfig();
private:
    void printFun(QPrinter* printer, const QRect& rect, bool isFullPage = false, CMDDrawObj* obj = NULL);
    //!将打印自定义字符串转换为实际字符串
    QString replacePrintString(const QString& source, int count, int page);
/*!
\section script
*/
public:
    //设置脚本信息
    void fillObjectToScript(QList<QPair<QString, QString>>& infos);
private:
    void fillObjectToScript(CMDDrawObj* obj, QList<QPair<QString, QString>>& infos);
public:
    void setScriptCode(const QString& value){m_scriptCode = value;}
    QString scriptCode(){return m_scriptCode;}
private:
    QString m_scriptCode;
/*!
\section IDrawEnvrionment
*/
public:
    QString projectPath();
    bool isStudioMode();
    IMDDataArchives *dataArchives();
    IMDProjectExpert* projectExpert();
    IStudioInterface *studioProject();
public:
    void update(const QRect& rect);
/*!
\section action
*/
public:
    CMDHMIActionManager* mouseDownActions(){return &m_mouseDownActions;}
    CMDHMIActionManager* mouseUpActions(){return &m_mouseUpActions;}
    CMDHMIActionManager* rightMouseDownActions(){return &m_rightMouseDownActions;}
    CMDHMIActionManager* rightMouseUpActions(){return &m_rightMouseUpActions;}
    CMDHMIActionManager* openActions(){return &m_openActions;}
    CMDHMIActionManager* closeActions(){return &m_closeActions;}
private:
    CMDHMIActionManager m_mouseDownActions;//!左键按下动作
    CMDHMIActionManager m_mouseUpActions;//!左键弹起动作
    CMDHMIActionManager m_rightMouseDownActions;//!右键按下动作
    CMDHMIActionManager m_rightMouseUpActions;//!右键弹起动作
    CMDHMIActionManager m_openActions;//!窗体打开动作
    CMDHMIActionManager m_closeActions;//!窗体关闭动作
/*!
\section security
*/
public:
    void setOpenSecurity(CMDActionSP* value){m_openSecurity = *value;}
    CMDActionSP* openSecurity(){return & m_openSecurity;}
    void setCloseSecurity(CMDActionSP* value){m_closeSecurity = *value;}
    CMDActionSP* closeSecurity(){return & m_closeSecurity;}
private:
    CMDActionSP m_openSecurity;//!安全策略-打开
    CMDActionSP m_closeSecurity;//!安全策略-关闭
/*!
\section var
*/
public:
    static QList<QStringList> expNames();
public:
    CMDExpManager *expManager(){return &m_expManager;}
private:
    CMDExpManager m_expManager;
public:
    void initExpManager();
/*!
\section language
*/
public:
    virtual void changeLanguage();
/*!
\section form scroll and scale
*/
public:
    virtual QPointF formScale() = 0;
    virtual QRectF calculactPaintSize(const QRectF& value){return value;}
public:
    QPointF getRevertScrollAndScalePointF(const QPointF& point);
protected:
    QRectF getScrollAndScaleRect(const QRectF& rect);
    QRectF getRevertScrollAndScaleRect(const QRectF& rect);
};

#endif // ENVIRONMENT_H
