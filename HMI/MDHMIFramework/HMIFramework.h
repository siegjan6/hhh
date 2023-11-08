/*!
\author aGRw 2014.11.12
\section 窗体管理类
*/
#ifndef HMIFRAMEWORK_H
#define HMIFRAMEWORK_H

#include <QString>
#include <QTimer>
#include <QMutex>
#include <QMdiArea>

#include "IHMIFramework.h"
#include "MDRTDBInterfaces/IMDVariable.h"
#include "IFormOperation.h"
#include "MDProjectExpert/IMDProjectExpert.h"
#include "MDProjectConfiger.h"
#include "LaunchConfig.h"
#include "ILanguageObject.h"
#include "IDictionaryManager.h"
#include "Studio/IStudioInterface.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"
#include "FormTreeWidget.h"

class CHMIStudio;
class IHMIStudio;
class CFormOperation;
class CFormTreeWidget;

class CHMIFramework : public QObject, public IHMIFramework
{
    Q_OBJECT
public:
    CHMIFramework(CHMIStudio* manager = NULL);
    virtual ~CHMIFramework();
/*!
\section property
*/
public:
    IFormOperation* forms();
    QString hmiPath(){return m_hmiPath;}

    IStudioInterface* studioProject(){return m_studioProject;}
    void setProjectConfiger(CMDProjectConfiger *value);
    //仅在Run调用
    CMDProjectConfiger * projectConfiger(){return m_projectConfiger;}
    void setProjectExpert(IMDProjectExpert* value) {m_projectExpert = value;}
    IMDProjectExpert* projectExpert() {return m_projectExpert;}
    void setParent(QMainWindow* parent);
    QMdiArea* mdiArea(){return m_mdiArea;}
    CLaunchConfig* launchConfig(){return &m_launchConfig;}
    bool isLaunchFullScreen(){return m_launchConfig.launchFullScreen();}
    QWidget* formManager(){return &m_formManager;}
private:
    CFormOperation* m_forms;
    //!图形目录
    QString m_hmiPath;
    //!run environment
    IMDProjectExpert* m_projectExpert;
    //!目录管理器
    CMDProjectConfiger* m_projectConfiger;
    QMdiArea* m_mdiArea;
    CLaunchConfig m_launchConfig;
    CFormTreeWidget m_formManager;
/*!
\section public function
*/
public:
    //加载
    void load();
    void unload();
/*!
\section private function
*/
private:
    void init();
    void clear();
/*!
\section framework
*/
public:
    bool isStudioMode(){return m_isStudioMode;}
    IHMIStudio* manager();
    bool openSelectHIMFormDialog(QStringList& forms);
private:
    bool m_isStudioMode;
    CHMIStudio* m_manager;
//////////////////////////////////////////////////////////////////////////
//run mode
/*!
\section var
*/
public:
    void onDataChanged(IMDVariable* var);
private:
    //!定时刷新
    void timeRefresh();
private:
    QTimer m_timerRefresh;
public:
    //!变量值读写
    void setVariableValue(const QString& name, const QVariant& value);
    QVariant variableValue(const QString& name);
/*!
\section fullScreen
*/
public:
    void enterFullScreen();
    void exitFullScreen();
    bool isFullScreen(){return m_isFullScreen;}
private:
    bool m_isFullScreen;//!全屏,运行状态信息
//////////////////////////////////////////////////////////////////////////

/*!
\section notify
*/
private:
    //!检查通知事件
    void checkNotify();
private:
    //!日志最后刷新时间
    QDateTime m_lastLogTime;
    //!报警最后刷新时间
    QDateTime m_lastAlarmTime;

public:
    void moveForm(const QString& fullName, const QRect& rect);
    void moveForm(const QString& fullName, const QPoint& point);
/*!
\section language
*/
public:
    void changeLanguage();
    void collectWords();
/*!
\section form scale
*/
public:
    void setFormScale(const QPointF& value){m_formScale = value;}
    QPointF formScale(){return m_formScale;}
private:
    QPointF m_formScale;
/*!
\section image path
*/
public:
    QString storageImagePath();
    QString projectImagePath(){return m_projectImagePath;}
private:
    QString m_projectImagePath;//!项目图片目录
private:
    void initProjectImagePath();
/*!
\section test
*/
public:
    void setIsStudioMode(bool value){m_isStudioMode = value;}
/*!
\section studio module
*/
public:
    QString rootName();
    QIcon* rootIcon();
    QTreeWidgetItem* rootItem(){return m_rootItem;}
    void setRootItem(QTreeWidgetItem *value){m_rootItem = value;}
    void setStudioInterface(IStudioInterface* value){m_studioProject = value;}
private:
    QIcon* m_rootIcon;
    QTreeWidgetItem* m_rootItem;
    IStudioInterface* m_studioProject;
/*!
\section log
*/
public:
    void log(const QString& message);
private:
    void initLog();
private:
    CMDLogSwitch* m_log;
};


#endif // HMIFRAMEWORK_H





