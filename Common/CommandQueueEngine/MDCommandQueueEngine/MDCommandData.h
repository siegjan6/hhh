/*!

\author dGFuZ3p3

\section 功能

引擎命令数据类的实现类。

*/

#ifndef MDCOMMANDDATA_H
#define MDCOMMANDDATA_H

#include <QtGlobal>
#include <QDateTime>

class CMDCommandData
{
    friend class CMDCommand;
public:
    CMDCommandData();
protected:
    qint16 m_type;
    QDateTime m_timeOnCreated;
    QDateTime m_timeOnProcessed;
    void* m_para;
};

#endif // MDCOMMANDDATA_H
