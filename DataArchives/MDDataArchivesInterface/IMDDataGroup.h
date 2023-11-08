#ifndef MDDATAGROUP_H
#define MDDATAGROUP_H

class CMDDataGroup
{
public:
    CMDDataGroup();
    virtual ~CMDDataGroup();
    void serialize(QDataStream* ar,bool bSave);
public:
    void operator =(const CMDDataGroup& OtherObj);
public:
    //名称
    QString name();
    void setName(const QString &name);
    //启用
    bool isEnable();
    void setEnable(bool bEnable);
    //描述
    QString description();
    void setDescription(const QString &description);
public:
    bool addItem(CMDDataItem *item);
    bool removeItem(CMDDataItem *item);
    void removeAllItem();
    CMDDataItem *getItem(const QString &name);
    CMDDataItem *getItem(int index);
    int itemCount();
private:
    QList <CMDDataItem*> m_itemList;
};

#endif // MDDATAGROUP_H
