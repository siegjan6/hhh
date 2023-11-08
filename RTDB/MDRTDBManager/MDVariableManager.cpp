#include "MDVariableManager.h"
#include "MDRTDBVariableUaBase.h"
#include "MDVariableGroup.h"
#include <QJsonArray>
#include <QJsonObject>

CMDVariableManager::CMDVariableManager()
{
    m_varKeyIsLocalName = false;
    m_rtdbManager = NULL;
}

CMDVariableManager::~CMDVariableManager()
{

}

void CMDVariableManager::setRTDBManager(IMDRTDBManager* rtdbManager)
{
    m_rtdbManager = (CMDRTDBManager*)rtdbManager;
}

void CMDVariableManager::enableVarKeyIsLocalName(bool enable /* = true*/)
{
     m_varKeyIsLocalName = enable;
}
//!删除所有变量
void CMDVariableManager::removeAllVariable()
{
    QMutexLocker locker(&m_variableMapMutex);
    if(m_varKeyIsLocalName)
    {
        QMap<QString,IMDRTDBVariable*>::iterator it;
        for ( it = m_variableMap.begin(); it != m_variableMap.end(); ++it )
        {
            IMDRTDBVariable*variable = it.value();
            // 如果DB不为空，从db删除
            CMDRTDBVariable* pVar = (CMDRTDBVariable*)variable;
            IDataBlock* pDB = (IDataBlock*)pVar->parentItem(MDTreeItemType::DataBlock);
            if(pDB != NULL)
            {
                pDB->removeVariable(pVar);
            }

            delete variable;
        }
    }
    m_variableMap.clear();
}
//!新建变量
IMDRTDBVariable*  CMDVariableManager::createVariable(IMDVariableGroup* group/* = NULL*/)
{
//	if (group)
//		return group->uaFolder()->addChild<CMDRTDBVariable>("__VarTemp__");
//	else
//		return new CMDRTDBVariable(m_rtdbManager->uaServer());
    return new CMDRTDBVariableT();
}

//!添加变量
bool CMDVariableManager::addVariable(IMDRTDBVariable* nsVar,QString &error)
{
    bool ret = false;
    if(NULL != nsVar)
    {
        QMutexLocker locker(&m_variableMapMutex);
        QString varName = "";
        if(m_varKeyIsLocalName)
        {
            varName = nsVar->localName().toUpper();
        }
        else
        {
            varName = nsVar->name().toUpper();
        }
        if(!m_variableMap.contains(varName))
        {
            m_variableMap[varName] = nsVar;
            ret = true;
        }
        else
        {
            error ="变量已经存在!";
        }
    }
    return ret;
}
//!移除变量
bool CMDVariableManager::removeVariable(IMDRTDBVariable* nsVar)
{
    bool ret = false;
    if(NULL != nsVar)
    {
        QMutexLocker locker(&m_variableMapMutex);
        QString varName = "";
        if(m_varKeyIsLocalName)
        {
            varName = nsVar->localName().toUpper();
        }
        else
        {
            varName = nsVar->name().toUpper();
        }
        if(m_variableMap.contains(varName))
        {
            m_variableMap.remove(varName);
            ret = true;
            // 如果DB不为空，从db删除
            CMDRTDBVariable* pVar = (CMDRTDBVariable*)nsVar;
            IDataBlock* pDB = (IDataBlock*)pVar->parentItem(MDTreeItemType::DataBlock);
            if(pDB != NULL)
                pDB->removeVariable(pVar);
        }
    }
    return ret;
}
//!获取变量
IMDRTDBVariable* CMDVariableManager::getVariableAt(int index)
{
    IMDRTDBVariable* nsVar = NULL;
    QMutexLocker locker(&m_variableMapMutex);
    if(index>=0 && index<m_variableMap.count())
    {
        QMap<QString,IMDRTDBVariable*>::iterator it;
        it = m_variableMap.begin()+index;
        nsVar = it.value();
    }
    return nsVar;
}
IMDRTDBVariable* CMDVariableManager::getVariable(const QString &varName)
{
    IMDRTDBVariable* nsVar = NULL;
    if(!varName.isEmpty() && !varName.isNull())
    {
        QMutexLocker locker(&m_variableMapMutex);
        QString varNameTmep = varName.toUpper();
        if(m_variableMap.contains(varNameTmep))
        {
            nsVar = m_variableMap.find(varNameTmep).value();
        }
    }
    return nsVar;
}
//!获取变量数量
int CMDVariableManager::getVariableCount()
{
    int iCount = 0;
    QMutexLocker locker(&m_variableMapMutex);
    iCount =  m_variableMap.count();
    return iCount;
}
//! 变量安全策略
CMDActionSPList *CMDVariableManager::variableActionSPList(const QString &varName)
{
    IMDRTDBVariable* nsRTDBVar = getVariable(varName);
    if(nsRTDBVar == NULL)
    {
         return NULL;
    }
    return nsRTDBVar->actionSPList();
}
//!读写变量值
bool CMDVariableManager::readVariableValue(IMDVariable *var)
{
    if(var == NULL)
    {
        return false;
    }
    IMDRTDBVariable* nsRTDBVar = getVariable(var->name());
    if(nsRTDBVar == NULL)
    {
         return false;
    }
    var->setCurrentValue(nsRTDBVar->currentValue());
    var->setCurrentQuality(nsRTDBVar->currentQuality());
    var->setLastValue(nsRTDBVar->lastValue());
    var->setLastQuality(nsRTDBVar->lastQuality());
    var->setTimeStamp(nsRTDBVar->timeStamp());
    var->setDescription(nsRTDBVar->getDescription());
    var->setExt(nsRTDBVar->getExt());
	var->setUnit(nsRTDBVar->getUnit());
    var->setDataType(nsRTDBVar->dataType());
    var->setUaBaseDataVariable(nsRTDBVar->uaBaseDataVariable());

    return true;
}

QVariant CMDVariableManager::readVariableValue(const QString &varName)
{
    IMDRTDBVariable* nsRTDBVar = getVariable(varName);
    if(nsRTDBVar == NULL)
    {
         return QVariant();
    }
    return nsRTDBVar->currentValue();
}

bool CMDVariableManager::writeVariableValue(const QString &varName,QVariant varValue)
{
    IMDRTDBVariable* nsRTDBVar = getVariable(varName);
    if(nsRTDBVar == NULL)
    {
         return false;
    }
    return nsRTDBVar->writeValue(varValue);
}
/*!
\section 读写变量配置
*/
void  CMDVariableManager::readConfiguration(CMDVariableManager *nsRTDBVariableManager,IMDDriverManager *nsDriverManager,IMDVariableGroup *parentVariableGroup,QDomElement &domElement )
{
    if(!domElement.isNull())
    {
        QString error;
        QDomElement element = domElement.firstChildElement();
        m_uaNodeId = 0;
        while ( !element.isNull() )
        {
            if(0==QString::compare(element.tagName(),"Variable", Qt::CaseInsensitive))
            {
                CMDRTDBVariable *variable = new CMDRTDBVariable();
                variable->setParentItem(parentVariableGroup,MDTreeItemType::VariableGroup);
                variable->setRTDBManager(((CMDVariableGroup *)parentVariableGroup)->getRTDBManager());
                variable->readConfiguration(element);
                if(this->addVariable(variable,error)== false)
                {
                    delete variable;
                }
                else
                {
                    if(nsRTDBVariableManager!= NULL)
                    {
                        nsRTDBVariableManager->addVariable(variable,error);
                    }
                    if(nsDriverManager!= NULL && variable->variableType() == MDVariableType::IOVariable)
                    {
                        //nsDriverManager->addVariable()
                    }
                }
            }
            element = element.nextSiblingElement();
        }
    }
}

void CMDVariableManager::read(CMDVariableManager *nsRTDBVariableManager,IMDDriverManager *nsDriverManager,IMDVariableGroup *parentVariableGroup,QDataStream &in)
{
    if(nsRTDBVariableManager== NULL||nsDriverManager== NULL)
    {
        return;
    }

    int version =0;
    in>>version;  //版本

    int varCount =0;
    in >>varCount;

    QString error;
    for (int index =0;index< varCount;index++)
    {
        CMDRTDBVariable *variable = new CMDRTDBVariable();
        variable->setParentItem(parentVariableGroup,MDTreeItemType::VariableGroup);
        variable->setRTDBManager(((CMDVariableGroup *)parentVariableGroup)->getRTDBManager());
        variable->read(in);
        if(this->addVariable(variable,error)== false)
        {
            delete variable;
        }
        else
        {
            //!添加到Driver + 添加到数据块
            if(variable->variableType()==MDVariableType::IOVariable)
                checkVariableValid(m_rtdbManager->driverManager(),variable);

            nsRTDBVariableManager->addVariable(variable,error);
            if(variable->variableType()==MDVariableType::MiddleVariable)
            {
                CMDRTDBManager * nsRTDBManager = (CMDRTDBManager *)variable->getRTDBManager();
                if(nsRTDBManager)
                {
                    IMDVariableManager * nsVarManager = nsRTDBManager->middleVariableManager();
                    if(nsVarManager)
                    {
                        QString error;
                        nsVarManager->addVariable(variable,error);
                    }
                }
            }
        }
    }
}

void CMDVariableManager::save(QDataStream &out)
{
    int version =0;
    out<<version;  //版本
    out <<m_variableMap.size();

    QMutexLocker locker(&m_variableMapMutex);
    QMap<QString,IMDRTDBVariable*>::iterator it;
    for ( it = m_variableMap.begin(); it != m_variableMap.end(); ++it )
    {
        CMDRTDBVariable* nsVar = (CMDRTDBVariable*)it.value();
        nsVar->save(out);
    }
}
void CMDVariableManager::saveConfiguration(QDomDocument &doc,QDomElement &parentElement)
{
    QMutexLocker locker(&m_variableMapMutex);
    QMap<QString,IMDRTDBVariable*>::iterator it;
    for ( it = m_variableMap.begin(); it != m_variableMap.end(); ++it )
    {
        CMDRTDBVariable* nsVar = (CMDRTDBVariable*)it.value();
        nsVar->saveConfiguration(doc,parentElement);
    }
}

void CMDVariableManager::serialize(CSortJsonObject &json)
{
    QMutexLocker locker(&m_variableMapMutex);
    QMap<QString,IMDRTDBVariable*>::iterator it;
    CSortJsonArray varArr;
    for ( it = m_variableMap.begin(); it != m_variableMap.end(); ++it )
    {
        CSortJsonObject varOb;
        CMDRTDBVariableT* nsVar = (CMDRTDBVariableT*)it.value();
        nsVar->serialize(varOb);
        varArr.insertObject(varOb);
    }
    if(m_variableMap.count() > 0)
        // Variables
        json.insertArray("VS",varArr);
}

void CMDVariableManager::deserialize(const QJsonObject &json,
                                     CMDVariableManager *nsRTDBVariableManager,
                                     IMDDriverManager *nsDriverManager,
                                     IMDVariableGroup *parentVariableGroup)
{
    if(nsRTDBVariableManager== NULL||nsDriverManager== NULL)
    {
        return;
    }

    // Variables
    QJsonArray varArray = json.value("VS").toArray();
    int count = varArray.count();
    QJsonObject varOb;
    QString error;
    for(int i = 0; i < count; i++)
    {
        varOb = varArray.at(i).toObject();
		//CMDRTDBVariable *tmp_v = new CMDRTDBVariable(((CMDVariableGroup *)parentVariableGroup)->getRTDBManager()->uaServer());
		//tmp_v->setParentItem(parentVariableGroup, MDTreeItemType::VariableGroup);
		//tmp_v->setRTDBManager(((CMDVariableGroup *)parentVariableGroup)->getRTDBManager());
		//tmp_v->deserialize(varOb);
		//// create instance
		//QUaNodeId strNodeId = {
		//	0,
		//	tmp_v->name()
		//};

		//QUaQualifiedName valueId(0, tmp_v->localName());
		//if (tmp_v)
		//	delete tmp_v;
        // Name
        QString name = varOb.value("@N").toString();
        QString varFullPath = QString("%1.%2").arg(parentVariableGroup->name()).arg(name).replace(".","/");
        //qDebug() << varFullPath;
        CMDRTDBVariableT *variable = nullptr;
        if(m_rtdbManager->isRuntimeMode() && nullptr != m_rtdbManager->uaServer())
        {
//            ((CMDVariableGroup*)parentVariableGroup)->uaFolder()->addChild<TemperatureSensor>("Sensor1");
            variable = ((CMDVariableGroup*)parentVariableGroup)->uaFolder()->addChild<CMDRTDBVariable>(name/*, QUaNodeId(2, m_uaNodeId++)*/);
        }
        else
            variable = new CMDRTDBVariableT();

        //CMDRTDBVariable *variable = new CMDRTDBVariable(((CMDVariableGroup *)parentVariableGroup)->getRTDBManager()->uaServer());
        variable->setParentItem(parentVariableGroup,MDTreeItemType::VariableGroup);
        variable->setRTDBManager(((CMDVariableGroup *)parentVariableGroup)->getRTDBManager());
        variable->deserialize(varOb);

        if(nsDriverManager->isVirtualRunMode())
        {
            if(variable->variableType() == MDVariableType::IOVariable)
            {
                variable->setVariableType(MDVariableType::VirtualVariable);
            }
        }

        if(m_rtdbManager->isRuntimeMode() && nullptr != m_rtdbManager->uaServer())
        {
            // 运行时启用ua HISTORIZING
#ifdef UA_ENABLE_HISTORIZING
            CMDRTDBVariable *pv = (CMDRTDBVariable *)variable;
            if(pv){
                QUaBaseDataVariable *uav = pv->value();
                if(uav){
                    MDRTDataSaveRulerParameter para = variable->dataSaveRulerParameter();
                    if(para.m_saveRuler == MDRTDataSaveRuler::DataChanged){
                        uav->setHistorizing(true);
                        uav->setReadHistoryAccess(true);
                    }
                }
            }
#endif // UA_ENABLE_HISTORIZING
        }

        if(this->addVariable(variable,error)== false)
        {
            delete variable;
        }
        else
        {
            //!添加到Driver + 添加到数据块
            if(variable->variableType()==MDVariableType::IOVariable)
                checkVariableValid(m_rtdbManager->driverManager(),variable);

            nsRTDBVariableManager->addVariable(variable,error);
            if(variable->variableType()==MDVariableType::MiddleVariable)
            {
                CMDRTDBManager * nsRTDBManager = (CMDRTDBManager *)variable->getRTDBManager();
                if(nsRTDBManager)
                {
                    IMDVariableManager * nsVarManager = nsRTDBManager->middleVariableManager();
                    if(nsVarManager)
                    {
                        QString error;
                        nsVarManager->addVariable(variable,error);
                    }
                }
            }			
        }
    }

}

void CMDVariableManager::checkVariableValid(IMDDriverManager* nsDriverManager)
{
    /*
    if(nsDriverManager==NULL)
    {
        return;
    }
    QString error;
    QMutexLocker locker(&m_variableMapMutex);
    QMap<QString,IMDRTDBVariable*>::iterator it;
    for ( it = m_variableMap.begin(); it != m_variableMap.end(); ++it )
    {
        CMDRTDBVariable* nsVar = (CMDRTDBVariable*)it.value();

        nsVar->initialze();

        if(nsVar->variableType()==MDVariableType::IOVariable)
        {
           IDevice *device = nsDriverManager->getDevice(nsVar->deviceName());
           nsVar->setValid(false);
           if(device!=NULL)
           {
               if(device->addVariable(nsVar,error))
               {
                  nsVar->setValid(true);
               }
           }
        }
        else if(nsVar->variableType()==MDVariableType::VirtualVariable)
        {
            nsVar->setValid(true);
        }
    }*/
    //去除IO变量的检查
    if(nsDriverManager==NULL)
    {
        return;
    }
    QString error;
    QMutexLocker locker(&m_variableMapMutex);
    QMap<QString,IMDRTDBVariable*>::iterator it;
    for ( it = m_variableMap.begin(); it != m_variableMap.end(); ++it )
    {
        CMDRTDBVariable* nsVar = (CMDRTDBVariable*)it.value();

        nsVar->initialze();
        if(nsVar->variableType()==MDVariableType::VirtualVariable)
        {
            nsVar->setValid(true);
            nsVar->setValue(nsVar->currentValue());
        }
    }
}
void CMDVariableManager::checkVariableValid(IMDDriverManager* nsDriverManager,IMDRTDBVariable* nsVariable)
{
    if(nsDriverManager==NULL || nsVariable == NULL)
    {
        return;
    }
    QString error;
    IDevice *device = nsDriverManager->getDevice(nsVariable->deviceName());
    nsVariable->setValid(false);
   if(device != NULL && device->addVariable(nsVariable,error))
   {
      nsVariable->setValid(true);
   }
}

