/*!
\author Y2hhaWhq

\section 功能

微调按钮控件数据类
*/
#ifndef DRAWSPINBOXDATA_H
#define DRAWSPINBOXDATA_H

#include <QSpinBox>
#include <QJsonObject>
#include "DrawObjDefine.h"
class CMDDrawSpinBoxEvent;

class CDrawSpinBoxData
{
public:
    CDrawSpinBoxData();
    ~CDrawSpinBoxData(){}

public:
    QSpinBox* m_widget;
    CMDDrawSpinBoxEvent* m_event;

    qint32 m_singleStep;
    qint32 m_maximum;
    qint32 m_minimum;
    qint32 m_initialValue;

/*!
\section 序列化
*/
public:
    void operator=(const CDrawSpinBoxData& other);

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // DRAWSPINBOXDATA_H

