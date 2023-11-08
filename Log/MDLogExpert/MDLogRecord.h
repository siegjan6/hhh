/*!

\author dGFuZ3p3

\section 功能

运行时的日志记录类。

*/

#ifndef MDLOGRECORD_H
#define MDLOGRECORD_H

#include <QString>
#include <QDateTime>
#include "MDLogMessage.h"

#include "MDDataArchivesInterface/IMDDataGroup.h"
#include "MDDataArchivesInterface/IMDDataItem.h"
#include "MDDataArchivesNotifyParam.h"

class CMDDatabaseRecordset;
class CMDLogContacter;
class CMDLogRecord
{
public:
    CMDLogRecord();
    virtual ~CMDLogRecord();
   CMDLogRecord(const CMDLogRecord& obj);
   CMDLogRecord& operator = (const CMDLogRecord& obj);
    bool copy(const CMDLogRecord* source);
/*!
\section 初始化
*/
public:
    bool init(const CMDLogMessage& msg, IMDDataGroup* logGroup, CMDLogContacter* contacter);
    void setData(const CMDLogMessage& msg);
    QVariant initItemValue(const QString& itemName, const CMDLogMessage& msg);
public:
    CMDDataArchivesNotifyParam m_fields;
    CMDLogContacter* m_contacter ;// NULL;

/*!
\section 在 recordSet 中增加一行，并填充该行的内容
*/
public:
    void toDatabaseRecord(CMDDatabaseRecordset* recordSet);
};

#endif // MDLOGRECORD_H
