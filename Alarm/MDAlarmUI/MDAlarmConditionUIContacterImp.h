/*!

\author dGFuZ3p3

\section 功能

报警条件界面与外界联系的类，通过此类可获取【报警等级】和【报警变量】。

*/

#ifndef MDALARMCONDITIONUICONTACTERIMP_H
#define MDALARMCONDITIONUICONTACTERIMP_H

#include "MDAlarmConditionUI.h"

class CMDAlarmUIImp;
class CMDAlarmConditionUIContacterImp : public CMDAlarmConditionUIContacter
{
public:
    CMDAlarmConditionUIContacterImp();
    CMDAlarmUIImp* m_uiImp ;// NULL;
public:
    virtual bool onSelectAlarmLevel(const QString& source, QString& dest);
    virtual bool onSelectVariable(const QString& source, QString& dest);
};

#endif // MDALARMCONDITIONUICONTACTERIMP_H
