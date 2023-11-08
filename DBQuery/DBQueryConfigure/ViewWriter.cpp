#include "MDDBQueryConfigure.h"
#include "XMLViewStorage.h"
#include "JsonViewStorage.h"
#include "utils.h"
#include <QMutex>
#include <QFile>

static QMutex localMutex; //读写文件时 加锁 防止并发时出现竞态

namespace DBQ
{
//函数功能:从XML文件中加载视图
DBQUERYCONFIGURE_EXPORT bool View_Load(QString fileName, FILE_TYPE fileType, View ** ppViewOut, QString & errOut)
{
    QMutexLocker locker(&localMutex);

    if(FILE_TYPE_XML == fileType)
    {
        XMLViewStorage storage;
        return storage.Load(fileName, ppViewOut, errOut);
    }else if(FILE_TYPE_JSON == fileType)
    {
        JsonViewStorage storage;
        return storage.Load(fileName, ppViewOut, errOut);
    }
    WriteInfoLog("LoadView:type error");
    return false;
}

DBQUERYCONFIGURE_EXPORT bool View_LoadFromDir(const QString & viewPath, const QString & viewName, View ** ppViewOut, QString & errStrOut)
{
    QString fullFileName(viewPath);
    fullFileName.append("/view_").append(viewName).append(".json");

    return View_Load(fullFileName, FILE_TYPE_JSON, ppViewOut, errStrOut);
}

DBQUERYCONFIGURE_EXPORT bool View_WriteToDir(const QString & viewPath, const QString & viewName, View * pViewIn, QString & errStrOut)
{
    QString fullFileName(viewPath);
    fullFileName.append("/view_").append(viewName).append(".json");

    return View_Write(fullFileName, FILE_TYPE_JSON, pViewIn, errStrOut);
}

DBQUERYCONFIGURE_EXPORT bool View_Delete(const QString & viewPath, const QString & viewName)
{
    QMutexLocker locker(&localMutex);
    QString fullFileName(viewPath);
    fullFileName.append("/view_").append(viewName).append(".json");

    QFile file(fullFileName);
    if(!file.exists()) //不存在 此文件 则不用删除
        return true;

    return QFile::remove(fullFileName);
}

DBQUERYCONFIGURE_EXPORT bool View_Rename(const QString & viewPath, const QString & viewNameOld, const QString & viewNameNew)
{
    if(viewNameNew.isEmpty())
        return false;

    View * pView = 0;
    QString errStr;
    if(!View_LoadFromDir(viewPath, viewNameOld, &pView, errStr))
        return false;
    pView->viewName = viewNameNew;

    //重命名
    if(!View_WriteToDir(viewPath, viewNameNew, pView, errStr))
    {
        delete pView;
        return false;
    }

    delete pView;
    View_Delete(viewPath, viewNameOld);
    return true;
}

//函数功能:从视图文件中读取字段链表
DBQUERYCONFIGURE_EXPORT QList<DBQ::ViewField *> View_GetAllViewFieldsInViewFromDir(const QString & viewPath, const QString & viewName, QString & errStrOut)
{
    QList<DBQ::ViewField *> viewFields;
    DBQ::View *view = NULL;
    if(DBQ::View_LoadFromDir(viewPath,viewName,&view,errStrOut))
    {
        if(view)
        {
            foreach (DBQ::ViewField * pField, view->fields)
            {
                DBQ::ViewField * pFieldCp = new DBQ::ViewField;
                *pFieldCp = * pField;
                viewFields.append(pFieldCp);
            }
            delete view;
        }
        else
            WriteInfoLog("GetAllViewFieldsInViewFromDir:view is null");
    }
    else
        WriteInfoLog("GetAllViewFieldsInViewFromDir:fail");

    return viewFields;
}

DBQUERYCONFIGURE_EXPORT bool View_Write(QString fileName, FILE_TYPE fileType, View * pViewIn, QString & errStrOut)
{
    QMutexLocker locker(&localMutex);

    if(FILE_TYPE_XML == fileType)
    {
        XMLViewStorage storage;
        return storage.Save(fileName, pViewIn, errStrOut);
    }else if(FILE_TYPE_JSON == fileType)
    {
        JsonViewStorage storage;
        return storage.Save(fileName, pViewIn, errStrOut);
    }
    WriteInfoLog("WriteViewToFile:type error");
    return false;
}

}
