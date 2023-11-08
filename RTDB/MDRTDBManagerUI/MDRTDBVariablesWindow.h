/*!
\author enli 2015.01.08
\section 配置模式下，双击左边树结构的变量节点时创建或打开变量管理器
*/
#ifndef CMDRTDBVARIABLESWINDOW_H
#define CMDRTDBVARIABLESWINDOW_H

#include "IMDTreeItem.h"
#include "BaseForm.h"
class IMDRTDBManager;
class CMDRTDBVariablesWidget;
class CMDProjectConfiger;
class ITranslateWidgetTool;
class CMDRTDBManagerUI;
class CMDRTDBVariablesWindow : public CBaseForm
{
    Q_OBJECT
public:
    explicit CMDRTDBVariablesWindow(QWidget *parent = 0);
    ~CMDRTDBVariablesWindow();
/*!
\section 接口
*/
public:
    bool save();
    //!toolbar
    QList<QToolBar*>* toolBars() const;

    void setProjectConfiger(CMDProjectConfiger *projectConfiger);
    void bindVariableTreeModel();
    void changeLanguage();//Y2hw
    void unBindVariableTreeModel();//Y2hw
    void setRootTreeItem(CMDRTDBManagerUI* RTDBUI, const QString& groupName);
    //!ManagerUI获取，刷新属性时需要
    CMDRTDBVariablesWidget * getVariableWidget();
private:
    CMDRTDBVariablesWidget *m_variablesWidget ;
    //!多语言
    ITranslateWidgetTool* m_translateTool;
    QList<QToolBar*>* m_toolBarList;
public slots://private->public
    void on_Modified();
    void on_Run();
signals:
    void signal_Run();
};

#endif // CMDRTDBVARIABLESWINDOW_H
