#include "DrawPdfViewerData.h"

/*!
\section pdf viewer停靠控件
*/
CDrawPdfViewerData::CDrawPdfViewerData()
{
}

CDrawPdfViewerData::~CDrawPdfViewerData()
{

}

CDrawPdfViewerData& CDrawPdfViewerData::operator=(const CDrawPdfViewerData &other)
{
    if(this == &other)
    {
        return *this;
    }

    this->m_filePath = other.m_filePath;

    return *this;
}


void CDrawPdfViewerData::serialize(QJsonObject &json)
{
    json["filePath"] = m_filePath;
}

void CDrawPdfViewerData::deserialize(const QJsonObject &json)
{
    m_filePath = json["filePath"].toString();
}
