/*!

\author dGFuZ3p3

\section 功能

安全策略界面类的实现类，通过此类可访问【帐户界面】和【权限组界面】。

*/

#ifndef MDSECURITYPOLICYUIIMP_H
#define MDSECURITYPOLICYUIIMP_H

#include <QObject>
#include <QtWidgets/qtreewidget.h>
#include "IStudioInterface.h"

class CMDAccountTreeWindow;
class CMDSecurityPolicyConfiger;
class CMDSecurityPolicyExpert;
class CMDActionSPSession;
class CMDSecurityPolicyUIImp : public QObject
{
    Q_OBJECT
public:
    CMDSecurityPolicyUIImp(QObject *parent = 0);
    ~CMDSecurityPolicyUIImp();
/*!
\section 初始化
*/
public:
    void load();
    void unLoad();
    void setConfiger(CMDSecurityPolicyConfiger* configer);
   void setSecurityPolicyExpert(CMDSecurityPolicyExpert* spExpert);
    IStudioInterface* m_studioInterface;
    QTreeWidgetItem* m_rootInStudio;
public:
    CMDSecurityPolicyConfiger* m_configer ;// NULL;
    CMDSecurityPolicyExpert* m_spExpert ;// NULL;

/*!
\section 与开发器交互；通过树控件管理对象
*/
public:
    void onMouseDbClick(QTreeWidgetItem *pItem,int column);
protected:
    QTreeWidget *treeWidget();
    QTreeWidgetItem *treeRoot();
    void buildTree(QTreeWidgetItem *pTreeRoot);

/*!
\section 显示登录界面、安全验证界面；帐户和权限组管理界面
*/
public:
    bool login();
    bool checkSP(CMDActionSPSession* spSession);
    bool showMgrDialog();

/*!
\section 多语言
*/
public:
   void changeLanguage();
};

#endif // MDSECURITYPOLICYUIIMP_H
