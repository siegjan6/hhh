#include "MDProjectWrappers.h"
#include "MDProjectExpertImp.h"

#include "../../include/MDVariableExpert/MDVariableExpert.h"
#include "MDDataArchivesEngine.h"
#include "../../include/MDRTDBInterfaces/IMDRTDBVariable.h"
#include "MDRecipe.h"
#include "MDRecipeConfiger.h"
#include "../../include/MDDBQueryService/dbqueryexpert.h"

/*!
 * \brief 项目专家脚本包装类
 */
CMDProjectExpertWrapper::CMDProjectExpertWrapper(void* object, const QString& name)
                                      :CMDObjectWrapper(object, name)
{

}

void CMDProjectExpertWrapper::trace(const QString& msg, int level/* = 0*/, const QString& source/* = ""*/)
{
    CMDProjectExpertImp* obj = (CMDProjectExpertImp*)MDObject();
    obj->trace(msg, level, source);
}

QString CMDProjectExpertWrapper::readTextFile(const QString& fileName)
{
    CMDProjectExpertImp* obj = (CMDProjectExpertImp*)MDObject();
    return obj->readTextFile(fileName);
}

/*!
 * \brief 变量对象脚本包装类
 */
CMDVariableWrapper::CMDVariableWrapper(void* object, const QString& name)
                                      :CMDObjectWrapper(object, name)
{
}

QVariant CMDVariableWrapper::getValue()
{
    IMDRTDBVariable* obj = (IMDRTDBVariable*)MDObject();
    return obj->currentValue();
}

void CMDVariableWrapper::setValue(const QVariant& value)
{
    IMDRTDBVariable* obj = (IMDRTDBVariable*)MDObject();
    if(obj->dataType() == MDDataType::Bool)
        obj->setCurrentValue(QVariant(value.toBool()));
    else
        obj->setCurrentValue(value);
}

/*!
 * \brief 变量专家脚本包装类
 */
CMDVariableExpertWrapper::CMDVariableExpertWrapper(void* object, const QString& name)
                                      :CMDObjectWrapper(object, name)
{

}

QVariant CMDVariableExpertWrapper::readValue(const QString& name)
{
    CMDVariableExpert* obj = (CMDVariableExpert*)MDObject();
    QVariant data = obj->readVariableValue(name);
    return data;
}

bool CMDVariableExpertWrapper::writeValue(const QString& name, const QVariant& value)
{
    CMDVariableExpert* obj = (CMDVariableExpert*)MDObject();
    return obj->writeVariableValue(name, value);
}

bool CMDVariableExpertWrapper::writeValueRelative(const QString& name, const QVariant& relativeValue)
{
    CMDVariableExpert* obj = (CMDVariableExpert*)MDObject();
    return obj->writeVariableValueRelative(name, relativeValue);
}

bool CMDVariableExpertWrapper::writeValueSwitch01(const QString& name)
{
    CMDVariableExpert* obj = (CMDVariableExpert*)MDObject();
    return obj->writeVariableValueSwitch01(name);
}

/*!
 * \brief 归档专家脚本包装类
 */
CMDDataArchiveExpertWrapper::CMDDataArchiveExpertWrapper(void* object, const QString& name)
                                      :CMDObjectWrapper(object, name)
{

}

void CMDDataArchiveExpertWrapper::startSetting()
{
    CMDHistoryExpert* obj = (CMDHistoryExpert*)MDObject();
    obj->startSetting();
}

void CMDDataArchiveExpertWrapper::endSetting()
{
    CMDHistoryExpert* obj = (CMDHistoryExpert*)MDObject();
    obj->endSetting();
}

bool CMDDataArchiveExpertWrapper::setDatabaseParam(const QString &datasourceName, int paraType, const QString &para)
{
    CMDHistoryExpert* obj = (CMDHistoryExpert*)MDObject();
    return obj->setDatabaseParam(datasourceName, paraType, para);
}

bool CMDDataArchiveExpertWrapper::saveHisGroup(const QString &groupName)
{
    CMDHistoryExpert* obj = (CMDHistoryExpert*)MDObject();
    return obj->saveHisGroup(groupName);
}

int CMDDataArchiveExpertWrapper::excuteSQLCommand(const QString& datasourceName, const QString& sqlCmd)
{
    CMDHistoryExpert* obj = (CMDHistoryExpert*)MDObject();
    return obj->excuteSQLCommand(datasourceName, sqlCmd);
}

QString CMDDataArchiveExpertWrapper::getLastError(const QString& datasourceName)
{
    CMDHistoryExpert* obj = (CMDHistoryExpert*)MDObject();
    return obj->getLastError(datasourceName);
}

/*!
 * \brief 查询专家脚本包装类
 */
CMDDataQueryExpertWrapper::CMDDataQueryExpertWrapper(void* object, const QString& name)
                                      :CMDObjectWrapper(object, name)
{

}

void CMDDataQueryExpertWrapper::setCurTaskIndex(int curTaskIndex)
{
    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
    obj->setCurTaskIndex(curTaskIndex);
}

int CMDDataQueryExpertWrapper::getCurTaskIndex()
{
    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
    return obj->curTaskIndex();
}

void CMDDataQueryExpertWrapper::copyDataSourceParameter(const QString& dsName)
{
    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
    obj->copyDataSourceParameter(dsName);
}

void CMDDataQueryExpertWrapper::setDataSourceParameter(int paraType, const QString &para)
{
    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
    obj->setDataSourceParameter(paraType, para);
}

void CMDDataQueryExpertWrapper::setQueryTable(const QString &tableName)
{
    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
    obj->setQueryTable(tableName);
}

void CMDDataQueryExpertWrapper::setQueryFilter(const QString &filter)
{
    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
    obj->setQueryFilter(filter);
}

void CMDDataQueryExpertWrapper::setQueryFields(const QString &fields)
{
    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
    obj->setQueryFields(fields);
}

bool CMDDataQueryExpertWrapper::query()
{
    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
    return obj->query();
}

QVariant CMDDataQueryExpertWrapper::getValue(int row, int col)
{
    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
    QVariant value = obj->value(row, col);
    return value;
}

int CMDDataQueryExpertWrapper::getRowCount()
{
    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
    return obj->rowCount();
}

int CMDDataQueryExpertWrapper::getColCount()
{
    CMDQueryTaskMgr* obj = (CMDQueryTaskMgr*)MDObject();
    return obj->colCount();
}


/*!
 * \brief 数据库自定义查询专家脚本包装类
 */
CMDDBQueryExpertWrapper::CMDDBQueryExpertWrapper(void* object, const QString& name)
                                      :CMDObjectWrapper(object, name)
{

}

void CMDDBQueryExpertWrapper::addDataBaseConnection(const QString& dbConName)
{
    CMDDBQueryExpert* obj = (CMDDBQueryExpert*)MDObject();
    obj->addDataBaseConnection(dbConName);
}
int CMDDBQueryExpertWrapper::connectDataBase(const QString& dbConName, const QVariant& serverProperty, const QString& dbName)
{
    CMDDBQueryExpert* obj = (CMDDBQueryExpert*)MDObject();
    return obj->connectDataBase(dbConName, serverProperty, dbName);
}
int CMDDBQueryExpertWrapper::disconnectDataBase(const QString& dbConName)
{
    CMDDBQueryExpert* obj = (CMDDBQueryExpert*)MDObject();
    return obj->disconnectDataBase(dbConName);
}
int CMDDBQueryExpertWrapper::deleteDataBaseConnection(const QString& dbConName)
{
    CMDDBQueryExpert* obj = (CMDDBQueryExpert*)MDObject();
    return obj->deleteDataBaseConnection(dbConName);
}
int CMDDBQueryExpertWrapper::excuteSQLCommand(const QString& dbConName, const QString& sqlCmd)
{
    CMDDBQueryExpert* obj = (CMDDBQueryExpert*)MDObject();
    return obj->excuteSQLCommand(dbConName, sqlCmd);
}
QString CMDDBQueryExpertWrapper::getLastError(const QString& dbConName)
{
    CMDDBQueryExpert* obj = (CMDDBQueryExpert*)MDObject();
    return obj->getLastError(dbConName);
}
void CMDDBQueryExpertWrapper::addRecordSet(const QString& recordSetName)
{
    CMDDBQueryExpert* obj = (CMDDBQueryExpert*)MDObject();
    obj->addRecordSet(recordSetName);
}
int CMDDBQueryExpertWrapper::updateRecordSetData(const QString& dbConName, const QString& recordSetName, const QString& sqlCmd)
{
    CMDDBQueryExpert* obj = (CMDDBQueryExpert*)MDObject();
    return obj->updateRecordSetData(dbConName, recordSetName, sqlCmd);
}
int CMDDBQueryExpertWrapper::getRecordSetRowCount(const QString& recordSetName)
{
    CMDDBQueryExpert* obj = (CMDDBQueryExpert*)MDObject();
    return obj->getRecordSetRowCount(recordSetName);
}
int CMDDBQueryExpertWrapper::getRecordSetColumnCount(const QString& recordSetName)
{
    CMDDBQueryExpert* obj = (CMDDBQueryExpert*)MDObject();
    return obj->getRecordSetColumnCount(recordSetName);
}
QVariant CMDDBQueryExpertWrapper::getRecordSetValue(const QString& recordSetName, int row, int col)
{
    CMDDBQueryExpert* obj = (CMDDBQueryExpert*)MDObject();
    return obj->getRecordSetValue(recordSetName, row, col);
}
int CMDDBQueryExpertWrapper::deleteRecordSet(const QString& recordSetName)
{
    CMDDBQueryExpert* obj = (CMDDBQueryExpert*)MDObject();
    return obj->deleteRecordSet(recordSetName);
}



/*!
 * \brief HMI 专家脚本包装类
 */
CMDHMIExpertWrapper::CMDHMIExpertWrapper(void* object, const QString& name)
                                      :CMDObjectWrapper(object, name)
{

}

bool CMDHMIExpertWrapper::open(const QString& name)
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    return obj->open(name);
}

void CMDHMIExpertWrapper::close(const QString& name)
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    obj->close(name);
}

void CMDHMIExpertWrapper::hide(const QString& name)
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    obj->hide(name);
}

void CMDHMIExpertWrapper::move(const QString& name, int left, int top)
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    obj->move(name, left, top);
}

void CMDHMIExpertWrapper::callJsFunc(const QString& name, QString funcName, QString params)
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    obj->callJsFunc(name, funcName, params);
}

void CMDHMIExpertWrapper::enterFullScreen()
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    obj->enterFullScreen();
}

void CMDHMIExpertWrapper::exitFullScreen()
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    obj->exitFullScreen();
}

void CMDHMIExpertWrapper::showRecipeRuntime()
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    obj->showRecipeRuntime();
}

void CMDHMIExpertWrapper::hideRecipeRuntime()
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    obj->hideRecipeRuntime();
}

void CMDHMIExpertWrapper::showRuntime()
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    obj->showRuntime();
}

void CMDHMIExpertWrapper::hideRuntime()
{
    CMDHMIExpertProxy* obj = (CMDHMIExpertProxy*)MDObject();
    obj->hideRuntime();
}

/*!
 * \brief 安全专家脚本包装类
 */
CMDSecurityExpertWrapper::CMDSecurityExpertWrapper(void* object, const QString& name)
                                      :CMDObjectWrapper(object, name)
{

}

void CMDSecurityExpertWrapper::login()
{
    CMDProjectExpertImp* obj = (CMDProjectExpertImp*)MDObject();
    obj->login();
}

int CMDSecurityExpertWrapper::login(const QString& name, const QString& password, int checkGroup/* = 0*/)
{
    CMDProjectExpertImp* obj = (CMDProjectExpertImp*)MDObject();
    return obj->login(name, password, checkGroup);
}

void CMDSecurityExpertWrapper::loginByAccount(const QString& name)
{
    CMDProjectExpertImp* obj = (CMDProjectExpertImp*)MDObject();
    obj->loginByAccount(name);
}

void CMDSecurityExpertWrapper::logout()
{
    CMDProjectExpertImp* obj = (CMDProjectExpertImp*)MDObject();
    obj->logout();
}

void CMDSecurityExpertWrapper::showManager()
{
    CMDProjectExpertImp* obj = (CMDProjectExpertImp*)MDObject();
    obj->showAccountAndPowerGroupManager();
}
QString CMDSecurityExpertWrapper::currentAccount()
{
    CMDProjectExpertImp* obj = (CMDProjectExpertImp*)MDObject();
    return obj->currentAccount();
}
int CMDSecurityExpertWrapper::currentLoginResult()
{
    CMDProjectExpertImp* obj = (CMDProjectExpertImp*)MDObject();
    return (int)obj->currentLoginResult();
}

/*!
 * \brief 配方专家脚本包装类
 */
CMDRecipeExpertWrapper::CMDRecipeExpertWrapper(void* object, const QString& name)
                                      :CMDObjectWrapper(object, name)
{

}

bool CMDRecipeExpertWrapper::beginEdit(const QString& recipeName)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    return obj->configer()->beginEdit(recipeName);
}

bool CMDRecipeExpertWrapper::endEdit(const QString& recipeName, bool save)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    return obj->configer()->endEdit(recipeName, save);
}

bool CMDRecipeExpertWrapper::saveEdit(const QString& recipeName)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    return obj->configer()->saveEdit(recipeName);
}

bool CMDRecipeExpertWrapper::addItem(const QString& recipeName, const QString& id, const QString& value)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    CMDRecipe* recipe = obj->configer()->findEdit(recipeName);
    if(!recipe)
        return false;

    return recipe->configProperty()->items()->addItem(id, value);
}

bool CMDRecipeExpertWrapper::deleteItem(const QString& recipeName, const QString& id)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    CMDRecipe* recipe = obj->configer()->findEdit(recipeName);
    if(!recipe)
        return false;

    return recipe->configProperty()->items()->deleteItem(id);
}

bool CMDRecipeExpertWrapper::setItemValue(const QString& recipeName, const QString& id, const QString& value)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    CMDRecipe* recipe = obj->configer()->findEdit(recipeName);
    if(!recipe)
        return false;

    return recipe->configProperty()->items()->setItemValue(id, value);
}

QString CMDRecipeExpertWrapper::getItemValue(const QString& recipeName, const QString& id)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    CMDRecipe* recipe = obj->configer()->findEdit(recipeName);
    if(!recipe)
        return "";

    return recipe->configProperty()->items()->itemValue(id);
}

int CMDRecipeExpertWrapper::create(const QString& recipeName)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    bool b = obj->configer()->create(recipeName);
    return b ? 0 : -1;
}

int CMDRecipeExpertWrapper::copy(const QString& destRecipeName, const QString& srcRecipeName)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    bool b = obj->configer()->create(destRecipeName, srcRecipeName);
    return b ? 0 : -1;
}

int CMDRecipeExpertWrapper::remove(const QString& recipeName)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    bool b = obj->configer()->remove(recipeName);
    return b ? 0 : -1;
}

int CMDRecipeExpertWrapper::exists(const QString& recipeName)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    bool b = obj->configer()->exists(recipeName);
    return b ? 0 : -1;
}

QString CMDRecipeExpertWrapper::toFileName(const QString& recipeName)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    return obj->configer()->toFileName(recipeName);
}

int CMDRecipeExpertWrapper::download(const QString& recipeName)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    bool b = obj->download(recipeName);
    return b ? 0 : -1;
}

int CMDRecipeExpertWrapper::upload(const QString& recipeName)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    bool b = obj->upload(recipeName);
    return b ? 0 : -1;
}

int CMDRecipeExpertWrapper::getRecipeCount(bool onlyRecipe)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    int count = obj->configer()->getRecipeCount(onlyRecipe);
    return count;
}

QString CMDRecipeExpertWrapper::getRecipeName(int index)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    QString name = obj->configer()->recipeName(index);
    return name;
}

QList<QString> CMDRecipeExpertWrapper::enumRecipe(bool onlyRecipe)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    return obj->configer()->enumRecipe(onlyRecipe);
}

QString CMDRecipeExpertWrapper::getContent(const QString& recipeName)
{
    CMDRecipeExpert* obj = (CMDRecipeExpert*)MDObject();
    return obj->configer()->getContent(recipeName);
}

