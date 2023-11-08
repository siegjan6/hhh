/*!
\author bWVpeGw=

\section 功能

微调按钮类
*/
#ifndef MDDRAWSPINBOX_H
#define MDDRAWSPINBOX_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QSpinBox>
#include <QJsonObject>
#include "MDDrawBasicWidget.h"

class CDrawSpinBoxData;

class MDDRAWWIDGETSSHARED_EXPORT CMDDrawSpinBox : public CMDDrawBasicWidget
{
public:
    CMDDrawSpinBox();
    virtual ~CMDDrawSpinBox();

private:
    CDrawSpinBoxData* m_data;

/*!
\section 属性函数
*/
public:
    QSpinBox* widget();             //! 控件
    void setSingleStep(int value);  //! 设置单步值
    int singleStep();
    void setMaximum(int max);       //! 设置最大值
    int maximum();    
    void setMinimum(int min);       //! 设置最小值
    int minimum();
    void setInitialValue(int value);//! 设置初始值
    int initialValue();

/*!
\section 脚本属性函数
*/
public:
    void setCurrentValue(int value);    //! 读写当前值
    int getCurValue();

/*!
\section 控件对象属性
*/
public:
    DrawType type();                                //! 返回控件类型名
    QString className(){return "CMDDrawSpinBox";}
    CMDPropertyDrawObj *createPropertyDrawObj();    //! 返回属性框对象
    void onInitialization();                        //! 初始化
protected:
    CMDDrawObj *createSelf();                       //! 创建自身

/*!
\section 序列化
*/
public:
    CMDDrawObj* clone();

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

/*!
\section 绑定相关变量
*/
public:
    static QList<QStringList> expNames();
    void onDataChanged(CMDExpInfo *exp);
    void writeVariable();
    bool valueChanged();
    void setValueChanged(bool valueChanged);
    bool writeVarEnabled() const;
    void setWriteVarEnabled(bool writeVarEnabled);

private:
    bool m_writeVarEnabled;

/*!
\section batch edit
*/
public:
   virtual CMDPropertyDrawObj* createPropertyDrawObj(const QString& className);
protected:
        //初始化类信息
   virtual void initClassInfo(QStringList &classInfo);
private:
    virtual void setBackColor(const QColor& value);
    virtual void setDisableBackColor(const QColor& value);
};

#endif // MDDRAWSPINBOX_H
