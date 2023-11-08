#ifndef MDDATAFIELD_H
#define MDDATAFIELD_H

/*!

\author bHVvY2hhbw==

\section 功能
1.字段属性

*/

#include "IMDDataField.h"
#include "IMDDataType.h"
#include <QDataStream>
#include <QJsonObject>

class CMDDataField : public IMDDataField
{
public:
    CMDDataField();
    virtual ~CMDDataField();

/*!
\section 属性复制
*/
public:
    virtual IMDDataField *clone();
    void operator =(const CMDDataField &otherObj);

/*!
\section 序列化
*/
public:
    virtual void serialize(QDataStream *ar, bool bSave);
    virtual void serialize(QJsonObject& json);
    virtual void deserialize(QJsonObject json);

/*!
\section 关联CMDDataItem
*/
public:
    //! 设置/获取关联CMDDataItem的名称
    inline QString dataItemName(){return m_dataItemName;}
    inline void setDataItemName(const QString &name){m_dataItemName = name;}

    //! 设置/获取关联CMDDataItem的数据类型
    inline MDDataType dataItemDataType(){return m_itemType;}
    inline void	setDataItemDataType(MDDataType vt){m_itemType = vt;}

/*!
\section 字段属性
*/
public:
    //! 设置/获取字段名
    inline QString name(){return m_name;}
    inline void setName(const QString &name){m_name = name;}

    //! 设置/获取描述
    inline QString description(){return m_description;}
    inline void setDescription(const QString &description){m_description = description;}

    //! 设置/获取字段的数据类型
    inline MDDataType dataType(){return m_dataType;}
    inline void setDataType(MDDataType iType){m_dataType = iType;}

    //! 设置/获取是否设置为主键
    inline bool	isPrimarykey(){return m_isPrimarykey;}
    inline void	setPrimarykey(bool bPrimarykey){m_isPrimarykey = bPrimarykey;}

    //! 设置/获取是否允许为空
    inline bool	isEnableNull(){return m_isEnableNull;}
    inline void	enableNull(bool bEnableNull){m_isEnableNull = bEnableNull;}

    //! 设置/获取字段大小
    inline int     size(){return m_size;}
    inline void	setSize(int uSize){m_size = uSize;}

    //! 以下为扩展属性
    ///////////////////////////////////////////////
    //! 实时曲线使能
    inline bool	isRtCurveEnable(){return m_isRtCurveEnable;}
    inline void	enableRtCurve(bool bEnable){m_isRtCurveEnable = bEnable;}

    //! 历史曲线使能
    inline bool	isHisCurveEnable(){return m_isHisCurveEnable;}
    inline void	enableHisCurve(bool bEnable){m_isHisCurveEnable = bEnable;}

    //! 历史表格使能
    inline bool	isHisTableEnable(){return m_isHisTableEnable;}
    inline void	enableHisTable(bool bEnable){m_isHisTableEnable = bEnable;}

    //! 历史过滤使能
    inline bool	isHisFilterEnable(){return m_isHisFilterEnable;}
    inline void	enableHisFilter(bool bEnable){m_isHisFilterEnable = bEnable;}

    //! 排序
    inline int         order(){return m_order;}
    inline void	setOrder(int order){m_order = order;}

    //! 线宽
    inline int         lineWidth(){return m_lineWidth;}
    inline void	setlineWidth(int lwith){m_lineWidth = lwith;}

    //! series颜色
    inline QString color(){return m_color;}
    inline void setColor(const QString &color){m_color = color;}

    //! ext
    inline QString ext(){return m_ext;}
    inline void setExt(const QString &ext){m_ext = ext;}
    inline virtual QVariant ext(const QString &key){if(m_extJobj.contains(key)) return m_extJobj.value(key); else return QVariant();}

private:
    QString m_dataItemName  ;                   //! CMDDataItem的名称
    QString m_name          ;                   //! 字段名称
    QString m_description   ;                   //! 字段描述
    MDDataType m_itemType   ;                   //! CMDDataItem的类型
    MDDataType m_dataType   ;                   //! 字段类型
    bool m_isPrimarykey     ;                   //! 是否是主键标识
    bool m_isEnableNull     ;                   //! 是否允许为空标识
    int m_size              ;                   //! 字段大小
    //! 以下为扩展属性
    ///////////////////////////////////////////////
    //! 实时曲线使能
    bool	m_isRtCurveEnable;

    //! 历史曲线使能
    bool	m_isHisCurveEnable;

    //! 历史表格使能
    bool	m_isHisTableEnable;

    //! 历史过滤使能
    bool	m_isHisFilterEnable;

    //! 排序
    int         m_order;

    //! 线宽
    int         m_lineWidth;

    //! series颜色
    QString m_color;

    //! ext
    QString m_ext;
    QJsonObject m_extJobj;
};

#endif // MDDATAFIELD_H
