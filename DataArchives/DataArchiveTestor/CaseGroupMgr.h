#ifndef CASE_GROUP_MGR_2016_06_29_H
#define CASE_GROUP_MGR_2016_06_29_H
#include <QString>
#include <QStringList>
#include <ITestorDataGroupMgr.h>

class CaseGroupMgr : public ITestorDataGroupMgr
{
protected:
    CaseGroupMgr();

    /*!
      \section 用例文件管理
     */
public:
    //!初始化用例数据保存路径
    bool Init(QString dirName);
    bool Reload(QString dirName);
    bool SaveCase(QString testDataFileName, QString & fileContent);
    bool DeleteCase(QString testDataFileName);
    bool GetCase(QString testDataFileName, QString & fileContentOut);
    void GetAllCaseName(QStringList & caseNameListOut);
protected:
    void SetFilePostfix(QString postfix);
private:
    QString m_FullDirName; //用例文件所在完整目录
    QStringList m_CaseNameList; //用例文件名集合
    QString m_Postfix;
};

#endif // ABSTRACTGROUPMGR_H
