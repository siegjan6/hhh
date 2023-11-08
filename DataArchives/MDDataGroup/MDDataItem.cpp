#include "MDDataItem.h"

CMDDataItem::CMDDataItem()
{
    m_strName           = "";                   //! 名称
    m_strDescription    = "";                   //! 描述
    m_varData           = QVariant();           //! 数据值
    m_varType           = MDDataType::Int32;    //! 数据类型
    m_isFixed           = true;                 //! 是否为系统item类型标识
    m_iVersion          = 1;                    //! 序列化版本号
}

CMDDataItem::~CMDDataItem()
{
}

/*!
\section 克隆
*/
CMDDataItem *CMDDataItem::clone()
{
    CMDDataItem *pDataItem = new CMDDataItem;
    *pDataItem = *this;
    return pDataItem;
}

/*!
\section 赋值运算符
*/
void CMDDataItem::operator =(const CMDDataItem &otherObj)
{
    m_strName           = otherObj.m_strName;
    m_strDescription    = otherObj.m_strDescription;
    m_varData           = otherObj.m_varData;
    m_varType           = otherObj.m_varType;
    m_isFixed           = otherObj.m_isFixed;
    m_iVersion          = otherObj.m_iVersion;
}

/*!
\section 序列化
*/
void CMDDataItem::serialize(QDataStream *ar, bool bSave)
{
    if(ar)
    {
        if(bSave)
        {
            int iVersion = 1;
            *ar << iVersion;
            m_iVersion = iVersion;

            if(iVersion >= 1)
            {
                *ar << m_strName;
                *ar << m_strDescription;
                *ar << m_varData;
                int type = (int)m_varType;
                *ar << type;
                *ar << m_isFixed;
                *ar << m_iVersion;
            }
        }
        else
        {
            int iVersion = 0;
            *ar >> iVersion;
            m_iVersion = iVersion;

            if(iVersion >= 1)
            {
                *ar >> m_strName;
                *ar >> m_strDescription;
                *ar >> m_varData;
                int type = 0;
                *ar >> type;
                m_varType = (MDDataType)type;
                *ar >> m_isFixed;
                *ar >> m_iVersion;
            }
        }
    }
}

void CMDDataItem::serialize(QJsonObject& json)
{
    json.insert("IsFixed", m_isFixed);
    json.insert("Name", m_strName);
    json.insert("Desc", m_strDescription);
    int type = (int)m_varType;
    json.insert("Type", type);
}

void CMDDataItem::deserialize(QJsonObject json)
{
    m_isFixed = json.value("IsFixed").toBool();
    m_strName = json.value("Name").toString();
    m_strDescription = json.value("Desc").toString();
    int type = json.value("Type").toInt();
    m_varType = (MDDataType)type;
}
