#include "MDFuncSwitchFactory.h"

CMDFuncSwitchFactory::CMDFuncSwitchFactory()
{
}

CMDFuncSwitchFactory::CMDFuncSwitchFactory(const CMDMObjPara& para)
    : CMDSwitchFactory(para)
{

}

CMDFuncSwitchFactory::~CMDFuncSwitchFactory()
{
}

static CMDFuncSwitchFactory g_staticFuncSwitchFactory;
CMDSwitch* CMDFuncSwitchFactory::createSwitch(const QString& pid, const QString& group, const QString& comment)
{
    CMDMObjPara para(pid, "FUNC", group, comment);
    return CMDSwitchFactory::createSwitch(&g_staticFuncSwitchFactory, para);
}

CMDSwitch* CMDFuncSwitchFactory::findByGroup(const QString& pid, const QString& group)
{
    CMDMObjPara para(pid, "FUNC", group, "");
    CMDMObjFactory* f = CMDMObjFactory::findFactory(para);
    CMDMObj* mo = (f ? f->findByGroup(group) : NULL);
    return (CMDSwitch*)mo;
}
