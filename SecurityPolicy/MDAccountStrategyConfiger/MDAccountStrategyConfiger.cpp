#include "MDAccountStrategyConfiger.h"
#include "MDAccountStrategyConfigerImp.h"
#include <QJsonDocument>

CMDAccountStrategyConfiger::CMDAccountStrategyConfiger()
{
    m_imp = new CMDAccountStrategyConfigerImp;
}

CMDAccountStrategyConfiger::~CMDAccountStrategyConfiger()
{
    if(m_imp)
        delete m_imp;
}

bool CMDAccountStrategyConfiger::loadConfig(const QString& projectPath)
{
    return (m_imp ? m_imp->loadConfig(projectPath) : false);
}

void CMDAccountStrategyConfiger::unloadConfig()
{
    if(m_imp)
        m_imp->unloadConfig();
}

QString CMDAccountStrategyConfiger::configFileName()
{
    return (m_imp ? m_imp->configFileName() : "");
}

bool CMDAccountStrategyConfiger::saveToFile()
{
    return (m_imp ? m_imp->saveToFile() : false);
}

CMDAccountStrategy* CMDAccountStrategyConfiger::accountStrategy()
{
    return (m_imp ? &m_imp->m_accountStrategy : NULL);
}


QString CMDAccountStrategyConfiger::getAccountOption()
{
    QJsonObject json;
    if(m_imp){
        m_imp->m_accountStrategy.serialize(json);
        QJsonDocument doc(json);
        return QString(doc.toJson().data());
    }
    return "";
}
bool CMDAccountStrategyConfiger::setAccountOption(QString option)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(option.toUtf8(), &error);
    if(error.error == QJsonParseError::NoError){
        if(doc.isObject()){
            QJsonObject json = doc.object();
            if(m_imp){
                m_imp->m_accountStrategy.deserialize(json);
                return true;
            }
        }
    }
    return false;
}
