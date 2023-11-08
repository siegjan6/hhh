/*!
\author Y2hhaWhq

\section 功能

复选框控件数据类
*/
#ifndef DRAWCHECKBOXDATA_H
#define DRAWCHECKBOXDATA_H

#include <QCheckBox>
#include <QButtonGroup>
#include <QJsonObject>
#include "DrawObjDefine.h"
class CMDDrawCheckBoxEvent;

class CDrawCheckBoxData
{
public:
    CDrawCheckBoxData();
    ~CDrawCheckBoxData(){}

public:
    QCheckBox* m_widget;            //! 复选框
    CMDDrawCheckBoxEvent* m_event;  //! 事件绑定类

    bool m_checked;                 //! 选中状态
    QString m_text;                 //! 文本
    QString m_showText;             //! 翻译文本

/*!
\section  序列化
*/
public:
    void operator = (const CDrawCheckBoxData& other);

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // DRAWCHECKBOXDATA_H

