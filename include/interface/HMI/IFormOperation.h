/*!
\author aGRw 2015.04.07
\section 窗体文件操作接口
*/
#ifndef IFROMOPERATION_H
#define IFROMOPERATION_H

#include "QMainWindow"

class IFormOperation
{
public:
    virtual ~IFormOperation(){}
public:
    virtual QMainWindow *create(const QString& fullName) = 0;
    virtual bool save(QMainWindow* form) = 0;
    virtual void saveAll() = 0;
    virtual QMainWindow *open(const QString& fullName, int pwid = 0, int x = 0, int y = 0, int w = 0, int h = 0) = 0;
    virtual bool isOpened(const QString& fullName) = 0;
    virtual bool close(const QString& fullName, bool notSave = false) = 0;
    virtual bool closeAll() = 0;
    virtual bool rename(const QString& newFullName, const QString& oldFullName) = 0;
    virtual bool hide(const QString& fullName) = 0;
    virtual bool runFormScriptDynamically(const QString &fullName, const QString &script) = 0;
    virtual bool showByPid(int pid, int phwnd = 0) = 0;
    virtual bool hideByPid(int pid, int phwnd = 0) = 0;
    virtual bool closeByPid(int pid, int phwnd = 0) = 0;
};

#endif // IFROMOPERATION_H



