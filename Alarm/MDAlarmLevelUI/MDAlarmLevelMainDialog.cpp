#include "MDAlarmLevelMainDialog.h"

#include "MDAlarmLevel.h"
#include "MDAlarmLevelConfiger.h"
#include "MDAlarmConfiger.h"

#include "MDAlarmLevelGeneralDialog.h"
#include "MDAlarmLevelColorDialog.h"

#include "MDStringResource.h"

CMDAlarmLevelMainDialog::CMDAlarmLevelMainDialog(QWidget *parent) :
    CMDObjectPropertyMainDialog(parent)
{
    setWindowTitle(STRANS(STRING_LEVEL_TEXT));
    m_pageID = -1;
}

CMDAlarmLevelMainDialog::~CMDAlarmLevelMainDialog()
{
}

void CMDAlarmLevelMainDialog::setTheOnlyPage(int pageID)
{
    m_pageID = pageID;
}

void* CMDAlarmLevelMainDialog::newProperty(qint16 subType, void* sourceProperty)
{
    CMDAlarmLevel* newProp = NULL;
    if(!sourceProperty)
        newProp = new CMDAlarmLevel();
    else
        newProp = new CMDAlarmLevel(*((CMDAlarmLevel*)sourceProperty));
    return newProp;
}

void CMDAlarmLevelMainDialog::initPropertyManagerUI(QListWidget* listWidget, QStackedWidget* stackWidget, void* sourceProperty, void* destProperty)
{
    if(1 == m_pageID)
    {
        listWidget->insertItem(0, STRING_PROP_COLOR);

        CMDAlarmLevelColorDialog* dlg = new CMDAlarmLevelColorDialog;
        dlg->m_alarmConfiger = (CMDAlarmConfiger*)m_para.configer();
        stackWidget->addWidget(dlg);
    }
    else
    {
        listWidget->insertItem(0, STRING_PROP_COMMON);
        listWidget->insertItem(1, STRING_PROP_COLOR);

        stackWidget->addWidget(new CMDAlarmLevelGeneralDialog());
        CMDAlarmLevelColorDialog* dlg = new CMDAlarmLevelColorDialog;
        dlg->m_alarmConfiger = (CMDAlarmConfiger*)m_para.configer();
        stackWidget->addWidget(dlg);
    }
}

bool CMDAlarmLevelMainDialog::modifyProperty(void* configer, void* sourceProperty, void* destProperty, QString& resultMessage)
{
    if(!configer)
        return false;

    if(!sourceProperty)
        return false;

    CMDAlarmLevelConfiger* alConfiger = ((CMDAlarmConfiger*)configer)->levelConfiger();
    return alConfiger->modify((CMDAlarmLevel*)sourceProperty, (CMDAlarmLevel*)destProperty);
}

bool CMDAlarmLevelMainDialog::addProperty(void* configer, void* destProperty, QString& resultMessage)
{
    if(!configer)
        return false;

    CMDAlarmLevelConfiger* alConfiger = ((CMDAlarmConfiger*)configer)->levelConfiger();
    return alConfiger->add((CMDAlarmLevel*)destProperty);
}
