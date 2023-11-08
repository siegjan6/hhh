#ifndef VIEWVARMGR_2016_11_01_H
#define VIEWVARMGR_2016_11_01_H
#include <MDDBQueryConfigure.h>
#include <QMap>
#include <QString>

namespace DBQE
{
typedef QMap<QString, DBQ::ViewVariable *> ViewVarsMap;

class ViewVarMgr
{
public:
    //函数功能:初始化全局视图变量.在模块初始化时调用.只能调用一次.
    bool Init(const QString & viewVarsFileName);
    bool FindValue(const QString & key, QString & valueOut);
    DBQ::ViewVariable * Find(const QString & key);
private:
    //函数功能:检查视图变量合法性
    bool CheckViewVariable(DBQ::ViewVariable * pViewVar);
private:
    ViewVarsMap m_ViewVarsMap;
};
}
#endif // VIEWVARMGR_2016_11_01_H
