/*!

\author eHVr

\section 功能

    历史数据查询时,需要选择数据来源
    与历史曲线搭配使用.
*/
#ifndef CDATASOURCE_H
#define CDATASOURCE_H

#include <QString>
#include <QStringList>
#include <QJsonObject>
class CDataSource
{
public:
    CDataSource(){}
    ~CDataSource(){}
/*!
  \section 基本属性
*/
public:
    QString source() const;
    void setSource(const QString &source);

    QString table() const;
    void setTable(const QString &table);

    QStringList fields() const;
    void setFields(const QStringList &fields);
private:
    QString     m_source;
    QString     m_table;
    QStringList m_field;    //!一个字段
/*!
  \section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

};

#endif // CDATASOURCE_H
