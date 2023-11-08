/*!

\author dGFuZ3p3

\section 功能

对象属性对话类的参数类的实现类。

*/

#ifndef MDOBJECTPROPERTYDIALOGPARAIMP_H
#define MDOBJECTPROPERTYDIALOGPARAIMP_H

class ITranslateWidgetTool;
class CMDObjectPropertyDialogParaImp
{
public:
    CMDObjectPropertyDialogParaImp();
public:
    void* m_objectUI;
    ITranslateWidgetTool* m_twTool;
};

#endif // MDOBJECTPROPERTYDIALOGPARAIMP_H
