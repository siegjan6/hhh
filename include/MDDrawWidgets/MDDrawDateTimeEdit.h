/*!
\author bWVpeGw=

\section 功能

时间日期控件类
*/
#ifndef MDDRAWDATETIMEEDIT_H
#define MDDRAWDATETIMEEDIT_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif
#include <QDateTimeEdit>
#include <QJsonObject>
#include "MDDrawBasicWidget.h"

class CDrawDateTimeEditData; //! 时间数据类

class MDDRAWWIDGETSSHARED_EXPORT CMDDrawDateTimeEdit : public CMDDrawBasicWidget
{
public:
    CMDDrawDateTimeEdit();
    virtual ~CMDDrawDateTimeEdit();

private:
    CDrawDateTimeEditData* m_data;

/*!
\section 属性函数
*/
public:
    QDateTimeEdit* widget();            //! 控件
    void setShowType(int value);        //! 显示类型
    int showType();
    void setCalendarPop(bool value);    //! 下拉月历
    bool calendarPop();

/*!
\section 脚本应用函数, 读写年月日时分秒
*/
public:
    void setYear(int year);
    int getYear();
    void setMonth(int month);
    int getMonth();
    void setDay(int day);
    int getDay();
    void setHour(int hour);
    int getHour();
    void setMinute(int minute);
    int getMinute();
    void setSecond(int second);
    int getSecond();

    void setTime(QDateTime time);       //! 读写时间日期
    QDateTime getTime();

    void setMaxDateTime(QDateTime datetime);
    QDateTime maxDateTime();
    void setMinDateTime(QDateTime datetime);
    QDateTime minDateTime();

/*!
\section 控件对象属性
*/
public:
    DrawType type();                                //! 返回控件类型名
    QString className(){return "CMDDrawDateTimeEdit";}
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
    bool writeVarEnabled();
    void setWriteVarEnabled(bool writeVarEnabled);

private:
    bool m_writeVarEnabled;                         //! 可写入变量

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
#endif // MDDRAWDATETIMEEDIT_H

