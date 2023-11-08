#ifndef IMDDATAITEM_H
#define IMDDATAITEM_H

/*!

\author luochao

\section 功能
1.数据组item接口类--抽像类。
2.dataItem属性。

*/

#include <QVariant>
#include <QJsonObject>
#include "../../include/interface/Common/IMDDataType.h"

class IMDDataItem
{
public:
    virtual ~IMDDataItem(){}
    //! 序列化
    virtual void serialize(QDataStream *ar, bool bSave) = 0;
    virtual void serialize(QJsonObject& obj) = 0;
    virtual void deserialize(QJsonObject obj) = 0;
    //! 克隆
    virtual IMDDataItem *clone() = 0;

/*!
\section 属性
*/
public:
    //! 设置/获取dataItem名称
    virtual QString name() = 0;
    virtual void setName(const QString &name) = 0;

    //! 设置/获取数据类型
    virtual MDDataType dataType() = 0;
    virtual void setDataType(MDDataType type) = 0;

    //! 设置/获取描述
    virtual QString description() = 0;
    virtual void setDescription(const QString &description) = 0;

    //! 设置/获取 系统类型/自定义类型 true系统item，false为自定义item
    virtual bool isFixed() = 0;
    virtual void setFixed(bool) = 0;

    //! 设置/获取数据值
    virtual QVariant data() = 0;
    virtual void setData(const QVariant &vData) = 0;
};

#endif // IMDDATAITEM_H
