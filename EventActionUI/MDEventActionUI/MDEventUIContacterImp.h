/*!

\author dGFuZ3p3

\section 功能

CMDEventUIContacter 的派生类，实现了 【事件界面】 对外界的访问。

*/

#ifndef MDEVENTUICONTACTERIMP_H
#define MDEVENTUICONTACTERIMP_H
#include "MDEventUI.h"

class CMDEventActionUIImp;
class CMDEventUIContacterImp : public CMDEventUIContacter
{
public:
    CMDEventUIContacterImp();
public:
    CMDEventActionUIImp *m_uiImp ;// NULL;
protected:
    bool onSelectActions(const QStringList& sourceList, QStringList& destList);
    bool onSelectSingleObject(MDEventType type, const QString& source, QString& dest);
};

#endif // MDEVENTUICONTACTERIMP_H
