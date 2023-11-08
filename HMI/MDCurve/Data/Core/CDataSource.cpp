#include "CDataSource.h"
#include <QDataStream>
#include <QJsonObject>
#include <QJsonArray>

QString CDataSource::source() const
{
    return m_source;
}

void CDataSource::setSource(const QString &source)
{
    m_source = source;
}
QString CDataSource::table() const
{
    return m_table;
}

void CDataSource::setTable(const QString &table)
{
    m_table = table;
}
QStringList CDataSource::fields() const
{
    return m_field;
}

void CDataSource::setFields(const QStringList &field)
{
    m_field = field;
}

void CDataSource::serialize(QJsonObject &json)
{
    json.insert("Source",m_source);
    json.insert("Table",m_table);
    QJsonArray array;
    foreach(QString str,m_field)
    {
        array.append(str);
    }
    json.insert("Field",array);
}

void CDataSource::deserialize(const QJsonObject &json)
{
    m_field.clear();
    m_source = json.value("Source").toString();
    m_table = json.value("Table").toString();
    QJsonArray array = json.value("Field").toArray();
    foreach (QJsonValue value, array)
    {
        m_field.append(value.toString());
    }
}



