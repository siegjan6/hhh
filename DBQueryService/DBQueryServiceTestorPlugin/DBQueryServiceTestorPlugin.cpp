#include "DBQueryServiceTestorPlugin.h"
#include <QScriptEngine>
#include <QScriptValue>
#include <IMDDataArchives.h>
#include <JSQueryTestor.h>

DBQueryServiceTestorPlugin::DBQueryServiceTestorPlugin()
    :m_pJSQueryTestor(0)
{
}

bool DBQueryServiceTestorPlugin::SetParams(QScriptEngine * pJSEngine, IMDDataArchives * pIMDDataArchive, ITestorDataGroupMgr * pTestDataGroupMgr)
{
    //TODO:验证输入参数
    if(!m_pJSQueryTestor)
    {
        m_pJSQueryTestor = new JSQueryTestor;
        //m_pJSQueryTestor->start();
        QScriptValue testorObjValue = pJSEngine->newQObject(m_pJSQueryTestor);
        pJSEngine->globalObject().setProperty("queryTestor", testorObjValue);
    }

    m_pJSQueryTestor->setParams(pIMDDataArchive, pTestDataGroupMgr);
    return true;
}

bool DBQueryServiceTestorPlugin::Start()
{
    return true;
}

bool DBQueryServiceTestorPlugin::Stop()
{
    return true;
}
