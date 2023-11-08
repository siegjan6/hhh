#include "DrawMultiData.h"
#include "MDDrawObj.h"
#include "MDDrawTool.h"
//////////////////////////////////////////////////////////////////////////
CDrawMultiData::CDrawMultiData()
{
    m_baseVersion = 0;
}
void CDrawMultiData::operator=(const CDrawMultiData &other)
{
    if (this == &other)
        return;

    for(int i = 0; i < other.m_objList.count(); i++)
        m_objList.append(other.m_objList.at(i)->clone());
}
void CDrawMultiData::serialize(QJsonObject &json)
{
    QJsonArray array;
    for(int i = 0; i < m_objList.count(); i++)
    {
        QJsonObject jsObj;
        CMDDrawObj* obj = m_objList.at(i);
        jsObj.insert("Type", (int)obj->type());
        obj->serialize(jsObj);
        array.append(jsObj);
    }
    json.insert("ObjList", array);
}
void CDrawMultiData::deserialize(const QJsonObject &json)
{
    QJsonArray array = json.value("ObjList").toArray();
    for(int i = 0; i < array.count(); i++)
    {
        QJsonObject jsObj = array.at(i).toObject();
        int type = jsObj.value("Type").toInt();
        CMDDrawObj* obj = CMDDrawTool::instance()->create()->createDrawObj((DrawType)type);
        obj->deserialize(jsObj);
        m_objList.append(obj);
    }
}
//////////////////////////////////////////////////////////////////////////

