#include "MDSwitchFactory.h"
#include "MDSwitchFactoryImp.h"
#include <QList>
#include <QStringList>

CMDSwitch::CMDSwitch()
{
    m_imp = new CMDSwitchImp;
}

CMDSwitch::CMDSwitch(const CMDMObjPara& para, CMDMObjFactory* factory)
    : CMDMObj(para, factory)
{
    m_imp = new CMDSwitchImp;
}

CMDSwitch::~CMDSwitch()
{
    if(m_imp)
        delete m_imp;
}

bool CMDSwitch::isOn()
{
    return m_imp->m_isOn;
}

bool CMDSwitch::turnOn()
{
    m_imp->m_isOn = true;
    return m_imp->m_isOn;
}

bool CMDSwitch::turnOff()
{
    m_imp->m_isOn = false;
    return m_imp->m_isOn;
}

QString CMDSwitch::groupValue()
{
    return (isOn() ? "on" : "off");
}

CMDSwitchFactory::CMDSwitchFactory()
{
}

CMDSwitchFactory::CMDSwitchFactory(const CMDMObjPara& para)
    : CMDMObjFactory(para)
{

}

CMDSwitchFactory::~CMDSwitchFactory()
{
}

void CMDSwitchFactory::turnOff()
{
    QList<CMDMObj*> sList;
    enumAll(sList);

    foreach (CMDMObj* mobj, sList)
    {
        CMDSwitch* sw = (CMDSwitch*)mobj;
        sw->turnOff();
    }
}

void CMDSwitchFactory::turnOn()
{
    QList<CMDMObj*> sList;
    enumAll(sList);

    foreach (CMDMObj* mobj, sList)
    {
        CMDSwitch* sw = (CMDSwitch*)mobj;
        sw->turnOn();
    }
}

void CMDSwitchFactory::enumOn(QList<CMDSwitch*>& switchList)
{
    QList<CMDMObj*> sList;
    enumAll(sList);

    foreach (CMDMObj* mobj, sList)
    {
        CMDSwitch* sw = (CMDSwitch*)mobj;
        if(sw->isOn())
            switchList.append(sw);
    }
}

void CMDSwitchFactory::enumOnNames(QStringList& switchNameList)
{
    QList<CMDSwitch*> sourceList;
    enumOn(sourceList);
    foreach (CMDSwitch* sw, sourceList)
        switchNameList.append(sw->para().group());
}

void CMDSwitchFactory::enumOff(QList<CMDSwitch*>& switchList)
{
    QList<CMDMObj*> sList;
    enumAll(sList);

    foreach (CMDMObj* mobj, sList)
    {
        CMDSwitch* sw = (CMDSwitch*)mobj;
        if(!sw->isOn())
            switchList.append(sw);
    }
}

void CMDSwitchFactory::enumOffNames(QStringList& switchNameList)
{
    QList<CMDSwitch*> sourceList;
    enumOff(sourceList);
    foreach (CMDSwitch* sw, sourceList)
        switchNameList.append(sw->para().group());
}

static CMDSwitchFactory g_staticSwitchFactory;
CMDSwitch* CMDSwitchFactory::createSwitch(CMDMObjFactory* staticFactoryInstance, const CMDMObjPara& para, bool on)
{
    CMDMObjFactory* fc = (staticFactoryInstance ? staticFactoryInstance : &g_staticSwitchFactory);
    CMDSwitch* s = (CMDSwitch*)createMObj(fc, para);
    if(s)
    {
        bool isOn = on;

        CMDMObjFactory* f = s->factory();
        QString value = f->groupValue(s->para().group());
        if(!value.isEmpty())
        {
            isOn = (0 == value.compare("on", Qt::CaseInsensitive));
        }

        if(isOn)
            s->turnOn();
        else
            s->turnOff();
    }
    return s;
}

bool CMDSwitchFactory::deleteSwitch(CMDSwitch* s)
{
    return CMDMObjFactory::deleteMObj(s);
}

CMDMObj* CMDSwitchFactory::newMObj(const CMDMObjPara& para)
{
    CMDSwitch* s = new CMDSwitch(para, this);
    return s;
}

CMDMObjFactory* CMDSwitchFactory::newFactory(const CMDMObjPara& para)
{
    CMDSwitchFactory* f = new CMDSwitchFactory(para);
    f->load();
    return f;
}

QString CMDSwitchFactory::subPathForFactoryFile()
{
    return "switch";
}

