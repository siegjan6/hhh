#include "XMLRuntimeViewVarStorage.h"
#include "JsonRuntimeViewVarStorage.h"
#include "utils.h"
#include <QFile>
#include <QDir>
#include <QMutex>

static QMutex localMutex; //读写文件时 加锁 防止并发时出现竞态

namespace DBQ
{
 static bool IsRuntimeViewVariableFile(const QString & strIn);

 DBQUERYCONFIGURE_EXPORT bool RuntimeViewVariable_UpdateInDir(const QString & viewPath, const QString & viewName, QList<RuntimeViewVariable *> & rtVarsNew, QString & errStrOut)
 {
     QString fullFileName(viewPath);
     fullFileName.append("/view_").append(viewName).append("_run_view_vars.json");
     return RuntimeViewVariable_Update(fullFileName, FILE_TYPE_JSON, rtVarsNew, errStrOut);
 }

 DBQUERYCONFIGURE_EXPORT bool RuntimeViewVariable_LoadFromDir(const QString & viewPath, const QString & viewName, QList<RuntimeViewVariable *> & rtVarsOut, QString & errStrOut)
 {
     QString fullFileName(viewPath);
     fullFileName.append("/view_").append(viewName).append("_run_view_vars.json");
     return RuntimeViewVariable_Load(fullFileName, FILE_TYPE_JSON, rtVarsOut, errStrOut);
 }

//函数功能:从XML文件中加载运行时视图变量文
//rtVarsOut必须为空
DBQUERYCONFIGURE_EXPORT bool RuntimeViewVariable_Load(QString fileName, FILE_TYPE fileType, QList<RuntimeViewVariable *> & rtVarsOut, QString & errStrOut)
{
    QMutexLocker locker(&localMutex);

    if(FILE_TYPE_XML == fileType)
    {
        XMLRuntimeViewVarStorage storage;
        return storage.Load(fileName, rtVarsOut, errStrOut);
    }else if(FILE_TYPE_JSON == fileType)
    {
        JsonRuntimeViewVarStorage storage;
        return storage.Load(fileName, rtVarsOut, errStrOut);
    }
    errStrOut = QString("%1 file:type error").arg(fileName);
    WriteInfoLog("LoadRuntimeViewVariable:"+errStrOut);
    return false;
}

DBQUERYCONFIGURE_EXPORT bool RuntimeViewVariable_Write(const QString &fileName, FILE_TYPE fileType, QList<RuntimeViewVariable *> & rtVarsIn, QString & errStrOut)
{
    QMutexLocker locker(&localMutex);

    if(FILE_TYPE_XML == fileType)
    {
        XMLRuntimeViewVarStorage storage;
        return storage.Save(fileName, rtVarsIn, errStrOut);
    }else if(FILE_TYPE_JSON == fileType)
    {
        JsonRuntimeViewVarStorage storage;
        return storage.Save(fileName, rtVarsIn, errStrOut);
    }

    errStrOut=QString("%1 file:type error").arg(fileName);
    WriteInfoLog("WriteRuntimeViewVariableToFile:"+errStrOut);
    return false;
}

DBQUERYCONFIGURE_EXPORT bool RuntimeViewVariable_Update(const QString & fileName, FILE_TYPE fileType, QList<RuntimeViewVariable *> & rtVarsNew, QString & errStrOut)
{
    Q_UNUSED(fileType);
    Q_UNUSED(errStrOut);

    QMutexLocker locker(&localMutex);

    //目前只使用json格式文件 暂不支持xml
    JsonRuntimeViewVarStorage storage;
    QList<RuntimeViewVariable *> rtVarsOld;
    QString errStr;
    //先判断文件存不存在 不存在则创建一个空的文件
    QFile josnFile(fileName);
    if(!josnFile.exists())
    {
        if(!storage.Save(fileName, rtVarsOld, errStr))
        {
            errStrOut = QString("%1 file:save error").arg(fileName);
            WriteInfoLog("UpdateRuntimeViewVariableToFile:"+errStrOut);
            return false;
        }
    }

    if(!storage.Load(fileName, rtVarsOld, errStr))
    {
        errStrOut = QString("%1 file:load error").arg(fileName);
        WriteInfoLog("UpdateRuntimeViewVariableToFile:"+errStrOut);
        return false;
    }

    //rtVarsNew更新到rtVarsOld.不存在的项添加到rtVarsOld中.最后保存rtVarsOld
    foreach(RuntimeViewVariable * pVarNew, rtVarsNew)
    {
        bool bIn = false; //是否在rtVarOld中
        foreach (RuntimeViewVariable * pVarOld, rtVarsOld)
        {
            if(pVarNew->key.compare(pVarOld->key, Qt::CaseInsensitive) == 0)
            {
                pVarOld->value = pVarNew->value;
                bIn = true;
                break;
            }
        }

        if(!bIn)
            rtVarsOld.append(new RuntimeViewVariable(*pVarNew));
    }

    if(!storage.Save(fileName, rtVarsOld, errStr))
    {
        errStrOut = QString("%1 file:error when save again").arg(fileName);
        WriteInfoLog("UpdateRuntimeViewVariableToFile:"+errStrOut);
        qDeleteAll(rtVarsOld);
        return false;
    }

    qDeleteAll(rtVarsOld);
    return true;
}

DBQUERYCONFIGURE_EXPORT bool RuntimeViewVariable_DeleteAllFile(const QString & viewPath)
{
    QMutexLocker locker(&localMutex);

    QDir dir(viewPath);
    if (!dir.exists())
    {
        WriteInfoLog("DeleteRuntimeViewVariableFiles:"+viewPath+" hasn't exist");
        return false;
    }

    //遍历文件名
    QStringList filters;
    filters << QString("*.%1").arg("json") ;
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files); // | QDir::NoDotAndDotDot

    QStringList fileNameList = dir.entryList();

    //判断是否为动态视图变量文件
    int size = fileNameList.size();
    QString fullName;

    for(int i=0; i<size;i++)
    {
        if(IsRuntimeViewVariableFile(fileNameList[i]))
        {
            if(!dir.remove(fileNameList[i]))
            {
                fullName.clear();
                fullName.append(viewPath).append("/").append(fileNameList[i]);
                WriteInfoLog("DeleteRuntimeViewVariableFiles:Delete file:"+fullName+" is fail");
                return false;
            }
        }
    }

    return true;
}

 static bool IsRuntimeViewVariableFile(const QString & strIn)
{
     //以"view_"开头，以"run_view_vars.json"结尾，才是动态视图变量文件.
     //前提假设:文件名strIn以.json结尾.
     QString str = strIn;
     int index = str.indexOf("view_", 0, Qt::CaseInsensitive); //文件名必须以view_t开始
     if(index != 0)
         return false;

     index = str.indexOf("run_view_vars.json");
     if(index<0)
         return false;

     index += strlen("run_view_vars.json");
     if(strIn.size() == index)
         return true;

     return false;
 }
}//end namespace
