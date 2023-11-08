#include "MDProjectPara.h"
#include "MDProjectParaData.h"
#include <QDebug>

CMDProjectPara::CMDProjectPara()
{
    m_data = new CMDProjectParaData;
}

CMDProjectPara::~CMDProjectPara()
{
    if(m_data)
        delete m_data;
}

CMDProjectPara::CMDProjectPara(const CMDProjectPara& obj)
{
    m_data = new CMDProjectParaData();

    copy(&obj);
}

CMDProjectPara& CMDProjectPara::operator = (const CMDProjectPara& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

/*!
\section 属性
*/
CMDActionSPList* CMDProjectPara::spList()
{
    return (m_data ? &m_data->m_spList : NULL);
}
bool CMDProjectPara::setSPList(const CMDActionSPList* spList)
{
    if(!m_data || !spList)
        return false;

    m_data->m_spList = *spList;
    return true;
}

bool CMDProjectPara::isDeviceCloudClientEnabled()
{
    return (m_data ? m_data->m_enableDeviceCloudClient : false);
}

void CMDProjectPara::setDeviceCloudClientEnabled(bool enable)
{
    if(m_data)
        m_data->m_enableDeviceCloudClient = enable;
}

QString CMDProjectPara::cloudClientID()
{
    return (m_data ? m_data->m_cloudClientID : "");
}

void CMDProjectPara::setCloudClientID(const QString& cloudClientID)
{
    if(m_data)
        m_data->m_cloudClientID = cloudClientID;
}

int CMDProjectPara::port()
{
    return (m_data ? m_data->m_port : 4840);
}
void CMDProjectPara::setPort(const int port)
{
    if(m_data)
        m_data->m_port = port;
    qDebug() << "CMDProjectPara::setPort" << m_data->m_port;
}

quint16 CMDProjectPara::caMode()
{
    return (m_data ? m_data->m_caMode : 1);
}
void CMDProjectPara::setCAMode(const quint16 caMode)
{
    if(m_data)
        m_data->m_caMode = caMode;
}

QString CMDProjectPara::certificatePath()
{
    return (m_data ? m_data->m_certificatePath : "");
}
void CMDProjectPara::setCertificatePath(const QString& certPath)
{
    if(m_data)
        m_data->m_certificatePath = certPath;
}

QString CMDProjectPara::privateKeyPath()
{
    return (m_data ? m_data->m_privateKeyPath : "");
}
void CMDProjectPara::setPrivateKeyPath(const QString& pkeyPath)
{
    if(m_data)
        m_data->m_privateKeyPath = pkeyPath;
}

bool CMDProjectPara::copy(const CMDProjectPara* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

/*!
\section 序列化
*/
bool CMDProjectPara::serialize(QDataStream& stream, bool useCurrentVersion)
{
    return (m_data ? m_data->serialize(stream, useCurrentVersion) : false);
}

void CMDProjectPara::deserialize(QDataStream& stream)
{
    if(m_data)
        m_data->deserialize(stream);
}

