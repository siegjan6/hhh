# Make_MindSCADA.pro
#----------------------------------------------------------------------------------------------------------
#修改历史：2021.02.27 / luyj / 创建用于构建完整的 MindSCADA 执行程序，
#                   包括开发环境和运行环境。
#----------------------------------------------------------------------------------------------------------
TRANSLATIONS += Mind.ts
TEMPLATE = subdirs

#开始编译后台库
#---------------------------------------------------------------------
SUBDIRS += Common/Utility
SUBDIRS += Common/CommonHelper/MDCommonHelper
SUBDIRS += Common/MObjFactory/MDMObjFactory
SUBDIRS += Common/Expression/Expression
SUBDIRS += Common/CommandQueueEngine/MDCommandQueueEngine
SUBDIRS += Log/MDSysLogExpert
SUBDIRS += Common/MObjFactory/MDSwitchFactory
SUBDIRS += Common/MDDBConnect
SUBDIRS += SQLDataBase/MDSQLDataBaseProxy
SUBDIRS += Common/MDQJson2SortJson

SUBDIRS += Action/MDActionIDHelper
SUBDIRS += SecurityPolicy/MDSecurityPolicyHelper
SUBDIRS += Log/MDLogHelper
SUBDIRS += Project/MDProjectHelper
SUBDIRS += Common/MDMultiLanguage
SUBDIRS += Common/MDPropertyBrowser

#SUBDIRS += Drivers/CommunicationLibs/QSerialPort
SUBDIRS += Common/MDExtSerialPort
SUBDIRS += Drivers/MDDriverCommunicationLibs
SUBDIRS += Drivers/MDDriverLibs
SUBDIRS += Drivers/MDModbusRTU
SUBDIRS += Drivers/MDModbusTCP
SUBDIRS += Drivers/MDSiemensS7E
SUBDIRS += Drivers/MDOmronHostLink
#SUBDIRS += Drivers/MDOmronFINS
SUBDIRS += Drivers/MDSecpTCP
SUBDIRS += Drivers/MDPLC
SUBDIRS += Drivers/MDMitsubishiE71
SUBDIRS += Drivers/MDMitsubishiFX2
SUBDIRS += Drivers/MDMitsubishiFX3
SUBDIRS += Drivers/MDOPCUa
win32:SUBDIRS += Drivers/MDSiemensMPIProdave6
SUBDIRS += Drivers/MDSiemensSnap7
SUBDIRS += RTDB/MDVariableArchives
SUBDIRS += RTDB/MDRTDBManager

SUBDIRS += Script/MDScriptEngine

SUBDIRS += Action/MDActionHelper
SUBDIRS += Event/MDEventHelper
SUBDIRS += Alarm/MDAlarmHelper
SUBDIRS += Recipe/MDRecipeHelper

SUBDIRS += Action/MDActionConfiger
SUBDIRS += Event/MDEventConfiger
SUBDIRS += SecurityPolicy/MDAccountConfiger
SUBDIRS += SecurityPolicy/MDAccountStrategyConfiger
SUBDIRS += SecurityPolicy/MDPowerGroupConfiger
SUBDIRS += SecurityPolicy/MDSecurityPolicyConfiger
SUBDIRS += DataArchives/MDDataGroup
SUBDIRS += DataArchives/MDDataSource
SUBDIRS += DataArchives/MDDataArchives
SUBDIRS += DataArchives/MDDataArchivesEngine
SUBDIRS += Alarm/MDAlarmLevelConfiger
SUBDIRS += Alarm/MDAlarmConditionConfiger
SUBDIRS += Alarm/MDAlarmConfiger
SUBDIRS += Recipe/MDRecipeConfiger
SUBDIRS += Project/MDProjectConfiger

#开始编译界面库
#---------------------------------------------------------------------
SUBDIRS += Common/CommandQueueEngine/MDCommandQueueEngineUI
SUBDIRS += Log/MDSysLogUI
SUBDIRS += Common/BaseForm
SUBDIRS += MDFilterTreePlugin/MDFilterTreePlugin
SUBDIRS += Common/MDDataQueryUI
SUBDIRS += Common/MDObjBatchOperation

SUBDIRS += ObjectManagerUI/MDObjectManagerUI
SUBDIRS += SecurityPolicy/MDActionSPUI
SUBDIRS += Action/MDActionUI

SUBDIRS += RTDB/MDRTDBManagerUI
SUBDIRS += DataArchives/MDDataSelectUI
SUBDIRS += DataArchives/MDDataArchivesUI
SUBDIRS += DBQuery/DBQueryConfigure
SUBDIRS += DBQuery/DBQueryEngine
SUBDIRS += DBQuery/DBQueryUI
SUBDIRS += DBQueryService/DBQueryService

SUBDIRS += HMI/MDColorManager/MDColorManager
SUBDIRS += HMI/MDDrawObj
SUBDIRS += HMI/MDDrawNode
SUBDIRS += HMI/MDCurve
SUBDIRS += HMI/MDGauge
SUBDIRS += HMI/MDDiagram
SUBDIRS += HMI/MDDataGroupGrid
SUBDIRS += HMI/MDAlarmGrid
SUBDIRS += HMI/MDLogGrid
SUBDIRS += NCReport/NCReportOutput
SUBDIRS += HMI/MDDataArchivesWidget
SUBDIRS += HMI/MDRecipeGrid
SUBDIRS += HMI/MDDrawSpecial
SUBDIRS += HMI/MDDrawVectors
SUBDIRS += HMI/MDDrawWidgets
SUBDIRS += ScriptEditor/QScintilla-gpl-2.8.4/qscintilla2
SUBDIRS += ScriptEditor/visiscript
SUBDIRS += HMI/MDHMIForm
SUBDIRS += HMI/MDGraphicsStorage
SUBDIRS += HMI/MDHMIFramework

SUBDIRS += SecurityPolicy/MDSecurityPolicyExpert
SUBDIRS += History/MDHistoryExpert

SUBDIRS += NCReport/MDReportDesigner
SUBDIRS += Script/MDScriptUI
SUBDIRS += Event/MDEventUI
SUBDIRS += EventActionUI/MDEventActionUI
SUBDIRS += SecurityPolicy/MDAccountUI
SUBDIRS += SecurityPolicy/MDPowerGroupUI
SUBDIRS += SecurityPolicy/MDSecurityPolicyUI
SUBDIRS += Alarm/MDAlarmLevelUI
SUBDIRS += Alarm/MDAlarmConditionUI
SUBDIRS += Alarm/MDAlarmUI
SUBDIRS += Recipe/MDRecipeExpert
SUBDIRS += Recipe/MDRecipeUI
SUBDIRS += Runtime/MDRuntimeUI

SUBDIRS += Common/SingleApplication/MDSingleApplication
SUBDIRS += Studio/Studio

#开始编译运行时库
#---------------------------------------------------------------------
SUBDIRS += Common/MDUaServer
SUBDIRS += Common/MDUaClient
SUBDIRS += Common/UaSingleton
SUBDIRS += Variable/MDVariableExpert
SUBDIRS += Action/MDActionExpert
SUBDIRS += Event/MDEventExpert
SUBDIRS += Script/MDScriptExpert
SUBDIRS += Alarm/MDAlarmExpert
SUBDIRS += Log/MDLogExpert
SUBDIRS += RedisClient/MDRedisClient
SUBDIRS += DeviceCloud/MDDeviceCloudExpert
SUBDIRS += Common/MObjFactory/MDSwitchFactoryUI
SUBDIRS += Project/MDProjectExpert
SUBDIRS += UaExpert/MDUaExpert

SUBDIRS += Runtime/Runtime

# for test
#---------------------------------------------------------------------
#SUBDIRS += Script/MDScriptEngine_Tester
#SUBDIRS += Script/MDScriptEngine_UI_Tester
SUBDIRS += Script/MDScriptEditor

