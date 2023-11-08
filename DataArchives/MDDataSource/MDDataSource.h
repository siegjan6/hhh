#ifndef MDDATASOURCE_H
#define MDDATASOURCE_H

/*!

\author bHVvY2hhbw==

\section 功能
1.数据源属性。
2.增加、删除、获取数据表。

*/

#include <QList>
#include <QDataStream>

#include "IMDDataSource.h"

class IMDDataBaseParam;

class  CMDDataSource : public IMDDataSource
{
public:
    CMDDataSource();
    virtual ~CMDDataSource();

/*!
\section 复制数据源结构
*/
public:
    IMDDataSource *clone();
    void operator= (const CMDDataSource &otherObj);

/*!
\section 序列化
*/
public:
    void serialize(QDataStream *ar, bool bSave);
    void serialize(QJsonObject& json);
    void deserialize(QJsonObject json);

/*!
\section 数据源属性
*/
public:
    //! 设置/获取名称
    inline QString name(){return m_name;}
    inline void setName(const QString &name){m_name = name;}

    //! 设置/获取描述
    inline QString description(){return m_description;}
    inline void setDescription(const QString &description){m_description = description;}

    //! 设置/获取启用
    inline bool isEnable(){return m_isEnable;}
    inline void setEnable(bool bEnable = true){m_isEnable = bEnable;}

    //! 设置/获取启用数据表前缀
    inline bool isEnableDataTablePrefix(){return m_isEnableDataTablePrefix;}
    inline void setEnableDataTablePrefix(bool bEnable){m_isEnableDataTablePrefix = bEnable;}

    //! 设置/获取数据表前缀
    inline QString	dataTablePrefix(){return m_dataTablePrefix;}
    inline void	setDataTablePrefix(const QString &prefix){m_dataTablePrefix = prefix;}

    //! 设置/获取是否创建数据库
    inline bool isEnableCreateDataBase(){return m_isEnableCreateDataBase;}
    inline void setEnableCreateDataBase(bool bEnable){m_isEnableCreateDataBase = bEnable;}

    //! 设置/获取启用数据验证方法A
    inline bool isEnableVerificationA(){return m_isEnableVerificationA;}
    inline void enableVerificationA(bool isEnabled){m_isEnableVerificationA = isEnabled;}

    //! 设置/获取启用数据验证方法B
    inline bool isEnableVerificationB(){return m_isEnableVerificationB;}
    inline void enableVerificationB(bool isEnabled){m_isEnableVerificationB = isEnabled;}

    //! 设置/获取启用创建默认与数据组对应的表
    inline bool createDefaultTable(){return m_createDefaultTable;}
    inline void setCreateDefaultTable(bool isEnabled){m_createDefaultTable = isEnabled;}

    //! 设置/获取显示表
    inline bool isShowTable(){return m_isShowTable;}
    inline void setShowTable(bool isEnabled){m_isShowTable = isEnabled;}

    //! 设置/获取临时数据库保存路径
    inline void setTempDatabasePath(const QString &path);

    //! 设置/获取数据库参数
    inline IMDDataBaseParam *dbParam(){return m_mdDataBaseBParam;}
    inline void setDBaram(IMDDataBaseParam *pDataBaseBParam){m_mdDataBaseBParam = pDataBaseBParam;}

/*!
\section 功能
1.增加、删除、获取数据表。
*/
public:
    //! 增增加表
    bool addDataTable(IMDDataTable *dataTable);

    //! 删除表
    bool removeDataTable(IMDDataTable *dataTable);

    //! 删除所有表
    void removeAllDataTable();

    //! 查找表--通过表名
    IMDDataTable *getDataTable(const QString &name);

    //! 查找表--通过索引
    IMDDataTable *getDataTable(int index);

    //! 查找表--通过数据组名
    /*!
      通过数据组名查找表

    \param name 查找的数据组名。
    \param nList 返回所有数据组名相同的表。
    \returns 无。
    \note 无。
    */
    void getDataTableByGroupName(const QString &name, QList<IMDDataTable*> &nList);

    //! 表总数
    inline int dataTableCount(){return m_dataTableList.size();}    
private:
    QString m_name                          ;       //! 数据源名字
    QString m_description                   ;       //! 数据源描述
    QString m_dataTablePrefix               ;       //! 数据表前缀
    bool m_isEnable                         ;    //! 数据源启用标识
    bool m_isEnableDataTablePrefix          ;    //! 数据源创建表是否启用表前缀标识
    bool m_isEnableCreateDataBase           ;    //! 是否创建数据库标识
    bool m_isEnableVerificationA            ;    //! 启用数据验证方法A标识
    bool m_isEnableVerificationB            ;    //! 启用数据验证方法B标识
    bool m_createDefaultTable               ;    //! 是否创建默认表标识
    bool m_isShowTable                      ;    //! 是否显示表标识
    IMDDataBaseParam *m_mdDataBaseBParam    ;     //! 数据库参数
    QList <IMDDataTable*> m_dataTableList;              //! 数据源所有表链表

};

#endif // MDDATASOURCE_H
