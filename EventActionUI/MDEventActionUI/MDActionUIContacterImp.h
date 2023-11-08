/*!

\author dGFuZ3p3

\section 功能

CMDActionUIContacter 的派生类，实现了 【动作界面】 对外界的访问。

*/

#ifndef MDACTIONUICONTACTERIMP_H
#define MDACTIONUICONTACTERIMP_H

#include "MDActionUI.h"

class CMDEventActionUIImp;
class CMDActionUIContacterImp : public CMDActionUIContacter
{
public:
    CMDActionUIContacterImp();
public:
    CMDEventActionUIImp *m_uiImp ;// NULL;
protected:
    bool onSelectSingleObject(MDActionType type, const QString& source, QString& dest);
};

#endif // MDACTIONUICONTACTERIMP_H
