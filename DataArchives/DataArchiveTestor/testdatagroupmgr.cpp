#include "testdatagroupmgr.h"

//TODO:去掉此类
TestDataGroupMgr * TestDataGroupMgr::m_Inst = 0;

TestDataGroupMgr * TestDataGroupMgr::GetInst()
{
    if(!m_Inst)
    {
        m_Inst = new TestDataGroupMgr();
        m_Inst->SetFilePostfix("dt");
    }

    return m_Inst;
}

TestDataGroupMgr::TestDataGroupMgr()
{
}

/*
TestDataGroupMgr * TestDataGroupMgr::m_Inst = 0;

TestDataGroupMgr * TestDataGroupMgr::GetInst()
{
    if(!TestDataGroupMgr::m_Inst)
    {
        TestDataGroupMgr::m_Inst = new TestDataGroupMgr();
    }

    return TestDataGroupMgr::m_Inst;
}

bool TestDataGroupMgr::Init(QString dirName)
{
    //创建目录
    m_FullDirName = dirName;
    if(m_FullDirName.isEmpty())
        return false;

    QDir dir(m_FullDirName);
    if (!dir.exists())
    {
        if(!dir.mkdir(m_FullDirName))
            return false;
    }

    //遍历文件名
    QStringList filters;
    filters << "*.dt" ;
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files); // | QDir::NoDotAndDotDot

    //QStringList QDir::entryList(const QStringList & nameFilters, Filters filters = NoFilter, SortFlags sort = NoSort) const
    //Returns a list of the names of all the files and directories in the directory, ordered according to the name and attribute filters
    //previously set with setNameFilters() and setFilter(), and sorted according to the flags set with setSorting().

    m_CaseNameList = dir.entryList();

    for(int i=0; i<m_CaseNameList.size(); i++)
    {
        m_CaseNameList[i] = m_CaseNameList[i].left(m_CaseNameList[i].length() - 3); //3 = .dt
        qDebug()<<m_CaseNameList[i]<<",";
    }
    return true;
}

TestDataGroupMgr::TestDataGroupMgr()
{
}

bool TestDataGroupMgr::SaveTestDataCase(QString testDataFileName, QString & fileContent)
{
    QString fullFileName = m_FullDirName + testDataFileName + ".dt";
    QFile file(fullFileName);
    if(!file.open(QIODevice::ReadWrite))
    {
        file.close();
        return false;
    }

    QTextStream storage(&file);
    storage<<fileContent; //追加

    file.close();

   if(!m_CaseNameList.contains(testDataFileName))
       m_CaseNameList.append(testDataFileName);
}

bool TestDataGroupMgr::GetTestDataCase(QString testDataFileName, QString & fileContentOut)
{
    QString fullFileName = m_FullDirName + testDataFileName + ".dt";
    QFile file(fullFileName);
    if(!file.open(QIODevice::ReadWrite))
    {
        file.close();
        return false;
    }

    QTextStream out(&file);
    fileContentOut = out.readAll();

    return true;
}

void TestDataGroupMgr::GetAllTestDataCaseName(QStringList & caseNameListOut)
{
    caseNameListOut = m_CaseNameList; //测试数据文件名集合
}
*/
