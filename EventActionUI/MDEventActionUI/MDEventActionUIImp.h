/*!

\author dGFuZ3p3

\section 功能

事件和动作界面类的实现类，通过该类可以访问 【事件界面】 和 【动作界面】的实例。

*/

#ifndef MDEVENTACTIONUIIMP_H
#define MDEVENTACTIONUIIMP_H

#include "MDEventUIContacterImp.h"
#include "MDEventUI.h"

#include "MDActionUI.h"
#include "MDActionUIContacterImp.h"

class CMDEventActionUIImp
{
public:
    CMDEventActionUIImp();
    IStudioInterface* m_studioInterface;
    QTreeWidgetItem* m_rootInStudio;
public:
    CMDEventUI m_eventUI;
    CMDEventUIContacterImp m_eventUIContacter;
public:
    CMDActionUI m_actionUI;
    CMDActionUIContacterImp m_actionUIContacter;
};

#endif // MDEVENTACTIONUIIMP_H
