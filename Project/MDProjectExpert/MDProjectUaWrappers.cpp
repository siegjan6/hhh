#include "MDProjectUaWrappers.h"
#include "MDProjectExpertImp.h"

#include "../../include/MDVariableExpert/MDVariableExpert.h"
#include "MDDataArchivesEngine.h"
#include "../../include/MDRTDBInterfaces/IMDRTDBVariable.h"
#include "MDRecipe.h"
#include "MDRecipeConfiger.h"
#include <QJsonArray>
#include <QJsonDocument>

/*!
 * \brief 项目专家脚本包装类
 */
CMDProjectExpertUaWrapper::CMDProjectExpertUaWrapper(QUaServer *server)
                                            :QUaBaseObject(server)
{

}

void CMDProjectExpertUaWrapper::setProp(void* object)
{
    m_p = object;
}

void* CMDProjectExpertUaWrapper::MDObject()
{
    return m_p;
}

///*!
// * \brief 变量对象脚本包装类
// */
//CMDVariableWrapper::CMDVariableWrapper(void* object, const QString& name)
//                                      :CMDObjectWrapper(object, name)
//{
//}

//QVariant CMDVariableWrapper::getValue()
//{
//    IMDRTDBVariable* obj = (IMDRTDBVariable*)MDObject();
//    return obj->currentValue();
//}

//void CMDVariableWrapper::setValue(const QVariant& value)
//{
//    IMDRTDBVariable* obj = (IMDRTDBVariable*)MDObject();
//    if(obj->dataType() == MDDataType::Bool)
//        obj->setCurrentValue(QVariant(value.toBool()));
//    else
//        obj->setCurrentValue(value);
//}

/*!
 * \brief 变量专家Ua包装类
 */
CMDVariableExpertUaWrapper::CMDVariableExpertUaWrapper(QUaServer *server)
                                        :QUaBaseObject(server)
{

}

void CMDVariableExpertUaWrapper::setProp(void* object)
{
    m_p = object;
}

void* CMDVariableExpertUaWrapper::MDObject()
{
    return m_p;
}

QString CMDVariableExpertUaWrapper::readVariable(const QString& name)
{
    // 变量路径名兼容ua格式
    QString varPath = name;
    varPath = varPath.replace("/",".");
    if(varPath.endsWith(".Value"))
        varPath = varPath.left(varPath.length()-QString(".Value").length());
    CMDVariableExpert* obj = (CMDVariableExpert*)MDObject();
    QVariant data = obj->readVariableValue(name);
    //qDebug() << data << ":" << data.userType() << ":" << data.toString();
    return QString("%1:%2").arg(data.userType()).arg(data.toString());
}

bool CMDVariableExpertUaWrapper::writeVariable(const QString& name, const QString& value)
{
    // 变量路径名兼容ua格式
    QString varPath = name;
    varPath = varPath.replace("/",".");
    if(varPath.endsWith(".Value"))
        varPath = varPath.left(varPath.length()-QString(".Value").length());
    CMDVariableExpert* obj = (CMDVariableExpert*)MDObject();
    return obj->writeVariableValue(varPath, value);
}

bool CMDVariableExpertUaWrapper::writeVariableRelative(const QString& name, const QString& relativeValue)
{
    // 变量路径名兼容ua格式
    QString varPath = name;
    varPath = varPath.replace("/",".");
    if(varPath.endsWith(".Value"))
        varPath = varPath.left(varPath.length()-QString(".Value").length());
    CMDVariableExpert* obj = (CMDVariableExpert*)MDObject();
    return obj->writeVariableValueRelative(varPath, relativeValue);
}

bool CMDVariableExpertUaWrapper::writeVariableSwitch01(const QString& name)
{
    // 变量路径名兼容ua格式
    QString varPath = name;
    varPath = varPath.replace("/",".");
    if(varPath.endsWith(".Value"))
        varPath = varPath.left(varPath.length()-QString(".Value").length());
    CMDVariableExpert* obj = (CMDVariableExpert*)MDObject();
    return obj->writeVariableValueSwitch01(varPath);
}

bool CMDVariableExpertUaWrapper::writeVariables(const QList<QString>& names, const QList<QString>& values)
{
    if(names.length() != values.length())
        return false;
    int index = 0;
    bool bret = true;
    foreach (QString n, names) {
        bret &= writeVariable(n,values[index++]);
    }
    return bret;
}

bool CMDVariableExpertUaWrapper::writeVariableWithoutSig(const QString& name, const QString& value)
{
    // 变量路径名兼容ua格式
    QString varPath = name;
    varPath = varPath.replace("/",".");
    if(varPath.endsWith(".Value"))
        varPath = varPath.left(varPath.length()-QString(".Value").length());
    CMDVariableExpert* obj = (CMDVariableExpert*)MDObject();
    return obj->writeVariableValueWithoutSig(varPath, value);
}

bool CMDVariableExpertUaWrapper::writeVariableRelativeWithoutSig(const QString& name, const QString& relativeValue)
{
    // 变量路径名兼容ua格式
    QString varPath = name;
    varPath = varPath.replace("/",".");
    if(varPath.endsWith(".Value"))
        varPath = varPath.left(varPath.length()-QString(".Value").length());
    CMDVariableExpert* obj = (CMDVariableExpert*)MDObject();
    return obj->writeVariableValueRelativeWithoutSig(varPath, relativeValue);
}

bool CMDVariableExpertUaWrapper::writeVariableSwitch01WithoutSig(const QString& name)
{
    // 变量路径名兼容ua格式
    QString varPath = name;
    varPath = varPath.replace("/",".");
    if(varPath.endsWith(".Value"))
        varPath = varPath.left(varPath.length()-QString(".Value").length());
    CMDVariableExpert* obj = (CMDVariableExpert*)MDObject();
    return obj->writeVariableValueSwitch01WithoutSig(varPath);
}

bool CMDVariableExpertUaWrapper::writeVariablesWithoutSig(const QList<QString>& names, const QList<QString>& values)
{
    if(names.length() != values.length())
        return false;
    int index = 0;
    bool bret = true;
    foreach (QString n, names) {
        bret &= writeVariableWithoutSig(n,values[index++]);
    }
    return bret;
}

///*!
// * \brief 归档专家脚本包装类
// */
//CMDDataArchiveExpertWrapper::CMDDataArchiveExpertWrapper(void* object, const QString& name)
//                                      :CMDObjectWrapper(object, name)
//{

//}

//void CMDDataArchiveExpertWrapper::startSetting()
//{
//    CMDHistoryExpert* obj = (CMDHistoryExpert*)MDObject();
//    obj->startSetting();
//}

//void CMDDataArchiveExpertWrapper::endSetting()
//{
//    CMDHistoryExpert* obj = (CMDHistoryExpert*)MDObject();
//    obj->endSetting();
//}

//bool CMDDataArchiveExpertWrapper::setDatabaseParam(const QString &datasourceName, int paraType, const QString &para)
//{
//    CMDHistoryExpert* obj = (CMDHistoryExpert*)MDObject();
//    return obj->setDatabaseParam(datasourceName, paraType, para);
//}

//bool CMDDataArchiveExpertWrapper::saveHisGroup(const QString &groupName)
//{
//    CMDHistoryExpert* obj = (CMDHistoryExpert*)MDObject();
//    return obj->saveHisGroup(groupName);
//}

///*!
// * \brief 查询专家脚本包装类
// */
//CMDDataQueryExpertWrapper::CMDDataQueryExpertWrapper(void* object, const QString& name)
//                                      :CMDObjectWrapper(object, name)
//{

//}

//void CMDDataQueryExpertWrapper::setCurTaskIndex(int curTaskIndex)
//{
//    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
//    obj->setCurTaskIndex(curTaskIndex);
//}

//int CMDDataQueryExpertWrapper::getCurTaskIndex()
//{
//    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
//    return obj->curTaskIndex();
//}

//void CMDDataQueryExpertWrapper::copyDataSourceParameter(const QString& dsName)
//{
//    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
//    obj->copyDataSourceParameter(dsName);
//}

//void CMDDataQueryExpertWrapper::setDataSourceParameter(int paraType, const QString &para)
//{
//    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
//    obj->setDataSourceParameter(paraType, para);
//}

//void CMDDataQueryExpertWrapper::setQueryTable(const QString &tableName)
//{
//    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
//    obj->setQueryTable(tableName);
//}

//void CMDDataQueryExpertWrapper::setQueryFilter(const QString &filter)
//{
//    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
//    obj->setQueryFilter(filter);
//}

//void CMDDataQueryExpertWrapper::setQueryFields(const QString &fields)
//{
//    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
//    obj->setQueryFields(fields);
//}

//bool CMDDataQueryExpertWrapper::query()
//{
//    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
//    return obj->query();
//}

//QVariant CMDDataQueryExpertWrapper::getValue(int row, int col)
//{
//    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
//    QVariant value = obj->value(row, col);
//    return value;
//}

//int CMDDataQueryExpertWrapper::getRowCount()
//{
//    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
//    return obj->rowCount();
//}

//int CMDDataQueryExpertWrapper::getColCount()
//{
//    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
//    return obj->colCount();
//}

/*!
 * \brief HMI 专家Ua包装类
 */
CMDHMIExpertUaWrapper::CMDHMIExpertUaWrapper(QUaServer *server)
                                        :QUaBaseObject(server)
{

}

void CMDHMIExpertUaWrapper::setProp(void* object)
{
    m_p = object;
}

void* CMDHMIExpertUaWrapper::MDObject()
{
    return m_p;
}

bool CMDHMIExpertUaWrapper::open(const QString& name, int pwid/* = 0*/, int x/* = 0*/, int y/* = 0*/, int w/*= 0*/, int h /*= 0*/)
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    return obj->open(name, pwid, x, y, w, h);
}

void CMDHMIExpertUaWrapper::close(const QString& name)
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    obj->close(name);
}

void CMDHMIExpertUaWrapper::move(const QString& name, int left, int top)
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    obj->move(name, left, top);
}

void CMDHMIExpertUaWrapper::hide(const QString& name)
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    obj->hide(name);
}

void CMDHMIExpertUaWrapper::callJsFunc(const QString& name, QString funcName, QString params)
{
    qDebug() << "name:" << name << "funcName:" << funcName << "params:" << params;
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    obj->callJsFunc(name, funcName, params);
}

void CMDHMIExpertUaWrapper::enterFullScreen()
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    obj->enterFullScreen();
}

void CMDHMIExpertUaWrapper::exitFullScreen()
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    obj->exitFullScreen();
}

void CMDHMIExpertUaWrapper::showByPid(int pid, int phwnd/* = 0*/)
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    return obj->showByPid(pid, phwnd);
}

void CMDHMIExpertUaWrapper::hideByPid(int pid, int phwnd/* = 0*/)
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    return obj->hideByPid(pid, phwnd);
}

void CMDHMIExpertUaWrapper::closeByPid(int pid, int phwnd/* = 0*/)
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    obj->closeByPid(pid, phwnd);
}

void CMDHMIExpertUaWrapper::showRecipeRuntime()
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    return obj->showRecipeRuntime();
}
void CMDHMIExpertUaWrapper::hideRecipeRuntime()
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    return obj->hideRecipeRuntime();
}

void CMDHMIExpertUaWrapper::showRuntime()
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    return obj->showRuntime();
}
void CMDHMIExpertUaWrapper::hideRuntime()
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    return obj->hideRuntime();
}

/*!
 * \brief 安全专家Ua包装类
 */
CMDSecurityExpertUaWrapper::CMDSecurityExpertUaWrapper(QUaServer *server)
                                      :QUaBaseObject(server)
{

}

void CMDSecurityExpertUaWrapper::setProp(void* object)
{
    m_p = object;
}

int CMDSecurityExpertUaWrapper::login(QString name, QString password, int checkGroup/* = 0*/)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return (int)(obj->login(name, password, checkGroup));
}

void CMDSecurityExpertUaWrapper::logout()
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    obj->logout();
}

int CMDSecurityExpertUaWrapper::accountsCount()
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->accountsCount();
}

//QString CMDSecurityExpertUaWrapper::getAccounts()
//{
//    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
//    QJsonArray arr;
//    QStringList l = obj->getAccounts();
//    for(QString s : l)
//    {
//        arr.append(s);
//    }
//    auto doc = QJsonDocument(arr);
//    return QString(doc.toJson().data());
//}

QList<QString> CMDSecurityExpertUaWrapper::getAccounts()
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->getAccounts();
}

QList<QString> CMDSecurityExpertUaWrapper::getAccountGroups(QString name)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->getAccountGroups(name);
}

QUaProperty * CMDSecurityExpertUaWrapper::currentAccount() const
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    QUaProperty *prop = const_cast<CMDSecurityExpertUaWrapper*>(this)->browseChild<QUaProperty>("currentAccount");
    prop->setValue(obj->currentAccount());
    return prop;
}

QList<QString> CMDSecurityExpertUaWrapper::getDisabledAccounts()
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->getDisabledAccounts();
}
QList<QString> CMDSecurityExpertUaWrapper::getLockedAccounts()
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->getLockedAccounts();
}
bool CMDSecurityExpertUaWrapper::enableAccount(QString name)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->enableAccount(name);
}
bool CMDSecurityExpertUaWrapper::disableAccount(QString name)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->disableAccount(name);
}
int CMDSecurityExpertUaWrapper::deleteAccount(QString name)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->deleteAccount(name);
}
bool CMDSecurityExpertUaWrapper::lockAccount(QString name)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->lockAccount(name);
}
bool CMDSecurityExpertUaWrapper::unlockAccount(QString name)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->unlockAccount(name);
}
bool CMDSecurityExpertUaWrapper::isAccountEnabled(QString name)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->isAccountEnabled(name);
}
bool CMDSecurityExpertUaWrapper::isAccountLocked(QString name)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->isAccountLocked(name);
}
bool CMDSecurityExpertUaWrapper::isAccountLogin(QString name)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->isAccountLogin(name);
}
QString CMDSecurityExpertUaWrapper::getCurrentAccount()
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->currentAccount();
}
bool CMDSecurityExpertUaWrapper::upsertAccount(QString name, QString comment, QString password, bool enable, bool lock)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->upsertAccount(name, comment, password, enable, lock);
}
QString CMDSecurityExpertUaWrapper::getAccount(QString name)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->getAccount(name);
}
int CMDSecurityExpertUaWrapper::modifyPassword(QString name, QString oldPassword, QString newPassword)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->modifyPassword(name, oldPassword, newPassword);
}
QString CMDSecurityExpertUaWrapper::getAccountOption()
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->getAccountOption();
}
bool CMDSecurityExpertUaWrapper::setAccountOption(QString option)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->setAccountOption(option);
}
bool CMDSecurityExpertUaWrapper::resetAccount(QString name)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->resetAccount(name);
}
bool CMDSecurityExpertUaWrapper::resetAllAccount()
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->resetAllAccount();
}


int CMDSecurityExpertUaWrapper::groupsCount()
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    if(getGroup("*")!="")
        return obj->groupsCount()-1;
    return obj->groupsCount();
}
QString CMDSecurityExpertUaWrapper::getGroup(QString name)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->getGroup(name);
}

QList<QString> CMDSecurityExpertUaWrapper::getGroups()
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    if(getGroup("*")!="") {
        QList<QString> list = obj->getGroups();
        if(list.removeOne("*"))
            return list;
    }
    return obj->getGroups();
}
QList<QString> CMDSecurityExpertUaWrapper::getAccountsInGroup(QString group)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->getAccountsInGroup(group);
}
QList<QString> CMDSecurityExpertUaWrapper::getDisabledGroups()
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->getDisabledGroups();
}
bool CMDSecurityExpertUaWrapper::enableGroup(QString name)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->enableGroup(name);
}
bool CMDSecurityExpertUaWrapper::disableGroup(QString name)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->enableGroup(name);
}
bool CMDSecurityExpertUaWrapper::deleteGroup(QString name)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->deleteGroup(name);
}

bool CMDSecurityExpertUaWrapper::addAccountToGroup(QString account,QString group)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->addAccountToGroup(account, group);
}
bool CMDSecurityExpertUaWrapper::removeAccountFromGroup(QString account,QString group)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->removeAccountFromGroup(account, group);
}
bool CMDSecurityExpertUaWrapper::isAccountInGroup(QString account,QString group)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->isAccountInGroup(account, group);
}

bool CMDSecurityExpertUaWrapper::commit()
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->commit();
}

bool CMDSecurityExpertUaWrapper::upsertGroup(QString name, QString comment, bool enable)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->upsertGroup(name, comment, enable);
}

int CMDSecurityExpertUaWrapper::accountSignature(QString actionInfo,QString sigConf)
{
    CMDSecurityPolicyExpert* obj = (CMDSecurityPolicyExpert*)m_p;
    return obj->accountSignature(actionInfo, sigConf);
}





/*!
 * \brief 报警专家Ua包装类
 */
CMDAlarmExpertUaWrapper::CMDAlarmExpertUaWrapper(QUaServer *server)
                                      :QUaBaseObject(server)
{

}

void CMDAlarmExpertUaWrapper::setProp(void* object)
{
    m_p = object;
}


int CMDAlarmExpertUaWrapper::getAlarmCount()
{
    CMDAlarmExpert* obj = (CMDAlarmExpert*)m_p;
    return obj->getAlarmCount();
}

QString CMDAlarmExpertUaWrapper::getAlarmByIndex(int index)
{
    CMDAlarmExpert* obj = (CMDAlarmExpert*)m_p;
    return obj->getAlarmByIndex(index);
}

QString CMDAlarmExpertUaWrapper::getAlarmById(QString id)
{
    CMDAlarmExpert* obj = (CMDAlarmExpert*)m_p;
    return obj->getAlarmById(id);
}

QString CMDAlarmExpertUaWrapper::getAlarmConfById(QString id)
{
    CMDAlarmExpert* obj = (CMDAlarmExpert*)m_p;
    return obj->getAlarmConfById(id);
}

bool CMDAlarmExpertUaWrapper::acknowledge(QString alarmId, QString comment/* = ""*/)
{
    CMDAlarmExpert* obj = (CMDAlarmExpert*)m_p;
    return obj->acknowledge(alarmId, comment);
}

bool CMDAlarmExpertUaWrapper::acknowledgeAll()
{
    CMDAlarmExpert* obj = (CMDAlarmExpert*)m_p;
    obj->acknowledgeAll();
    return true;
}

bool CMDAlarmExpertUaWrapper::confirm(QString alarmId, QString comment/* = ""*/)
{
    CMDAlarmExpert* obj = (CMDAlarmExpert*)m_p;
    return obj->confirm(alarmId, comment);
}

bool CMDAlarmExpertUaWrapper::confirmAll()
{
    CMDAlarmExpert* obj = (CMDAlarmExpert*)m_p;
    obj->confirmAll();
    return true;
}

bool CMDAlarmExpertUaWrapper::suppress(QString alarmId, QString comment/* = ""*/)
{
    CMDAlarmExpert* obj = (CMDAlarmExpert*)m_p;
    return obj->suppress(alarmId, comment);
}
bool CMDAlarmExpertUaWrapper::shelve(QString alarmId, QString comment/* = ""*/)
{
    CMDAlarmExpert* obj = (CMDAlarmExpert*)m_p;
    return obj->shelve(alarmId, comment);
}
bool CMDAlarmExpertUaWrapper::addComment(QString alarmId, QString comment/* = ""*/)
{
    CMDAlarmExpert* obj = (CMDAlarmExpert*)m_p;
    return obj->addComment(alarmId, comment);
}



/*!
 * \brief 配方专家Ua包装类
 */
CMDRecipeExpertUaWrapper::CMDRecipeExpertUaWrapper(QUaServer* server)
                                      :QUaBaseObject(server)
{

}

void CMDRecipeExpertUaWrapper::setProp(void* object)
{
    m_p = object;
}

//int CMDRecipeExpertUaWrapper::beginEdit(const QString& recipeName)
//{
//    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
//    return obj->configer()->beginEdit(recipeName);
//}

//int CMDRecipeExpertUaWrapper::endEdit(const QString& recipeName, bool save)
//{
//    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
//    return obj->configer()->endEdit(recipeName, save);
//}

//int CMDRecipeExpertUaWrapper::saveEdit(const QString& recipeName)
//{
//    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
//    return obj->configer()->saveEdit(recipeName);
//}

//int CMDRecipeExpertUaWrapper::addItem(const QString& recipeName, const QString& id, const QString& value)
//{
//    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
//    CMDRecipe* recipe = obj->configer()->findEdit(recipeName);
//    if(!recipe)
//        return false;

//    return recipe->configProperty()->items()->addItem(id, value);
//}

//int CMDRecipeExpertUaWrapper::deleteItem(const QString& recipeName, const QString& id)
//{
//    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
//    CMDRecipe* recipe = obj->configer()->findEdit(recipeName);
//    if(!recipe)
//        return false;

//    return recipe->configProperty()->items()->deleteItem(id);
//}

//int CMDRecipeExpertUaWrapper::setItemValue(const QString& recipeName, const QString& id, const QString& value)
//{
//    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
//    CMDRecipe* recipe = obj->configer()->findEdit(recipeName);
//    if(!recipe)
//        return false;

//    return recipe->configProperty()->items()->setItemValue(id, value);
//}

//QString CMDRecipeExpertUaWrapper::getItemValue(const QString& recipeName, const QString& id)
//{
//    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
//    CMDRecipe* recipe = obj->configer()->findEdit(recipeName);
//    if(!recipe)
//        return "";

//    return recipe->configProperty()->items()->itemValue(id);
//}

int CMDRecipeExpertUaWrapper::create(const QString& recipeName)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
    bool b = obj->configer()->create(recipeName);
    return b ? 0 : -1;
}

int CMDRecipeExpertUaWrapper::copy(const QString& srcRecipeName, const QString& destRecipeName)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
    bool b = obj->configer()->create(srcRecipeName, destRecipeName);
    return b ? 0 : -1;
}

int CMDRecipeExpertUaWrapper::remove(const QString& recipeName)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
    bool b = obj->configer()->remove(recipeName);
    return b ? 0 : -1;
}

int CMDRecipeExpertUaWrapper::exists(const QString& recipeName)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
    bool b = obj->configer()->exists(recipeName);
    return b ? 0 : -1;
}

int CMDRecipeExpertUaWrapper::save(const QString& recipeName, const QString& content)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
    bool b = obj->configer()->save(recipeName, content);
    return b ? 0 : -1;
}

QString CMDRecipeExpertUaWrapper::getContent(const QString& recipeName)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
    return obj->configer()->getContent(recipeName);
}

QString CMDRecipeExpertUaWrapper::toFileName(const QString& recipeName)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
    return obj->configer()->toFileName(recipeName);
}

int CMDRecipeExpertUaWrapper::download(const QString& recipeName, int showProcessDialog/* = 1*/)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
    bool b = obj->download(recipeName, showProcessDialog);
    return b ? 0 : -1;
}

int CMDRecipeExpertUaWrapper::upload(const QString& recipeName)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
    bool b = obj->upload(recipeName);
    return b ? 0 : -1;
}

int CMDRecipeExpertUaWrapper::getRecipeCount(bool onlyRecipe)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
    int count = obj->configer()->getRecipeCount(onlyRecipe);
    return count;
}

QString CMDRecipeExpertUaWrapper::getRecipeName(int index)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
    QString name = obj->configer()->recipeName(index);
    return name;
}

QList<QString> CMDRecipeExpertUaWrapper::enumRecipe(bool onlyRecipe)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)m_p;
    return obj->configer()->enumRecipe(onlyRecipe);
}
