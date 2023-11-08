/*!
\author aGRw 2015.03.09
\section widget基类数据
*/
#ifndef DRAWWIDGETDATA_H
#define DRAWWIDGETDATA_H

#include <QWidget>
#include "MDDrawWidgetEvent.h"

class CDrawWidgetData
{
public:
    CDrawWidgetData();
public:
    QWidget* m_widget;
    CMDDrawWidgetEvent* m_event;
/*!
\section common
*/
public:
    void operator=(const CDrawWidgetData& other);
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
};

#endif // DRAWWIDGETDATA_H
