#include "MDDataField.h"
#include <QJsonObject>
#include <QJsonDocument>

CMDDataField::CMDDataField()
{
    m_dataItemName  = "";                   //! CMDDataItem的名称
    m_name          = "";                   //! 字段名称
    m_description   = "";                   //! 字段描述
    m_itemType      = MDDataType::Int32;    //! CMDDataItem的类型
    m_dataType      = MDDataType::Int32;    //! 字段类型
    m_isPrimarykey     = false;                //! 是否是主键标识
    m_isEnableNull     = false;                //! 是否允许为空标识
    m_size              = 0;                    //! 字段大小

    m_isRtCurveEnable = false;//! 实时曲线使能
    m_isHisCurveEnable = false;//! 历史曲线使能
    m_isHisTableEnable = false;//! 历史表格使能
    m_isHisFilterEnable = false;//! 历史过滤使能
    m_order = 0;//! 排序
    m_lineWidth = 1;//! 线宽
    m_color = "#0000FF";//! series颜色
    m_ext = "";
}

CMDDataField::~CMDDataField()
{

}

/*!
\section 克隆函数
*/
IMDDataField *CMDDataField::clone()
{
    CMDDataField *pDataField = new CMDDataField();
    *pDataField = *this;
    return pDataField;
}

/*!
\section 赋值运算符
*/
void CMDDataField::operator =(const CMDDataField &otherObj)
{
    m_dataItemName  = otherObj.m_dataItemName;
    m_name          = otherObj.m_name;
    m_description   = otherObj.m_description;
    m_itemType      = otherObj.m_itemType;
    m_dataType      = otherObj.m_dataType;
    m_isPrimarykey  = otherObj.m_isPrimarykey;
    m_isEnableNull  = otherObj.m_isEnableNull;
    m_size          = otherObj.m_size;

    m_isRtCurveEnable = otherObj.m_isRtCurveEnable;//! 实时曲线使能
    m_isHisCurveEnable = otherObj.m_isHisCurveEnable;//! 历史曲线使能
    m_isHisTableEnable = otherObj.m_isHisTableEnable;//! 历史表格使能
    m_isHisFilterEnable = otherObj.m_isHisFilterEnable;//! 历史过滤使能
    m_order = otherObj.m_order;//! 排序
    m_lineWidth = otherObj.m_lineWidth;//! 线宽
    m_color = otherObj.m_color;//! series颜色
    m_ext = otherObj.m_ext;
    m_extJobj = otherObj.m_extJobj;
}

/*!
\section 序列化
*/
void CMDDataField::serialize(QDataStream *ar, bool bSave)
{
    if(ar)
    {
        if(bSave)
        {
            quint32 uVersion = 1;
            *ar << uVersion;
            if(uVersion >= 1)
            {
                int iFieldType =(int) m_dataType;
                int iObjType = (int)m_itemType;

                *ar << m_dataItemName;
                *ar << m_name;
                *ar << m_description;
                *ar << iFieldType;
                *ar << iObjType;
                *ar << m_isPrimarykey;
                *ar << m_isEnableNull;
                *ar << m_size;

            }
        }
        else
        {
            quint32 uVersion = 0;
            *ar >> uVersion;
            if(uVersion >= 1)
            {
                int iFieldType = 0;
                int iObjType = 0;
                *ar >> m_dataItemName;
                *ar >> m_name;
                *ar >> m_description;
                *ar >> iFieldType;
                *ar >> iObjType;
                m_dataType = (MDDataType)iFieldType;
                m_itemType = (MDDataType)iObjType;
                *ar >> m_isPrimarykey;
                *ar >> m_isEnableNull;
                *ar >> m_size;
            }
        }
    }
}

void CMDDataField::serialize(QJsonObject& json)
{
    int iFieldType =(int) m_dataType;
    int iObjType = (int)m_itemType;

    json.insert("NameInFieldGroup", m_dataItemName);
    json.insert("Name", m_name);
    json.insert("Desc", m_description);
    json.insert("TypeInFieldGroup", iFieldType);
    json.insert("Type", iObjType);
    json.insert("IsPrimaryKey", m_isPrimarykey);
    json.insert("EnableNull", m_isEnableNull);
    json.insert("Length", m_size);

    json.insert("RtCurveEnable", m_isRtCurveEnable);
    json.insert("HisCurveEnable", m_isHisCurveEnable);
    json.insert("HisTableEnable", m_isHisTableEnable);
    json.insert("HisFilterEnable", m_isHisFilterEnable);
    json.insert("Order", m_order);
    json.insert("LineWidth", m_lineWidth);
    json.insert("Color", m_color);
    json.insert("Ext", m_ext);
}

void CMDDataField::deserialize(QJsonObject json)
{
    int iFieldType = 0;
    int iObjType = 0;
    m_dataItemName = json.value("NameInFieldGroup").toString();
    m_name = json.value("Name").toString();
    m_description = json.value("Desc").toString();
    iFieldType = json.value("TypeInFieldGroup").toInt();
    iObjType = json.value("Type").toInt();
    m_dataType = (MDDataType)iFieldType;
    m_itemType = (MDDataType)iObjType;
    m_isPrimarykey = json.value("IsPrimaryKey").toBool();
    m_isEnableNull = json.value("EnableNull").toBool();
    m_size = json.value("Length").toInt();

    m_isRtCurveEnable = json.value("RtCurveEnable").toBool();
    m_isHisCurveEnable = json.value("HisCurveEnable").toBool();
    m_isHisTableEnable = json.value("HisTableEnable").toBool();
    m_isHisFilterEnable = json.value("HisFilterEnable").toBool();
    m_order = json.value("Order").toInt();
    m_lineWidth = json.value("LineWidth").toInt();
    m_color = json.value("Color").toString();
    if(json.contains("Ext")) {
        m_ext = json.value("Ext").toString();
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(m_ext.toUtf8(), &error);
        if(error.error == QJsonParseError::NoError){
            if(doc.isObject())
                m_extJobj = doc.object();
        }
    }
}
