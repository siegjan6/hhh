/*!
\author Y2hhaWhq

\section 功能

时间日期编辑框数据类
*/
#ifndef DRAWDATETIMEEDITDATA_H
#define DRAWDATETIMEEDITDATA_H

#include <QDateTimeEdit>
#include <QJsonObject>
#include "DrawObjDefine.h"
class CMDDrawDateTimeEditEvent;

class CDrawDateTimeEditData
{
public:
    CDrawDateTimeEditData();
    ~CDrawDateTimeEditData(){}

public:
    QDateTimeEdit* m_widget;            //! 时间日期控件
    CMDDrawDateTimeEditEvent* m_event;  //! 事件绑定类

    qint32 m_showType;                  //! 显示类型
    bool m_calendarPop;                 //! 下拉月历
    QDateTime m_maxDateTime;
    QDateTime m_minDateTime;

/*!
\section  序列化
*/
public:
    void operator=(const CDrawDateTimeEditData& other);

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // DRAWDATETIMEEDITDATA_H

