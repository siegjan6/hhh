#ifndef MDDATAGROUP_H
#define MDDATAGROUP_H

/*!

\author bHVvY2hhbw==

\section 功能
1.IMDDataGroup实现类。
2.数据组属性。
3.数据组增加、删除、获取item。

*/

#include <QDataStream>
#include <QVariant>
#include "IMDDataGroup.h"
#include "../../include/interface/Common/IMDDataType.h"

class CMDDataGroup : public IMDDataGroup
{
public:
    CMDDataGroup();
    virtual ~CMDDataGroup();
    //! 序列化
    void serialize(QDataStream *ar, bool bSave);
    void serialize(QJsonObject& json);
    void deserialize(QJsonObject json);

/*!
\section 复制属性
*/
public:
    //! 克隆
    IMDDataGroup *clone();
    void operator =(const CMDDataGroup &otherObj);

/*!
\section 属性
*/
public:
    //! 设置/获取名称
    inline QString name(){return m_name;}
    inline void setName(const QString &name){m_name = name;}

    //! 设置/获取启用
    inline bool isEnable(){return m_iEnable;}
    inline void setEnable(bool bEnable){m_iEnable = bEnable;}

    //! 设置/获取描述
    inline QString description(){return m_description;}
    inline void setDescription(const QString &description){m_description = description;}

/*!
\section 增加、删除、获取item
*/
public:
    //! 增加item
    bool addItem(IMDDataItem *item);

    //! 删除item
    bool removeItem(IMDDataItem *item);

    //! 删除所有item
    void removeAllItem();

    //! 查找item--通过item名称
    IMDDataItem *getItem(const QString &name);

    //! 查找item--通过索引
    IMDDataItem *getItem(int index);

    //! item总数
    inline int itemCount(){return m_itemList.size();}

/*!
\section 创建默认报警、日志item
*/
public:
    void createDefaultAlarmItem();
    void createDefaultLogItem();
    void addDefaultItem(const QString &itemName, const QString &descript, MDDataType dataType);

private:
    QString m_name                  ;       //! 名称
    QString m_description           ;       //! 描述
    bool m_iEnable                  ;    //! 启用
    int m_iVersion                  ;        //! 版本号
    QList<IMDDataItem*> m_itemList;
};
#endif // MDDATAGROUP_H
