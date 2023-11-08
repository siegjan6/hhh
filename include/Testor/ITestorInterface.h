#ifndef ITESTOR_INTERFACE_2016_09_06_H
#define ITESTOR_INTERFACE_2016_09_06_H

class QScriptEngine;
class IMDDataArchives;
class ITestorDataGroupMgr;
class IMDDataSourceManager;

class ITestorInterface
{
public:
    virtual bool SetParams(QScriptEngine * pJSEngine, IMDDataArchives * pIMDDataArchive, ITestorDataGroupMgr * pTestDataGroupMgr) = 0;
    virtual bool Start() = 0;
    virtual bool Stop() = 0;
};

#define ITestorInterface_iid "com.hite.NetSCADA6.TestorPlugin.ITestorInterface"
Q_DECLARE_INTERFACE(ITestorInterface, ITestorInterface_iid)

#endif
