/*!

\author luyj

\section 功能

管理【图形窗体，进程主窗体停靠控件属性类】。

*/

#ifndef PROPERTYDRAWPROCDOCKER_H
#define PROPERTYDRAWPROCDOCKER_H

#include "MDPropertyDrawObj.h"
#include <QColor>

class CMDDrawProcDocker;


class CPropertyDrawProcDocker : public CMDPropertyDrawObj
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_Data)
    //    Q_PROPERTY(QString FileName READ getFileName WRITE setFileName)
    Q_PROPERTY(QString cmd READ getCmd WRITE setCmd)
    //Q_PROPERTY(QString DILReportBackColor READ getReportBackColor WRITE setReportBackColor)

public:
    CPropertyDrawProcDocker(CMDDrawObj *parent);
    virtual ~CPropertyDrawProcDocker(){}

    /*!
\section 进程主窗体停靠控件相关属性接口
*/

protected:
    CMDDrawProcDocker* object();

private:
    void setCmd(const QString& cmd);
    QString getCmd();

private:
    QString m_cmd;
};

#endif // PROPERTYDRAWPROCDOCKER_H
