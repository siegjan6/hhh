#include <QJsonArray>
#include "Dictionary.h"
//////////////////////////////////////////////////////////////
CDictionary::CDictionary()
{
    m_systemVersion = 0;
    m_currentDict = NULL;

    int count = 26;
    m_dicts = QVector<QMap<QString, QString>*>(count);
    for(int i = 0; i < count; i++)
        m_dicts[i] = new QMap<QString, QString>();
}
CDictionary::~CDictionary()
{
    qDeleteAll(m_dicts.begin(), m_dicts.end());
    m_dicts.clear();
}
void CDictionary::setCurrentDict(int value)
{
    if (value < 0 || value >= m_dicts.count())
        m_currentDict = NULL;
    else
        m_currentDict = m_dicts.at(value);
}
//////////////////////////////////////////////////////////////
bool CDictionary::addKey(const QString &key)
{
    if(key.length() == 0)
        return false;
    if (m_basicDict.find(key) != m_basicDict.end())
        return false;
    m_basicDict.insert(key, QDateTime::currentDateTime());
    return true;
}
void CDictionary::deleteKey(const QString &key)
{
    m_basicDict.remove(key);
}
QString CDictionary::value(const QString &key)
{
    if (m_currentDict == NULL)
        return key;

    QString value = m_currentDict->value(key);
    if (value.length() == 0)
        value = key;

    return value;
}
//////////////////////////////////////////////////////////////
void CDictionary::serialize(QDataStream &stream)
{
    qint32 version = 1;
    stream << version;

    stream << m_basicDict;

    qint32 count = m_dicts.count();
    stream << count;
    for(int i = 0; i < count; i++)
        stream << *m_dicts.at(i);
}
void CDictionary::deserialize(QDataStream &stream)
{
    qint32 version = 0;
    stream >> version;

    stream >> m_basicDict;

    qint32 count = 0;
    stream >> count;
    for(int i = 0; i < count; i++)
    {
        if (i >= m_dicts.count())
        {
            QMap<QString, QString> map;
            stream >> map;
        }

        stream >> *m_dicts.at(i);
    }
}
void CDictionary::serialize(QJsonObject &json)
{
    QJsonObject jsBD;
    QList<QString> bKeys = m_basicDict.keys();
    foreach(QString k, bKeys)
    {
        qint64 time = m_basicDict.value(k).toMSecsSinceEpoch();
        jsBD.insert(k, (double)time);
    }
    json.insert("BasicDict", jsBD);

    QJsonArray arrayDict;
    for(int i = 0; i < m_dicts.count(); i++)
    {
        QMap<QString, QString>* dict = m_dicts.at(i);
        QJsonObject jsDict;
        QList<QString> keys = dict->keys();
        foreach(QString k, keys)
            jsDict.insert(k, dict->value(k));
        arrayDict.append(jsDict);
    }
    json.insert("Dicts", arrayDict);
}
void CDictionary::deserialize(const QJsonObject &json)
{
    QJsonObject jsBD = json.value("BasicDict").toObject();
    QVariantMap mapBD = jsBD.toVariantMap();
    QStringList bKeys = mapBD.keys();
    foreach(QString k, bKeys)
    {
        QDateTime t = QDateTime::fromMSecsSinceEpoch((qint64)(mapBD.value(k).toDouble()));
        m_basicDict[k] = t;
    }

    QJsonArray arrayDict = json.value("Dicts").toArray();
    int count = arrayDict.count();
    if(count > m_dicts.count())
        count = m_dicts.count();
    for(int i = 0; i < count; i++)
    {
        QJsonObject jsDict = arrayDict.at(i).toObject();
        QVariantMap mapDict = jsDict.toVariantMap();
        QMap<QString, QString>* d = m_dicts.at(i);
        QStringList keys = mapDict.keys();

        foreach(QString k, keys)
            (*d)[k] = mapDict.value(k).toString();
    }
}
//////////////////////////////////////////////////////////////
void CDictionary::reset(const QStringList &keys, const QStringList &values, int index)
{
    QMap<QString, QString>* dict = m_dicts.at(index);
    if (dict == NULL)
        return;

    dict->clear();
    int count = keys.count();
    for(int i = 0; i < count; i++)
        dict->insert(keys.at(i), values.at(i));
}
void CDictionary::insert(const QString &key, const QString &value, int index)
{
    QMap<QString, QString>* dict = m_dicts.at(index);
    if (dict == NULL)
        return;
    if(key.isEmpty() || value.isEmpty())
        return;

    dict->insert(key, value);
}
void CDictionary::updateFromDict(CDictionary *source)
{
    QList<QString> keys = source->m_basicDict.keys();
    int count = keys.count();
    for(int i = 0; i < count; i++)
        addKey(keys.at(i));

    for(int i = 0; i < m_dicts.count(); i++)
    {
        QMap<QString, QString>* sourceDict = source->m_dicts.at(i);
        QMap<QString, QString>* dict = m_dicts.at(i);

        QList<QString> keys = sourceDict->keys();
        count = keys.count();
        for(int j = 0; j < count; j++)
        {
            QString key = keys.at(j);
            QString value = (*dict)[key];
            if(value.isEmpty())
            {
                QString v = (*sourceDict)[key];
                dict->insert(key, v);
            }

        }
    }
}
void CDictionary::fillModel(QStandardItemModel &model, QList<int> &indexs)
{
    int rowCount = m_basicDict.count();
    int colCount = indexs.count();
    QList<QString> keys = m_basicDict.keys();

    model.setColumnCount(indexs.count()+2);
    model.setRowCount(rowCount);

    for(int i = 0; i < rowCount; i++)
    {
        QString key = keys.at(i);
        model.setItem(i, 0, new QStandardItem(m_basicDict[key].toString("yyyy-MM-dd HH:mm:ss")));
        model.setItem(i, 1, new QStandardItem(key));

        for(int j = 0; j < colCount; j++)
        {
            QString value = m_dicts.at(indexs.at(j))->value(key);
            model.setItem(i, j+2, new QStandardItem(value));
        }
    }
}
void CDictionary::appendModel(QStandardItemModel &model, const QString &key)
{
    int count = model.columnCount();
    QList<QStandardItem*> list;
    list.append(new QStandardItem(m_basicDict[key].toString("yyyy-MM-dd HH:mm:ss")));
    list.append(new QStandardItem(key));
    for(int i = 0; i < count-2; i++)
        list.append(new QStandardItem());

    model.insertRow(0, list);
}
void CDictionary::saveDataFromModel(QStandardItemModel &model)
{
    int colCount = model.columnCount() - 2;
    int rowCount = model.rowCount();

    for(int i = 0; i < colCount; i++)
    {
        int index = model.horizontalHeaderItem(i+2)->data().toULongLong();
        QMap<QString, QString>* map = m_dicts.at(index);
        map->clear();

        for(int j = 0; j < rowCount; j++)
        {
            QString value = model.item(j, i+2)->text();
            if(value.length() == 0)
                continue;
            QString key = model.item(j, 1)->text();
            map->insert(key, value);
        }
    }
}
//////////////////////////////////////////////////////////////
