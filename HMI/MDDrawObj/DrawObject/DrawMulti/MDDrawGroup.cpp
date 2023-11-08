#include "MDDrawGroup.h"
#include "DrawGroupData.h"
#include "MDCalculation.h"
#include "MDDrawWidget.h"
#include "MDPropertyDrawGroup.h"
#include "MDUndoData.h"

//////////////////////////////////////////////////////////////////////////
CMDDrawGroup::CMDDrawGroup(QList<CMDDrawObj *> *list/* = NULL*/)
    :CMDDrawMulti(list)
{
    m_group = new CDrawGroupData;
    foreach(CMDDrawObj* obj, *objList())
    {
       if(!obj->isVector())
       {
           m_group->m_isVector = false;
           break;
       }
    }
}
CMDDrawGroup::~CMDDrawGroup()
{
    clear();
    delete m_group;
}
void CMDDrawGroup::clear()
{
    m_group->m_customPropertys.clear();
}
//////////////////////////////////////////////////////////////////////////
bool CMDDrawGroup::isVector()
{
    return m_group->m_isVector;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawGroup::draw(QPainter *painter, bool isPrinting/* = false*/)
{
    for(int i = 0; i < objList()->count(); i++)
    {
        CMDDrawObj* obj = objList()->at(i);
        obj->draw(painter, isPrinting);
    }
}
bool CMDDrawGroup::isVisible(const QPointF &point)
{
    foreach(CMDDrawObj* obj, *objList())
    {
        if(obj->canSelect(point))
            return true;
    }

    return false;
}
void CMDDrawGroup::loadInitalizationEvent()
{
    CMDDrawMulti::loadInitalizationEvent();

    foreach(CMDDrawObj* obj, *objList())
    {
       if(!obj->isVector())
       {
           m_group->m_isVector = false;
           break;
       }
    }
    removeVectorExp();
}
void CMDDrawGroup::generateBound()
{
    QRectF rf;
    for(int i = 0; i < objList()->count(); i++)
    {
        CMDDrawObj* obj = objList()->at(i);
        obj->generateBound();
        rf = rf.isEmpty() ? obj->bound() : rf.united(obj->bound());
    }

    setBound(rf);
}
CMDPropertyDrawObj *CMDDrawGroup::createPropertyDrawObj()
{
    if (isVector())
        return new CMDPropertyDrawGroupVector(this);
    else
        return new CMDPropertyDrawGroup(this);
}
//////////////////////////////////////////////////////////////////////////
CMDDrawObj *CMDDrawGroup::visibleChild(const QPointF &point)
{
    int count = objList()->count();
    for (int i = count - 1; i >= 0; i--)
    {
        CMDDrawObj* obj = objList()->at(i);
        if (obj->canSelect(point))
        {
            if (obj->type() == DrawType::Group)
                return static_cast<CMDDrawGroup*>(obj)->visibleChild(point);
            else
                return obj;
        }
    }

    return NULL;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawGroup::onGeneratePath()
{
    for(int i = 0; i < objList()->count(); i++)
    {
        CMDDrawObj* obj = objList()->at(i);
        basePath()->addPath(*obj->matrixPath());
    }

    setDirectRect(basePath()->boundingRect());
}
CMDDrawObj *CMDDrawGroup::createSelf()
{
    return new CMDDrawGroup();
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawGroup::serialize(QJsonObject &json)
{
    CMDDrawMulti::serialize(json);

    QJsonObject js;
    m_group->serialize(js);
    json.insert("DrawGroup", js);
}
void CMDDrawGroup::deserialize(const QJsonObject &json)
{
    clear();
    CMDDrawMulti::deserialize(json);

    QJsonObject js = json.value("DrawGroup").toObject();
    m_group->deserialize(js);
}
CMDDrawObj *CMDDrawGroup::clone()
{
    CMDDrawGroup* obj = static_cast<CMDDrawGroup*>(CMDDrawMulti::clone());
    *obj->m_group = *m_group;

    return obj;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawGroup::loadDoubleClickEvent(const QPointF &point)
{
    int count = objList()->count();
    for (int i = count - 1; i >= 0; i--)
    {
        CMDDrawObj* obj = objList()->at(i);
        if (obj->canSelect(point))
        {
            obj->loadDoubleClickEvent(point);
            return;
        }
    }
}
//////////////////////////////////////////////////////////////////////////
QList<QStringList> CMDDrawGroup::expNames()
{
    QList<QStringList> list;
    list << QStringList({"$drawvector.fill"});
    list << QStringList({"$drawvector.fillBrush"});
    list << QStringList({"$drawvector.brush"});
    list << QStringList({"$drawvector.pen"});
    return list;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawGroup::replaceCustomProperty()
{
    CMDDrawMulti::replaceCustomProperty();
    for(int i = 0; i < objList()->count(); i++)
    {
        CMDDrawObj* obj = objList()->at(i);
        obj->replaceCustomProperty();
    }
}
void CMDDrawGroup::analyseExpression()
{
    CMDDrawMulti::analyseExpression();
    for(int i = 0; i < objList()->count(); i++)
    {
        CMDDrawObj* obj = objList()->at(i);
        obj->analyseExpression();
    }
}
void CMDDrawGroup::removeVectorExp()
{
    if(isVector())
        return;

    int count = CMDExpConfig::expManager((int)DrawType::Obj)->list()->count();
    QList<CMDExpInfo*>* list = expManager()->list();
    while(list->count() > count)
        delete list->takeLast();
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawGroup::updateFormScale()
{
    for(int i = 0; i < objList()->count(); i++)
    {
        CMDDrawObj* obj = objList()->at(i);
        obj->updateFormScale();
    }
}
void CMDDrawGroup::updateVisible()
{
    for(int i = 0; i < objList()->count(); i++)
    {
        CMDDrawObj* obj = objList()->at(i);
        obj->updateVisible();
    }
}
void CMDDrawGroup::updateEnabled()
{
    for(int i = 0; i < objList()->count(); i++)
    {
        CMDDrawObj* obj = objList()->at(i);
        obj->updateEnabled();
    }
}
void CMDDrawGroup::top()
{
    for(int i = 0; i < objList()->count(); i++)
    {
        CMDDrawObj* obj = objList()->at(i);
        obj->top();
    }
}
void CMDDrawGroup::last()
{
    for(int i = 0; i < objList()->count(); i++)
    {
        CMDDrawObj* obj = objList()->at(i);
        obj->last();
    }
}
//////////////////////////////////////////////////////////////////////////
QList<CMDCustomProperty> *CMDDrawGroup::customPropertys()
{
    return &m_group->m_customPropertys;
}
//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawObj *CMDDrawGroup::createPropertyDrawObj(const QString &className)
{
    return CMDDrawMulti::createPropertyDrawObj(className);
}
void CMDDrawGroup::initClassInfo(QStringList &classInfo)
{
    if(isVector())
        CMDDrawVector::initClassInfo(classInfo);
    else
        CMDDrawObj::initClassInfo(classInfo);
}
//////////////////////////////////////////////////////////////////////////



