#include "MDDrawCombine.h"
#include "PropertyDrawCombine.h"
#include "DrawCombineData.h"
#include "MDCalculation.h"
#include "MDUndoData.h"

//////////////////////////////////////////////////////////////////////////
CMDDrawCombine::CMDDrawCombine(QList<CMDDrawObj *> *list/* = NULL*/)
    :CMDDrawMulti(list)
{
    m_combine = new CDrawCombineData;
}
CMDDrawCombine::~CMDDrawCombine()
{
    delete m_combine;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawCombine::setConnectLine(bool value)
{
    m_combine->m_connectLine = value;
    loadGeneratePathEvent();
}
bool CMDDrawCombine::connectLine()
{
    return m_combine->m_connectLine;
}
void CMDDrawCombine::setFillMode(Qt::FillRule value)
{
    m_combine->m_fillMode = value;
    basePath()->setFillRule(value);
    invalidate();
}
Qt::FillRule CMDDrawCombine::fillMode()
{
    return m_combine->m_fillMode;
}
//////////////////////////////////////////////////////////////////////////
bool CMDDrawCombine::isVisible(const QPointF &point)
{
    return CMDDrawMulti::isVisible(point);
}
CMDPropertyDrawObj *CMDDrawCombine::createPropertyDrawObj()
{
    return new CPropertyDrawCombine(this);
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawCombine::onGeneratePath()
{
    for(int i = 0; i < objList()->count(); i++)
    {
        CMDDrawObj* obj = objList()->at(i);
        if(m_combine->m_connectLine)
            basePath()->connectPath(*obj->matrixPath());
        else
            basePath()->addPath(*obj->matrixPath());
    }

    setDirectRect(basePath()->boundingRect());
    basePath()->setFillRule(m_combine->m_fillMode);
}
CMDDrawObj *CMDDrawCombine::createSelf()
{
    return new CMDDrawCombine();
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawCombine::serialize(QJsonObject &json)
{
    CMDDrawMulti::serialize(json);

    QJsonObject js;
    m_combine->serialize(js);
    json.insert("DrawCombine", js);
}
void CMDDrawCombine::deserialize(const QJsonObject &json)
{
    CMDDrawMulti::deserialize(json);

    QJsonObject js = json.value("DrawCombine").toObject();
    m_combine->deserialize(js);
}
CMDDrawObj *CMDDrawCombine::clone()
{
    CMDDrawCombine* obj = static_cast<CMDDrawCombine*>(CMDDrawMulti::clone());
    *obj->m_combine = *m_combine;
    return obj;
}
//////////////////////////////////////////////////////////////////////////
QList<QStringList> CMDDrawCombine::expNames()
{
    return QList<QStringList>();
}
//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawObj *CMDDrawCombine::createPropertyDrawObj(const QString &className)
{
    if(className.compare("DrawCombine", Qt::CaseInsensitive) == 0)
        return new CPropertyDrawCombine(this);

    return CMDDrawMulti::createPropertyDrawObj(className);
}
void CMDDrawCombine::initClassInfo(QStringList &classInfo)
{
    CMDDrawMulti::initClassInfo(classInfo);
    classInfo.append("DrawCombine");
}
//////////////////////////////////////////////////////////////////////////
