/*!

\author dGFuZ3p3

\section 功能

定义了几个运行参数界面类，便于利用对象管理框架模块。

CMDProjectParaDialog - 项目运行参数类 ；
CMDAlarmParaDialog - 报警运行参数类 ；
CMDHMIParaDialog - HMI 运行参数类 。

*/

#ifndef MDRUNTIMEPARADIALOG_H
#define MDRUNTIMEPARADIALOG_H

#include "MDObjectPropertyDialog.h"

namespace Ui {
class CMDRuntimeParaDialog;
}

/*!
 * \brief 运行参数界面基类
 */
class CMDRuntimeParaDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDRuntimeParaDialog(QWidget *parent = 0);
    ~CMDRuntimeParaDialog();

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
protected:
    Ui::CMDRuntimeParaDialog *ui;
};

class CMDProjectParaForm;
class CMDProjectConfiger;
/*!
 * \brief 项目运行参数类
 */
class CMDProjectParaDialog : public CMDRuntimeParaDialog
{
    Q_OBJECT
public:
    explicit CMDProjectParaDialog(CMDProjectConfiger* configer, QWidget *parent = 0);
    ~CMDProjectParaDialog();

    CMDProjectParaForm* m_form ;// NULL; //! 实际完成功能的界面类

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
};

/*!
 * \brief OPC UA Server参数类
 */
class CMDOPCUAServerParaForm;
class CMDOPCUAServerParaDialog : public CMDRuntimeParaDialog
{
    Q_OBJECT
public:
    explicit CMDOPCUAServerParaDialog(CMDProjectConfiger* configer, QWidget *parent = 0);
    ~CMDOPCUAServerParaDialog();

    CMDOPCUAServerParaForm* m_form ;// NULL; //! 实际完成功能的界面类

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
};

/*!
 * \brief 报警运行参数类
 */
class CMDAlarmParaForm;
class CMDAlarmConfiger;
class CMDAlarmParaDialog : public CMDRuntimeParaDialog
{
    Q_OBJECT
public:
    explicit CMDAlarmParaDialog(CMDAlarmConfiger* configer, QWidget *parent = 0);
    ~CMDAlarmParaDialog();

    CMDAlarmParaForm* m_form ;// NULL; //! 实际完成功能的界面类

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
};

/*!
 * \brief HMI 运行参数类
 */
class IHMIWidget;
class IHMIFramework;
class CMDHMIParaDialog : public CMDRuntimeParaDialog
{
    Q_OBJECT
public:
    explicit CMDHMIParaDialog(IHMIFramework* HMIFramework, QWidget *parent = 0);
    ~CMDHMIParaDialog();

    IHMIWidget* m_form ;// NULL; //! 实际完成功能的界面类

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
};

#endif // MDRUNTIMEPARADIALOG_H
