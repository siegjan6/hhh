/*!
\author enli 2015.01.08
\section 驱动信息实现类，由MDDriverManager创建
*/
#ifndef CMDDRIVERINFORMATION_H
#define CMDDRIVERINFORMATION_H

#include "IMDDriverInformation.h"
class CMDDriverInformation:public IMDDriverInformation
{
public:
    CMDDriverInformation();
    ~CMDDriverInformation();
public:
    /*!
    \section 驱动属性
    */
    //ID
    QString id();
    void setID(QString id);
    //name
    QString name();
    void setName(QString name);
    //Description
    QString description();
    void setDescription(QString description);
private:
    QString m_id;
    QString m_name;
    QString m_description;
};

#endif // CMDDRIVERINFORMATION_H
