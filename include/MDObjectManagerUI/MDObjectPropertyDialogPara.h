/*!

\author dGFuZ3p3

\section 功能

对象属性对话类的参数类，目的是尽量减少主对话类暴露的数据成员。
该类被 CMDObjectPropertyDialog 所使用。

*/

#ifndef MDOBJECTPROPERTYDIALOGPARA_H
#define MDOBJECTPROPERTYDIALOGPARA_H

#include "MDObjectManagerUI_Global.h"

class CMDObjectPropertyDialogParaImp;
class ITranslateWidgetTool;
class MDOBJECTMANAGERUISHARED_EXPORT CMDObjectPropertyDialogPara
{
    friend class CMDObjectPropertyDialog;
public:
    CMDObjectPropertyDialogPara();
    ~CMDObjectPropertyDialogPara();
public:
    void* objectUI();
    ITranslateWidgetTool* twTool();
protected:
    CMDObjectPropertyDialogParaImp* m_imp;
};

#endif // MDOBJECTPROPERTYDIALOGPARA_H
