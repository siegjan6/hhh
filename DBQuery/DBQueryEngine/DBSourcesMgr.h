#ifndef DBSOURCESMGR_H
#define DBSOURCESMGR_H
#include <QMap>
#include <MDDBQueryConfigure.h>
#include "DatabaseParam.h"
#include <MDDBConnect/MDDBConnect.h>

//加上命名空间 保证模块之间没有重名的类
//不然，在Linux下，加载so时，会找到多个重名的函数，导致运行时崩溃。
namespace DBQE
{
typedef QMap<QString, DBQ::DBSource *> DBSourcesMap;

class DBSourcesMgr
{
public:
    //函数功能:初始化数据源信息.在模块初始化时调用.只能调用一次.
    bool Init(const QString & xmlFileName);
    CDataBaseParam * GetDBConnectionParam(const QString & dbSourceName);
    //函数功能:获得默认数据源 由调用者释放得到的DBSource指针
    DBQ::DBSource * GetDefaultDBSource();
    //函数功能;由数据源名获得其对应的表前缀 不存在则为空
    QString GetTablePrefix(const QString & dbsName);
private:
    //函数功能:检查数据源合法性
    bool CheckDBSource(DBQ::DBSource * pDBSrc);
private:
    DBSourcesMap m_DBSrcsMap;
    QString m_xmlFileName;
};
}

#endif // DBSOURCESMGR_H
