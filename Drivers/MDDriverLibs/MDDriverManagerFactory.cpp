/*!
\author enli 2015.01.08
\section 驱动管理器创建类厂
*/
#include<QEvent>
#include "MDDriverManagerFactory.h"
#include "MDDriverManager.h"
CMDDriverManagerFactory::CMDDriverManagerFactory()
{
}

CMDDriverManagerFactory::~CMDDriverManagerFactory()
{
}

IMDDriverManager *CMDDriverManagerFactory::createMDDriverManager()
{
    //注册数据块数据收发事件，进程只能注册一次
    (QEvent::Type)QEvent::registerEventType(QEvent::User+2000);
    CMDDriverManager *drvManager=new CMDDriverManager();
    return drvManager;
}
