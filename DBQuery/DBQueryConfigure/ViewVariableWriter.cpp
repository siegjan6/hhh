#include "XMLViewVariableStorage.h"
#include "JsonViewVariableStorage.h"
#include "utils.h"
#include <QMutex>
#include <QFile>

static QMutex localMutex; //读写文件时 加锁 防止并发时出现竞态

namespace DBQ
{
//函数功能:从XML文件中加载视图变量文件
//viewVarsOut必须为空
DBQUERYCONFIGURE_EXPORT bool ViewVariable_Load(const QString & fileName, FILE_TYPE fileType, QList<ViewVariable *> & viewVarsOut, QString & errStrOut)
{
    QMutexLocker locker(&localMutex);

    if(FILE_TYPE_XML == fileType)
    {
        XMLViewVariableStorage storage;
        return storage.Load(fileName, viewVarsOut, errStrOut);
    }else if(FILE_TYPE_JSON == fileType)
    {
        JsonViewVariableStorage storage;
        return storage.Load(fileName, viewVarsOut, errStrOut);
    }

    WriteInfoLog("LoadViewVariable:type error");
    return false;
}

DBQUERYCONFIGURE_EXPORT bool ViewVariable_LoadFromDir(const QString & viewPath, QList<ViewVariable *> & viewVarsOut, QString & errStrOut)
{
    QString fullFileName(viewPath);
    fullFileName.append("/global_view_vars.json");

    return ViewVariable_Load(fullFileName, FILE_TYPE_JSON, viewVarsOut, errStrOut);
}

DBQUERYCONFIGURE_EXPORT bool ViewVariable_Write(const QString & fileName, FILE_TYPE fileType, QList<ViewVariable *> & viewVarsIn, QString & errStrOut)
{
    QMutexLocker locker(&localMutex);

    if(FILE_TYPE_XML == fileType)
    {
        XMLViewVariableStorage storage;
        return storage.Save(fileName, viewVarsIn, errStrOut);
    }else if(FILE_TYPE_JSON == fileType)
    {
        JsonViewVariableStorage storage;
        return storage.Save(fileName, viewVarsIn, errStrOut);
    } 
    WriteInfoLog("WriteViewVariableToFile:type error");
    return false;
}
}
