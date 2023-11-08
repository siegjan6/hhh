#include "DrawReportData.h"

/*!
\section 拷贝报表控件
*/
CDrawReportData::CDrawReportData()
{
}

CDrawReportData::~CDrawReportData()
{

}

CDrawReportData& CDrawReportData::operator=(const CDrawReportData &other)
{
    if(this == &other)
    {
        return *this;
    }

    this->m_report = other.m_report;

    return *this;
}


void CDrawReportData::serialize(QJsonObject &json)
{
    m_report.serialize(json);
}

void CDrawReportData::deserialize(const QJsonObject &json)
{
    m_report.deserialize(json);
}
