/*!
\author bWVpeGw=

\section 功能

时间日期控件属性类
*/
#ifndef MDPROPERTYDRAWDATETIMEEDIT_H
#define MDPROPERTYDRAWDATETIMEEDIT_H

#include "MDPropertyDrawObj.h"
#include "MDDrawDateTimeEdit.h"
#include "MDPropertyDrawBasicWidget.h"
class MDDRAWWIDGETSSHARED_EXPORT CMDPropertyDrawDateTimeEdit:public CMDPropertyDrawBasicWidget
{
    Q_OBJECT
    Q_ENUMS(PropertyDateTimeStyle)

    Q_PROPERTY(QString GUID_Action)
    Q_PROPERTY(bool calendarPop READ calendarPop WRITE setCalendarPop)

    Q_PROPERTY(QString GUID_Appearance)
    Q_PROPERTY(PropertyDateTimeStyle showType READ showType WRITE setShowType)

/*!
\section 控件显示样式
*/
public:
     enum class PropertyDateTimeStyle{shortDate,longDate,time};
/*!
\section 属性函数
*/
public:
    //! 显示类型
     void setShowType(PropertyDateTimeStyle value);
     PropertyDateTimeStyle showType();
    //! 下拉月历
     void setCalendarPop(bool value);
     bool calendarPop();

public:
     CMDPropertyDrawDateTimeEdit(CMDDrawObj *parent);
     virtual ~CMDPropertyDrawDateTimeEdit(){}

protected:
     CMDDrawDateTimeEdit *widget();
/*!
\section 设置属性值
*/
public:
     void resetAttribute();

/*!
\section batch edit
*/
private:
    void setBatchShowType(PropertyDateTimeStyle value);
    void setBatchCalendarPop(bool value);
};

#endif // MDPROPERTYDRAWDATETIMEEDIT_H

