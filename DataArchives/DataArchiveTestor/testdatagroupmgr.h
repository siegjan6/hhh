#ifndef TESTDATAGROUPMGR_H
#define TESTDATAGROUPMGR_H
#include "CaseGroupMgr.h"

class TestDataGroupMgr : public CaseGroupMgr
{
 public:
    static TestDataGroupMgr * GetInst();
private:
    TestDataGroupMgr();
 private:
    static TestDataGroupMgr * m_Inst;
};

/*
class TestDataGroupMgr
{
 public:
    static TestDataGroupMgr * GetInst();
    //!初始化测试用例数据保存路径 每个测试用例是一个XML文件 保存在此目录下
    bool Init(QString dirName);
private:
    TestDataGroupMgr();
public:
    bool SaveTestDataCase(QString testDataFileName, QString & fileContent);
    bool GetTestDataCase(QString testDataFileName, QString & fileContentOut);
    void GetAllTestDataCaseName(QStringList & caseNameListOut);
private:
    static TestDataGroupMgr * m_Inst;
    QString m_FullDirName; //测试数据文件所在完整目录
    QStringList m_CaseNameList; //测试数据文件名集合
};
*/

#endif // TESTDATAGROUPMGR_H
