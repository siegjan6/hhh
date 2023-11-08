/*!
author Y2hvdWps 2016.10.19
section 数值映射表管理类
详细说明
提供数值映射表对外服务的的接口
*/
#ifndef VALUEMAPMANAGER_H
#define VALUEMAPMANAGER_H

#include <QWidget>
#include <QHash>




class IDrawEnvrionment;

class CValueMapManager
{
    friend class CEditValueMapDialog;

public:
    typedef QPair<QString, QString> ValuePair;
    typedef QList<ValuePair> Values;
    typedef QMap<QVariant, qint32> ValueIndex;
    typedef struct valueinfo{
        QString name;
        QString description;
        Values values;
    }ValueInfo;
public:
    static CValueMapManager *getInstance();

    static QString editValueMapUI(IDrawEnvrionment *environment, QString mapName="", QWidget* parent = 0);
    bool isValueMapExist(IDrawEnvrionment *environment, QString mapName);
    void valueMapNames(IDrawEnvrionment *environment, QList<QString>& names);    //! 获取当前工程所有数值映射表的表名名单
    void readFile(IDrawEnvrionment *environment);
    void deleteValueMap(IDrawEnvrionment *environment, QString mapName);
    bool valueMap(IDrawEnvrionment *environment, QString mapName, ValueInfo& info);
    bool valueList(IDrawEnvrionment *environment, QString mapName, Values& values);
protected:
    #define ValueMapFileName    "valueMap.xml"
    typedef QList<ValueInfo> ValueList;
    typedef QHash<QString, qint32> ListIndex;  //! ValueList的索引查询表
    typedef struct prjinfo{
        ValueList list;
        ListIndex index;
    }PrjInfo;
    typedef QHash<QString, PrjInfo> TotalValues;
    TotalValues m_prjValues;

protected:
    CValueMapManager();
    void saveValueMap(IDrawEnvrionment *environment, ValueInfo &info);
    void saveFile(IDrawEnvrionment *environment);


    static CValueMapManager* g_ptr;
};



extern CValueMapManager* gValueMapManager;

#endif // VALUEMAPMANAGER_H
