#ifndef ITESTOR_DATA_GROUP_MGR_2016_09_06_H
#define ITESTOR_DATA_GROUP_MGR_2016_09_06_H
#include <QString>

class ITestorDataGroupMgr
{
public:
    virtual bool SaveCase(QString testDataFileName, QString & fileContent) = 0;
    virtual bool DeleteCase(QString testDataFileName) = 0;
    virtual bool GetCase(QString testDataFileName, QString & fileContentOut) = 0;
    virtual void GetAllCaseName(QStringList & caseNameListOut) = 0;
};

#endif