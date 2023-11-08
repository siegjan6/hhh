/*!
\author enli 2015.01.08
\section 实时数据库配置接口
*/
#ifndef IMDRTDBMANAGERUI
#define IMDRTDBMANAGERUI

#include<QList>
#include<QStringList>

#include "interface/Studio/IStudioModule.h"

class IMDVariable;
class  CMDProjectConfiger;
class  IMDRTDBManagerUI:public  QObject, public IStudioModule
{
public:
    virtual ~IMDRTDBManagerUI(){}
public:
    //virtual void setProjectConfiger(CMDProjectConfiger *projectConfiger) = 0;
    //virtual void setMDIMoudleParam(IMDIMoudleParam *mdiMoudleParam) = 0;
    virtual bool selectVariables(QList<IMDVariable*>&varList) = 0;
    virtual bool selectVariables(QStringList &varNameList) = 0;
    //virtual void changeLanguage() = 0;
};

#endif // IMDRTDBMANAGERUI

