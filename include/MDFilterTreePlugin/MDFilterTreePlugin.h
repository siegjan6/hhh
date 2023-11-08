#ifndef MDFILTERTREEPLUGIN_H
#define MDFILTERTREEPLUGIN_H

/*!

\author bHVvY2hhbw==

\section 功能
1.对外接口类--过滤树控件以及用户自定义的数据字段。
2.CMDFilterTreeDataItem类为过滤条件每一个条件属性接口。
    也作为用户自定义的字段类型，可用于第三方的数据库的过滤条件配置。
3.CMDFilterTreeData类为单个过滤条件存储序列化接口类。
4.CMDFilterTreePlugin类为过滤树控件的封装接口类。
5.CMDFilterTreeDataManager类为多个过滤条件存储读取序列化接口类。

*/

#include <QVariant>
#include "mdfiltertreeplugin_global.h"

class CFilterTreePlugin;
class QDialog;

class IMDDataTable;       

class MDFILTERTREEPLUGIMDHARED_EXPORT CMDFilterTreeDataItem
{
public:
    explicit CMDFilterTreeDataItem();

    enum CFilterType
    {
        Filter_Invalid = -1,
        Filter_Static  = 0,
        Filter_Dynamic = 1,
        Filter_Enum    = 2
    };

    void serialize(QDataStream *ar, bool bSave);

    CMDFilterTreeDataItem *clone();
    void operator=(const CMDFilterTreeDataItem &otherObj);

    inline void setCount(int count){m_iCount = count;}
    inline int count(){return m_iCount;}

    inline int getDataType(){return m_iDataType;}
    inline void setDataType(int type){m_iDataType = type;}

    inline CMDFilterTreeDataItem::CFilterType getType(){return m_iType;}
    inline void setType(CMDFilterTreeDataItem::CFilterType type){m_iType = type;}

    inline void setName(const QString &name){m_strName = name;}
    inline QString getName(){return m_strName;}

    inline void setDescripe(const QString &descripe){m_strDescripe = descripe;}
    inline QString getDescripe(){return m_strDescripe;}

    inline void setHintInfo(const QString &hint){m_strHint = hint;}
    inline QString getHintInfo(){return m_strHint;}

    inline void setOperator(const QString &operate){m_strOperator = operate;}
    inline QString getOperator(){return m_strOperator;}

    inline void setValue(const QString &value){m_strValue = value;}
    inline QString getValue(){return m_strValue;}

    inline int getID(){return m_ID;}
    inline void setID(int id){m_ID = id;}

    inline QVariant data(){return m_varData;}
    inline void setData(const QVariant &vData){m_varData = vData;}

private:
    CMDFilterTreeDataItem::CFilterType m_iType;
    QString m_strName;
    QString m_strDescripe;
    QString m_strHint;
    QString m_strOperator;
    QString m_strValue;
    int m_iCount;
    int m_ID;
    int m_iDataType;
    QVariant m_varData;    
};

class MDFILTERTREEPLUGIMDHARED_EXPORT CMDFilterTreeData
{
public:
    explicit CMDFilterTreeData();

    ~CMDFilterTreeData();

    void serialize(QDataStream *ar, bool bSave);

    CMDFilterTreeData *clone();
    void operator=(const CMDFilterTreeData &otherObj);

    inline int count(){return m_iCount;}

    inline int getPosition(){return m_iPosition;}
    inline void setPosition(int pos){m_iPosition = pos;}

    CMDFilterTreeDataItem *getItem(int index);
    void insert(CMDFilterTreeDataItem *pItem);
    void insert(CMDFilterTreeData *pData);

    void removeAllItem();

    inline int getType(){return m_iType;}
    inline void setType(int type){m_iType = type;}

    inline void setName(const QString &name){m_strName = name;}
    inline QString getName(){return m_strName;}

    inline void setTableName(const QString &name){m_strTableName = name;}
    inline QString getTableName(){return m_strTableName;}

    inline QString getCondition(){return m_strCondition;}
    inline void setCondition(const QString &condition){m_strCondition = condition;}

    QList<CMDFilterTreeDataItem*> dynamicInputList();
    QString updateCondition(QList<CMDFilterTreeDataItem*> items);

private:
    QList<CMDFilterTreeDataItem*> m_itemList;
    QString m_strName;
    QString m_strTableName;
    QString m_strCondition;
    int m_iType;
    int m_iCount;
    int m_iPosition;
};

class MDFILTERTREEPLUGIMDHARED_EXPORT CMDFilterTreeDataManager
{
public:
    explicit CMDFilterTreeDataManager();
    ~CMDFilterTreeDataManager();

    CMDFilterTreeDataManager *clone();
    void operator=(const CMDFilterTreeDataManager &obj);

    void load(QDataStream *ar);
    void save(QDataStream *ar);
    void save();

    int getCount(){return m_FilterDataManaList.count();}
    CMDFilterTreeData *getFilterData(int index);
    CMDFilterTreeData *getFilterData(const QString &name);

    void removeAllData();
    void removeData(int index);
    void removeData(const QString &name);

    void addData(CMDFilterTreeData *pData){m_FilterDataManaList.append(pData);}

    QString updateCondition(const QString &name, QList<CMDFilterTreeDataItem*> items);
    QList<CMDFilterTreeDataItem*> dynamicInputList(const QString &name);

private:
    QList<CMDFilterTreeData*> m_FilterDataManaList;
    QDataStream *m_pDataStream;
};

class MDFILTERTREEPLUGIMDHARED_EXPORT CMDFilterTreePlugin
{
public:
    explicit CMDFilterTreePlugin();
    ~CMDFilterTreePlugin();

    /*!
    \section 过滤树控件
    */
public:
    QDialog *widget();

    /*!
    \section 加载存储序列化
    */
public:
    bool load(const QString &condition);
    bool load(CMDFilterTreeData *pFilterData);
    bool save(CMDFilterTreeData *pFilterData);

    /*!
    \section 封装的对外接口。
             初始化数据以及获取过滤条件配置接口。
    */
public:
    void initData(IMDDataTable *pTable);
    void initData(const QString &tableName, QList<CMDFilterTreeDataItem*> &customFieldList);
    QString getFilterCondition();

private:
    CFilterTreePlugin *m_pFilterTree; //! 过滤控件内部实现类
};

#endif // MDFILTERTREEPLUGIN_H
