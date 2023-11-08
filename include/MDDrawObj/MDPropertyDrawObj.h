/*!
\author aGRw
\section 控件属性类
*/
#ifndef MDPROPERTYDRAWOBJ_H
#define MDPROPERTYDRAWOBJ_H

class CMDHMIActionManager;

#include "MDDrawObj.h"
#include "MDDrawObjCommand.h"
#include "MDPropertyBrowser/PropertyBrowser.h"

class MDDRAWOBJSHARED_EXPORT CMDPropertyDrawObj : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QMap translateName READ translateName)

    Q_PROPERTY(QString GUID_Variable)
    Q_PROPERTY(QString DILvariable READ variable WRITE setVariable)

    Q_PROPERTY(QString GUID_Layout)
    Q_PROPERTY(QRectF rect READ rect WRITE setRect)
    Q_PROPERTY(int layer READ layer WRITE setLayer)

    Q_PROPERTY(QString GUID_Common)
    Q_PROPERTY(QString name READ name WRITE setName)

    Q_PROPERTY(QString GUID_Action)
    Q_PROPERTY(bool showSelectedFrame READ showSelectedFrame WRITE setShowSelectedFrame)

    Q_PROPERTY(QString GUID_Appearance)
    Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)
//////////////////////////////////////////////////////////////////////////
public:
    CMDPropertyDrawObj(CMDDrawObj* parent);
    virtual ~CMDPropertyDrawObj(){}
public:
    QMap<QString,QVariant> translateName(){return m_translateMap;}
/*!
\section property
*/
public:
    void setRect(const QRectF& value);
    QRectF rect();
    void setLayer(int value);
    int layer();
    void setVariable(const QString& value);
    QString variable();
    void setName(const QString& value);
    QString name();
    void setShowSelectedFrame(bool value);
    bool showSelectedFrame();
    void setToolTip(const QString& value);
    QString toolTip();
//////////////////////////////////////////////////////////////////////////
protected:
    QUndoStack* undo();
    QWidget* container();
protected:
    CMDDrawObj* m_object;
public:
    CPropertyBrowser* propertyGrid();
/*!
\section virtual function
*/
public:
    virtual void resetAttribute(){}
/*!
\section protected function
*/
protected:
    void setModify(bool value);
/*!
\section translate
*/
public:
    void insertTranslateMap(const QString& id, const QString& name);
private:
    QMap<QString,QVariant> m_translateMap;
/*!
 \section batch edit
*/
public:
    void appendBatchObjs(QList<CMDDrawObj*>* objs);
    //设置批量模式下的属性
    virtual void resetBatchProperty(CPropertyBrowser* browser);
protected:
    enum class RectType
    {
        X, Y, Width, Height
    };
    enum class PointType
    {
        X, Y
    };
    enum class FontType
    {
        Family,PointSize,Bold,Italic,Underline,Strikeout,Kerning,All
    };
protected:
    //是否是批量模式
    bool isBatch();
    //批量操作后，更新控制点
    void invalidateBatchControlPoint();

    //获取QRectF的修改值类型
    RectType getRectChange(const QRectF& v1, const QRectF& v2);
    PointType getPointChange(const QPointF& v1, const QPointF& v2);
    FontType getFontChange(const QFont& v1, const QFont& v2);
protected:
    QList<CMDDrawObj*>* batchObjs(){return &m_batchObjs;}
private:
    //批量对象
    QList<CMDDrawObj*> m_batchObjs;
private:
    void setBatchRect(const QRectF &value);
    void setBatchLayer(int value);
    void setBatchShowSelectedFrame(bool value);
    void setBatchToolTip(const QString& value);
};

#endif // MDPROPERTYDRAWOBJ_H
