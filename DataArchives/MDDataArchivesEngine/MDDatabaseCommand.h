#ifndef CMDDATABASE_COMMAND_2016_07_28_H
#define CMDDATABASE_COMMAND_2016_07_28_H
#include <MDCommandQueueEngine/MDCommandQueueEngine.h>
//linux下,#include <MDCommandQueueEngine\MDCommandQueueEngine.h>,就会找不到文件

//TODO:修改命令体系 形成继承关系
class CMDDatabaseCommand : public CMDCommand
{
public:
    virtual void Do() = 0;
    /*
    virtual ~CMDDatabaseCommand()
    {
        CMDNotifyParamCMD* pNotifyCMD = reinterpret_cast<CMDNotifyParamCMD*>(cmdPara());
        if (pNotifyCMD)
        {
            delete pNotifyCMD;
            pNotifyCMD = NULL;
        }
    }
    */
};

#endif // CNDSDBCHECKCONNECTOINCMD_H
