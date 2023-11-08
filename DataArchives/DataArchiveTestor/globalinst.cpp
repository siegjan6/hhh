#include "TestorGlobalInst.h"
#include <MDDataArchivesFactory.h>
#include <IMDDataSource.h>
#include <IMDDataSourceManager.h>
#include <IMDDataGroupManager.h>
#include <QCoreApplication>
#include <QString>
#include "testdatagroupmgr.h"
#include "scriptcasegroupmgr.h"
#include <IMDDataArchives.h>

static IMDDataArchives  * pDataArchives = 0;

bool TestorGlobalInst::Init()
{
    pDataArchives = CMDDataArchivesFactory::createDataArchives();
    QString dataSrcFullPath = QCoreApplication::applicationDirPath() + "/datasource.bin";
    pDataArchives->dataSourceManager()->load(dataSrcFullPath);
    pDataArchives->dataSourceManager()->save(dataSrcFullPath);

    QString dataGroupFullPath = QCoreApplication::applicationDirPath() + "/datagroup.bin";
    pDataArchives->dataGroupManager()->load(dataGroupFullPath);
    pDataArchives->dataGroupManager()->save(dataGroupFullPath);

    QString dataGroupDirFullPath = QCoreApplication::applicationDirPath() + "/testdata/";
    //设置测试用例保存路径
    TestDataGroupMgr::GetInst()->Init(dataGroupDirFullPath);

    //设置测试脚本保存路径
    QString scriptGroupDirFullPath = QCoreApplication::applicationDirPath() + "/testscript/";
    ScriptCaseGroupMgr::GetInst()->Init(scriptGroupDirFullPath);
    return true;
}

bool TestorGlobalInst::Reload(QString dirFullName)
{
    if(pDataArchives)
    {
        pDataArchives->unLoad();
        delete pDataArchives;
        pDataArchives = CMDDataArchivesFactory::createDataArchives();
    }

    pDataArchives = CMDDataArchivesFactory::createDataArchives();
    QString dataSrcFullPath = dirFullName + "/datasource.bin";
    pDataArchives->dataSourceManager()->load(dataSrcFullPath);
    pDataArchives->dataSourceManager()->save(dataSrcFullPath);

    QString dataGroupFullPath = dirFullName + "/datagroup.bin";
    pDataArchives->dataGroupManager()->load(dataGroupFullPath);
    pDataArchives->dataGroupManager()->save(dataGroupFullPath);

    QString dataGroupDirFullPath = dirFullName + "/testdata/";
    //设置测试用例保存路径
    TestDataGroupMgr::GetInst()->Reload(dataGroupDirFullPath);

    //设置测试脚本保存路径
    QString scriptGroupDirFullPath = dirFullName + "/testscript/";
    ScriptCaseGroupMgr::GetInst()->Reload(scriptGroupDirFullPath);

    return true;
}

IMDDataArchives * TestorGlobalInst::GetDataArchives()
{
    return pDataArchives;
}
