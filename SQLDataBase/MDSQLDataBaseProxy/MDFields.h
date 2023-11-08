#ifndef CMDFIELDS_H
#define CMDFIELDS_H

#include<QString>
#include<QList>
class CMDField
{
public:
    CMDField();
    ~CMDField();
public:
    QString name();
    void setName(const QString &name);
private:
    QString m_name;
};


class CMDFields
{
public:
    CMDFields();
    ~CMDFields();
public:
    int size();
    bool addFiled(const QString &name);
    void deleteFiled(const QString &name);
    void deleteAllFiled();
    int getFiledIndex(const QString &name);
    QString getFiledName(int index);
private:
    QList<CMDField*> m_fieldList;
};

#endif // CMDFIELDS_H
