/*!

\author dGFuZ3p3

\section 功能

报警界面类的实现类，通过此类可访问【报警等级界面】和【报警条件界面】。

*/

#ifndef MDALARMUIIMP_H
#define MDALARMUIIMP_H

#include "MDAlarmLevelUI.h"
#include "MDAlarmConditionUI.h"
#include "MDAlarmConditionUIContacterImp.h"
#include "IStudioInterface.h"

class CMDAlarmUIImp
{
public:
    CMDAlarmUIImp();
    IStudioInterface* m_studioInterface;
    QTreeWidgetItem* m_rootInStudio;
public:
    CMDAlarmLevelUI m_levelUI;
public:
    CMDAlarmConditionUI m_conditionUI;
    CMDAlarmConditionUIContacterImp m_conditionContacter;
};

#endif // MDALARMUIIMP_H
