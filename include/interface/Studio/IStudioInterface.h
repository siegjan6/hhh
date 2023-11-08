/*!
\author aGRw 2016.11.03
\section Studio接口
*/
#ifndef ISTUDIOINTERFACE_H
#define ISTUDIOINTERFACE_H

#include <QMdiArea>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class CBaseForm;
class CPropertyBrowser;
class CMDProjectConfiger;
class IHMIFramework;
class IMDRTDBManagerUI;

class IStudioInterface
{
public:
    virtual ~IStudioInterface(){}
/*!
\section property
*/
public:
    //多页面窗口容器
    virtual QMdiArea* mdiArea() = 0;
    //树控件
    virtual QTreeWidget* treeWidget() = 0;
    //属性框
    virtual CPropertyBrowser *propertyBrowser() = 0;
    virtual CMDProjectConfiger* projectConfiger() = 0;
    //modules
    virtual IMDRTDBManagerUI *rtdbManagerUI() = 0;
    virtual IHMIFramework* hmiFramework() = 0;
/*!
\section function
*/
public:
    //激活窗口
    virtual bool setActiveForm(CBaseForm *form) = 0;
    //打开窗口
    virtual void openMDIWindow(CBaseForm *form) = 0;
};

#endif // ISTUDIOINTERFACE_H



