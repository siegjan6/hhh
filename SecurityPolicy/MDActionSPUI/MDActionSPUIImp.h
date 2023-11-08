/*!

\author dGFuZ3p3

\section 功能

动作安全策略配置界面类的实现类。

*/

#ifndef MDACTIONSPUIIMP_H
#define MDACTIONSPUIIMP_H

#include <QString>
#include "MDActionSP.h"

class CMDActionSP;
class CMDActionSPListDialog;
class CMDPowerGroupConfiger;
class CMDActionSPUIImp
{
public:
    CMDActionSPUIImp();
    ~CMDActionSPUIImp();
/*!
\section 初始化
*/
public:
    bool init(CMDPowerGroupConfiger* configer);
protected:
    CMDPowerGroupConfiger* m_pgConfiger;

/*!
\section 显示模态界面
*/
public:
   bool doModalActionSPDialog(CMDActionSP *sourceProperty);
public:
    bool doModalActionSPListDialog(CMDActionSPList& sourceList, CMDActionSPList& destList);

/*!
\section 显示选择权限组界面
*/
public:
    static bool selectPowerGroups(CMDPowerGroupConfiger* configer, const QStringList& sourceList, QStringList& destList);
    bool selectPowerGroups(const QStringList& sourceList, QStringList& destList);
};

#endif // MDACTIONSPUIIMP_H
