/*!
\author enli 2015.01.08
\section 配置模式下，双击左边树结构的设备节点时创建或打开设备管理器
*/
#ifndef CMDRTDBDEVICESWINDOW_H
#define CMDRTDBDEVICESWINDOW_H

#include "IMDTreeItem.h"
#include "BaseForm.h"
class IMDRTDBManager;
class CMDRTDBDevicesWidget;
class CMDProjectConfiger;
class ITranslateWidgetTool;
class CMDRTDBManagerUI;
class CMDRTDBDevicesWindow : public CBaseForm
{
    Q_OBJECT
public:
    explicit CMDRTDBDevicesWindow(QWidget *parent = 0);
    ~CMDRTDBDevicesWindow();

public:
    //!toolbar
    QList<QToolBar*>* toolBars() const;

public:
    bool save();
    void closeForm();
    void setProjectConfiger(CMDProjectConfiger *projectConfiger);
    void bindDeviceTreeModel();
    void changeLanguage();
    void unBindDeviceTreeModel();//Y2hw
    void setRootTreeItem(CMDRTDBManagerUI* RTDBUI, const QString& devName);

    //!ManagerUI获取，刷新属性时需要
    CMDRTDBDevicesWidget * getDeviceWidget();

private:
    CMDRTDBDevicesWidget *m_devicesWidget ;
    ITranslateWidgetTool* m_translateTool;
    QList<QToolBar*>* m_toolBarList;
public slots://private->public UI也可以调用
    void on_Modified();
    void on_Run();
signals:
    void signal_Run();
};
#endif // CMDRTDBDEVICESWINDOW_H
