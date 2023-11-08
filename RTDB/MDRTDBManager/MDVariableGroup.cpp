#include "MDVariableGroup.h"
#include "MDRTDBVariableUaBase.h"
#include "IMDTreeItem.h"
#include <QJsonArray>
#include <QJsonObject>


CMDVariableGroup::CMDVariableGroup()
{
    m_parentVariableGroup = NULL;
    m_name = "";
    m_localName= "";
    m_itemType = MDTreeItemType::VariableGroup;
    m_mdVariableManager.enableVarKeyIsLocalName(true);
    m_mdRTDBManager = NULL;
	m_uaFolder = NULL;
    binitSystemVar = false;
    m_systemVariableCheckCnt = -1;
}


CMDVariableGroup::~CMDVariableGroup()
{
    removeAllChildVariableGroup();
    removeAllVariable();
}
bool CMDVariableGroup::isSystemVariableGroup()
{
    if(m_localName=="SystemVariableGroup")
    {
        return true;
    }
    return false;
}
/*!
\section 系统变量组操作
*/
void  CMDVariableGroup::addSystemVariable(const QString &name,MDDataType dtType,const QString & description )
{
    QString error;
    CMDRTDBVariable *var = NULL;
    if(m_uaFolder)
        var = m_uaFolder->addChild<CMDRTDBVariable>(name);
    else
        //var = new CMDRTDBVariable(this->m_mdRTDBManager->uaServer());
        var = new CMDRTDBVariable();

    var->setLocalName(name);
    var->setDescription(description);
    var->setDataType(dtType);
    var->setOriginalDataType(dtType);
    var->setVariableType(MDVariableType::SystemVariable);
    var->initialze();
    if(!addVariable(var,error))
    {
        delete var;
    }
    else
    {
         if(var->localName() == "StartDateTime")
         {
            var->refreshData(QVariant(QDateTime::currentDateTime()),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
         }
    }
}

void  CMDVariableGroup::initialSystemVariableGroup()
{
      addSystemVariable("User",MDDataType::String,"用户");
      addSystemVariable("StartDateTime",MDDataType::DateTime,"项目启动日期与时间");
      addSystemVariable("UpDateTime",MDDataType::DateTime,"系统启动日期与时间");
      addSystemVariable("DateTime",MDDataType::DateTime,"系统日期与时间");
      addSystemVariable("Date",MDDataType::String,"系统日期");
      addSystemVariable("Time",MDDataType::String,"系统时间");
      addSystemVariable("Year",MDDataType::Int16,"系统日期--年");
      addSystemVariable("Month",MDDataType::Int16,"系统日期--月");
      addSystemVariable("Day",MDDataType::Int16,"系统日期--日");
      addSystemVariable("Hour",MDDataType::Int16,"系统时间--时");
      addSystemVariable("Minute",MDDataType::Int16,"系统时间--分");
      addSystemVariable("Second",MDDataType::Int16,"系统时间--秒");
      addSystemVariable("SystemName",MDDataType::String,"系统名称");
      addSystemVariable("ProjectName",MDDataType::String,"项目名称");
      addSystemVariable("ProjectPath",MDDataType::String,"项目路径");
      addSystemVariable("VHash",MDDataType::String,"变量身份标识");
      addSystemVariable("BatteryLowVoltage",MDDataType::Int16,"APC电池低电压");
      addSystemVariable("ApcLowVoltage",MDDataType::Int16,"APC电源低电压");
}
void  CMDVariableGroup::updateSystemVariable()
{
    IMDRTDBVariable *var =getVariable("StartDateTime");
    if(NULL!=var)
    {
        //if(binitSystemVar)
            //var->refreshData(QVariant(QDateTime::currentDateTime()),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
    }
    var =getVariable("DateTime");
    if(NULL!=var)
    {
        var->refreshData(QVariant(QDateTime::currentDateTime()),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
    }
    var =getVariable("Date");
    if(NULL!=var)
    {
        var->refreshData(QVariant(QDateTime::currentDateTime().date().toString("yyyy-MM-dd")),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
    }
    var =getVariable("Time");
    if(NULL!=var)
    {
        var->refreshData(QVariant(QDateTime::currentDateTime().time().toString("hh:mm:ss.zzz")),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
    }
    var =getVariable("Year");
    if(NULL!=var)
    {
        var->refreshData(QVariant(QDateTime::currentDateTime().date().year()),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
    }
    var =getVariable("Month");
    if(NULL!=var)
    {
        var->refreshData(QVariant(QDateTime::currentDateTime().date().month()),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
    }
    var =getVariable("Day");
    if(NULL!=var)
    {
        var->refreshData(QVariant(QDateTime::currentDateTime().date().day()),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
    }
    var =getVariable("Hour");
    if(NULL!=var)
    {
        var->refreshData(QVariant(QDateTime::currentDateTime().time().hour()),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
    }
    var =getVariable("Minute");
    if(NULL!=var)
    {
        var->refreshData(QVariant(QDateTime::currentDateTime().time().minute()),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
    }
    var =getVariable("Second");
    if(NULL!=var)
    {
        var->refreshData(QVariant(QDateTime::currentDateTime().time().second()),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
    }
#ifdef Q_OS_UNIX
    if(m_systemVariableCheckCnt == -1 || m_systemVariableCheckCnt >= 60*1000/50){
        // update every 60s
        var =getVariable("BatteryLowVoltage");
        int battery_ok = 1;
        if(NULL!=var)
        {

            QFile configFile("/tmp/battery_ok");
            if(configFile.exists())
            {
                configFile.open(QIODevice::ReadOnly);
                QTextStream stream(&configFile);
                stream.setCodec("UTF-8");
                QString conf = stream.readAll();
                bool ok = false;
                int iconf = conf.toInt(&ok);
                if(ok)
                    battery_ok = iconf;
            }

            var->refreshData(battery_ok==1 ? 0 : 1,QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
        }

        var =getVariable("ApcLowVoltage");
        int apcLowVoltage = 0;
        if(NULL!=var)
        {
            QProcess process;
            process.start("vcgencmd get_throttled");
            process.waitForFinished();
            QByteArray output = process.readAllStandardOutput();
            QString str_output = output;
            str_output = str_output.trimmed();
            if(str_output.compare("throttled=0x50005") == 0)
                apcLowVoltage = 1;

            var->refreshData(apcLowVoltage,QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
        }


        m_systemVariableCheckCnt = 0;
    }
    m_systemVariableCheckCnt++;
#endif

    binitSystemVar = true;
}

void CMDVariableGroup::copy(IMDVariableGroup *otherVarGroup,const QString &localName)
{
    if(otherVarGroup== NULL)
    {
        return;
    }
    m_name = "";
    m_localName= localName;
    m_description= otherVarGroup->description();
    QString error;
    for(int i=0;i<otherVarGroup->getVariableCount();i++)
    {
        IMDRTDBVariable* var = otherVarGroup->getVariableAt(i);
        if(var==NULL)
            continue;
        IMDRTDBVariable* newVar =m_mdVariableManager.createVariable();
        newVar->copy(var);
        newVar->setName("");
        if(false == addVariable(newVar,error))
        {
            delete newVar;
        }
    }
}
//!变量树节点操作
MDTreeItemType CMDVariableGroup::itemType()
{
    return m_itemType;
}

void CMDVariableGroup::setItemType(MDTreeItemType itemType)
{
    m_itemType = itemType;
}

void *CMDVariableGroup::parentItem(MDTreeItemType parentItemType)
{
    Q_UNUSED(parentItemType);
    return m_parentVariableGroup;
}
void CMDVariableGroup::setParentItem(void* parent,MDTreeItemType parentItemType)
{
    Q_UNUSED(parentItemType);
    m_parentVariableGroup = (CMDVariableGroup*)parent;
}
//!实时数据库管理器
CMDRTDBManager * CMDVariableGroup::getRTDBManager()
{
    return  m_mdRTDBManager;
}
void CMDVariableGroup::setRTDBManager(CMDRTDBManager *nsRTDBManager)
{
    m_mdRTDBManager = nsRTDBManager;
    m_mdVariableManager.setRTDBManager(m_mdRTDBManager);
}

//!设置组名全称
void CMDVariableGroup::setName(QString name)
{
    if(0!=QString::compare(m_localName, name,Qt::CaseSensitive))
    {
        m_name  = name;
        QMutexLocker locker(&m_childVariableGroupListMutex);
        for(int i=0;i<m_childVariableGroupList.count();i++)
        {
            m_childVariableGroupList[i]->setName("");
        }
        for(int i=0;i<m_mdVariableManager.getVariableCount();i++)
        {
           m_mdVariableManager.getVariableAt(i)->setName("");
        }
    }
}

//!获取组名全称
QString CMDVariableGroup::name()
{
    if(m_name.isEmpty())
    {
        if(NULL!=m_parentVariableGroup)
        {
               QString parentName =  m_parentVariableGroup->name();
               if(!parentName.isEmpty()&&!parentName.isNull())
               {
                 m_name = QString("%1.%2").arg(parentName).arg(m_localName);
               }
               else
               {
                   m_name = m_localName;
               }
        }
        else
        {
            m_name = m_localName;
        }
    }
    return m_name;
}
//!Group名
QString CMDVariableGroup::localName()
{
    return m_localName;
}

void CMDVariableGroup::setLocalName(QString name)
{
    if(0!=QString::compare(m_localName, name,Qt::CaseSensitive))
    {
        m_localName = name;
        setName(name);
    }
}
//!组描述
void CMDVariableGroup::setDescription(QString description)
{
    m_description  = description;
}

QString CMDVariableGroup::description()
{
	return m_description;
}
//!组id
void CMDVariableGroup::setId(uint id)
{
    m_id  = id;
}

uint CMDVariableGroup::id()
{
    return m_id;
}


//!删除所有子变量组
void CMDVariableGroup::removeAllChildVariableGroup()
{
    QMutexLocker locker(&m_childVariableGroupListMutex);
    for(int i=0;i<m_childVariableGroupList.count();i++)
    {
        delete m_childVariableGroupList[i];
    }
    m_childVariableGroupList.clear();

}
//!添加子变量组
bool CMDVariableGroup::addChildVariableGroup(IMDVariableGroup* variableGroup,QString &error)
{
    bool ret = false;
    if(NULL != variableGroup)
    {
        if(NULL == getChildVariableGroup(variableGroup->localName()))
        {
            QMutexLocker locker(&m_childVariableGroupListMutex);
            m_childVariableGroupList.append(variableGroup);
            variableGroup->setParentItem(this,MDTreeItemType::VariableGroup);
            ((CMDVariableGroup*)variableGroup)->setRTDBManager(m_mdRTDBManager);
            ret = true;

        }
        else
        {
            error  = "变量组已经存在";
        }
    }
    return ret;
}
bool CMDVariableGroup::addChildVariableGroup(QString name,QString description,QString &error)
{
    bool ret = false;

    if(NULL == getChildVariableGroup(name))
    {
        CMDVariableGroup* variableGroup = new CMDVariableGroup();
        variableGroup->setDescription(description);
        variableGroup->setLocalName(name);
        QMutexLocker locker(&m_childVariableGroupListMutex);
        m_childVariableGroupList.append(variableGroup);
        variableGroup->setParentItem(this,MDTreeItemType::VariableGroup);
        variableGroup->setRTDBManager(m_mdRTDBManager);
        ret = true;

    }
    else
    {
        error  = "变量组已经存在";
    }
    return ret;
}
//!移除子变量组
bool CMDVariableGroup::removeChildVariableGroup(IMDVariableGroup* variableGroup)
{
    bool ret = false;
    if(NULL != variableGroup)
    {
        QMutexLocker locker(&m_childVariableGroupListMutex);
        ret = m_childVariableGroupList.removeOne(variableGroup);

    }
    return ret;
}
//!移除子变量组
bool CMDVariableGroup::removeChildVariableGroup(QString groupName)
{
    bool ret = false;
    QMutexLocker locker(&m_childVariableGroupListMutex);
    for(int i=0;i<m_childVariableGroupList.count();i++)
    {
        if(NULL==m_childVariableGroupList[i])
        {
            continue;
        }
        if(0==QString::compare(m_childVariableGroupList[i]->localName(), groupName,Qt::CaseInsensitive))
        {
            delete m_childVariableGroupList[i];
            m_childVariableGroupList.removeAt(i);
            ret = true;

            break;
        }
    }
    return ret;
}
//!获取子变量组
IMDVariableGroup* CMDVariableGroup::getChildVariableGroupAt(int index)
{
    IMDVariableGroup* variableGroup = NULL;
    QMutexLocker locker(&m_childVariableGroupListMutex);
    if (index < m_childVariableGroupList.count() && index>=0)
    {
        variableGroup = m_childVariableGroupList[index];
    }
    return variableGroup;
}
IMDVariableGroup* CMDVariableGroup::getChildVariableGroup(QString groupName)
{
    IMDVariableGroup* variableGroup = NULL;
    QMutexLocker locker(&m_childVariableGroupListMutex);
    for(int i=0;i<m_childVariableGroupList.count();i++)
    {
        if(NULL==m_childVariableGroupList[i])
        {
            continue;
        }

        if(0==QString::compare(m_childVariableGroupList[i]->localName(), groupName,Qt::CaseInsensitive))
        {
            variableGroup = m_childVariableGroupList[i];
            break;
        }
    }
    return variableGroup;
}
IMDVariableGroup *CMDVariableGroup::getChildVariableGroupWithName(QString fullGroupName)
{
    IMDVariableGroup* variableGroup = NULL;
    int nIndex = fullGroupName.indexOf('.');
    IMDVariableGroup* parentGroup = this;
    IMDVariableGroup* nextParentGroup = NULL;
    QString strGroupName = fullGroupName;
    if(nIndex > 0)
    {
        strGroupName = fullGroupName.left(nIndex);
        fullGroupName = fullGroupName.mid(nIndex + 1);
    }
    else
    {
        fullGroupName = "";
    }
    while(parentGroup != NULL && !strGroupName.isEmpty())
    {
        nextParentGroup = parentGroup->getChildVariableGroup(strGroupName);
        if(nextParentGroup != NULL)
        {
            if(fullGroupName.isEmpty())
                variableGroup = nextParentGroup;
        }

        if(variableGroup != NULL || nextParentGroup == NULL)
            break;

        parentGroup = nextParentGroup;
        int nIndex = fullGroupName.indexOf('.');
        if(nIndex > 0)
        {
            strGroupName = fullGroupName.left(nIndex);
            fullGroupName = fullGroupName.mid(nIndex + 1);
        }
        else
        {
            strGroupName = fullGroupName;
            fullGroupName = "";
        }
    }
    return variableGroup;
}

IMDVariableGroup *CMDVariableGroup::getGroupById(uint id)
{
    IMDVariableGroup *finded = NULL;
    int count = getChildVariableGroupCount();
    for(int i=0;i<count; i++)
    {
        IMDVariableGroup *gp = getChildVariableGroupAt(i);
        if(gp)
        {
            if(gp->id() == id)
                return gp;
            else
            {
                finded = gp->getGroupById(id);
                if(finded)
                    return finded;
            }
        }
    }
    return finded;
}

IMDVariableGroup * CMDVariableGroup::copyPasteVariableGroup(IMDVariableGroup* varGroup)
{
    IMDVariableGroup * retGroup = NULL;
    if(varGroup == NULL)
    {
        return retGroup;
    }
    QString error;
    for(int i=0;i<65535;i++)
    {
        QString localName =varGroup->localName();
        if(i>0)
        {
            localName = QString("%1%2").arg(varGroup->localName()).arg(i);
        }
        if(NULL == getChildVariableGroup(localName))
        {
            CMDVariableGroup *newVarGroup = new CMDVariableGroup();
            newVarGroup->setRTDBManager(this->getRTDBManager());
            newVarGroup->setParentItem(this,MDTreeItemType::VariableGroup);
            newVarGroup->copy(varGroup,localName);

            for(int n=0;n<varGroup->getChildVariableGroupCount();n++)
            {
                newVarGroup->copyPasteVariableGroup(varGroup->getChildVariableGroupAt(n));
            }

            if(false == addChildVariableGroup(newVarGroup,error))
            {
                delete newVarGroup;
                return retGroup;
            }
            else
            {
                retGroup = newVarGroup;
                return retGroup;
            }

        }
    }
    return retGroup;
}
bool CMDVariableGroup::copyPasteVariable(IMDRTDBVariable* var)
{
    if(var == NULL)
    {
        return false;
    }
    QString error;
    for(int i=0;i<65535;i++)
    {
        QString localName =var->localName();
        if(i>0)
        {
            localName = QString("%1%2").arg(var->localName()).arg(i);
        }
        if(NULL == getVariable(localName))
        {
            IMDRTDBVariable* newVar =m_mdVariableManager.createVariable();
            newVar->copy(var);
            newVar->setName("");
            newVar->setLocalName(localName);
            if(false == addVariable(newVar,error))
            {
                delete newVar;
                return false;
            }
            else
            {
                return true;
            }

        }
    }
    return false;
}

//!获取子变量组数量
int CMDVariableGroup::getChildVariableGroupCount()
{
    int iCount = 0;
    QMutexLocker locker(&m_childVariableGroupListMutex);
    iCount = m_childVariableGroupList.count();
    return iCount;
}

//!删除所有变量
void CMDVariableGroup::removeAllVariable()
{
    if(m_mdRTDBManager != NULL)
    {
        for(int i=0;i<m_mdVariableManager.getVariableCount();i++)
        {
            IMDRTDBVariable* nsVar = m_mdVariableManager.getVariableAt(i);
            if(nsVar==NULL)
                continue;
            m_mdRTDBManager->variableManager()->removeVariable(nsVar);
        }

    }
    m_mdVariableManager.removeAllVariable();
}
//!添加变量
bool CMDVariableGroup::addVariable(IMDRTDBVariable* nsVar,QString &error)
{
    ((CMDRTDBVariable*)nsVar)->setParentItem(this,MDTreeItemType::VariableGroup);
    ((CMDRTDBVariable*)nsVar)->setRTDBManager(m_mdRTDBManager);
    bool bRet = m_mdVariableManager.addVariable(nsVar,error);
    if(bRet== true)
    {
        bRet=m_mdRTDBManager->variableManager()->addVariable(nsVar,error);
        if(bRet == false)
        {
            m_mdVariableManager.removeVariable(nsVar);
        }
        else//Y2hw
        {
            nsVar->setValid(true);
            if(nsVar->variableType() == MDVariableType::IOVariable)
            {
                nsVar->setValid(false);
                IDevice* pdev = m_mdRTDBManager->driverManager()->getDevice(nsVar->deviceName());
                if(pdev != NULL)
                {
                    if(pdev->addVariable(nsVar,error))
                        nsVar->setValid(true);
                }
            }
            else if(nsVar->variableType() == MDVariableType::MiddleVariable)
            {
                m_mdRTDBManager->middleVariableManager()->addVariable(nsVar,error);
            }
        }
    }
    return bRet;
}
//!移除变量
bool CMDVariableGroup::removeVariable(IMDRTDBVariable* nsVar)
{
    bool bRet = m_mdVariableManager.removeVariable(nsVar);
    if(bRet== true)
    {
        m_mdRTDBManager->variableManager()->removeVariable(nsVar);
        if(nsVar->variableType() == MDVariableType::IOVariable)
        {
            //Y2hw 从设备移除
            IDevice* pdev = m_mdRTDBManager->driverManager()->getDevice(nsVar->deviceName());
            if(pdev != NULL)
                pdev->removeVariable(nsVar);
        }
        else if(nsVar->variableType() == MDVariableType::MiddleVariable)
        {
            m_mdRTDBManager->middleVariableManager()->removeVariable(nsVar);
        }
    }
    return bRet;
}
//!获取变量
IMDRTDBVariable* CMDVariableGroup::getVariableAt(int index)
{
    return m_mdVariableManager.getVariableAt(index);
}
IMDRTDBVariable* CMDVariableGroup::getVariable(QString varShortName)
{
    return m_mdVariableManager.getVariable(varShortName);
}
//!获取变量数量
int CMDVariableGroup::getVariableCount()
{
    return m_mdVariableManager.getVariableCount();
}
//!读写配置
bool CMDVariableGroup::readConfiguration(CMDVariableManager *nsRTDBVariableManager,IMDDriverManager *nsDriverManager,IMDVariableGroup *parentVariableGroup,QDomElement &domElement)
{
    bool ret = false;
    if(domElement.isNull()||nsRTDBVariableManager== NULL||nsDriverManager== NULL)
    {
        return ret;
    }
    if(parentVariableGroup == NULL)//根节点
    {
        QDomElement element = domElement.firstChildElement();
        while ( !element.isNull() )
        {
            CMDVariableGroup *variableGroup = new CMDVariableGroup();
            variableGroup->setRTDBManager(this->getRTDBManager());
            if(variableGroup->readConfiguration(nsRTDBVariableManager,nsDriverManager,this,element)==false)
            {
                delete variableGroup;
            }
            element = element.nextSiblingElement();
        }
        ret = true;
    }
    else
    {
        QString error;
        QDomElement element = domElement.firstChildElement();
        while ( !element.isNull() )
        {
            if(0==QString::compare(element.tagName(),"Name", Qt::CaseInsensitive))
            {
                this->setLocalName(element.text());
                this->setParentItem(parentVariableGroup,MDTreeItemType::VariableGroup);
                this->setRTDBManager(((CMDVariableGroup*)parentVariableGroup)->getRTDBManager());
                ret = ((CMDVariableGroup*)parentVariableGroup)->addChildVariableGroup(this,error);
            }
            else if(0==QString::compare(element.tagName(),"Description", Qt::CaseInsensitive))
            {
                this->setDescription(element.text());
            }
            else if(0==QString::compare(element.tagName(),"ChildGroups", Qt::CaseInsensitive))
            {
                QDomElement childElement = element.firstChildElement();
                while ( !childElement.isNull() )
                {
                    CMDVariableGroup *variableGroup = new CMDVariableGroup();
                    variableGroup->setRTDBManager(this->getRTDBManager());
                    if(variableGroup->readConfiguration(nsRTDBVariableManager,nsDriverManager,this,childElement)==false)
                    {
                        delete variableGroup;
                    }
                    childElement = childElement.nextSiblingElement();
                }
            }
            else if(0==QString::compare(element.tagName(),"Variables", Qt::CaseInsensitive))
            {
                m_mdVariableManager.readConfiguration(nsRTDBVariableManager,nsDriverManager,this,element);
            }
            element = element.nextSiblingElement();
        }
    }
    return ret;
}
void CMDVariableGroup::serialize(CSortJsonObject &json)
{
    if(m_parentVariableGroup == NULL)//根节点
    {
        CSortJsonArray jsGroupArr;
        //out<<m_childVariableGroupList.count();
        for (int index =0;index< m_childVariableGroupList.count();index++)
        {
            CMDVariableGroup* variableGroup = (CMDVariableGroup*)m_childVariableGroupList[index];
            if(variableGroup->isSystemVariableGroup())
                continue;

            CSortJsonObject jsGroup;
            variableGroup->serialize(jsGroup);
            jsGroupArr.insertObject(jsGroup);
        }
        json.insertArray("GS",jsGroupArr);
    }
    else//存储变量组
    {
        //变量组自己的属性
        json.insertSimpleValue("@N",m_localName);
        json.insertSimpleValue("@D",m_description);
        json.insertSimpleValue("@OI",m_orderby);

        //variable
        m_mdVariableManager.serialize(json);

        //Has child variableGroup
        CSortJsonArray jschildgrouparr;
        if(m_childVariableGroupList.count() > 0)
        {
            for (int index =0;index< m_childVariableGroupList.count();index++)
            {
                CSortJsonObject jschildGroup;
                CMDVariableGroup* variableGroup = (CMDVariableGroup*)m_childVariableGroupList[index];
                variableGroup->serialize(jschildGroup);
                jschildgrouparr.insertObject(jschildGroup);
            }

            json.insertArray("GS",jschildgrouparr);
        }        
    }
}

bool CMDVariableGroup::deserialize(const QJsonObject &json,
                                   CMDVariableManager *nsRTDBVariableManager,
                                   IMDDriverManager *nsDriverManager,
                                   IMDVariableGroup *parentVariableGroup
								   )
{
    if(nsRTDBVariableManager== NULL||nsDriverManager== NULL)
    {
        return false;
    }
    setParentItem(parentVariableGroup,MDTreeItemType::VariableGroup);
    if(parentVariableGroup!= NULL)
    {
        setRTDBManager(((CMDVariableGroup*)parentVariableGroup)->getRTDBManager());
    }

    if(parentVariableGroup == NULL)//!根节点
    {
        QJsonObject groupOb;
        QJsonArray groupArr;
        if(m_mdRTDBManager->uaServer())
        {
            m_uaFolder = m_mdRTDBManager->uaServer()->objectsFolder();
            m_uaNodeId = 0;
        }
        groupArr = json.value("GS").toArray();
        int nGroupCnt = groupArr.count();
        for(int i = 0; i<nGroupCnt; i++)
        {
            groupOb = groupArr.at(i).toObject();
            CMDVariableGroup *variableGroup = new CMDVariableGroup();
            variableGroup->setRTDBManager(this->getRTDBManager());
            //m_name = groupOb.value("@N").toString();
            if(variableGroup->deserialize(groupOb,nsRTDBVariableManager,nsDriverManager,this)==false)
            {
                delete variableGroup;
            }
        }
    }
    else
    {
        m_localName = json.value("@N").toString();
        setName(m_localName);

        m_description = json.value("@D").toString();

        m_orderby = json.value("@OI").toString();

		if (((CMDVariableGroup*)parentVariableGroup)->uaFolder())
        {
            m_uaFolder = ((CMDVariableGroup*)parentVariableGroup)->uaFolder()->addFolderObject(m_localName/*, QUaNodeId(1, m_uaNodeId++)*/);
        }

        //!variables
        m_mdVariableManager.deserialize(json,nsRTDBVariableManager,nsDriverManager,this);

        QString error;
        ((CMDVariableGroup*)parentVariableGroup)->addChildVariableGroup(this,error);

        //!ChildGroups
        QJsonArray childGroups = json.value("GS").toArray();
        int childGroupcount = childGroups.count();
        QJsonObject groupOb;
        for (int index =0;index< childGroupcount;index++)
        {
            groupOb = childGroups.at(index).toObject();
            CMDVariableGroup *variableGroup = new CMDVariableGroup();
            variableGroup->setRTDBManager(this->getRTDBManager());
            if(variableGroup->deserialize(groupOb,nsRTDBVariableManager,nsDriverManager,this)==false)
            {
                delete variableGroup;
            }
        }
    }
    return true;
}

bool CMDVariableGroup::read(CMDVariableManager *nsRTDBVariableManager,IMDDriverManager *nsDriverManager,IMDVariableGroup *parentVariableGroup,QDataStream &in)
{
    bool ret = false;
    if(nsRTDBVariableManager== NULL||nsDriverManager== NULL)
    {
        return ret;
    }
    setParentItem(parentVariableGroup,MDTreeItemType::VariableGroup);
    if(parentVariableGroup!= NULL)
    {
        setRTDBManager(((CMDVariableGroup*)parentVariableGroup)->getRTDBManager());
    }

    int version =0;
    in>>version;  //版本
    in>>m_localName;
    setName("");

    in>>m_description;
    m_mdVariableManager.read(nsRTDBVariableManager,nsDriverManager,this,in);

    if(parentVariableGroup!= NULL)
    {
        QString error;
        ret = ((CMDVariableGroup*)parentVariableGroup)->addChildVariableGroup(this,error);
    }

    int childGroupcount =0;
    in >>childGroupcount;
    for (int index =0;index< childGroupcount;index++)
    {
        CMDVariableGroup *variableGroup = new CMDVariableGroup();
        variableGroup->setRTDBManager(this->getRTDBManager());
        if(variableGroup->read(nsRTDBVariableManager,nsDriverManager,this,in)==false)
        {
            delete variableGroup;
        }
    }
    return ret;
}
void CMDVariableGroup::save(QDataStream &out)
{
     out<<int(1);  //版本
     out<<m_localName;
     out<<m_description;

     //!以前总variable也序列化变量。
     //m_mdVariableManager.save(out);//注意别人的测试项目可能存储了这个
     if(!m_localName.isEmpty())
     {
        m_mdVariableManager.save(out);
     }
     else
     {
         int version =0;
         int varcnt = 0;
         out<<version;  //版本
         out <<varcnt;
     }

     out<<m_childVariableGroupList.count();
     for (int index =0;index< m_childVariableGroupList.count();index++)
     {
         CMDVariableGroup* variableGroup = (CMDVariableGroup*)m_childVariableGroupList[index];
         variableGroup->save(out);
     }
}
void CMDVariableGroup::saveConfiguration(QDomDocument &doc,QDomElement &parentElement)
{
    if(m_parentVariableGroup == NULL)//根节点
    {
        QMutexLocker locker(&m_childVariableGroupListMutex);
        for (int index =0;index< m_childVariableGroupList.count();index++)
        {
            CMDVariableGroup* variableGroup = (CMDVariableGroup*)m_childVariableGroupList[index];
            if(!variableGroup->isSystemVariableGroup())
            {
                variableGroup->saveConfiguration(doc,parentElement);
            }
        }
    }
    else
    {
        QDomElement variableGroupElement = doc.createElement("VariableGroup");
        parentElement.appendChild(variableGroupElement);

        QDomElement nameElement = doc.createElement("Name");
        variableGroupElement.appendChild(nameElement);
        QDomText nameElementText = doc.createTextNode(m_localName);
        nameElement.appendChild(nameElementText);

        QDomElement descriptionElement = doc.createElement("Description");
        variableGroupElement.appendChild(descriptionElement);
        QDomText descriptionElementText = doc.createTextNode(m_description);
        descriptionElement.appendChild(descriptionElementText);

        QMutexLocker locker(&m_childVariableGroupListMutex);
        if(m_childVariableGroupList.count()>0)
        {
            QDomElement variableChildGroupElement = doc.createElement("ChildGroups");
            variableGroupElement.appendChild(variableChildGroupElement);
            for (int index =0;index< m_childVariableGroupList.count();index++)
            {
                CMDVariableGroup* variableGroup = (CMDVariableGroup*)m_childVariableGroupList[index];
                variableGroup->saveConfiguration(doc,variableChildGroupElement);
            }
        }
        QDomElement variablesElement = doc.createElement("Variables");
        variableGroupElement.appendChild(variablesElement);
        m_mdVariableManager.saveConfiguration(doc,variablesElement);
    }
}

QUaFolderObject *CMDVariableGroup::uaFolder()
{
	return m_uaFolder;
}

void CMDVariableGroup::setUAFolder(QUaFolderObject * folder)
{
	m_uaFolder = folder;
}
