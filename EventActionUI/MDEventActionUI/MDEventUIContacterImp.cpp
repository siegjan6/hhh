#include "MDEventUIContacterImp.h"
#include "MDActionUI.h"
#include "MDEventActionUIImp.h"
#include "../../include/MDRTDBManagerUI/IMDRTDBManagerUI.h"

CMDEventUIContacterImp::CMDEventUIContacterImp()
{
    m_uiImp = NULL;
}

bool CMDEventUIContacterImp::onSelectActions(const QStringList& sourceList, QStringList& destList)
{
    return (m_uiImp ? m_uiImp->m_actionUI.selectActions(sourceList, destList) : false);
}

bool CMDEventUIContacterImp::onSelectSingleObject(MDEventType type, const QString& source, QString& dest)
{
    bool result = false;

    switch (type) {
    case MDEventType::combined:
    {
    }break;

    case MDEventType::variable_ValueChanged:
    case MDEventType::variable_ValueChangedInRange:
    case MDEventType::variable_QualityChanged:
    case MDEventType::variable_ValueRead:
    case MDEventType::variable_ValueWrite:
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

    case MDEventType::window_Opened:
    case MDEventType::window_Closed:
    {

    }break;

    default:
        break;
    }

    return result;
}
