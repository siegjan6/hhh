#ifndef IMDDATASOURCE_H
#define IMDDATASOURCE_H

/*!

\author luochao

\section 功能
1.数据源接口类--抽像类。
2.数据源属性。

\section

\code

\endcode

*/

#include<QList>
#include<QJsonObject>
class IMDDataTable;
class IMDDataBaseParam;
class  IMDDataSource
{
public:
    virtual ~IMDDataSource(){}
    virtual IMDDataSource *clone() = 0;
    virtual void serialize(QDataStream *ar, bool bSave) = 0;
    virtual void serialize(QJsonObject& json) = 0;
    virtual void deserialize(QJsonObject json) = 0;

    //! 设置/获取名称
    virtual QString name() = 0;
    virtual void setName(const QString &name) = 0;

    //! 设置/获取描述
    virtual QString description() = 0;
    virtual void setDescription(const QString &description) = 0;

    //! 设置/获取启用
    virtual bool isEnable() = 0;
    virtual void setEnable(bool bEnable = true) = 0;

    //! 设置/获取启用数据表前缀
    virtual bool isEnableDataTablePrefix() = 0;
    virtual void setEnableDataTablePrefix(bool bEnable) = 0;

    //! 设置/获取数据表前缀
    virtual QString	dataTablePrefix() = 0;
    virtual void	setDataTablePrefix(const QString &prefix) = 0;

    //! 设置/获取是否创建数据库
    virtual bool isEnableCreateDataBase() = 0;
    virtual void setEnableCreateDataBase(bool bEnable) = 0;

    //! 设置/获取启用数据验证方法A
    virtual bool isEnableVerificationA() = 0;
    virtual void enableVerificationA(bool isEnabled) = 0;

    //! 设置/获取启用数据验证方法B
    virtual bool isEnableVerificationB() = 0;
    virtual void enableVerificationB(bool isEnabled) = 0;

    //! 设置/获取启用创建默认与数据组对应的表
    virtual bool createDefaultTable() = 0;
    virtual void setCreateDefaultTable(bool isEnabled) = 0;

    //! 设置/获取显示表
    virtual bool isShowTable() = 0;
    virtual void setShowTable(bool isEnabled) = 0;

    //! 数据库参数
    virtual void setTempDatabasePath(const QString &path) = 0;

public:
    //! 设置/获取数据库参数
    virtual IMDDataBaseParam *dbParam() = 0;
    virtual void setDBaram(IMDDataBaseParam*) = 0;

/*!
\section 增加、删除、获取表。
*/
public:
    //! 增增加表
    virtual bool addDataTable(IMDDataTable *dataTable) = 0;

    //! 删除表
    virtual bool removeDataTable(IMDDataTable *dataTable) = 0;

    //! 删除所有表
    virtual void removeAllDataTable() = 0;

    //! 查找表--通过表名
    virtual IMDDataTable *getDataTable(const QString &name) = 0;

    //! 查找表--通过索引
    virtual IMDDataTable *getDataTable(int index) = 0;

    //! 查找表--通过数据组名
    virtual void getDataTableByGroupName(const QString &name, QList<IMDDataTable*> &nList) = 0;

    //! 表总数
    virtual int dataTableCount() = 0;
};

#endif // IMDDATASOURCE_H
