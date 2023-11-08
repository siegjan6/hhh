#include "MDSwitchFactoryUI.h"
#include "MDSwitchTreeWindow.h"
#include "../../include/MDCommonHelper/MDCommonConstant.h"
#include "MDSwitchFactory.h"
#include <QObject>

CMDSwitchFactoryUI::CMDSwitchFactoryUI()
{
}

CMDSwitchFactoryUI::~CMDSwitchFactoryUI()
{
}

void CMDSwitchFactoryUI::configFactory(CMDSwitchFactory* f)
{
    QString title = QObject::tr("开关");
    if(f->para().type() == "FUNC")
        title = QObject::tr("功能开关");
    else if(f->para().type() == "LOG")
        title = QObject::tr("日志开关");
    else;

    QStringList sourceList;
    f->enumOnNames(sourceList);
    QList<void*> destList;
    CMDSwitchTreeWindow sw;
    sw.m_para.setAddObjOnSelect(false);
    sw.init(f);
    if(!sw.doModalSelectObjectUI(title, (qint16)MDObjectType::mdswitch, sourceList, destList))
        return;

    f->turnOff();
    foreach (void* ptr, destList)
    {
        CMDSwitch* sw = (CMDSwitch*)ptr;
        sw->turnOn();
    }
    f->save();
}
