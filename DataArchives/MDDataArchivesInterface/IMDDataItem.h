#ifndef MDDATAITEM_H
#define MDDATAITEM_H

class CMDDataItem
{
public:
    CMDDataItem();
    virtual ~CMDDataItem();
    void operator =(const CMDDataItem& OtherObj);
    void serialize(QDataStream* ar,bool bSave);
public:
    QString name();
    void setName(const QString &name);

    QVariant::Type dataType();
    void setDataType(QVariant::Type type);

    QString description();
    void setDdescription(const QString &description);
private:
    QString m_strName;
    QVariant::Type m_vtData;
    int		m_iVersion;
    QString m_strComment;
};

#endif // MDDATAITEM_H
