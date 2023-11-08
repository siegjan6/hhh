/*!
\author enli 2015.01.08
\section 实时数据库运行接口
*/
#ifndef IMDRTDRUNTIMEUI
#define IMDRTDRUNTIMEUI

#include "IMDRTDBManager.h"
class CMDProjectConfiger;
class  IMDRTDBRuntimeUI
{
public:
    virtual ~IMDRTDBRuntimeUI(){}
public:
    virtual void setProjectConfiger(CMDProjectConfiger *projectConfiger) =0;
    virtual void openRTDBWindow(QWidget *parent)= 0;
    virtual void closeRTDBWindow()= 0;
    virtual void changeLanguage() = 0;
};
#endif // IMDRTDRUNTIMEUI

