/*!
\author aGRw 2015.04.07
\section 图形框架类接口
*/
#ifndef IHMIFRAMEWORK_H
#define IHMIFRAMEWORK_H

#include "IFormOperation.h"
#include "../Studio/IStudioModule.h"

class IHMIStudio;
class IMDVariable;
class IMDProjectExpert;
class CMDProjectConfiger;

class IHMIFramework: public IStudioModule
{
public:
    virtual ~IHMIFramework(){}
/*!
\section property
*/
public:
    virtual IFormOperation* forms() = 0;
    virtual QString hmiPath() = 0;
    virtual IStudioInterface *studioProject() = 0;
    virtual void setProjectExpert(IMDProjectExpert* value) = 0;
    virtual IMDProjectExpert* projectExpert() = 0;
    virtual void setProjectConfiger(CMDProjectConfiger *value) = 0;
    virtual CMDProjectConfiger * projectConfiger() = 0;
    //!设置父窗体
    virtual void setParent(QMainWindow* parent) = 0;
    //窗体管理
    virtual QWidget* formManager() = 0;
/*!
\section framework
*/
public:
    //!是否是编辑模式
    virtual bool isStudioMode() = 0;
    //!框架管理类，只用于编辑版
    virtual IHMIStudio* manager() = 0;
    //!选择窗体对话框
    virtual bool openSelectHIMFormDialog(QStringList& forms) = 0;
//////////////////////////////////////////////////////////////////////////
//!run mode
/*!
\section var
*/
public:
    virtual void onDataChanged(IMDVariable* var) = 0;
    virtual void setVariableValue(const QString& name, const QVariant& value) = 0;
    virtual QVariant variableValue(const QString& name) = 0;
//////////////////////////////////////////////////////////////////////////
public:
    //!全屏
    virtual void enterFullScreen() = 0;
    //!退出全屏
    virtual void exitFullScreen() = 0;
    //!是否全屏
    virtual bool isFullScreen() = 0;
//////////////////////////////////////////////////////////////////////////
public:
    //!加载
    virtual void load() = 0;
    //!启动时全屏状态
    virtual bool isLaunchFullScreen() = 0;

public:
    virtual void moveForm(const QString &fullName, const QRect &rect) = 0;
    virtual void moveForm(const QString &fullName, const QPoint &point) = 0;
/*!
\section language
*/
public:
    virtual void changeLanguage() = 0;
    virtual void collectWords() = 0;
/*!
\section formScale
*/
public:
    virtual QPointF formScale() = 0;
/*!
\section image path
*/
public:
    //!图库图片目录
    virtual QString storageImagePath() = 0;
    //!项目图片目录
    virtual QString projectImagePath() = 0;
};

#endif // IHMIFRAMEWORK_H

