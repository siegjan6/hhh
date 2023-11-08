/*!
\author aGRw 2014.11.12
\section 图形框架管理类,只在编辑版中使用s
*/
#ifndef IHMISTUDIO_H
#define IHMISTUDIO_H

#include "MDPropertyBrowser/PropertyBrowser.h"
#include "IHMIForm.h"
#include "IObjectChangedEvent.h"
#include "IGraphicsStorage.h"

class IHMIFramework;
class IHMIForm;

class IHMIStudio
{
public:
    virtual ~IHMIStudio(){}
/*!
\section property
*/
public:
    //!属性框
    virtual CPropertyBrowser* property() = 0;
    //!控件变化事件
    virtual IObjectChangedEvent* objectChangedEvent() = 0;
    //!控件列表框
    virtual QWidget* objectTreeDialog() = 0;
    //!图库
    virtual IGraphicsStorage* graphicsStorage() = 0;
    //!当前窗体
    virtual void setCurrentForm(IHMIForm* value) = 0;
    virtual IHMIForm* currentForm() = 0;
    //!工具箱
    virtual QWidget* toolKitDialog() = 0;
/*!
\section function
*/
public:
    //!设置属性框选择控件
    virtual void selectObject(QObject* obj) = 0;
    //!属性框刷新
    virtual void refreshProperty() = 0;
    //!设置控件树
    virtual void setObjectTree(IHMIForm* form) = 0;
    //!工具箱置位到指针
    virtual void resetToolKit() = 0;
    //!布局预览
    virtual void initFormPreview(IHMIFramework* framework) = 0;
    virtual QDialog* formPreviewDialog() = 0;
/*!
\section HMIFramework
*/
public:
    virtual IHMIFramework* open() = 0;
};

#endif // IHMISTUDIO_H

