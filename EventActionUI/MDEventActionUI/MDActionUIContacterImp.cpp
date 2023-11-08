#include "MDActionUIContacterImp.h"
#include "MDEventActionUIImp.h"
#include "../../include/MDRTDBManagerUI/IMDRTDBManagerUI.h"
#include "MDSelectHistoryGroupDialog.h"
#include "../../include/interface/HMI/IHMIFramework.h"

CMDActionUIContacterImp::CMDActionUIContacterImp()
{
    m_uiImp = NULL;
}

bool CMDActionUIContacterImp::onSelectSingleObject(MDActionType type, const QString& source, QString& dest)
{
    bool result = false;

    switch (type) {

    case MDActionType::variable_WriteValue:
    case MDActionType::variable_ReadValue:
    {
        QStringList destList;
        if(m_uiImp)
        {
            m_uiImp->m_studioInterface->rtdbManagerUI()->selectVariables(destList);
            if(!destList.isEmpty())
            {
                dest = destList.first();
                result = true;
            }
        }
    }break;

    case MDActionType::window_Open:
    case MDActionType::window_Close:
    case MDActionType::window_Move:
    case MDActionType::window_Hide:
    {
        if(m_uiImp)
        {
            QStringList forms;
            forms.append(source);
            m_uiImp->m_studioInterface->hmiFramework()->openSelectHIMFormDialog(forms);
            if(!forms.isEmpty())
            {
                dest = forms.first();
                result = true;
            }
        }
    }break;

    case MDActionType::historyGroup_Save:
    {
        CMDSelectHistoryGroupDialog dlg;
        dlg.initial(m_uiImp->m_studioInterface->projectConfiger());
        if(QDialog::Accepted == dlg.exec())
        {
            dest = dlg.m_selectedGroup;
            result = true;
        }
    }break;

    default:
        break;
    }

    return result;
}
