/*!
\author aGRw 2014.04.29
\section 绘图基类
*/
#ifndef MDDRAWOBJ_H
#define MDDRAWOBJ_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWOBJ_LIBRARY)
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QPainter>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "DrawObjDefine.h"
#include "IHMIForm.h"
#include "IDrawData.h"
#include "MDExpInfo.h"
#include "IDrawEnvironment.h"
#include "IVarObject.h"
#include "MDMultiLanguage/MDMultiLanguage.h"
#include "MDColorManager/MDColorManager.h"

class CMDDrawMulti;
class CMDDrawGroup;
class CDrawObjData;
class CMDPropertyDrawObj;


class MDDRAWOBJSHARED_EXPORT CMDDrawObj : public IVarObject
{
public:
    CMDDrawObj();
    virtual ~CMDDrawObj();
    CMDDrawObj(const CMDDrawObj& other) = delete;
    void operator=(const CMDDrawObj& other) = delete;
    //!C++中不能在构造和析构函数中调用虚函数,所以一些需要调用虚函数的初始化要放在这里
    virtual void initCreate();

private:
    CDrawObjData* m_object;
/*!
\section property
*/
public:
    virtual void setFactRect(const QRectF& rf);
    virtual void setRect(const QRectF& rf);
    QRectF rect();
    void setMultiParent(CMDDrawMulti* parent);
    //多组控件，基类
    CMDDrawMulti *multiParent();
    //成组控件
    CMDDrawGroup *groupParent();
    virtual qreal widenWidth();
    void setBound(const QRectF& rf);
    QRectF bound();
    virtual DrawType type() = 0;
    virtual QString className() = 0;
    virtual void setParent(IHMIForm* parent);
    IHMIForm *parent();
    QPainterPath* basePath();
    virtual QPainterPath* matrixPath();
    virtual QPainterPath* path();
    void setName(const QString& value);
    QString name();
    QRectF newRect();
    //!设置Rect时，需要先写入NewRect，然后计算，再将NewRect赋值给Rect
    virtual void setNewRect(const QRectF& rf);
    void setDirectRect(const QRectF& rf);
    IDataObj* data();
    virtual OrthoMode orthoMode();
    bool isStudioMode();
    void setSupportGroupEvent(bool value);
    bool supportGroupEvent();
    void setShowSelectedFrame(bool value);
    bool showSelectedFrame();
    void setToolTip(const QString& value);
    QString toolTip();
    void setSupportHidedEvent(bool value);
    bool supportHidedEvent();
    virtual bool canCombine(){return false;}
/*!
\section public function
*/
public:
    virtual void draw(QPainter* painter, bool isPrinting = false);

    virtual bool isVector() = 0;
    virtual bool isWidget() = 0;

    virtual void mouseFrameMove(const QPointF& point, int pos);
    virtual bool isVisible(const QPointF& point);
    virtual void boundMove(qreal xOff, qreal yOff, qreal wOff, qreal hOff);
    virtual void mouseDown(const QPointF& point);
    virtual void mouseMove(const QPointF& point);
    //!通知事件
    virtual void notify(NotifyType type){}

/*!
\section property obj
*/
public:
    QObject *propertyDrawObj();
protected:
    virtual CMDPropertyDrawObj* createPropertyDrawObj();
/*!
\section protect function
*/
protected:
    virtual CMDDrawObj* createSelf() = 0;
    //!边框移动
    virtual void frameMove(qreal xOff, qreal yOff, qreal wOff, qreal hOff);
/*!
\section public function
*/
public:
    IDrawEnvrionment* environment();
/*!
\section refresh
*/
public:
    virtual void invalidate();
    virtual void generateMatrix(){}
    virtual void generateBound();
protected:
    virtual void generatePath();
    virtual void generateContent(){}
    virtual void initContent(){}
/*!
\section common
*/
public:
    virtual void serialize(QJsonObject &json);
    virtual void deserialize(const QJsonObject &json);
    virtual CMDDrawObj* clone();
/*!
\section group
*/
public:
    //!解组操作
    virtual void unMultiOper();
protected:
    //子控件发生变化后，重新设置成组控件的Path
    void generateGroupPath();
/*!
\section data backup
*/
public:
    ICalcData* dataBk();
    virtual void backupData();
    virtual void backupMousePos(const QPointF& point);
/*!
\section event
*/
public:
    virtual void loadMouseEvent(Qt::MouseButton /*button*/, const QPointF& /*location*/, MouseType /*type*/){}
    virtual void loadInitalizationEvent();
    void loadMouseEnterEvent();
    void loadMouseLeaveEvent();
    virtual void loadDoubleClickEvent(const QPointF& point);
    virtual void loadGeneratePathEvent();
protected:
    virtual void onSizeChanged(){}
    virtual void onLocationChanged(){}
    virtual void onMouseDown(Qt::MouseButton /*button*/, qreal /*x*/, qreal/* y*/){}
    virtual void onMouseUp(Qt::MouseButton /*button*/, qreal /*x*/, qreal /*y*/){}
    virtual void onMouseMove(Qt::MouseButton /*button*/, qreal /*x*/, qreal /*y*/){}
    virtual void onMouseEnter();
    virtual void onMouseLeave();
    virtual void onPaint(QPainter* /*p*/){}
    virtual void onPrint(QPainter* p);
    virtual void onInitialization(){}
    virtual void onDoubleClick(){}

    //!生成控件路径
    virtual void onGeneratePath();
    void loadSizeEvent();

/*!
\section layer
*/
public:
    virtual void setVisible(bool value);
    bool visible();
    bool finalVisible();
    void setLayer(int layer);
    int layer();
    //!锁定
    virtual void setLocked(bool value);
    bool locked();
    bool finalLocked();
    bool canSelect();
    bool canSelect(const QPointF& point);
    virtual bool canSelect(const QRectF& rect);
/*!
\section var
*/
public:
    static QList<QStringList> expNames();
    static QDialog* externalBindVarDialog(QWidget* parent, QList<CMDExpInfo*>* list,
            IDrawEnvrionment* envir, const QString& selectName = "");
    static void analyseExternalExpression(CMDExpManager* exp, IHMIFramework *framework);
public:
    CMDExpManager *expManager();
public:
    virtual void initExpManager();
    //!替换自定义属性为实际表达式
    virtual void replaceCustomProperty();
    virtual void analyseExpression();
    //!变量变化通知
    virtual void loadDataChangedEvent(CMDExpInfo *exp);
protected:
    //!附加的表达式
    virtual void addAddtionalExpression(){}
    //添加表达式到管理器中
    void addExpressionToManager(const QList<QString>& exps, QList<int>* indexs = NULL);
    //变量变化通知事件
    virtual void onDataChanged(CMDExpInfo* exp){}
private:
    //!替换自定义字符串
    void replaceCustomString(QString& name);
    //!分析单个表达式
    void analyseSingleExpression(CMDExpInfo* exp);

/*!
\section static
*/
public:
    static ICalcData* createCalcData();
    static CMDExpInfo* createPropertyExpression(const QString& name, const QString& expression);
/*!
\section widget
*/
public:
    //!更新widget控件的尺寸
    virtual void updateFormScale(){}
    //!更新widget控件的visible
    virtual void updateVisible(){}
    //!更新widget控件的enabled
    virtual void updateEnabled(){}
    //!提到最前
    virtual void top(){}
    //!提到最后
    virtual void last(){}
/*!
\section child changed
子控件数据发生变化时，需要通知成组父控件
编辑环境：编辑框
运行环境：脚本，变量变化
*/
public:
    void setDynamicRect(const QRectF& value);
/*!
\section undo
*/
protected:
    virtual IUndoDataObj* createUndoData();
public:
    //!恢复undo数据
    virtual bool revertUndoData(IUndoDataObj* value);
    //!记录undo数据
    virtual IUndoDataObj *fillUndoData(bool isOld = true);
    virtual void undoOperate(IUndoDataObj* value);
    //!判断数据是否发生变化
    virtual bool undoEqual();
    //!添加控件时调用函数
    virtual void undoAdd(){}
    //!删除控件时调用函数
    virtual void undoDelete(){}
/*!
\section language
*/
public:
    CMDMultiLanguage* language();
    //!切换语言
    virtual void changeLanguage(){}
    //!添加项目字典
    virtual void collectWords(){}
/*!
\section image
*/
public:
    //!设置图库图片路径,空值表示不在图库中
    void setStorageImagePath(const QString& value);
    QString getImagePath();
    //!重设图片路径
    virtual void resetImagePath(const QString& path);
    //!拷贝图片
    virtual void copyImage(const QString& sourcePath, const QString& targetPath, bool needRename);
public:
    virtual void getBrushes(QList<CMDBrushManager*>& list);
    virtual void getPens(QList<CMDPenManager*>& list);
/*!
\section script function
*/
public:
    //!动态设置表达式
    void setScriptExpression(const QString& name, const QString& expression);
/*!
 \section batch edit
*/
public:
    QStringList* classInfo();
public:
    virtual CMDPropertyDrawObj* createPropertyDrawObj(const QString& className);
protected:
    //初始化类信息
    virtual void initClassInfo(QStringList &classInfo);
};

#endif // MDDRAWOBJ_H
