/*!

\author dGFuZ3p3

\section 功能

管理对象树的窗口类的参数类，目的是尽量减少窗口类暴露的数据成员。
该类被 CMDObjectTreeWindow 所使用。

*/

#ifndef MDOBJECTTREEWINDOWPARA_H
#define MDOBJECTTREEWINDOWPARA_H

#include "MDObjectManagerUI_Global.h"

class CMDObjectTreeWindowParaImp;
class MDOBJECTMANAGERUISHARED_EXPORT CMDObjectTreeWindowPara
{
    friend class CMDObjectTreeWindow;
public:
    CMDObjectTreeWindowPara();
    ~CMDObjectTreeWindowPara();
    void setAddObjOnSelect(bool enable);
    bool isAddObjOnSelect();
protected:
    CMDObjectTreeWindowParaImp* m_imp;
};

#endif // MDOBJECTTREEWINDOWPARA_H
