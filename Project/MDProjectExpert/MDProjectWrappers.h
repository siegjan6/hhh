/*!

\author dGFuZ3p3

\section 功能

子模块专家的脚本包装类，封装了模块可供脚本调用的接口。

*/

#ifndef MDPROJECTWRAPPERS_H
#define MDPROJECTWRAPPERS_H

#include <QString>
#include <QVariant>

#include "../../include/MDScriptEngine/MDObjectWrapper.h"

/*!
 * \brief 项目专家脚本包装类
 */
class CMDProjectExpertImp;
class CMDProjectExpertWrapper : public CMDObjectWrapper
{
    Q_OBJECT
public:
    CMDProjectExpertWrapper(void* object, const QString& name);
public Q_SLOTS:
    void trace(const QString& msg, int level = 0, const QString& source = "");
    QString readTextFile(const QString& fileName);
};

/*!
 * \brief 变量对象脚本包装类
 */
class CMDVariable;
class CMDVariableWrapper : public CMDObjectWrapper
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ getValue WRITE setValue)
public:
    CMDVariableWrapper(void* object, const QString& name);
protected:
    QVariant getValue();
    void setValue(const QVariant& value);
};

/*!
 * \brief 变量专家脚本包装类
 */
class CMDVariableExpert;
class CMDVariableExpertWrapper : public CMDObjectWrapper
{
    Q_OBJECT
public:
    CMDVariableExpertWrapper(void* object, const QString& name);
public Q_SLOTS:
    QVariant readValue(const QString& name);
    bool writeValue(const QString& name, const QVariant& value);
    bool writeValueRelative(const QString& name, const QVariant& relativeValue);
    bool writeValueSwitch01(const QString& name);
};

/*!
 * \brief 归档专家脚本包装类
 */
class CMDHistoryExpert;
class CMDDataArchiveExpertWrapper : public CMDObjectWrapper
{
    Q_OBJECT
public:
    CMDDataArchiveExpertWrapper(void* object, const QString& name);
public Q_SLOTS:
    bool setDatabaseParam(const QString &datasourceName, int paraType, const QString &para);
    void startSetting();
    void endSetting();
    bool saveHisGroup(const QString &groupName);
    int excuteSQLCommand(const QString &datasourceName, const QString& sqlCmd);
    QString getLastError(const QString& datasourceName);
};

/*!
 * \brief 查询专家脚本包装类
 */
class CMDQueryTaskMgr;
class CMDDataQueryExpertWrapper : public CMDObjectWrapper
{
    Q_OBJECT
public:
    CMDDataQueryExpertWrapper(void* object, const QString& name);
public Q_SLOTS:
    void setCurTaskIndex(int curTaskIndex);
    int getCurTaskIndex();

    void copyDataSourceParameter(const QString& dsName);
    void setDataSourceParameter(int paraType, const QString &para);
    void setQueryTable(const QString &tableName);
    void setQueryFilter(const QString &filter);
    void setQueryFields(const QString &fields);
    bool query();

    QVariant getValue(int row, int col);
    int getRowCount();
    int getColCount();
};

/*!
 * \brief 数据库自定义查询专家脚本包装类
 */
class CMDDBQueryExpert;
class CMDDBQueryExpertWrapper : public CMDObjectWrapper
{
    Q_OBJECT
public:
    CMDDBQueryExpertWrapper(void* object, const QString& name);
public Q_SLOTS:
    void addDataBaseConnection(const QString& dbConName);
    int connectDataBase(const QString& dbConName, const QVariant& serverProperty, const QString& dbName);
    int disconnectDataBase(const QString& dbConName);
    int deleteDataBaseConnection(const QString& dbConName);

    int excuteSQLCommand(const QString& dbConName, const QString& sqlCmd);
    QString getLastError(const QString& dbConName);
    void addRecordSet(const QString& recordSetName);
    int updateRecordSetData(const QString& dbConName, const QString& recordSetName, const QString& sqlCmd);
    int getRecordSetRowCount(const QString& recordSetName);
    int getRecordSetColumnCount(const QString& recordSetName);
    QVariant getRecordSetValue(const QString& recordSetName, int row, int col);
    int deleteRecordSet(const QString& recordSetName);
};

/*!
 * \brief HMI 专家脚本包装类
 */
class CMDHMIExpertProxy;
class CMDHMIExpertWrapper : public CMDObjectWrapper
{
    Q_OBJECT
public:
    CMDHMIExpertWrapper(void* object, const QString& name);
public Q_SLOTS:
    bool open(const QString& name);
    void close(const QString& name);
    void hide(const QString& name);
    void move(const QString& name, int left, int top);
    void callJsFunc(const QString& name, QString funcName, QString params);


    void enterFullScreen();
    void exitFullScreen();

    void showRecipeRuntime();
    void hideRecipeRuntime();

    void showRuntime();
    void hideRuntime();
};

/*!
 * \brief 安全专家脚本包装类
 */
class CMDProjectExpertImp;
class CMDSecurityExpertWrapper : public CMDObjectWrapper
{
    Q_OBJECT
public:
    CMDSecurityExpertWrapper(void* object, const QString& name);
public Q_SLOTS:
    void login();
    int login(const QString& name, const QString& password, int checkGroup = 0);
    void loginByAccount(const QString& name);
    void logout();
    void showManager();
    QString currentAccount();
    int currentLoginResult();
};

/*!
 * \brief 配方专家脚本包装类
 */
class CMDRecipeExpert;
class CMDRecipeExpertWrapper : public CMDObjectWrapper
{
    Q_OBJECT
public:
    CMDRecipeExpertWrapper(void* object, const QString& name);
public Q_SLOTS:
    bool beginEdit(const QString& recipeName);
    bool endEdit(const QString& recipeName, bool save);
    bool saveEdit(const QString& name);

    bool addItem(const QString& recipeName, const QString& id, const QString& value);
    bool deleteItem(const QString& recipeName, const QString& id);
    bool setItemValue(const QString& recipeName, const QString& id, const QString& value);
    QString getItemValue(const QString& recipeName, const QString& id);

    int create(const QString& recipeName);
    int copy(const QString& destRecipeName, const QString& srcRecipeName);
    int remove(const QString& recipeName);
    int exists(const QString& recipeName);
    QString toFileName(const QString& recipeName);
    int download(const QString& recipeName);
    int upload(const QString& recipeName);

    int getRecipeCount(bool onlyRecipe);
    QString getRecipeName(int index);
    QList<QString> enumRecipe(bool onlyRecipe);
    QString getContent(const QString& recipeName);
};

#endif // MDPROJECTWRAPPERS_H
