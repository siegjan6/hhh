#include <QDateTime>
#include <QJsonArray>
#include "MDExpInfo.h"
#include "MDParameter.h"
#include "DrawObjDefine.h"
/////////////////////////////////////////////////////////////////////////////////
const int MaxType = 200;
QVector<int> CMDExpConfig::m_indexCount(MaxType, 0);
QMap<QString, CMDExpGroup*> CMDExpConfig::m_groupDict;
QMap<QString, CMDParameter*> CMDExpConfig::m_paraDict;
QVector<CMDExpManager*> CMDExpConfig::m_expManagerList(MaxType, NULL);
/////////////////////////////////////////////////////////////////////////////////
CMDExpGroup::CMDExpGroup(const QString &name, const QString &showText,
    const QString &comment)
{
    m_name = name.toLower();
    m_showText = showText;
    m_comment = comment;

    if(m_comment.isEmpty())
        m_comment = m_showText;
}
/////////////////////////////////////////////////////////////////////////////////
int CMDExpConfig::getNextIndex(int type)
{
    if(type<0 || type>=MaxType)
        return 0;

    return m_indexCount[type]++;
}
CMDExpGroup *CMDExpConfig::setCurrentGroup(const QString &name, const QString &showText, const QString &comment)
{
   QString n = name.toLower();
   if(n.isEmpty())
       return NULL;

   if(m_groupDict.contains(n))
       return m_groupDict[n];

   CMDExpGroup* group = new CMDExpGroup(n, showText, comment);
   m_groupDict.insert(n, group);
   return group;
}
CMDExpManager *CMDExpConfig::expManager(int type)
{
    if(type<0 || type>=MaxType)
        return NULL;

    return m_expManagerList.at(type);
}
void CMDExpConfig::initExpManager(int type, QList<QStringList> names)
{
    if(type<0 || type>=MaxType)
        return;

    CMDExpManager* value = m_expManagerList.at(type);
    if(value != NULL)
        delete value;

    m_expManagerList[type] = new CMDExpManager(type, &names);
}
void CMDExpConfig::initFactExpManager(int type, QList<QStringList> names, bool isVector, bool isWidget)
{
    if(type<0 || type>=MaxType)
        return;

    CMDExpManager* value = m_expManagerList.at(type);
    if(value != NULL)
        delete value;

    CMDExpManager* m = m_expManagerList.at((int)DrawType::Obj)->clone();
    if(isVector)
        m->append(m_expManagerList.at((int)DrawType::Vector));
    if(isWidget)
        m->append(m_expManagerList.at((int)DrawType::Widget));
    m->append(type, &names);
    m_expManagerList[type] = m;
}
void CMDExpConfig::clear()
{
    qDeleteAll(m_groupDict);
    qDeleteAll(m_paraDict);
    qDeleteAll(m_expManagerList);
    m_groupDict.clear();
    m_paraDict.clear();
    m_expManagerList = QVector<CMDExpManager*>(MaxType, NULL);
    m_indexCount = QVector<int>(MaxType, 0);
}
/////////////////////////////////////////////////////////////////////////////////
CMDExpInfo::CMDExpInfo(const QString &name, const QString &showText, const QString &scriptName,
    const QString &comment)
{
    m_group = NULL;
    m_container = NULL;
    m_index = 0;
    m_type = 0;

    m_name = name.toLower();
    m_showText = showText;
    m_comment = comment;
    m_scriptName = scriptName;

    if(m_comment.isEmpty())
        m_comment = m_showText;
    if(m_scriptName.isEmpty())
        m_scriptName = m_name;
}
CMDExpInfo::CMDExpInfo(const QString &expression, int type, int index)
{
    m_group = NULL;
    m_container = NULL;

    m_expression = expression;
    m_type = type;
    m_index = index;
}
CMDExpInfo *CMDExpInfo::clone()
{
    CMDExpInfo* obj = new CMDExpInfo();
    obj->m_name = m_name;
    obj->m_showText = m_showText;
    obj->m_comment = m_comment;
    obj->m_scriptName = m_scriptName;
    obj->m_index = m_index;
    obj->m_type = m_type;
    obj->m_expression = m_expression;
    obj->m_group = m_group;
    obj->m_container = NULL;
    obj->m_parameters.clear();
    return obj;
}
qreal CMDExpInfo::decimalValue()
{
    return m_value.toDouble();
}
QString CMDExpInfo::stringValue()
{
    return m_value.toString();
}
QString CMDExpInfo::stringValue(int decimal, const QString &dateFormat, int maxLength)
{
    switch (m_value.type())
    {
    case QVariant::DateTime:
    case QVariant::Date:
    case QVariant::Time:
        return m_value.toDateTime().toString(dateFormat);
    case QMetaType::Int:
    case QMetaType::LongLong:
    case QMetaType::Double:
    case QMetaType::UInt:
    case QMetaType::ULongLong:
        return QString::number(m_value.toDouble(), 'f', decimal);
    case QMetaType::QString:
    default:
    {
        QString str = m_value.toString();
        if(maxLength < str.length())
            str = str.left(maxLength);
        return str;
    }
        break;
    }

    return m_value.toString();
}
QDateTime CMDExpInfo::dateTimeValue()
{
    return m_value.toDateTime();
}
void CMDExpInfo::onDataChanged()
{
    calculateExpression();
    m_container->loadDataChangedEvent(this);
}
QList<QString> CMDExpInfo::getReferencedVarList()
{
    m_expTool.compile(lowerExpression());
    return m_expTool.referencedVarList();
}
void CMDExpInfo::calculateExpression()
{
    for (int i = 0; i < m_parameters.count(); i++)
        m_expTool.setVariableValue(m_parameters.at(i)->name(), m_parameters.at(i)->value());

    bool isOk = true;
    m_value = m_expTool.excute(&isOk);
}
/////////////////////////////////////////////////////////////////////////////////
CMDExpManager::CMDExpManager(int type, QList<QStringList> *list)
{
    append(type, list);
}
CMDExpManager::~CMDExpManager()
{
    clear();
}
CMDExpManager *CMDExpManager::clone()
{
    CMDExpManager* obj = new CMDExpManager;
    for(int i = 0; i < m_list.count(); i++)
        obj->m_list.append(m_list.at(i)->clone());
    return obj;
}
void CMDExpManager::serialize(QJsonObject &json)
{
    QJsonArray array;
    foreach (CMDExpInfo* e, m_list)
    {
        if(e->expression().isEmpty())
            continue;

        QJsonObject jsInfo;
        jsInfo.insert("Name", e->name());
        jsInfo.insert("Expression", e->expression());
        array.append(jsInfo);
    }
    json.insert("List", array);
}
void CMDExpManager::deserialize(const QJsonObject &json)
{
    QJsonArray array = json.value("List").toArray();
    int index = 0;
    foreach (QJsonValue v, array)
    {
        QJsonObject jsInfo = v.toObject();
        QString name = jsInfo.value("Name").toString();
        QString expression = jsInfo.value("Expression").toString();
        setExpression(name, expression, index);
    }
}
void CMDExpManager::append(int type, QList<QStringList> *list)
{
    if(type<0 || type>=MaxType)
        return;

    CMDExpGroup* group = NULL;
    for(int i = 0; i < list->count(); i++)
    {
        //init paras
        QString name;
        QString showText;
        QString scriptName;
        QString comment;
        bool isGroup = false;

        QStringList l = list->at(i);
        int c = l.count();
        if(c == 0)
            continue;

        name = l.at(0);
        if(name.startsWith('$'))
        {
            removeExpInfo(name.right(name.length() - 1));
            continue;
        }
        if(name.startsWith('#'))
        {
            name = name.right(name.length() - 1);
            isGroup = true;
        }
        if(c > 1)
            showText = l.at(1);
        if(c > 2)
        {
            if(isGroup)
                comment = l.at(2);
            else
                scriptName = l.at(2);
        }
        if(c > 3)
        {
            if(!isGroup)
                comment = l.at(3);
        }

        if(isGroup)
            group = CMDExpConfig::setCurrentGroup(name, showText, comment);
        else
        {
            CMDExpInfo* info = new CMDExpInfo(name, showText, scriptName, comment);
            info->setType(type);
            info->setIndex(CMDExpConfig::getNextIndex(type));
            info->setGroup(group);
            m_list.append(info);
        }
    }
}
void CMDExpManager::append(CMDExpManager *manager)
{
    if(manager == NULL)
        return;

    for(int i = 0; i < manager->list()->count(); i++)
        m_list.append(manager->list()->at(i)->clone());
}
void CMDExpManager::append(CMDExpInfo *exp)
{
    m_list.append(exp);
}
void CMDExpManager::setContainer(IVarObject *container)
{
    for(int i = 0; i < m_list.count(); i++)
        m_list.at(i)->setContainer(container);
}
int CMDExpManager::expressionCount()
{
    int count = 0;
    for(int i = 0; i < m_list.count(); i++)
    {
        if (!m_list.at(i)->expression().isEmpty())
            count++;
    }

    return count;
}
void CMDExpManager::clear()
{
    qDeleteAll(m_list);
    m_list.clear();
}
/////////////////////////////////////////////////////////////////////////////////
void CMDExpManager::setExpression(const QString &name, const QString &expression, int &index)
{
    //index用于优化,减少搜索范围
    for(int i = index; i < m_list.count(); i++)
    {
        CMDExpInfo* e = m_list.at(i);
        if (name.compare(e->name(), Qt::CaseInsensitive) == 0)
        {
            e->setExpression(expression);
            index = i+1;
        }
    }
}
void CMDExpManager::removeExpInfo(const QString &name)
{
    for(int i = 0; i < m_list.count(); i++)
    {
        CMDExpInfo* exp = m_list.at(i);
        if(exp->name().compare(name, Qt::CaseInsensitive) == 0)
        {
            m_list.removeOne(exp);
            return;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////
