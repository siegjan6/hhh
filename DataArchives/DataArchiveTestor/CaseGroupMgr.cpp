#include "CaseGroupMgr.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>

CaseGroupMgr::CaseGroupMgr()
{
}

//!初始化测试用例数据保存路径 每个测试用例是一个XML文件 保存在此目录下
bool CaseGroupMgr::Init(QString dirName)
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
    filters << QString("*.%1").arg(m_Postfix) ;
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files); // | QDir::NoDotAndDotDot
    /*
     * QStringList QDir::entryList(const QStringList & nameFilters, Filters filters = NoFilter, SortFlags sort = NoSort) const
        Returns a list of the names of all the files and directories in the directory, ordered according to the name and attribute filters
        previously set with setNameFilters() and setFilter(), and sorted according to the flags set with setSorting().
     */
    m_CaseNameList = dir.entryList();

    int postfixLen = m_Postfix.length();
    postfixLen++; //3 = .dt

    for(int i=0; i<m_CaseNameList.size(); i++)
    {
        m_CaseNameList[i] = m_CaseNameList[i].left(m_CaseNameList[i].length() - postfixLen);
        qDebug()<<m_CaseNameList[i]<<",";
    }
    return true;
}

bool CaseGroupMgr::Reload(QString dirName)
{
        Init(dirName);
}

bool CaseGroupMgr::SaveCase(QString testDataFileName, QString & fileContent)
{
    QString fullFileName = m_FullDirName + testDataFileName + "."+m_Postfix;
    QFile file(fullFileName);
    if(!file.open(QIODevice::ReadWrite))
    {
        file.close();
        return false;
    }

    if(!fileContent.isEmpty())
        file.resize(0);//清空

    QTextStream storage(&file);
    storage<<fileContent; //追加

    file.close();

   if(!m_CaseNameList.contains(testDataFileName))
       m_CaseNameList.append(testDataFileName);
   return true;
}

bool CaseGroupMgr::DeleteCase(QString testDataFileName)
{
    QString fullFileName = m_FullDirName + testDataFileName + "."+m_Postfix;
    return QFile::remove(fullFileName);
}

bool CaseGroupMgr::GetCase(QString testDataFileName, QString & fileContentOut)
{
    QString fullFileName = m_FullDirName + testDataFileName + "."+m_Postfix;
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

void CaseGroupMgr::GetAllCaseName(QStringList & caseNameListOut)
{
    caseNameListOut = m_CaseNameList; //测试数据文件名集合
}

void CaseGroupMgr::SetFilePostfix(QString postfix)
{
    m_Postfix = postfix;
}
