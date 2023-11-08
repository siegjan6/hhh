/*!
\author aGRw 2015.12.25
\section 字典类
*/
#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QVector>
#include <QMap>
#include <QDateTime>
#include <QDataStream>
#include <QStringList>
#include <QStandardItemModel>
#include <QJsonObject>

class CDictionary
{
public:
    CDictionary();
    ~CDictionary();

private:
    //!语言字典，value为空时不保存，key不完整
    QVector<QMap<QString, QString>*> m_dicts;
    //!基本字典，包含key和时间
    QMap<QString, QDateTime> m_basicDict;
    qint32 m_systemVersion;

    //!当前语言字典
    QMap<QString, QString>* m_currentDict;

public:
    void setCurrentDict(int index);
    bool addKey(const QString& key);
    void deleteKey(const QString& key);
    QString value(const QString& key);
    void reset(const QStringList& keys, const QStringList& values, int index);
    void insert(const QString &key, const QString &value, int index);
    void updateFromDict(CDictionary* source);
public:
    void serialize(QDataStream& stream);
    void deserialize(QDataStream& stream);
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);

private:
    void init();
/*!
\section model
*/
public:
    void fillModel(QStandardItemModel& model, QList<int>& indexs);
    void appendModel(QStandardItemModel &model, const QString &key);
    void saveDataFromModel(QStandardItemModel& model);
};
#endif // DICTIONARY_H
