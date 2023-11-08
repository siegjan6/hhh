#ifndef IMDDATAFIELD_H
#define IMDDATAFIELD_H

/*!

\author luochao

\section 功能
1.字段接口类--抽象类。
2.字段属性。

\section

\code

\endcode

*/

#include <QVariant>
#include "../../include/interface/Common/IMDDataType.h"

class IMDDataField
{
public:
    virtual ~IMDDataField(){}
    virtual IMDDataField *clone() = 0;
    virtual void serialize(QDataStream *ar, bool bSave) = 0;
    virtual void serialize(QJsonObject& json) = 0;
    virtual void deserialize(QJsonObject json) = 0;

/*!
\section 关联CMDDataItem
*/
public:
    //! 设置/获取关联CMDDataItem的名称
    virtual QString dataItemName() = 0;
    virtual void setDataItemName(const QString &name) = 0;

    //! 设置/获取关联CMDDataItem的数据类型
    virtual MDDataType  dataItemDataType() = 0;
    virtual void	setDataItemDataType(MDDataType vt) = 0;

/*!
\section 字段属性
*/
public:
    //! 设置/获取字段名
    virtual QString name() = 0;
    virtual void setName(const QString &name) = 0;

    //! 设置/获取描述
    virtual QString description() = 0;
    virtual void setDescription(const QString &description) = 0;

    //! 设置/获取字段的数据类型
    virtual MDDataType dataType() = 0;
    virtual void setDataType(MDDataType iType) = 0;

    //! 设置/获取是否设置为主键
    virtual bool	isPrimarykey() = 0;
    virtual void	setPrimarykey(bool bPrimarykey) = 0;

    //! 设置/获取是否允许为空
    virtual bool	isEnableNull() = 0;
    virtual void	enableNull(bool bEnableNull) = 0;

    //! 设置/获取字段大小
    virtual int     size() = 0;
    virtual void	setSize(int uSize) = 0;

    //! 以下为扩展属性
    ///////////////////////////////////////////////
    //! 实时曲线使能
    virtual bool	isRtCurveEnable() = 0;
    virtual void	enableRtCurve(bool bEnable) = 0;

    //! 历史曲线使能
    virtual bool	isHisCurveEnable() = 0;
    virtual void	enableHisCurve(bool bEnable) = 0;

    //! 历史表格使能
    virtual bool	isHisTableEnable() = 0;
    virtual void	enableHisTable(bool bEnable) = 0;

    //! 历史过滤使能
    virtual bool	isHisFilterEnable() = 0;
    virtual void	enableHisFilter(bool bEnable) = 0;

    //! 排序
    virtual int         order() = 0;
    virtual void	setOrder(int order) = 0;

    //! 线宽
    virtual int         lineWidth() = 0;
    virtual void	setlineWidth(int lwith) = 0;

    //! series颜色
    virtual QString color() = 0;
    virtual void setColor(const QString &color) = 0;

    //! ext
    virtual QString ext() = 0;
    virtual void setExt(const QString &ext) = 0;
    //! 获取ext属性内json key对应的值
    virtual QVariant ext(const QString &key) = 0;
};

#endif // IMDDATAFIELD_H
