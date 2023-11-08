/*!

\author dGFuZ3p3

\section 功能 - 单实例运行控制类，具体功能如下：
、实现应用的单实例运行；
、通过客户端类可发送数据到其他派生于 CMDSingleApplication 的应用。

\section 用法
、如果只是控制单实例运行，可直接使用 CMDSingleApplication，
    且必须调用 setAppMainWnd；
、如果已经有一个实例在运行，尝试运行多个实例的操作会
    将第一个实例的主界面显示出来，要改变此缺省行为，
    必须重载  onResumeSingleApp，并返回 true；
、派生于 CMDSingleApplication 的应用都具有一个应用标识，缺省标识是
    应用程序名 < QFileInfo(QCoreApplication::applicationFilePath()).fileName(); >，
    也可自行制定标识，参考 setAppServerName；
、如果希望控制其他派生于 CMDSingleApplication 的应用，则必须使用
    CMDSingleAppClient 类，并通过 init 指定想要控制的应用标识，该标识
    必须与应用的标识一致；
、使用 sendToServer 发送数据给应用，应用要处理数据，则必须重载
    onClientDataReceived。

\section 示例代码
\code
int main(int argc, char *argv[])
{
    CMDSingleApplication app(argc, argv);   // 使用 CMDSingleApplication 或其派生类
    if(app.run())
    {
        MainWindow w;
        app.setAppMainWnd(&w);  // 必须指定应用主窗口
        w.showMaximized();

        return app.exec();
    }
    return 0;
}
\endcode

\section 修改历史
2015.7.10 / dGFuZ3p3 / 创建；
2015.7.17 / dGFuZ3p3 / 功能完成并写注释。
*/

#ifndef MDSINGLEAPPLICATION_H
#define MDSINGLEAPPLICATION_H

#include "MDSingleApplication_Gobal.h"
#include <QApplication>

class CMDSingleApplicationImp;
/*!
 * \brief 单实例运行控制类
 */
class MDSINGLEAPPLICATIONSHARED_EXPORT CMDSingleApplication : public QApplication
{
        Q_OBJECT
public:
    CMDSingleApplication(int &argc, char **argv);
    virtual ~CMDSingleApplication();
/*!
\section 运行单实例
\note 派生于 CMDSingleApplication 的应用都具有一个应用标识，缺省标识是
应用程序名 < QFileInfo(QCoreApplication::applicationFilePath()).fileName(); >，
要改变此缺省值，可在调用 run 时指定，或在调用 run 之前调用 setAppServerName。
*/
public:
    bool run(const QString& appServerName = "");
public:
    void setAppServerName(const QString& name); //! 在 run 之前调用有效
    QString appServerName();

/*!
\section 管理应用主窗口
*/
public:
     void setAppMainWnd(QWidget *appMainWindow); //! run 之后必须调用
     QWidget *appMainWindow();
public:
    void activateAppMainWindow();            //! 激活主窗口

/*!
\section 可重载接口
、 onResumeSingleApp -
    如果已经有一个实例在运行，尝试运行多个实例的操作会
    将第一个实例的主界面显示出来，要改变此缺省行为，
    必须重载  onResumeSingleApp，并返回 true；
、 onClientDataReceived - 需要处理客户端数据时重载此接口。
*/
public:
    virtual bool onResumeSingleApp();
    virtual void onClientDataReceived(const QString& data);
protected:
    CMDSingleApplicationImp* m_imp;
};

class CMDSingleAppClientImp;
/*!
 * \brief 单实例运行控制类的客户端类
如果希望控制其他派生于 CMDSingleApplication 的应用，则必须使用
CMDSingleAppClient 类，并通过 init 指定想要控制的应用标识，该标识
必须与应用的标识一致
 */
class MDSINGLEAPPLICATIONSHARED_EXPORT CMDSingleAppClient
{
public:
    CMDSingleAppClient();
    ~CMDSingleAppClient();
public:
    void init(const QString& appServerName); //! appServerName 必须与应用的标识一致
    void release();
public:
    bool sendToServer(const QString& data);
protected:
    CMDSingleAppClientImp* m_imp;
};

#endif // MDSINGLEAPPLICATION_H
