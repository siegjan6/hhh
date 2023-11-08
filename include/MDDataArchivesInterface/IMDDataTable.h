#ifndef IMDDATATABLE_H
#define IMDDATATABLE_H

/*!

\author luochao

\section 功能
1.数据表接口类--抽像类。
2.数据表属性。

\section

\code

\endcode

*/

#include <QDataStream>

class IMDDataField;

class IMDDataTable
{
public:
    virtual ~IMDDataTable(){}
    virtual IMDDataTable *clone() = 0;
    virtual void serialize(QDataStream *ar, bool bSave) = 0;
    virtual void serialize(QJsonObject& json) = 0;
    virtual void deserialize(QJsonObject json) = 0;

/*!
\section 关联CMDDataGroup
*/
public:
    //! 数据组名称
    virtual QString dataGroupName() = 0;
    virtual void setDataGroupName(const QString &name) = 0;

/*!
\section 表的属性
*/
public:
    //! 表名
    virtual QString name() = 0;
    virtual void setName(const QString &name) = 0;

    //! 描述
    virtual QString description() = 0;
    virtual void setDescription(const QString &description) = 0;

    //! 使能数据表
    virtual bool isEnable() = 0;
    virtual void setEnable(bool bEnable) = 0;

    //! 是否创建数据表
    virtual bool isEnableCreateDataTable() = 0;
    virtual void setEnableCreateDataTable(bool bEnable) = 0;

/*!
\section 增加、删除、获取字段。
*/
public:
    //! 增加字段
    virtual bool addDataField(IMDDataField *dataField) = 0;

    //! 删除字段
    virtual bool removeDataField(IMDDataField *dataField) = 0;

    //! 删除所有字段
    virtual void removeAllDataField() = 0;

    //! 查找字段--通过字段名
    virtual IMDDataField *getDataField(const QString &name) = 0;

    //! 查找字段--通过索引
    virtual IMDDataField *getDataField(int index) = 0;

    //! 查找字段--通过item名
    virtual IMDDataField *getDataFieldByItemName(const QString &name) = 0;

    //! 字段总数
    virtual int dataFieldCount(const QString &type = "") = 0;
};

#endif // IMDDATATABLE_H
