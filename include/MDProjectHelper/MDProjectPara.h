/*!

\author dGFuZ3p3

\section 功能

项目参数数据类。

*/

#ifndef MDPROJECTPARA_H
#define MDPROJECTPARA_H

#include <QString>
#include "MDProjectHelper_Global.h"

class CMDActionSPList;
class CMDProjectParaData;
class MDPROJECTHELPERSHARED_EXPORT CMDProjectPara
{
public:
    CMDProjectPara();
    virtual ~CMDProjectPara();
   CMDProjectPara(const CMDProjectPara& obj);
   CMDProjectPara& operator = (const CMDProjectPara& obj);
/*!
\section 属性
*/
public:
    CMDActionSPList* spList();
    bool setSPList(const CMDActionSPList* spList);
public:
    bool isDeviceCloudClientEnabled();
    void setDeviceCloudClientEnabled(bool enable);
public:
    QString cloudClientID();
    void setCloudClientID(const QString& cloudClientID);
// opc ua server config
public:
    int port();
    void setPort(const int port);
    quint16 caMode();
    void setCAMode(const quint16 caMode);
    QString certificatePath();
    void setCertificatePath(const QString& certPath);
    QString privateKeyPath();
    void setPrivateKeyPath(const QString& pkeyPath);
public:
    virtual bool copy(const CMDProjectPara* source);

/*!
\section 序列化
*/
public:
    virtual bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    virtual void deserialize(QDataStream& stream);
protected:
    CMDProjectParaData* m_data;
};

#endif // MDPROJECTPARA_H
