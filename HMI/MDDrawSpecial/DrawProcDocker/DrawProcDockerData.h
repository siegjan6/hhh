/*!

\author luyj

\section 功能

管理【进程主窗体停靠控件数据类】。

*/

#ifndef DRAWPROCDOCKERDATA_H
#define DRAWPROCDOCKERDATA_H

#include <QJsonObject>
#include "procdockerwidget.h"

//! 进程主窗体停靠控件,数据类
class CDrawProcDockerData
{
public:
    QString m_cmd;

    ProcDockerWidget m_ProcDocker;

/*!
\section 报表控件拷贝和序列化相关接口
*/
public:
    CDrawProcDockerData();
    ~CDrawProcDockerData();
    CDrawProcDockerData& operator=(const CDrawProcDockerData& other); //! 拷贝

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // DRAWPROCDOCKERDATA_H
