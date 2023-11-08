#ifndef MDDATAITEM_H
#define MDDATAITEM_H

/*!

\author bHVvY2hhbw==

\section 功能
1.IMDDataItem接口实现类。
2.DataItem属性。

*/

#include "IMDDataItem.h"
#include <QDataStream>

class CMDDataItem : public IMDDataItem
{
public:
    CMDDataItem();
    virtual ~CMDDataItem();
    //! 克隆
    CMDDataItem *clone();
    void operator =(const CMDDataItem &otherObj);
    //! 序列化
    void serialize(QDataStream* ar,bool bSave);
    void serialize(QJsonObject& json);
    void deserialize(QJsonObject json);

/*!
\section 属性
*/
public:
    //! 设置/获取dataItem名称
    inline QString name(){return m_strName;}
    inline void setName(const QString &name){m_strName = name;}

    //! 设置/获取数据类型
    inline MDDataType dataType(){return m_varType;}
    inline void setDataType(MDDataType type){m_varType = type;}

    //! 设置/获取描述
    inline QString description(){return m_strDescription;}
    inline void setDescription(const QString &description){m_strDescription = description;}

    //! 设置/获取 系统类型/自定义类型 true系统item，false为自定义item
    inline bool isFixed(){return m_isFixed;}
    inline void setFixed(bool isFixed){m_isFixed = isFixed;}

    //! 设置/获取数据值
    inline QVariant data(){return m_varData;}
    inline void setData(const QVariant &vData){m_varData = vData;}
private:
    QString m_strName          ;     //! 名称
    QString m_strDescription   ;    //! 描述
    QVariant m_varData         ;   //! 数据值
    MDDataType m_varType       ;  //! 数据类型
    bool m_isFixed             ; //! 是否为系统item类型标识
    int m_iVersion             ;//! 序列化版本号
};

#endif // MDDATAITEM_H
