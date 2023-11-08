#include "MDAlarmExpertImp.h"
#include <QString>
#include <QObject>
#include <QDebug>
#include "MDAlarmPara.h"
#include "MDAlarmConfiger.h"
#include "MDAlarmConditionRT.h"
#include "MDAlarmContacter.h"
#include "MDHistoryExpert.h"
#include "MDAlarmRecord.h"
#include "MDDataDefine.h"
#include "MDDatabaseRecordset.h"

#include "MDActionSPSession.h"

#include "MDDataArchivesEngine.h"
#include "MDDataArchivesInterface/IMDDataGroupManager.h"
#include "MDDataArchivesInterface/IMDDataArchives.h"
#include "MDRTDBInterfaces/IMDRTDBVariable.h"

#include <QUaServer>
//#include "../../include/interface/Common/IUaServer.h"
//#include "../../include/MDUaSingleton/CSigleton.h"
//#include "../../include/MDUaSingleton/UaSingleton.h"


#ifdef UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS
#include <QUaOffNormalAlarm>
#include <QUaExclusiveLevelAlarm>
#include "MDOffNormalAlarm.h"
#include "MDExclusiveLevelAlarm.h"
//#include <QUaConditionVariable>
#endif // UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS

CMDAlarmExpertImp::CMDAlarmExpertImp()
{
    m_recordMgr.setAlarmExpert(this);
    m_alarmEngine.setAlarmExpert(this);

    m_pid = "";
    m_configer = NULL;
    m_contacter =  NULL;
    m_timeout = 0;

    m_alarmMode = MDAlarmMode::common;
    m_alarmSource = 0; //! 缺省来自项目，即 CMDAlarmConfiger
    m_alarmGroup = NULL;
    m_historyExpert = NULL;
    m_dataArchivesEngine = NULL;

    m_lsAlarmProcess = NULL;
	m_alarmsFolder = NULL;
}

CMDAlarmExpertImp::~CMDAlarmExpertImp()
{

}

/*!
\section 初始化与释放
*/
void CMDAlarmExpertImp::setParaBeforeLoad(const QString& pid, CMDAlarmConfiger* configer, CMDAlarmContacter* contacter,
                                                                            short source, CMDHistoryExpert* historyExpert, unsigned long timeout)
{
    m_pid = pid;
    m_configer = configer;
    m_contacter = contacter;
    m_timeout = timeout;

    m_alarmMode = configer->alarmPara()->alarmMode();

    m_historyExpert = historyExpert;
    m_dataArchivesEngine = (historyExpert ? historyExpert->dataArchivesEngine() : NULL);

    m_alarmSource = source;
}

void CMDAlarmExpertImp::load(void* ptr /*= NULL*/)
{
    createSwitchs(true);

    CMDLogSwitch* lsProjectLoad = CMDLogSwitchFactory::findByGroup(m_pid, "ProjectExpert.Load_and_Unload");

    if(0 == m_alarmSource)
    {
        lsProjectLoad->LOG_INFO("从本地文件构建报警配置");

        buildConditionsFromConfiger(ptr);
    }
    else
    {
        lsProjectLoad->LOG_INFO("等待【设备云】的信息以构建报警配置");
    }
    // else; 等待来自【设备云】的消息

    IMDDataArchives* dataArchivesConfiger = m_historyExpert->dataArchivesConfiger();
    m_alarmGroup = (dataArchivesConfiger ?
                                    dataArchivesConfiger->dataGroupManager()->getAlarmDataGroup() : NULL);

    lsProjectLoad->LOG_INFO("启动报警引擎");
    m_alarmEngine.start();

    if(ptr) {
        QUaServer* _server = (QUaServer*)ptr;
        if (_server) {
            //m_pServer = _server;
        }
    }

	//if (uaServer) {
	//	QUaServer* _server = (QUaServer*)uaServer;
	//	if (_server) {
	//		_server->start();
	//	}
	//}
    //m_server.start();
		
}

void CMDAlarmExpertImp::unload()
{
    CMDLogSwitch* lsProjectLoad = CMDLogSwitchFactory::findByGroup(m_pid, "ProjectExpert.Load_and_Unload");

    //m_server.stop();

    lsProjectLoad->LOG_INFO("停止报警引擎");
    m_alarmEngine.stop();

    lsProjectLoad->LOG_INFO("清除报警记录");
    m_variableMgr.deleteAll();
    m_recordMgr.release();

    createSwitchs(false);

	
}

void CMDAlarmExpertImp::createSwitchs(bool create)
{
    if(create)
    {
        QString subFileDir = "ProjectExpert";
        m_lsAlarmProcess = CMDLogSwitchFactory::createSwitchWithSubFileDir(m_pid, "AlarmExpert.AlarmProcess", "报警处理流程", subFileDir);
    }
    else
    {
        m_lsAlarmProcess = NULL;
    }
}

/*!
\section 设置报警条件来源；解析报警条件
*/
void CMDAlarmExpertImp::setAlarmSource(short source)
{
    m_alarmSource = source;
}

short CMDAlarmExpertImp::alarmSource()
{
    return m_alarmSource;
}

void CMDAlarmExpertImp::helper_parseConditions(QList<CMDAlarmCondition*>& conditionList,
                                                                            CMDAlarmLevelConfiger* levelConfiger, void* ptr /*= NULL*/)
{
	//return;
//    if(G_UA)
//    {
//        //QUaServer* _server = (QUaServer*)m_pUaServer;
//        //if (_server) {
//            QUaFolderObject* objsFolder = G_UA->objectsFolder();
////            G_UASERVER->registerType<QUaBaseObject>();
////            G_UASERVER->registerType<QUaOffNormalAlarm>();
//            if (objsFolder) {
//                qDebug() << "uaserver=" << G_UA;
//                m_alarmsFolder = objsFolder->addFolderObject("Alarm");
//                //_server->registerType<MDOffNormalAlarm>();
//            }
//        //}
//    }
//    else
        if (ptr) {
//        UA_Server* _server = (UA_Server*)ptr;
        m_pServer = (QUaServer*)ptr;
        if (m_pServer) {
//            m_pServer = QUaServer::getServerNodeContext(_server);
            QUaFolderObject* objsFolder = m_pServer->objectsFolder();
//            _server->registerType<QUaBaseObject>();
//            _server->registerType<QUaOffNormalAlarm>();
            if (objsFolder) {
                m_alarmsFolder = objsFolder->addFolderObject("Alarm_");
                m_pServer->registerType<MDOffNormalAlarm>(QUaNodeId(0,"MDOffNormalAlarm"));
                //_server->registerType<MDOffNormalAlarm>();
            }
        }
	}
	else {
		// Add ua server description
//		m_server.setApplicationName("MindSCADA-Alarm");
//		m_server.setApplicationUri("urn:www.mindteco.com:mind_scada");
//		m_server.setProductName("MindSCADOPCAUaServer");
//		m_server.setProductUri("mindteco.com");
//		m_server.setManufacturerName("Mindteco Inc.");
//		m_server.setSoftwareVersion("1.1.2-master");
//		m_server.setBuildNumber("02021D71");
//		m_server.setPort(4841);
        QUaFolderObject* objsFolder = m_pServer->objectsFolder();
		if (objsFolder) {
            m_alarmsFolder = objsFolder->addFolderObject("Alarm_");
		}
	}
	
    foreach (CMDAlarmCondition* cond, conditionList)
    {
        if(!cond->enabled())
            continue;
        if(!cond->bindVar().isEmpty())
        {
            // 绑定变量，是否启用变量限值报警
            CMDExpression exp(cond->bindVar());
            QList<QString> refVarList = exp.referencedVarList();
            foreach (const QString varName, refVarList)
            {
                helper_bindConditionWithAlarmVariable(cond, varName, levelConfiger);
            }
        }
        else
        if(!cond->expression().isEmpty())
        {
            // 条件表达式
            CMDExpression exp(cond->expression());
            QList<QString> refVarList = exp.referencedVarList();
            foreach (const QString varName, refVarList)
            {
                helper_bindConditionWithAlarmVariable(cond, varName, levelConfiger);
            }
        }
//        else
//            continue;


#ifdef UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS
		
		if (m_alarmsFolder) {

			// OffNormal Alarm
			//auto motionSensor = alarmsFolder->addBaseObject("motionSensor1");
//            auto alm = m_alarmsFolder->addChild<QUaBaseObject>(cond->name());
//            auto alm = addChild<QUaBaseObject>(m_alarmsFolder, cond->name());
            auto alm = m_alarmsFolder->addBaseObject(cond->customId());

            QString bindVarPath = cond->bindVar();
            if(!bindVarPath.isEmpty())
            {
                // 根据bindvar获取绑定变量的属性
                IMDRTDBVariable* bindVar = m_contacter->getVariable(bindVarPath);
                if(bindVar)
                {
                    MDVariableAlarmParameter vap = bindVar->variableAlarmParameter();
                    if(vap.m_enableAlarm)
                    {
                        // 使能限制报警
                        auto triggerVar = alm->addBaseDataVariable("triggerVar");// 创建Double虚拟变量对应此报警
                        triggerVar->setWriteAccess(true);
                        triggerVar->setDataType(QMetaType::Double);
//                        triggerVar->setValue(0.0);

                        //auto levelAlarm = alm->addChild<QUaExclusiveLevelAlarm>("alarm");
                        //auto levelAlarm = alm->addExclusiveLevelAlarm("alarm");
                        auto levelAlarm = alm->addChild<MDExclusiveLevelAlarm>("alarm");
                        levelAlarm->setConditionName(cond->name());
                        levelAlarm->setSeverity(cond->serverity());
                        // TODO: 扩展属性，需要确认是否可以通知给客户端
                        //levelAlarm->setUserData()
//                        QUaBaseDataVariable* pv =  (QUaBaseDataVariable*)bindVar->uaBaseDataVariable();
//                        if(pv)
//                            levelAlarm->setInputNode(pv);

                        bool bHHL_ok = false;
                        double dHHL = vap.m_HHLValue.toDouble(&bHHL_ok);
                        if(!bHHL_ok)
                        {
                            if(!vap.m_HHLValue.isEmpty())
                            {
                                // HHL expression
                                levelAlarm->setHighHighLimitRequired(true);
                                CMDExpression exp(vap.m_HHLValue);
                                QList<QString> refVarList = exp.referencedVarList();
                                cond->setCurrentLimitAlarmMode(CMDAlarmCondition::HHL);
                                foreach (const QString varName, refVarList)
                                {
                                    helper_bindConditionWithAlarmVariable(cond, varName, levelConfiger);
                                }
                            }
                        }
                        else
                        {
                            //常量
                            levelAlarm->setHighHighLimitRequired(bHHL_ok);
                            levelAlarm->setHighHighLimit(dHHL);
                        }

                        bool bHL_ok = false;
                        double dHL = vap.m_HLValue.toDouble(&bHL_ok);
                        if(!bHL_ok)
                        {
                            if(!vap.m_HLValue.isEmpty())
                            {
                                // HL expression
                                levelAlarm->setHighLimitRequired(true);
                                CMDExpression exp(vap.m_HLValue);
                                QList<QString> refVarList = exp.referencedVarList();
                                cond->setCurrentLimitAlarmMode(CMDAlarmCondition::HL);
                                foreach (const QString varName, refVarList)
                                {
                                    helper_bindConditionWithAlarmVariable(cond, varName, levelConfiger);
                                }
                            }
                        }
                        else
                        {
                            //常量
                            levelAlarm->setHighLimitRequired(bHL_ok);
                            levelAlarm->setHighLimit(dHL);
                        }

                        bool bLL_ok = false;
                        double dLL = vap.m_LLValue.toDouble(&bLL_ok);
                        if(!bLL_ok)
                        {
                            if(!vap.m_LLValue.isEmpty())
                            {
                                // LL expression
                                levelAlarm->setLowLimitRequired(true);
                                CMDExpression exp(vap.m_LLValue);
                                QList<QString> refVarList = exp.referencedVarList();
                                cond->setCurrentLimitAlarmMode(CMDAlarmCondition::LL);
                                foreach (const QString varName, refVarList)
                                {
                                    helper_bindConditionWithAlarmVariable(cond, varName, levelConfiger);
                                }
                            }
                        }
                        else
                        {
                            //常量
                            levelAlarm->setLowLimitRequired(bLL_ok);
                            levelAlarm->setLowLimit(dLL);
                        }

                        bool bLLL_ok = false;
                        double dLLL = vap.m_LLLValue.toDouble(&bLLL_ok);
                        if(!bLLL_ok)
                        {
                            if(!vap.m_LLLValue.isEmpty())
                            {
                                // LLL expression
                                levelAlarm->setLowLowLimitRequired(true);
                                CMDExpression exp(vap.m_LLLValue);
                                QList<QString> refVarList = exp.referencedVarList();
                                cond->setCurrentLimitAlarmMode(CMDAlarmCondition::LLL);
                                foreach (const QString varName, refVarList)
                                {
                                    helper_bindConditionWithAlarmVariable(cond, varName, levelConfiger);
                                }
                            }
                        }
                        else
                        {
                            //常量
                            levelAlarm->setLowLowLimitRequired(bLLL_ok);
                            levelAlarm->setLowLimit(dLLL);
                        }

                        levelAlarm->setComment(cond->comment());
                        levelAlarm->setInputNode(triggerVar);
                        levelAlarm->setConfirmRequired(cond->latched());
                        levelAlarm->setEnabled(true);

                        QObject::connect(levelAlarm, SIGNAL(activated()), this, SLOT(slotAlarmActivated()));
                        QObject::connect(levelAlarm, SIGNAL(deactivated()), this, SLOT(slotAlarmDeactivated()));
                        QObject::connect(levelAlarm, SIGNAL(triggered()), this, SLOT(slotAlarmTriggered()));
                    }
                }
            }
            else
            {
                auto triggerVar = alm->addBaseDataVariable("triggerVar");// 创建Bool虚拟变量对应此报警
                triggerVar->setWriteAccess(true);
                triggerVar->setDataType(QMetaType::Bool);
                triggerVar->setValue(false);

    //            auto offNormalAlarm = alm->addChild<QUaOffNormalAlarm>("alarm");
//                auto offNormalAlarm = alm->addOffNormalAlarm("alarm");
                auto offNormalAlarm = alm->addChild<MDOffNormalAlarm>("alarm");
                offNormalAlarm->setConditionName(cond->name());
                offNormalAlarm->setSeverity(cond->serverity());
                //QUaConditionVariable* p = offNormalAlarm->browseChild<QUaConditionVariable>("Comment");
                //QUaNode* n = offNormalAlarm->browseChild("Comment");
                //QUaConditionVariable* cv = (QUaConditionVariable *)n;
                //cv->setValue(cond->comment());
                offNormalAlarm->setComment(cond->comment());
                offNormalAlarm->setInputNode(triggerVar);
                offNormalAlarm->setNormalValue(false);
                offNormalAlarm->setConfirmRequired(cond->latched());
                offNormalAlarm->setEnabled(true);

                QObject::connect(offNormalAlarm, SIGNAL(activated()), this, SLOT(slotAlarmActivated()));
                QObject::connect(offNormalAlarm, SIGNAL(deactivated()), this, SLOT(slotAlarmDeactivated()));
                QObject::connect(offNormalAlarm, SIGNAL(triggered()), this, SLOT(slotAlarmTriggered()));
            }
            //offNormalAlarm->connect(offNormalAlarm, SIGNAL(activated()), this, SLOT(slotAlarmActivated()));


			// Level Alarm Example
//			auto levelSensor = m_alarmsFolder->addChild<QUaBaseObject>("levelSensor");

//			auto level = levelSensor->addBaseDataVariable("level");
//			level->setWriteAccess(true);
//			level->setDataType(QMetaType::Double);
//			level->setValue(0.0);

//			auto levelAlarm = levelSensor->addChild<QUaExclusiveLevelAlarm>("alarm");
//			levelAlarm->setConditionName("Level Sensor Alarm");
//			levelAlarm->setInputNode(level);

//			levelAlarm->setHighLimitRequired(true);
//			levelAlarm->setLowLimitRequired(true);
//			levelAlarm->setHighLimit(10.0);
//			levelAlarm->setLowLimit(-10.0);

			// Branches 

			// uncomment to support branches
			//motionAlarm->setBranchQueueSize(10);
			//levelAlarm->setBranchQueueSize(10);

#ifdef UA_ENABLE_HISTORIZING
//#ifndef SQLITE_HISTORIZER

//#else
//			if (!historizer.setSqliteDbName("history.sqlite", logOut))
//			{
//				for (auto log : logOut)
//				{
//					qDebug() << "[" << log.level << "] :" << log.message;
//				}
//				return -1;
//			}
//			historizer.setTransactionTimeout(2 * 1000); // db transaction every 2 secs
//#endif // !SQLITE_HISTORIZER

//	// set the historizer
//	// NOTE : historizer must live at least as long as server
//            m_pServer->setHistorizer(historizer);
			// enable event history on specific emitter nodes
            alm->setEventHistoryRead(true);
            //levelSensor->setEventHistoryRead(true);
			// enable event history on server node (all events)
            m_pServer->setEventHistoryRead(true);

			// uncomment to support historizing branches
			//motionAlarm->setHistorizingBranches(true);
			//levelAlarm->setHistorizingBranches(true);

#endif // UA_ENABLE_HISTORIZING
		}
#endif // UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS
    }
}

bool CMDAlarmExpertImp::helper_bindConditionWithAlarmVariable(CMDAlarmCondition* cond,                        
                                                              const QString& varName, CMDAlarmLevelConfiger* levelConfiger)
{
    qint8 result;
    CMDAlarmVariable* av = m_variableMgr.add(varName, result);
    if(!av)
        return false;

    av->setAlarmExpert(this);
    CMDAlarmLevel* level = levelConfiger->find(cond->levelName());
    CMDAlarmConditionRT* condRT = new CMDAlarmConditionRT(cond, level, m_contacter, helper_acknowledgeSP(cond), helper_confirmSP(cond));
    av->initAddAlarmCondition(condRT);
    return true;
}

void CMDAlarmExpertImp::buildConditionsFromConfiger(void* ptr /*= NULL*/)
{
    CMDAlarmConditionConfiger* conditionConfiger = m_configer->conditionConfiger();
    if(conditionConfiger)
    {
        QList<CMDAlarmCondition*> conditionList;
        conditionConfiger->configedList(conditionList);

        QList<CMDAlarmCondition*> conditionLimitList;
        foreach (CMDAlarmCondition* ad, conditionList)
        {
            if(ad->enabled())
            {
                QString bindVarPath = ad->bindVar();
                if(!bindVarPath.isEmpty())
                {
                    IMDRTDBVariable* bindVar = m_contacter->getVariable(bindVarPath);
                    if(bindVar)
                    {
                        MDVariableAlarmParameter vap = bindVar->variableAlarmParameter();
                        if(vap.m_enableAlarm)
                        {
                            CMDAlarmCondition* copy = NULL;

                            QString sHHL = vap.m_HHLValue;
                            if(!sHHL.isEmpty())
                            {
                                copy = new CMDAlarmCondition(*ad);
                                copy->setExpression(QString("%1 >= (%2)").arg(bindVarPath).arg(sHHL));
                                copy->setBindVar("");
                                copy->setName(ad->name()+tr("#HHL"));
                                copy->setCustomId(ad->customId()+tr("#HHL"));
                                conditionLimitList.append(copy);

                            }

                            QString sHL = vap.m_HLValue;
                            if(!sHL.isEmpty())
                            {
                                copy = new CMDAlarmCondition(*ad);
                                copy->setExpression(QString("%1 >= (%2)").arg(bindVarPath).arg(sHL));
                                copy->setBindVar("");
                                copy->setName(ad->name()+tr("#HL"));
                                copy->setCustomId(ad->customId()+tr("#HL"));
                                conditionLimitList.append(copy);
                            }

                            QString sLL = vap.m_LLValue;
                            if(!sLL.isEmpty())
                            {
                                copy = new CMDAlarmCondition(*ad);
                                copy->setExpression(QString("%1 <= (%2)").arg(bindVarPath).arg(sLL));
                                copy->setBindVar("");
                                copy->setName(ad->name()+tr("#LL"));
                                copy->setCustomId(ad->customId()+tr("#LL"));
                                conditionLimitList.append(copy);
                            }

                            QString sLLL = vap.m_LLLValue;
                            if(!sLLL.isEmpty())
                            {
                                copy = new CMDAlarmCondition(*ad);
                                copy->setExpression(QString("%1 <= (%2)").arg(bindVarPath).arg(sLLL));
                                copy->setBindVar("");
                                copy->setName(ad->name()+tr("#LLL"));
                                copy->setCustomId(ad->customId()+tr("#LLL"));
                                conditionLimitList.append(copy);
                            }
                        }
                    }
                }
            }
        }

        foreach (CMDAlarmCondition* ad, conditionLimitList) {
            conditionList.append(ad);
        }

        helper_parseConditions(conditionList, m_configer->levelConfiger(), ptr);
    }
}

void CMDAlarmExpertImp::buildConditionsFromCloud(CMDCommonDevice* device)
{
    if(m_lsAlarmProcess->isOn())
        m_lsAlarmProcess->LOG_INFO_IS("开始处理来自【设备云】的报警配置");

    CMDAlarmConditionConfiger* conditionConfiger = m_configer->conditionConfiger();
    if(conditionConfiger)
    {
        if(m_lsAlarmProcess->isOn())
            m_lsAlarmProcess->LOG_INFO_IS("删除与 device 有关的报警条件：" + device->m_deviceID);
        // 删除与 device 有关的报警条件
        conditionConfiger->deletePrefixMatched(device->m_deviceID);
        m_variableMgr.deletePrefixMatched(device->m_deviceID);

        if(m_lsAlarmProcess->isOn())
            m_lsAlarmProcess->LOG_INFO_IS("重新构造 device 的报警条件：" + device->m_deviceID);
        // 重新构造 device 的报警条件：
        QList<CMDAlarmCondition*> deviceAcListInConfiger;
        QList<CMDAlarmCondition*> deviceAcList;
        device->alarmToConditions(deviceAcList); //! deviceAcList 上的对象通过 new 创建
        foreach (CMDAlarmCondition* ac, deviceAcList)
        {
            //! 被成功添加到 conditionConfiger 的对象，由 deviceAcListInConfiger 记录。
            if(conditionConfiger->addNew(ac))
                deviceAcListInConfiger.append(ac);
        }
        helper_parseConditions(deviceAcListInConfiger, m_configer->levelConfiger());

        if(m_lsAlarmProcess->isOn())
            m_lsAlarmProcess->LOG_INFO_IS("保存报警条件到配置文件");
        // 保存到文件，作为下次加载的缺省配置
        conditionConfiger->saveToFile();
    }
}

//! 提交报警条件
bool CMDAlarmExpertImp::submitDeviceAlarm(const QString& deviceID, const QVariantList &deviceAlarmList)
{
    if(m_lsAlarmProcess->isOn())
        m_lsAlarmProcess->LOG_INFO_IS("来自【设备云】的报警配置->报警引擎");

    return m_alarmEngine.submitDeviceAlarm(deviceID, deviceAlarmList);
}

/*!
\section 报警触发；缓存最近报警到内存；获取最近报警
*/
bool CMDAlarmExpertImp::submitRawAlarmEvent(CMDRawVariableEvent* rawEvent)
{
    rawEvent->setLogSwitch(m_lsAlarmProcess);

    if(m_lsAlarmProcess->isOn())
        m_lsAlarmProcess->LOG_INFO_IS("收到原始变量变化事件: " + rawEvent->rawObjName());

    if(MDVariableDataQuality::Good != rawEvent->changedVariable()->currentQuality())
    {
        if(m_lsAlarmProcess->isOn())
            m_lsAlarmProcess->LOG_INFO_IS("原始变量品质错误，终止报警处理！ ");

        delete rawEvent;
        return false;
    }

    if(m_lsAlarmProcess->isOn())
        m_lsAlarmProcess->LOG_INFO_IS("原始变量变化->报警引擎");
    return m_alarmEngine.submitRawAlarmEvent(rawEvent);
}

bool CMDAlarmExpertImp::processRawAlarmEvent(CMDRawVariableEvent* rawEvent)
{
    if(m_lsAlarmProcess->isOn())
        m_lsAlarmProcess->LOG_INFO_IS("开始处理原始变量变化");

    m_variableMgr.ProcessRawEvent(rawEvent);

    return false;
}

bool CMDAlarmExpertImp::handleCondition(CMDAlarmConditionRT* cond, CMDRawVariableEvent* rawEvent)
{
    if(cond->m_condition->enabled() && cond->m_condition->enableAlarmExp() != "") //使能 && 使能表达式不为空
    {
        CMDExpression exp(cond->m_condition->enableAlarmExp());
        if(exp.compile(cond->m_condition->enableAlarmExp()))
        {
            QList<QString> relatedVarList = exp.referencedVarList();
            for (int i = 0; i < relatedVarList.size(); ++i)
            {
                QVariant value = m_contacter->variableValue(relatedVarList[i]);
                exp.setVariableValue(relatedVarList[i], value);
            }

            bool bok = false;
            QVariant vResult = exp.excute(&bok);
            if(bok)
            {
                bool result = vResult.toBool();
                if(!result)
                {
                    if (m_alarmsFolder)
                    {
                        // 设定TriggerVar当前值让opcua自动触发OffNormalAlarm
                        QUaBaseDataVariable* alm_v = m_alarmsFolder->browsePath<QUaBaseDataVariable>(QUaBrowsePath() << cond->m_condition->customId() << "triggerVar");
                        if(alm_v)
                            alm_v->setValue(false);
                    }
                   return false;
                }
            }
        }
    }

    QVariant variableData = rawEvent->changedVariable()->currentValue();
    if(!cond->m_condition->bindVar().isEmpty())
    {
        if (m_alarmsFolder)
        {
            // 设定TriggerVar当前值让opcua自动触发ExclusiveLevelAlarm
            QUaBaseDataVariable* alm_v = m_alarmsFolder->browsePath<QUaBaseDataVariable>(QUaBrowsePath() << cond->m_condition->customId() << "triggerVar");
            if(alm_v)
                alm_v->setValue(variableData);
        }
        return true;
    }
    else
    {
        bool conditionMatched = cond->handle();
        if(conditionMatched)
        {
            CMDAlarmRecord* existRecord = m_recordMgr.find(cond->m_condition->name());
            if(!existRecord)
            {
                if(m_lsAlarmProcess->isOn())
                   m_lsAlarmProcess->LOG_INFO_IS("报警条件满足，增加一条新报警");

                CMDAlarmRecord* newRecord = m_recordMgr.insert(m_alarmGroup, cond, variableData, m_contacter);
                if(newRecord)
                {
                    //if(cond->m_condition->currentLimitAlarmMode()== CMDAlarmCondition::LimitAlarmMode::HL)

                    QString recordID = newRecord->recordID();
                    QString extStr = getAlarmById(recordID);
                    if(m_alarmsFolder)
                    {
                        QUaAlarmCondition* aconditon = m_alarmsFolder->browsePath<QUaAlarmCondition>(QUaBrowsePath() << cond->m_condition->customId() << "alarm");
                        if(aconditon)
                        {
                            aconditon->setExtMsg("$"+extStr);
                        }
                    }
                }

                if(m_contacter)
                    m_contacter->handleAlarm(1, cond->m_condition->name(), cond->m_condition->comment(), newRecord->m_firstDT, QDateTime());
            }
            else
            {
                if(m_lsAlarmProcess->isOn())
                   m_lsAlarmProcess->LOG_INFO_IS("报警条件满足，且报警已经存在，保持");
            }

            writeUpdateTime();
        }
        else
        {
            QDateTime startTime;
            QDateTime endTime;
            if(m_recordMgr.end(cond, variableData, startTime, endTime))
            {
                if(m_lsAlarmProcess->isOn())
                   m_lsAlarmProcess->LOG_INFO_IS("报警条件不满足，消除报警");

                writeUpdateTime();

                CMDAlarmRecord* existRecord = m_recordMgr.find(cond->m_condition->name());
                if(existRecord)
                {
                    QString recordID = existRecord->recordID();

                    bool bdelete = false;
                    if(cond->m_condition->acknowledgeRequired() && existRecord->m_acknowledgeStatus == MDAlarmAcknowledgeStatus::acknowledged_not_end)
                    {
                        if(cond->m_condition->latched())
                        {
                            if(existRecord->m_confirmStatus == MDAlarmConfirmStatus::confirmed_not_end)
                                bdelete = true;
                        }
                        else
                            bdelete = true;
                    }

                    if(!cond->m_condition->acknowledgeRequired() && !cond->m_condition->latched())
                    {
                        // not acknowledgeRequired and not latched , need to auto ack
                        bool result = m_recordMgr.acknowledgeDirect(recordID, true);
                        bdelete = true;
                    }

                    MDAlarmStatus sts = existRecord->status();
                    if(sts == MDAlarmStatus::occur_acked)
                        existRecord->updateStatus(MDAlarmStatus::end_acked);
                    else if(sts == MDAlarmStatus::occur_need_ack)
                        existRecord->updateStatus(MDAlarmStatus::end_need_ack);
                    else if(sts == MDAlarmStatus::occur_acked_confirmed)
                        existRecord->updateStatus(MDAlarmStatus::end_acked_confirmed);

                    QString extStr = getAlarmById(recordID);
                    if(m_alarmsFolder)
                    {
                        QUaAlarmCondition* aconditon = m_alarmsFolder->browsePath<QUaAlarmCondition>(QUaBrowsePath() << cond->m_condition->customId() << "alarm");
                        if(aconditon)
                        {
                            aconditon->setExtMsg("$"+extStr);
                        }
                    }

                    if(bdelete)
                        m_recordMgr.delete1(cond->m_condition->name());
                }

                if(m_contacter)
                    m_contacter->handleAlarm(0, cond->m_condition->name(), cond->m_condition->comment(), startTime, endTime);
            }
            else
            {
                if(m_lsAlarmProcess->isOn())
                   m_lsAlarmProcess->LOG_ERROR("报警条件不满足，但是没有找到需要消除的报警！");
            }
        }

        if (m_alarmsFolder)
        {
            // 设定TriggerVar当前值让opcua自动触发OffNormalAlarm
            QUaBaseDataVariable* alm_v = m_alarmsFolder->browsePath<QUaBaseDataVariable>(QUaBrowsePath() << cond->m_condition->customId() << "triggerVar");
            qDebug() << "alm_v=" << alm_v << " " << "conditionMatched=" << conditionMatched << " " << cond->m_condition->customId() << "triggerVar";
            if(alm_v)
                alm_v->setValue(conditionMatched);
        }

        return conditionMatched;
    }
}

bool CMDAlarmExpertImp::alarmRecordSet(CMDDatabaseRecordset* dataRecordset)
{
    if(!m_alarmGroup)
        return false;

    QStringList fieldList;
    for(int i = 0; i < m_alarmGroup->itemCount(); i++)
    {
        IMDDataItem* groupItem = m_alarmGroup->getItem(i);
        fieldList.append(groupItem->name());
    }
    dataRecordset->bindFileds(fieldList);
    return (m_alarmMode == MDAlarmMode::common ?
                m_recordMgr.toDatabaseRecordset(dataRecordset) :
                m_recordMgr.m_statusMgr.toDatabaseRecordset(dataRecordset));
}

QDateTime CMDAlarmExpertImp::readUpdateTime()
{
    QReadLocker wLocker(&m_rwLock);
    return m_updateTime;
}

void CMDAlarmExpertImp::writeUpdateTime()
{
    QWriteLocker wLocker(&m_rwLock);
    m_updateTime = QDateTime::currentDateTime();
}

/*!
\section 报警确认及安全策略
*/
bool CMDAlarmExpertImp::acknowledgeRequired(const QString& alarmName)
{
    CMDAlarmConditionRT* acrt = m_recordMgr.findCondition(alarmName);
    if(acrt)
    {
        return acrt->m_condition->acknowledgeRequired();
    }
    return false;
}
bool CMDAlarmExpertImp::acknowledge(const QString& alarmID, const QString& comment/* = ""*/, const int mode/* = 0*/)
{
    return m_recordMgr.acknowledge(alarmID, comment, mode);
}
void CMDAlarmExpertImp::acknowledgeAll(const int mode/* = 0*/)
{
    m_recordMgr.acknowledgeAll(mode);
}

bool CMDAlarmExpertImp::confirmRequired(const QString& alarmName)
{
    CMDAlarmConditionRT* acrt = m_recordMgr.findCondition(alarmName);
    if(acrt)
    {
        return acrt->m_condition->latched();
    }
    return false;
}
bool CMDAlarmExpertImp::confirm(const QString& alarmID, const QString& comment/* = ""*/, const int mode/* = 0*/)
{
    return m_recordMgr.confirm(alarmID, comment, mode);
}

void CMDAlarmExpertImp::confirmAll(const int mode/* = 0*/)
{
    m_recordMgr.confirmAll(mode);
}

bool CMDAlarmExpertImp::suppress(const QString& alarmID, const QString& comment/* = ""*/)
{
    return m_recordMgr.suppress(alarmID, comment);
}

bool CMDAlarmExpertImp::shelve(const QString& alarmID, const QString& comment/* = ""*/)
{
    return m_recordMgr.shelve(alarmID, comment);
}

bool CMDAlarmExpertImp::addComment(const QString& alarmID, const QString& comment/* = ""*/)
{
    return m_recordMgr.addComment(alarmID, comment);
}


int CMDAlarmExpertImp::getAlarmCount()
{
    return m_recordMgr.getAlarmCount();
}

QString CMDAlarmExpertImp::getAlarmByIndex(int index)
{
    return m_recordMgr.getAlarmByIndex(index);
}

QString CMDAlarmExpertImp::getAlarmById(QString id)
{
    return m_recordMgr.getAlarmById(id);
}

QString CMDAlarmExpertImp::getAlarmConfById(QString id)
{
    QString aName = CMDAlarmRecord::recordIDToName(id);
    CMDAlarmConditionRT* acrt = m_recordMgr.findCondition(aName);
    if(acrt)
    {
        return acrt->m_condition->toJsonString();
    }
    return "";
}

CMDActionSP* CMDAlarmExpertImp::helper_acknowledgeSP(CMDAlarmCondition* cond)
{
    CMDActionSP* sp = cond->spList()->find(MDActionType::alarm_Acknowledge);
    if(sp && sp->isEnable())
        return sp;
    else return NULL;
}

CMDActionSP* CMDAlarmExpertImp::helper_confirmSP(CMDAlarmCondition* cond)
{
    CMDActionSP* sp = cond->spList()->find(MDActionType::alarm_Confirm);
    if(sp && sp->isEnable())
        return sp;
    else return NULL;
}

bool CMDAlarmExpertImp::checkSP_WithUI(CMDAlarmConditionRT* cond, int checkMode/* = 0*/)
{
    if(!cond->m_confirmSP)
        return true;
    CMDActionSP* sp = NULL;
    if(checkMode == 0)
        sp = cond->m_acknowledgeSP;
    else if(checkMode == 1)
        sp = cond->m_confirmSP;
    CMDActionSPSession spSession(*sp);
    return m_contacter->checkSP_WithUI(&spSession);
}

void CMDAlarmExpertImp::deleteRecord(const QString& alarmID)
{
    QString aName = CMDAlarmRecord::recordIDToName(alarmID);
    CMDAlarmConditionRT* cond = m_recordMgr.findCondition(aName);
    if(cond) {
        CMDAlarmRecord* existRecord = m_recordMgr.find(cond->m_condition->name());
        if(existRecord)
        {
            bool bdelete = false;
            if(cond->m_condition->acknowledgeRequired() && existRecord->m_acknowledgeStatus == MDAlarmAcknowledgeStatus::acknowledged_not_end)
            {
                if(cond->m_condition->latched())
                {
                    if(existRecord->m_confirmStatus == MDAlarmConfirmStatus::confirmed_not_end)
                        bdelete = true;
                }
                else
                    bdelete = true;
            }

            if(bdelete)
            {
                bool balm = cond->handle();
                if(!balm)
                    m_recordMgr.delete1(cond->m_condition->name());
            }
        }
    }
}

bool CMDAlarmExpertImp::submitAcknowledge(const QString& alarmID, const QString& comment/* = ""*/)
{
    return m_alarmEngine.submitAcknowledge(alarmID, comment);
}

bool CMDAlarmExpertImp::submitConfirm(const QString& alarmID, const QString& comment/* = ""*/)
{
    return m_alarmEngine.submitConfirm(alarmID, comment);
}

bool CMDAlarmExpertImp::confirmDirect(const QString& alarmID, const QString& comment/* = ""*/)
{
    bool result = m_recordMgr.confirmDirect(alarmID, comment);

    deleteRecord(alarmID);

    writeUpdateTime();

    return result;
}

bool CMDAlarmExpertImp::acknowledgeDirect(const QString& alarmID, const QString& comment/* = ""*/)
{
    bool result = m_recordMgr.acknowledgeDirect(alarmID, false, comment);

    deleteRecord(alarmID);

    writeUpdateTime();

    return result;
}

bool CMDAlarmExpertImp::submitSuppress(const QString& alarmID, const QString& comment/* = ""*/)
{
    return m_alarmEngine.submitSuppress(alarmID, comment);
}

bool CMDAlarmExpertImp::suppressDirect(const QString& alarmID, const QString& comment/* = ""*/)
{
    bool result = m_recordMgr.suppressDirect(alarmID, comment);

    writeUpdateTime();

    return result;
}

bool CMDAlarmExpertImp::submitShelve(const QString& alarmID, const QString& comment/* = ""*/)
{
    return m_alarmEngine.submitShelve(alarmID, comment);
}

bool CMDAlarmExpertImp::shelveDirect(const QString& alarmID, const QString& comment/* = ""*/)
{
    bool result = m_recordMgr.shelveDirect(alarmID, comment);

    writeUpdateTime();

    return result;
}

bool CMDAlarmExpertImp::submitAddComment(const QString& alarmID, const QString& comment/* = ""*/)
{
    return m_alarmEngine.submitAddComment(alarmID, comment);
}

bool CMDAlarmExpertImp::addCommentDirect(const QString& alarmID, const QString& comment/* = ""*/)
{
    bool result = m_recordMgr.addCommentDirect(alarmID, comment);

    writeUpdateTime();

    return result;
}

/*!
\section 报警归档，通过历史专家类实现报警归档
*/
void CMDAlarmExpertImp::setHistoryExpert(CMDHistoryExpert* historyExpert)
{
    m_historyExpert = historyExpert;
    m_dataArchivesEngine = (historyExpert ? historyExpert->dataArchivesEngine() : NULL);
}

bool CMDAlarmExpertImp::doArchive(CMDAlarmRecord* record, int insertOrUpdate)
{
    if(!m_dataArchivesEngine)
        return false;

    if(m_lsAlarmProcess->isOn())
       m_lsAlarmProcess->LOG_INFO_IS("将报警归档到数据库：" + record->recordID());

    DataArchives::DB_NOTIFY_TYPE type = (0 == insertOrUpdate ?
                                                                        DataArchives::DB_NOTIFY_ADD :
                                                                        DataArchives::DB_NOTIFY_UPDATE);
    return m_dataArchivesEngine->asyncCallDataArchives(type, &record->m_fields);
}


void CMDAlarmExpertImp::slotAlarmActivated()
{
    qDebug("slotAlarmActivated");
}

void CMDAlarmExpertImp::slotAlarmDeactivated()
{
    qDebug("slotAlarmDeactivated");
}

void CMDAlarmExpertImp::slotAlarmTriggered()
{
    qDebug("slotAlarmTriggered");
}


bool CMDAlarmExpertImp::updataUaConfirm(CMDAlarmRecord* record)
{
    if(record)
    {
        QString recordID = record->recordID();
        QString comment = record->comment();
        QString extStr = getAlarmById(recordID);
        if(m_alarmsFolder)
        {
            QUaAlarmCondition* acondition = m_alarmsFolder->browsePath<QUaAlarmCondition>(QUaBrowsePath() << record->m_cond->m_condition->customId() << "alarm");
            if(acondition)
            {
                acondition->setExtMsg("$"+extStr);
                acondition->setComment(comment);
                QUaProperty* evtId = m_alarmsFolder->browsePath<QUaProperty>(QUaBrowsePath() << record->m_cond->m_condition->customId() << "alarm" << "EventId");
                acondition->Confirm(evtId->value<QByteArray>() ,QUaLocalizedText(record->m_cond->m_condition->comment()));
                return true;
            }
        }
    }
    return false;
}
bool CMDAlarmExpertImp::updataUaAcknowledge(CMDAlarmRecord* record)
{
    if(record)
    {
        QString recordID = record->recordID();
        QString comment = record->comment();
        QString extStr = getAlarmById(recordID);
        if(m_alarmsFolder)
        {
            QUaAlarmCondition* acondition = m_alarmsFolder->browsePath<QUaAlarmCondition>(QUaBrowsePath() << record->m_cond->m_condition->customId() << "alarm");
            if(acondition)
            {
                acondition->setExtMsg("$"+extStr);
                acondition->setComment(comment);
                QUaProperty* evtId = m_alarmsFolder->browsePath<QUaProperty>(QUaBrowsePath() << record->m_cond->m_condition->customId() << "alarm" << "EventId");
                acondition->Acknowledge(evtId->value<QByteArray>() ,QUaLocalizedText(record->m_cond->m_condition->comment()));
                return true;
            }
        }
    }
    return false;
}
bool CMDAlarmExpertImp::updataUaSuppress(CMDAlarmRecord* record)
{
    if(record)
    {
        QString recordID = record->recordID();
        QString comment = record->comment();
        QString extStr = getAlarmById(recordID);
        if(m_alarmsFolder)
        {
            QUaAlarmCondition* acondition = m_alarmsFolder->browsePath<QUaAlarmCondition>(QUaBrowsePath() << record->m_cond->m_condition->customId() << "alarm");
            if(acondition)
            {
                acondition->setExtMsg("$"+extStr);
                acondition->setComment(comment);
                //QUaProperty* evtId = m_alarmsFolder->browsePath<QUaProperty>(QUaBrowsePath() << record->m_cond->m_condition->customId() << "alarm" << "EventId");
                acondition->Suppress();
                return true;
            }
        }
    }
    return false;
}
bool CMDAlarmExpertImp::updataUaShelve(CMDAlarmRecord* record)
{
    if(record)
    {
        QString recordID = record->recordID();
        QString comment = record->comment();
        QString extStr = getAlarmById(recordID);
        if(m_alarmsFolder)
        {
            QUaAlarmCondition* acondition = m_alarmsFolder->browsePath<QUaAlarmCondition>(QUaBrowsePath() << record->m_cond->m_condition->name() << "alarm");
            if(acondition)
            {
                acondition->setExtMsg("$"+extStr);
                acondition->setComment(comment);
                //QUaProperty* evtId = m_alarmsFolder->browsePath<QUaProperty>(QUaBrowsePath() << record->m_cond->m_condition->name() << "alarm" << "EventId");
                acondition->Suppress();
                return true;
            }
        }
    }
    return false;
}
bool CMDAlarmExpertImp::updataUaComment(CMDAlarmRecord* record)
{
    if(record)
    {
        QString recordID = record->recordID();
        QString comment = record->comment();
        QString extStr = getAlarmById(recordID);
        if(m_alarmsFolder)
        {
            QUaAlarmCondition* acondition = m_alarmsFolder->browsePath<QUaAlarmCondition>(QUaBrowsePath() << record->m_cond->m_condition->customId() << "alarm");
            if(acondition)
            {
                acondition->setExtMsg("$"+extStr);
                //QUaProperty* evtId = m_alarmsFolder->browsePath<QUaProperty>(QUaBrowsePath() << record->m_cond->m_condition->customId() << "alarm" << "EventId");
                acondition->setComment(QUaLocalizedText(comment));
                return true;
            }
        }
    }
    return false;
}
