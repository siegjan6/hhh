/*!

\author dGFuZ3p3

\section 功能

对象属性主对话类的参数类，目的是尽量减少主对话类暴露的数据成员。
该类被 CMDObjectPropertyMainDialog 所使用。

*/

#ifndef MDOBJECTPROPERTYMAINDIALOGPARA_H
#define MDOBJECTPROPERTYMAINDIALOGPARA_H

#include "MDObjectManagerUI_Global.h"

class CMDObjectPropertyMainDialogParaImp;
class MDOBJECTMANAGERUISHARED_EXPORT CMDObjectPropertyMainDialogPara
{
    friend class CMDObjectPropertyMainDialog;
public:
    CMDObjectPropertyMainDialogPara();
    ~CMDObjectPropertyMainDialogPara();
public:
    void* configer();
    void* destProperty();
protected:
    CMDObjectPropertyMainDialogParaImp* m_imp;
};

#endif // MDOBJECTPROPERTYMAINDIALOGPARA_H
