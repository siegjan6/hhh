#include "DataArchiveTestorPlugin.h"
#include <QScriptEngine>
#include <QScriptValue>
#include <IMDDataArchives.h>
#include "JSDataArchivesTestor.h"

DataArchiveTestorPlugin::DataArchiveTestorPlugin()
    :m_pJSDATestor(0)
{

}

bool DataArchiveTestorPlugin::SetParams(QScriptEngine * pJSEngine, IMDDataArchives * pIMDDataArchive, ITestorDataGroupMgr * pTestDataGroupMgr)
{
    //TODO:验证输入参数
    if(!m_pJSDATestor)
    {
        m_pJSDATestor = new JSDataArchivesTestor;
        QScriptValue testorObjValue = pJSEngine->newQObject(m_pJSDATestor);
        pJSEngine->globalObject().setProperty("dataArchivesTestor", testorObjValue);
    }

    m_pJSDATestor->SetParams(pIMDDataArchive, pTestDataGroupMgr, pIMDDataArchive->dataSourceManager());
    return true;
}

bool DataArchiveTestorPlugin::Start()
{
    return true;
}

bool DataArchiveTestorPlugin::Stop()
{
    return true;
}
