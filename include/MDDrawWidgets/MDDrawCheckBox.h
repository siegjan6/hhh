/*!
\author bWVpeGw=

\section 功能

复选框控件类
*/
#ifndef MDDRAWCHECKBOX_H
#define MDDRAWCHECKBOX_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QJsonObject>
#include <QCheckBox>
#include "MDDrawBasicWidget.h"

class CDrawCheckBoxData;

class MDDRAWWIDGETSSHARED_EXPORT CMDDrawCheckBox : public CMDDrawBasicWidget
{
public:
    CMDDrawCheckBox();
    virtual ~CMDDrawCheckBox();

private:
    CDrawCheckBoxData* m_data;

/*!
\section 属性函数
*/
public:
    QCheckBox* widget();                //! 控件
    void setText(const QString& value); //! 文本
    QString text();
    void setChecked(bool value);        //! 选中状态
    bool checked();

/*!
\section 脚本函数
*/
public:
    void setScriptText(QString str);    //! 读写文本
    QString getScriptText();

/*!
 \section 控件对象属性
*/
public:
    DrawType type();                            //! 返回控件类型名
    QString className(){return "CMDDrawCheckBox";}
    CMDPropertyDrawObj *createPropertyDrawObj();//! 返回属性框对象
    void onInitialization();                    //! 初始化
protected:
    CMDDrawObj *createSelf();                   //! 创建自身
/*!
\section   序列化
*/
public:
    CMDDrawObj* clone();

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

public:
    static QList<QStringList> expNames();
    void onDataChanged(CMDExpInfo *exp);
    void writeVariable();
    bool writeVarEnabled();
    void setWriteVarEnabled(bool writeVarEnabled);

private:
    bool m_writeVarEnabled;                     //! 可写入变量

/*!
\section batch edit
*/
public:
    virtual void changeLanguage();
    virtual void collectWords();

/*!
\section 多语言
*/
public:
   virtual CMDPropertyDrawObj* createPropertyDrawObj(const QString& className);
protected:
        //初始化类信息
   virtual void initClassInfo(QStringList &classInfo);
};

#endif // MDDRAWCHECKBOX_H

