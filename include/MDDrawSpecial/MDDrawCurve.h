#ifndef MDDRAWCURVE_H
#define MDDRAWCURVE_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWSPECIAL_LIBRARY)
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_IMPORT
#endif
//////////////////////////////////////////////////////////////////////////
#include "MDDrawWidget.h"

class CDrawCurveData;
//////////////////////////////////////////////////////////////////////////
//曲线控件
class MDDRAWSPECIALSHARED_EXPORT CMDDrawCurve : public CMDDrawWidget
{
public:
    CMDDrawCurve();
    virtual ~CMDDrawCurve();
private:
    //数据
    CDrawCurveData *m_data;
//////////////////////////////////////////////////////////////////////////
public:
    int setProperty();
//////////////////////////////////////////////////////////////////////////
//DrawWidget
//virtual property
public:
    DrawType type();
    QString className(){return "CMDDrawCurve";}
//////////////////////////////////////////////////////////////////////////
//virtual function
public:
    //返回属性框对象
    CMDPropertyDrawObj *createPropertyDrawObj() ;
    //初始化
    void onInitialization();
    void initContent();
protected:
    //创建自身
    CMDDrawObj *createSelf() ;

    void onDoubleClick();
//////////////////////////////////////////////////////////////////////////
//common
public:
    //序列化
    CMDDrawObj* clone() ;

    void setIsRealTime(bool isTrue);

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
//////////////////////////////////////////////////////////////////////////
    CDrawCurveData *data() const;
    void setData(CDrawCurveData *data);

    void setDataSourceParameter(int type, const QString &parameter); //! 设置数据库参数   

public:
    void changeLanguage();
    void collectWords();
    void getBrushes(QList<CMDBrushManager*>& list);

    //var
public:
    void addAddtionalExpression();
    void onDataChanged(CMDExpInfo *expression);
    //////////////////////////////////////////////////////////////////////////
public:
    void setStartTime(const QString &curveName,const QVariant &startTime);
public:
    void setReferenceCurveData(const QString &curveName, const QVariant &xValue, const QVariant &yValue);
};

/*!
 \brief 历史曲线控件
*/
class MDDRAWSPECIALSHARED_EXPORT CMDDrawHistory:public CMDDrawCurve
{
public:
    CMDDrawHistory(){setIsRealTime(false);}
    virtual ~CMDDrawHistory(){}
public:
    DrawType type();
    QString className(){return "CMDDrawHistory";}

    void setFilter(const QString &filter);
    void setTagFilter(const QString &filter);
    void query();

public:
    void setReferenceCurveData(const QString &curveName, const QVariant &xValue, const QVariant &yValue);
protected:
    CMDDrawObj *createSelf() ;
};

#endif // MDDRAWCurve_H
