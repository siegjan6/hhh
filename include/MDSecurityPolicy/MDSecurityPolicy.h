#ifndef MDSECURITYPOLICY_H
#define MDSECURITYPOLICY_H

#include <QtCore/qglobal.h>
#include <QVariant>

#if defined(SECURITYPOLICY_LIBRARY)
#  define SECURITYPOLICYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SECURITYPOLICYSHARED_EXPORT Q_DECL_IMPORT
#endif
//=================================================
class IProjectDirector;
class QTreeWidget;
class QTreeWidgetItem;
class IMDIMoudleParam;
class IMDSecurityPolicy;
//=================================================
enum class ACCOUNT_ERROR_TYPE
{
    // 未初始化
    ERROR_MOUDLE_NOT_INIT = 0,
    // 验证成功
    ERROR_SUCCESSED,
    // 非本地验证模式
    ERROR_NOT_LOCAL_MODE,
    // 帐户未找到
    ERROR_ACCOUNT_NOT_FOUND,
    // 错误的帐户密码
    ERROR_ACCOUNT_INVALID_PWD,
    // 帐户已禁用
    ERROR_ACCOUNT_DISABLE,
    // 帐户已锁定
    ERROR_ACCOUNT_LOCKED,
    // 帐户密码已过期
    ERROR_ACCOUNT_OVERDUE
};
//=================================================
// 编辑版调用
class CSecurityPolicy;
class SECURITYPOLICYSHARED_EXPORT CMDSecurityPolicyEditor
{

public:
    CMDSecurityPolicyEditor();
    virtual ~CMDSecurityPolicyEditor();
public:
    // 设置MDI窗口接口相关参数
    void setMDIMoudleParam(IMDIMoudleParam *pParam);
public:
    void load();
    void unLoad();
public:
    // 鼠标事件
    void onMouseRClick(QTreeWidgetItem *pItem ,const QPoint &pos);
    void onMouseLClick(QTreeWidgetItem *pItem,int column);
    void onMouseDbClick(QTreeWidgetItem *pItem,int column);
private:
    CSecurityPolicy *m_pSecurityPolicy;
};
//===========================================================
// 运行版调用
class SECURITYPOLICYSHARED_EXPORT CMDSecurityPolicyRun
{

public:
    CMDSecurityPolicyRun();
    virtual ~CMDSecurityPolicyRun();
public:
    // 设置项目指针
    void setProjectDirector(IProjectDirector *pProjectDirector);
public:
    bool checkOutSecurityPolicy(IMDSecurityPolicy *pSecurityPolicy,const QString &strDescription,ACCOUNT_ERROR_TYPE &dwError);
private:
    CSecurityPolicy *m_pSecurityPolicy;
};
#endif // MDSECURITYPOLICY_H
