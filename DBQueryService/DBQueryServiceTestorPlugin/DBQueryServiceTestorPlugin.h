#ifndef DB_QUERY_SERVICE_TESTOR_PLUGIN_2016_09_07_H
#define DB_QUERY_SERVICE_TESTOR_PLUGIN_2016_09_07_H

#include <QObject>
#include <ITestorInterface.h>

class JSQueryTestor;

class  DBQueryServiceTestorPlugin: public QObject, public ITestorInterface
{
public:
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.hite.NetSCADA6.TestorPlugin.ITestorInterface" FILE "testor.json")
    Q_INTERFACES(ITestorInterface )
public:
    DBQueryServiceTestorPlugin();
public:
    bool SetParams(QScriptEngine * pJSEngine, IMDDataArchives * pIMDDataArchive, ITestorDataGroupMgr * pTestDataGroupMgr) ;
    bool Start();
    bool Stop();
private:
    JSQueryTestor * m_pJSQueryTestor;
};

#endif // DATAARCHIVETESTORPLUGIN_H
