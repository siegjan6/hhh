#ifndef MDDRAWGAUGE_H
#define MDDRAWGAUGE_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWSPECIAL_LIBRARY)
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_IMPORT
#endif
//////////////////////////////////////////////////////////////////////////
#include "MDDrawObj/MDDrawVector.h"

class CDrawGaugeData;
//////////////////////////////////////////////////////////////////////////
//曲线控件
class MDDRAWSPECIALSHARED_EXPORT CMDDrawGauge : public CMDDrawVector
{
public:
    CMDDrawGauge();
    virtual ~CMDDrawGauge();
private:
    //数据
    CDrawGaugeData *m_data;
//////////////////////////////////////////////////////////////////////////
public:
    bool openDesign();
//////////////////////////////////////////////////////////////////////////
//DrawVector
//virtual property
public:
    DrawType type();
    QString className(){return "CMDDrawGauge";}
//////////////////////////////////////////////////////////////////////////
//virtual function
public:
    //返回属性框对象
    CMDPropertyDrawObj *createPropertyDrawObj();
    //初始化
    void onInitialization();
protected:
    //创建自身
    CMDDrawObj *createSelf();
    void onGeneratePath();
    void onPaint(QPainter* p);
    void onDoubleClick();
    //翻译
    void changeLanguage();
    void collectWords();
//////////////////////////////////////////////////////////////////////////
//common
public:
    //序列化
    CMDDrawObj* clone();

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

/*!
\section var
*/
public:
    static QList<QStringList> expNames();
protected:
    void addAddtionalExpression();
    void onDataChanged(CMDExpInfo *exp);
};

#endif // MDDRAWGAUGE_H
