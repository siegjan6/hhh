#include "MDDataTable.h"
//#include "IMDDataField.h"
#include "MDDataField.h"
#include <QJsonObject>
#include <QJsonArray>

CMDDataTable::CMDDataTable()
{
    m_dataGroupName         = "";       //! 数据组名
    m_name                  = "";       //! 表名
    m_description           = "";       //! 描述
    m_isEnable = false;
    m_isEnableCreateDataTable  = false;    //! 是否创建数据表标识
    m_dataFieldList.clear();
}

CMDDataTable::~CMDDataTable()
{
    removeAllDataField();
}

/*!
\section 克隆函数
*/
IMDDataTable *CMDDataTable::clone()
{
    CMDDataTable *pDataTable = new CMDDataTable();
    *pDataTable = *this;
    return pDataTable;
}

/*!
\section 赋值运算符重载
*/
void CMDDataTable::operator =(const CMDDataTable &otherObj)
{
    m_dataGroupName             = otherObj.m_dataGroupName;
    m_name                      = otherObj.m_name;
    m_description               = otherObj.m_description;
    m_isEnable = otherObj.m_isEnable;
    m_isEnableCreateDataTable   = otherObj.m_isEnableCreateDataTable;

    removeAllDataField();
    int iCount = otherObj.m_dataFieldList.count();
    for(int i = 0; i < iCount; ++i)
    {
        IMDDataField *pDataField = otherObj.m_dataFieldList.at(i);
        if(pDataField)
        {
            addDataField(pDataField->clone());
        }
    }
}

/*!
\section 序列化
*/
void CMDDataTable::serialize(QDataStream *ar, bool bSave)
{
    if(NULL == ar)
    {
        return;
    }

    if(bSave)
    {
        quint32 uVersion = 1;
        *ar << uVersion;
        if(uVersion >= 1)
        {
            *ar << m_dataGroupName;
            *ar << m_name;
            *ar << m_description;
            *ar << m_isEnableCreateDataTable;

            int iCount = m_dataFieldList.size();
            *ar << iCount;

            for (int i = 0; i < iCount; ++i)
            {
                IMDDataField *pDataField = m_dataFieldList.at(i);
                if(pDataField)
                {
                    pDataField->serialize(ar, bSave);
                }
            }
        }
    }
    else
    {
        quint32 uVersion = 0;

        *ar >> uVersion;
        if(uVersion >= 1)
        {
            *ar >> m_dataGroupName;
            *ar >> m_name;
            *ar >> m_description;
            *ar >> m_isEnableCreateDataTable;

            int iCount = 0;
            *ar >> iCount;
            for(int i = 0; i < iCount; i++)
            {
                IMDDataField *pDataField = new CMDDataField();
                if(pDataField)
                {
                    pDataField->serialize(ar, bSave);
                    if(!addDataField(pDataField))
                    {
                        delete pDataField;
                        pDataField = NULL;
                    }
                }
            }
        }
    }
}

void CMDDataTable::serialize(QJsonObject& json)
{
    json.insert("FieldGroupName", m_dataGroupName);
    json.insert("Name", m_name);
    json.insert("Desc", m_description);
    json.insert("Enable", m_isEnable);
    //json.insert("EnableCreateDataTable", m_isEnableCreateDataTable);

    QJsonArray FieldArr;
    int iCount = m_dataFieldList.size();
    for (int i = 0; i < iCount; ++i)
    {
        IMDDataField *pDataField = m_dataFieldList.at(i);
        if(pDataField)
        {
            QJsonObject field;
            pDataField->serialize(field);
            FieldArr.append(field);
        }
    }
    json.insert("FieldArray", FieldArr);
}

void CMDDataTable::deserialize(QJsonObject json)
{
    m_dataGroupName = json.value("FieldGroupName").toString();
    m_name = json.value("Name").toString();
    m_description = json.value("Desc").toString();
    m_isEnable = json.value("Enable").toBool();
    //m_isEnableCreateDataTable = json.value("EnableCreateDataTable").toBool();

    QJsonArray FieldArr = json.value("FieldArray").toArray();
    int iCount = FieldArr.count();
    for(int i = 0; i < iCount; i++)
    {
        IMDDataField *pDataField = new CMDDataField();
        if(pDataField)
        {
            pDataField->deserialize(FieldArr.at(i).toObject());
            if(!addDataField(pDataField))
            {
                delete pDataField;
                pDataField = NULL;
            }
        }
    }
}

/*!
\section 增加字段
*/
bool CMDDataTable::addDataField(IMDDataField *dataField)
{
    bool success = false;
    if (dataField && (NULL == getDataField(dataField->name())))
    {
        m_dataFieldList.append(dataField);
        success = true;
    }

    return success;
}

/*!
\section 移除指定字段
*/
bool CMDDataTable::removeDataField(IMDDataField *dataField)
{
    bool success = false;
    if (dataField)
    {
        int index = m_dataFieldList.indexOf(dataField);
        if (-1 != index)
        {
            m_dataFieldList.removeAt(index);
            delete dataField;
            dataField = NULL;
            success = true;
        }
    }

    return success;
}

/*!
\section 移除所有字段
*/
void CMDDataTable::removeAllDataField()
{
    if (0 < m_dataFieldList.size())
    {
        qDeleteAll(m_dataFieldList.begin(), m_dataFieldList.end());
        m_dataFieldList.clear();
    }
}

/*!
\section 通过字段名称查找字段
*/
IMDDataField *CMDDataTable::getDataField(const QString &name)
{
    IMDDataField *pDataField = NULL;
    for (int i = 0; i < m_dataFieldList.size(); ++i)
    {
        pDataField = m_dataFieldList.at(i);
        if (pDataField && (pDataField->name() == name))
        {
            return pDataField;
        }
    }

    return NULL;
}

/*!
\section 通过索引查找字段
*/
IMDDataField *CMDDataTable::getDataField(int index)
{
    IMDDataField *pDataField = NULL;
    if (0 <= index && index < m_dataFieldList.size())
    {
        pDataField = m_dataFieldList.at(index);
    }

    return pDataField;
}

/*!
\section 通过itemname查找字段
*/
IMDDataField *CMDDataTable::getDataFieldByItemName(const QString &name)
{
    IMDDataField *pDataField = NULL;
    for (int i = 0; i < m_dataFieldList.size(); ++i)
    {
        pDataField = m_dataFieldList.at(i);
        if (pDataField && (pDataField->dataItemName() == name))
        {
            return pDataField;
        }
    }

    return NULL;
}

int CMDDataTable::dataFieldCount(const QString &type)
{
    if(type.isEmpty())
        return m_dataFieldList.size();

    int cnt = 0;
    IMDDataField *pDataField = nullptr;
    for (int i = 0; i < m_dataFieldList.size(); ++i)
    {
        pDataField = m_dataFieldList.at(i);
        QVariant vExtType = pDataField->ext("type");
        if(vExtType.isNull() || !vExtType.isValid())
        {
            vExtType = "";
        }
        if (pDataField && (vExtType.toString() == type))
        {
            cnt++;
        }
    }
    return cnt;
}
