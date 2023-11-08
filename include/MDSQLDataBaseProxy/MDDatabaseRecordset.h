#ifndef MDDATABASERECORDSET_H
#define MDDATABASERECORDSET_H

#include "mdsqldatabaseproxy_global.h"
#include <QVariant>
class CMDDatabaseRecordsetPrivate;
class MDSQLDATABASEPROXYSHARED_EXPORT CMDDatabaseRecordset
{
public:
    CMDDatabaseRecordset();
    ~CMDDatabaseRecordset();
//属性操作
public:
    bool isValid();
    QString name();//!结果集的名称
    void setName(const QString & rsName);
    QString id();//!结果集ID
    void setID(const QString & rsID);
public:
    QString getFiledName(int index);
    int getFiledIndex(const QString &name);
//标准数据集合
public:
    void bindFileds(const QStringList &fieldNameList);
    int  addRow();
    bool deleteRow(int index);
    bool setValue(int row,int col,const QVariant &data);
    bool setValue(int row,const QString &fieldName,const QVariant &data);
//数据库数据集合
public:
    void bindSqlQuery(void *sqlQuery);
//数据读取接口
public:
    int rowCount();
    int columnCount();
    void moveTo(int index);
    void moveNext();
    void movePrevious();
    void moveFirst();
    void moveLast();
    bool isFirst();
    bool isLast();
    void clear();
    QVariant value(int row,int col);
    QVariant value(int row,const QString &fieldName);
    QVariant value(int col);
    QVariant value(const QString &fieldName);
private:
    CMDDatabaseRecordsetPrivate* m_recordsetPrivate;
};
#endif // MDDATABASERECORDSET_H
