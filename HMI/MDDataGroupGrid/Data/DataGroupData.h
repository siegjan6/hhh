/*!
\author bWVpeGw=
\section 数据类
*/
#ifndef DATAGROUPDATA_H
#define DATAGROUPDATA_H

#include <QString>
#include <QColor>
#include <QFont>
#include <QDateTime>
#include <QJsonObject>
#include "MDGroupGridDefine.h"
#include "../../include/interface/Common/IMDDataType.h"

/*!
\section 属性数据类
*/
class CDataGroupBasicData
{
public:
    CDataGroupBasicData();
    ~CDataGroupBasicData(){}

public:
    QFont m_rowFont;
    QFont m_colFont;
    QFont m_dataFont;

    QColor m_dataColor;
    QColor m_colColor;
    QColor m_colForeColor;
    QColor m_rowColor ;
    QColor m_rowForeColor;
    QColor m_tabColor;
    QColor m_cornerColor;

    qint32 m_rowWidth;
    qint32 m_colHeight;
    qint32 m_horizontalBarHeight;
    qint32 m_verticalBarWidth;

    QString m_frontColorField;
    QString m_backColorField;
    bool m_isFieldColor;            //! 是否启用后台数据
    QStringList m_allFieldsName;    //! 所有字段名的完整列表

    qint32 m_version;               //! 版本号
    GridType m_gridType;            //! 控件类型
    QString m_gridName;             //! 控件名
/*!
\section 序列化
*/
public:
    CDataGroupBasicData &operator =(const CDataGroupBasicData &other);
    void serialize(QDataStream& stream, bool isOrigVersion = false);
    void deserialize(QDataStream& stream);
    void serialize_1(QDataStream& stream);

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

/*!
\section 字段类
*/
class CDataGroupField
{
public:
    void setName(const QString& value){m_name = value;}
    QString name()const{return m_name;}
private:
    QString m_name; //! 名字

public:
    void setComment(const QString& value){m_comment = value;}
    QString comment()const{return m_comment;}
private:
    QString m_comment; //! 描述

public:
    void setType(const MDDataType value){m_type = value;}
    MDDataType type()const{return m_type;}
private:
    MDDataType m_type; //! 数据类型

public:
    void setWidth(const int value){m_width = value;}
    int width()const{return m_width;}
private:
    int m_width; //! 列宽

public:
    void setCheckState(const bool value){m_checkState = value;}
    bool checkState()const{return m_checkState;}
private:
    bool m_checkState; //! 选中状态

public:
    void setDataFormat(const QString value){m_dataFormat = value;}
    QString dataFormat()const{return m_dataFormat;}
private:
    QString m_dataFormat; //! 格式字符串

/*!
\section 序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // DATAGROUPDATA_H
