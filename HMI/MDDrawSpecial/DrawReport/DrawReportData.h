/*!

\author bWVpeGw=

\section 功能

管理【报表控件数据类】。

*/

#ifndef DRAWREPORTDATA_H
#define DRAWREPORTDATA_H

#include <QJsonObject>
#include "MDReportOutput.h"

//! 报表控件,数据类
class CDrawReportData
{
public:
    MDCNCReportOutputInc m_report;

/*!
\section 报表控件拷贝和序列化相关接口
*/
public:
    CDrawReportData();
    ~CDrawReportData();
    CDrawReportData& operator=(const CDrawReportData& other); //! 拷贝报表控件

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // DRAWREPORTDATA_H
