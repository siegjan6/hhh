/*!

\author bosmutus

\section 功能

子模块专家的Ua包装类，封装了模块可供Ua调用的接口。

*/

#ifndef MDPROJECTUAWRAPPERS_H
#define MDPROJECTUAWRAPPERS_H

#include <QString>
#include <QVariant>
#include <QUaBaseObject>

#include "../../include/MDScriptEngine/MDObjectWrapper.h"

/*!
 * \brief 项目专家脚本包装类
 */
class CMDProjectExpertImp;
class CMDProjectExpertUaWrapper : public QUaBaseObject
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit CMDProjectExpertUaWrapper(QUaServer *server);

public:
    void setProp(void* object);

private:
    void* m_p;
    void* MDObject();
};

///*!
// * \brief 变量对象脚本包装类
// */
//class CMDVariable;
//class CMDVariableWrapper : public CMDObjectWrapper
//{
//    Q_OBJECT
//    Q_PROPERTY(QVariant value READ getValue WRITE setValue)
//public:
//    CMDVariableWrapper(void* object, const QString& name);
//protected:
//    QVariant getValue();
//    void setValue(const QVariant& value);
//};

/*!
 * \brief 变量专家Ua包装类
 */
class CMDVariableExpert;
class CMDVariableExpertUaWrapper : public QUaBaseObject
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit CMDVariableExpertUaWrapper(QUaServer *server);
public:
    Q_INVOKABLE QString readVariable(const QString& name);
    Q_INVOKABLE bool writeVariable(const QString& name, const QString& value);
    Q_INVOKABLE bool writeVariableRelative(const QString& name, const QString& relativeValue);
    Q_INVOKABLE bool writeVariableSwitch01(const QString& name);
    Q_INVOKABLE bool writeVariables(const QList<QString>& names, const QList<QString>& values);
    Q_INVOKABLE bool writeVariableWithoutSig(const QString& name, const QString& value);
    Q_INVOKABLE bool writeVariablesWithoutSig(const QList<QString>& names, const QList<QString>& values);
    Q_INVOKABLE bool writeVariableRelativeWithoutSig(const QString& name, const QString& relativeValue);
    Q_INVOKABLE bool writeVariableSwitch01WithoutSig(const QString& name);
public:
    void setProp(void* object);

private:
    void* m_p;
    void* MDObject();
};

///*!
// * \brief 归档专家脚本包装类
// */
//class CMDHistoryExpert;
//class CMDDataArchiveExpertWrapper : public CMDObjectWrapper
//{
//    Q_OBJECT
//public:
//    CMDDataArchiveExpertWrapper(void* object, const QString& name);
//public Q_SLOTS:
//    bool setDatabaseParam(const QString &datasourceName, int paraType, const QString &para);
//    void startSetting();
//    void endSetting();
//    bool saveHisGroup(const QString &groupName);
//};

///*!
// * \brief 查询专家脚本包装类
// */
//class CMDQueryTaskMgr;
//class CMDDataQueryExpertWrapper : public CMDObjectWrapper
//{
//    Q_OBJECT
//public:
//    CMDDataQueryExpertWrapper(void* object, const QString& name);
//public Q_SLOTS:
//    void setCurTaskIndex(int curTaskIndex);
//    int getCurTaskIndex();

//    void copyDataSourceParameter(const QString& dsName);
//    void setDataSourceParameter(int paraType, const QString &para);
//    void setQueryTable(const QString &tableName);
//    void setQueryFilter(const QString &filter);
//    void setQueryFields(const QString &fields);
//    bool query();

//    QVariant getValue(int row, int col);
//    int getRowCount();
//    int getColCount();
//};

///*!
// * \brief HMI 专家Ua包装类
// */
//class CMDHMIExpertProxy;
class CMDHMIExpertUaWrapper : public QUaBaseObject
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit CMDHMIExpertUaWrapper(QUaServer *server);
public:
    // x==y==w==h==0: default pos and default size
    Q_INVOKABLE bool open(const QString& name, int pwid/* = 0*/, int x/* = 0*/, int y/* = 0*/, int w/* = 0*/, int h/* = 0*/);
    Q_INVOKABLE void close(const QString& name);
    Q_INVOKABLE void move(const QString& name, int left, int top);
    Q_INVOKABLE void hide(const QString& name);
    Q_INVOKABLE void callJsFunc(const QString& name, QString funcName, QString params);

    // phwnd=0: 全局窗口枚举, phwnd>0: 窗口句柄为phwnd的窗口的子窗体枚举
    Q_INVOKABLE void showByPid(int pid, int phwnd/* = 0*/);
    Q_INVOKABLE void hideByPid(int pid, int phwnd/* = 0*/);
    Q_INVOKABLE void closeByPid(int pid, int phwnd/* = 0*/);


    Q_INVOKABLE void enterFullScreen();
    Q_INVOKABLE void exitFullScreen();

    Q_INVOKABLE void showRecipeRuntime();
    Q_INVOKABLE void hideRecipeRuntime();

    Q_INVOKABLE void showRuntime();
    Q_INVOKABLE void hideRuntime();

public:
    void setProp(void* object);

private:
    void* m_p;
    void* MDObject();
};

/*!
 * \brief 安全专家Ua包装类
 */
class CMDSecurityExpertUaWrapper : public QUaBaseObject
{
    Q_OBJECT

    Q_PROPERTY(QUaProperty * currentAccount READ currentAccount)

public:
    Q_INVOKABLE explicit CMDSecurityExpertUaWrapper(QUaServer *server);

public:
    // checkGroup = 0 默认不检测账户name是否分配权限
    // checkGroup = 1 检测账户name是否分配权限,未分配至权限组的账户禁止登陆
    Q_INVOKABLE int login(QString name, QString password, int checkGroup = 0);
    Q_INVOKABLE void logout();
    Q_INVOKABLE int accountsCount();
    //Q_INVOKABLE QString getAccounts();
    Q_INVOKABLE QList<QString> getAccounts();
    Q_INVOKABLE QList<QString> getAccountGroups(QString name);
    Q_INVOKABLE QList<QString> getDisabledAccounts();
    Q_INVOKABLE QList<QString> getLockedAccounts();
    Q_INVOKABLE bool enableAccount(QString name);
    Q_INVOKABLE bool disableAccount(QString name);
    Q_INVOKABLE int deleteAccount(QString name);
    Q_INVOKABLE bool lockAccount(QString name);
    Q_INVOKABLE bool unlockAccount(QString name);
    Q_INVOKABLE bool isAccountEnabled(QString name);
    Q_INVOKABLE bool isAccountLocked(QString name);
    Q_INVOKABLE bool isAccountLogin(QString name);
    Q_INVOKABLE QString getCurrentAccount();
    Q_INVOKABLE bool upsertAccount(QString name, QString comment, QString password, bool enable, bool lock);
    Q_INVOKABLE QString getAccount(QString name);
    Q_INVOKABLE int modifyPassword(QString name, QString oldPassword, QString newPassword);
    Q_INVOKABLE bool resetAccount(QString name);
    Q_INVOKABLE bool resetAllAccount();


    Q_INVOKABLE int groupsCount();
    Q_INVOKABLE QString getGroup(QString name);
    Q_INVOKABLE QList<QString> getGroups();
    Q_INVOKABLE QList<QString> getAccountsInGroup(QString group);
    Q_INVOKABLE QList<QString> getDisabledGroups();
    Q_INVOKABLE bool enableGroup(QString name);
    Q_INVOKABLE bool disableGroup(QString name);
    Q_INVOKABLE bool deleteGroup(QString name);
    // modifyGroupByUI()
    Q_INVOKABLE bool addAccountToGroup(QString account,QString group);
    Q_INVOKABLE bool removeAccountFromGroup(QString account,QString group);
    Q_INVOKABLE bool isAccountInGroup(QString account,QString group);
    Q_INVOKABLE bool upsertGroup(QString name, QString comment, bool enable);

    Q_INVOKABLE QString getAccountOption();
    Q_INVOKABLE bool setAccountOption(QString option);

    Q_INVOKABLE bool commit();

    Q_INVOKABLE int accountSignature(QString actionInfo,QString sigConf);
public:
    QUaProperty * currentAccount() const;

public:
    void setProp(void* object);
private:
    void* m_p;
};

/*!
 * \brief 报警专家Ua包装类
 */
class CMDAlarmExpertUaWrapper : public QUaBaseObject
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit CMDAlarmExpertUaWrapper(QUaServer *server);

public:
    Q_INVOKABLE int getAlarmCount();
    Q_INVOKABLE QString getAlarmByIndex(int index);
    Q_INVOKABLE QString getAlarmById(QString id);
    Q_INVOKABLE QString getAlarmConfById(QString id);
    Q_INVOKABLE bool acknowledge(QString alarmId, QString comment = "");
    Q_INVOKABLE bool acknowledgeAll();
    Q_INVOKABLE bool confirm(QString alarmId, QString comment = "");
    Q_INVOKABLE bool confirmAll();
    Q_INVOKABLE bool suppress(QString alarmId, QString comment = "");
    Q_INVOKABLE bool shelve(QString alarmId, QString comment = "");
    Q_INVOKABLE bool addComment(QString alarmId, QString comment = "");

public:
    void setProp(void* object);
private:
    void* m_p;
};




/*!
 * \brief 配方专家Ua包装类
 */
//class CMDRecipeExpert;
class CMDRecipeExpertUaWrapper : public QUaBaseObject
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit CMDRecipeExpertUaWrapper(QUaServer* server);

public:
//    Q_INVOKABLE int beginEdit(const QString& recipeName);
//    Q_INVOKABLE int endEdit(const QString& recipeName, bool save);
//    Q_INVOKABLE int saveEdit(const QString& name);

//    Q_INVOKABLE int  addItem(const QString& recipeName, const QString& id, const QString& value);
//    Q_INVOKABLE int deleteItem(const QString& recipeName, const QString& id);
//    Q_INVOKABLE int setItemValue(const QString& recipeName, const QString& id, const QString& value);
//    Q_INVOKABLE QString getItemValue(const QString& recipeName, const QString& id);

    Q_INVOKABLE int create(const QString& recipeName);
    Q_INVOKABLE int copy(const QString& srcRecipeName, const QString& destRecipeName);
    Q_INVOKABLE int remove(const QString& recipeName);
    Q_INVOKABLE int exists(const QString& recipeName);
    Q_INVOKABLE int save(const QString& recipeName, const QString& content);
    Q_INVOKABLE QString getContent(const QString& recipeName);
    Q_INVOKABLE QString toFileName(const QString& recipeName);
    Q_INVOKABLE int download(const QString& recipeName, int showProcessDialog = 1);
    Q_INVOKABLE int upload(const QString& recipeName);

    Q_INVOKABLE int getRecipeCount(bool onlyRecipe);
    Q_INVOKABLE QString getRecipeName(int index);
    Q_INVOKABLE QList<QString> enumRecipe(bool onlyRecipe);

public:
    void setProp(void* object);
private:
    void* m_p;
};

#endif // MDPROJECTUAWRAPPERS_H
