#include "MDActionSPUIImp.h"
#include "MDPowerGroupTreeWindowForActionSPUI.h"
#include "../../include/MDCommonHelper/MDCommonConstant.h"
#include "MDActionSPDialog.h"
#include "MDActionSPListDialog.h"
#include "MDActionSPListDialogParaImp.h"
#include "MDActionSPDialogAsModal.h"
#include "MDStringResource.h"

CMDActionSPUIImp::CMDActionSPUIImp()
{
    m_pgConfiger = NULL;
}

CMDActionSPUIImp::~CMDActionSPUIImp()
{
}

/*!
\section 初始化
*/
bool CMDActionSPUIImp::init(CMDPowerGroupConfiger* configer)
{
    m_pgConfiger = configer;
    return true;
}

/*!
\section 显示模态界面
*/
bool CMDActionSPUIImp::doModalActionSPDialog(CMDActionSP *sourceProperty)
{
    CMDActionSPDialog dlg;
    dlg.initial(m_pgConfiger, sourceProperty);
    dlg.exec();
    bool result = false;
    if(QDialog::Accepted == dlg.result())
    {
        result = true;
    }
    return result;
}

bool CMDActionSPUIImp::doModalActionSPListDialog(CMDActionSPList& sourceList, CMDActionSPList& destList)
{
    CMDActionSPDialogAsModal dlg;
    dlg.setPowerGroupConfiger(m_pgConfiger);
    dlg.setDestList(&sourceList);
    dlg.initial(this, NULL);
    dlg.exec();
    bool result = false;
    if(QDialog::Accepted == dlg.result())
    {
        destList = *dlg.destList();
        result = true;
    }
    return result;
}

/*!
\section 显示选择权限组界面
*/
bool CMDActionSPUIImp::selectPowerGroups(CMDPowerGroupConfiger* configer, const QStringList& sourceList, QStringList& destList)
{
    CMDPowerGroupTreeWindowForActionSPUI win;
    win.init(configer);
    return win.doModalSelectObjectUI(STRANS(STRING_SELECT_POWER_GROUP), (qint16)MDObjectType::powerGroup, sourceList, destList);
}

bool CMDActionSPUIImp::selectPowerGroups(const QStringList& sourceList, QStringList& destList)
{
    return selectPowerGroups(m_pgConfiger, sourceList, destList);
}
