#ifndef DRAWDATAARCHIVESDATA_H
#define DRAWDATAARCHIVESDATA_H

#include <QJsonObject>
#include "MDDataArchivesWidget.h"

//! 数据归档，数据类
class CDrawDataArchivesData
{
public:
    CDrawDataArchivesData();
    ~CDrawDataArchivesData(){}

public:
    CMDDataArchivesWidget m_dataArchives;

//! common
public:
    void operator = (const CDrawDataArchivesData& other);

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // DRAWDATAARCHIVESDATA_H
