#ifndef MDDATATABLE_H
#define MDDATATABLE_H

/*!

\author bHVvY2hhbw==

\section 功能
1.表属性。
2.增加、删除、获取表字段。

*/

#include "IMDDataTable.h"

class CMDDataTable : public IMDDataTable
{
public:
    CMDDataTable();
    virtual ~CMDDataTable();

/*!
\section 复制表
*/
public:
    IMDDataTable *clone();
    void operator =(const CMDDataTable &otherObj);

/*!
\section 序列化表
*/
public:
    void serialize(QDataStream *ar, bool bSave);
    void serialize(QJsonObject& json);
    void deserialize(QJsonObject json);

/*!
\section 关联CMDDataGroup
*/
public:
    //
    inline QString dataGroupName(){return m_dataGroupName;} //! 获取数据组名
    inline void setDataGroupName(const QString &name){m_dataGroupName = name;} //! 设置数据组名

/*!
\section 表的属性--表名
*/
public:    
    inline QString name(){return m_name;}
    inline void setName(const QString &name){m_name = name;}

/*!
\section 表的属性--描述
*/
public:
    inline QString description(){return m_description;}
    inline void setDescription(const QString &description){m_description = description;}

/*!
\section 表的属性--使能
*/
public:
    inline bool isEnable(){return m_isEnable;}
    inline void setEnable(bool bEnable){m_isEnable = bEnable;}

/*!
\section 表的属性--是否创建数据表
*/
public:
    inline bool isEnableCreateDataTable(){return m_isEnableCreateDataTable;}
    inline void setEnableCreateDataTable(bool bEnable){m_isEnableCreateDataTable = bEnable;}
/*!
\section 增加、删除、获取表字段
*/
public:
    //! 增加字段
    bool addDataField(IMDDataField *dataField);

    //! 删除字段
    bool removeDataField(IMDDataField *dataField);

    //! 删除所有字段
    void removeAllDataField();

    //! 查找字段--通过字段名
    IMDDataField *getDataField(const QString &name);

    //! 查找字段--通过索引
    IMDDataField *getDataField(int index);

    //! 查找字段--通过item名
    IMDDataField *getDataFieldByItemName(const QString &name);

    //! type = "" : 所有字段总数
    //! type = "virtual" : 虚拟字段
    int dataFieldCount(const QString &type = "");

private:
    QString m_dataGroupName         ;       //! 数据组名
    QString m_name                  ;       //! 表名
    QString m_description           ;       //! 描述
    bool m_isEnableCreateDataTable  ;    //! 是否创建数据表标识
    bool m_isEnable  ;    //! 使能这张表
    QList <IMDDataField*> m_dataFieldList;      //! 表字段链表
};

#endif // MDDATATABLE_H
