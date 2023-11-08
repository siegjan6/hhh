#ifndef DATAARCHIVETESTORPLUGIN_H
#define DATAARCHIVETESTORPLUGIN_H

#include <QObject>
#include <ITestorInterface.h>
#include "dataarchivetestorplugin_global.h"

class JSDataArchivesTestor;

class  DataArchiveTestorPlugin: public QObject, public ITestorInterface
{
public:
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.hite.NetSCADA6.TestorPlugin.ITestorInterface" FILE "testor.json")
    Q_INTERFACES(ITestorInterface )
public:
    DataArchiveTestorPlugin();
public:
    bool SetParams(QScriptEngine * pJSEngine, IMDDataArchives * pIMDDataArchive, ITestorDataGroupMgr * pTestDataGroupMgr) ;
    bool Start();
    bool Stop();
private:
    JSDataArchivesTestor * m_pJSDATestor;
};

#endif // DATAARCHIVETESTORPLUGIN_H
