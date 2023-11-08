#include "ObjName.h"
#include "MDDrawObj.h"
#include "MDDrawGroup.h"
#include "MDDrawCombine.h"
#include "Environment.h"
//////////////////////////////////////////////////////////////////////////
CObjName::CObjName()
{
    m_container = NULL;

    int count = CDrawCreator::names()->count();
    for (int i = 0; i < count; i++)
        m_indexs.append(0);
}
//////////////////////////////////////////////////////////////////////////
void CObjName::addName(CMDDrawObj *obj)
{
    QString name = obj->name();
    name = name.trimmed().toLower();
    if (!name.isEmpty())
        m_nameDict.insert(name, NULL);
    if (obj->type() == DrawType::Group || obj->type() == DrawType::Combine)
    {
        CMDDrawMulti* multi = dynamic_cast<CMDDrawMulti*>(obj);
        for(int i = 0; i < multi->objList()->count(); i++)
            addName(multi->objList()->at(i));
    }
}
//////////////////////////////////////////////////////////////////////////
void CObjName::init(CEnvironment *container)
{
    m_container = container;
}
void CObjName::resetDict(QList<CMDDrawObj *> *objList)
{
    m_nameDict.clear();

    for(int i = 0; i < objList->count(); i++)
        addName(objList->at(i));
    for (int i = 0; i < m_indexs.count(); i++)
        m_indexs[i] = 0;
}
void CObjName::addName(const QString &name)
{
    QString str = name.toLower();
    if (!m_nameDict.contains(str))
        m_nameDict.insert(str, NULL);
}
bool CObjName::containsName(const QString &name)
{
    return m_nameDict.contains(name.toLower());
}
void CObjName::dynamicContainsName(const QString &name)
{
    bool result = false;
    QList<CMDDrawObj*>* objs = m_container->objs();

    for(int i = 0; i < objs->count(); i++)
    {
        if (containsObjName(objs->at(i), name))
        {
            result = true;
            break;
        }
    }

    if (!result)
        m_nameDict.remove(name.toLower());
}
void CObjName::createName(CMDDrawObj *obj, bool needChangeChild/* = true*/)
{
    QString name = obj->name();
    name = name.trimmed().toLower();
    QList<QString>* names = CDrawCreator::names();

    if (name.isEmpty() || m_nameDict.contains(name))
    {
        int type = (int)obj->type();
        int index = m_indexs.at(type) + 1;

        while(m_nameDict.contains(getName(names->at(type), index)))
        {
            index++;
        }

        m_indexs[type] = index;
        obj->setName(getName(names->at(type), index));
    }
    else
        addName(name);

    if (needChangeChild)
    {
        if(obj->type() == DrawType::Group || obj->type() == DrawType::Combine)
        {
            CMDDrawMulti* multi = dynamic_cast<CMDDrawMulti*>(obj);
            for(int i = 0; i < multi->objList()->count(); i++)
                createName(multi->objList()->at(i));
        }
    }
}
void CObjName::removeName(CMDDrawObj *obj)
{
    m_nameDict.remove(obj->name().toLower());
    if (obj->type() == DrawType::Group || obj->type() == DrawType::Combine)
    {
        CMDDrawMulti* multi = dynamic_cast<CMDDrawMulti*>(obj);
        for(int i = 0; i < multi->objList()->count(); i++)
            removeName(multi->objList()->at(i));
    }
}
//////////////////////////////////////////////////////////////////////////
QString CObjName::getName(const QString &name, int index)
{
    QString str;
    str.setNum(index);
    return name.toLower() + str;
}
bool CObjName::containsObjName(CMDDrawObj *obj, const QString &name)
{
    if (obj->name().compare(name, Qt::CaseInsensitive) == 0)
        return true;

    if (obj->type() == DrawType::Group || obj->type() == DrawType::Combine)
    {
        CMDDrawMulti* multi = dynamic_cast<CMDDrawMulti*>(obj);
        for(int i = 0; i < multi->objList()->count(); i++)
        {
            if (containsObjName(multi->objList()->at(i), name))
                return true;
        }
    }

    return false;
}
//////////////////////////////////////////////////////////////////////////
