#ifndef IMDDATAGROUP_H
#define IMDDATAGROUP_H

/*!

\author luochao

\section 功能
1.数据组接口类--抽像类。
2.数据组属性。
3.数据组增加、删除、获取item。

*/


#include <QDataStream>

class IMDDataItem;
class IMDDataGroup
{
public:
    virtual ~IMDDataGroup(){}
    //! 序列化
    virtual  void serialize(QDataStream *ar, bool bSave) = 0;
    virtual  void serialize(QJsonObject& json) = 0;
    virtual  void deserialize(QJsonObject json) = 0;
public:
    //! 克隆
    virtual IMDDataGroup *clone() = 0;

/*!
\section 属性
*/
public:
    //! 设置/获取名称
    virtual QString name() = 0;
    virtual void setName(const QString &name) = 0;

    //! 设置/获取启用
    virtual bool isEnable() = 0;
    virtual void setEnable(bool bEnable) = 0;

    //! 设置/获取描述
    virtual QString description() = 0;
    virtual void setDescription(const QString &description) = 0;

/*!
\section 增加、删除、获取item
*/
public:
    //! 增加item
    virtual bool addItem(IMDDataItem *item) = 0;

    //! 删除item
    virtual bool removeItem(IMDDataItem *item) = 0;

    //! 删除所有item
    virtual void removeAllItem() = 0;

    //! 查找item--通过item名称
    virtual IMDDataItem *getItem(const QString &name) = 0;

    //! 查找item--通过索引
    virtual IMDDataItem *getItem(int index) = 0;

    //! item总数
    virtual int itemCount() = 0;
};

#endif // IMDDATAGROUP_H
