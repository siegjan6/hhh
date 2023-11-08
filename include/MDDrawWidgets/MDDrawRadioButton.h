/*!
\author bWVpeGw=

\section 功能

单选按钮控件类
*/
#ifndef MDDRAWRADIOBUTTON_H
#define MDDRAWRADIOBUTTON_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QRadioButton>
#include <QMap>
#include <QJsonObject>
#include "MDDrawBasicWidget.h"
#include "MDMultiLanguage.h"
class CDrawRadioButtonData;

class MDDRAWWIDGETSSHARED_EXPORT CMDDrawRadioButton : public CMDDrawBasicWidget
{
public:
    CMDDrawRadioButton();
    virtual ~CMDDrawRadioButton();

private:
    CDrawRadioButtonData* m_data;

/*!
\section 属性函数
*/
public:
    QRadioButton* widget();                 //! 控件
    void setText(const QString& value);     //! 文本
    QString text();
    void setChecked(bool value);            //! 选中状态
    bool checked();
    void setGroupId(int value);             //! 分组
    int groupId();
    void setCheckValue(qreal value);        //! 选中后设置数值
    qreal checkValue();

/*!
\section 控件对象属性
*/
public:
    DrawType type();                            //! 返回控件类型名
    QString className(){return "CMDDrawRadioButton";}
    CMDPropertyDrawObj *createPropertyDrawObj();//! 返回属性框对象
    void onInitialization();                    //! 初始化
protected:
    CMDDrawObj *createSelf();                   //! 创建自身

/*!
\section 序列化
*/
public:
    CMDDrawObj* clone();

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
private:
    void resetMap();  //! 分组配置

/*!
\section 变量
*/
public:
    static QList<QStringList> expNames();
    void onDataChanged(CMDExpInfo *exp);

/*!
\section 事件
*/
public:
    void checkChanged(bool value);

/*!
\section 多语言
*/
public:
    virtual void changeLanguage();
    virtual void collectWords();

/*!
\section batch edit
*/
public:
   virtual CMDPropertyDrawObj* createPropertyDrawObj(const QString& className);
protected:
        //初始化类信息
   virtual void initClassInfo(QStringList &classInfo);
};

#endif  //MDDRAWRADIOBUTTON_H
