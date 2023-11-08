#include "MDDeviceCloudClient.h"
#include "MDDeviceCloudExpertImp.h"

#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include <QTextCodec>
#include <QTextStream>
#include <QDir>

#include "MDSwitchFactory/MDFuncSwitchFactory.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"
#ifdef _WIN32
    #include "MMKV/MMKV.h"
#elif defined(__linux__)
    #include "MMKV.h"
#endif

const uint CONNECTION_TM = 1100;
const uint CMD_EXE_TM = 1100;

CMDDeviceCloudClient::CMDDeviceCloudClient(QObject *parent) :
    QObject(parent)
{
    m_expert = NULL;
    m_subscribeRedis = NULL;
    m_publishRedis = NULL;
    m_variableRedis = NULL;
    m_alarmRedis = NULL;

   m_deviceGetter.bindCloudClient(this);
   m_testing = false;

   m_sHandleCloudMsg = NULL;
   m_sSendAlarm = NULL;
   m_sSendVar = NULL;
   m_sSendVar2KVCache = NULL;
   m_lsDeviceFlow = NULL;
   m_lsDeviceDetail = NULL;
   m_lsSendAlarm = NULL;
   m_lsSendVar = NULL;
   m_kvLoaded = false;
}

CMDDeviceCloudClient::~CMDDeviceCloudClient()
{

}

void CMDDeviceCloudClient::handleStatusChanged(const QString& msg)
{
    if(m_expert)
        m_expert->handleStatusChanged(msg);
}

/*!
\section 初始化与释放
*/
bool CMDDeviceCloudClient::load(CMDDeviceCloudExpertImp* expert, CDeviceCloudClientPara* clientPara)
{
    m_expert = expert;
    m_cloudClientID = clientPara->m_clientID;

    m_deviceCommandEngine.setCloudClient(this);
    m_deviceCommandEngine.startInCurrentThread();

    // 设备信息连接
    m_publishRedis = new CMDRedisClient(clientPara->m_serverIP, clientPara->m_serverPort);
    connect(m_publishRedis, SIGNAL(connected()), this, SLOT(onRedisConnected()));
    connect(m_publishRedis, SIGNAL(disconnected()), this, SLOT(onRedisDisconnected()));

    // 实时变量连接
    m_variableRedis = new CMDRedisClient(clientPara->m_serverIP, clientPara->m_serverPort);
    connect(m_variableRedis, SIGNAL(connected()), this, SLOT(onRedisConnected()));
    connect(m_variableRedis, SIGNAL(disconnected()), this, SLOT(onRedisDisconnected()));

    // 实时报警连接
    m_alarmRedis = new CMDRedisClient(clientPara->m_serverIP, clientPara->m_serverPort);
    connect(m_alarmRedis, SIGNAL(connected()), this, SLOT(onRedisConnected()));
    connect(m_alarmRedis, SIGNAL(disconnected()), this, SLOT(onRedisDisconnected()));

    // 信息订阅连接
    m_subscribeRedis = new CMDRedisClient(clientPara->m_serverIP, clientPara->m_serverPort);
    connect(m_subscribeRedis, SIGNAL(connected()), this, SLOT(onRedisConnected()));
    connect(m_subscribeRedis, SIGNAL(disconnected()), this, SLOT(onRedisDisconnected()));
    connect(m_subscribeRedis,SIGNAL(messageRet(QList<QByteArray>&)),this,SLOT(messageRet(QList<QByteArray>&)));
    m_lsDeviceFlow->LOG_INFO("尝试建立连接【信息订阅】...");
    m_subscribeRedis->openConnection();

    if(isIpAFile(clientPara->m_serverIP))
    {
        m_testing = true;
        test_load_from_file(clientPara->m_serverIP);
    }

    return true;
}

bool CMDDeviceCloudClient::loadKV(CMDDeviceCloudExpertImp* expert, CDeviceCloudClientPara* clientPara)
{
    m_expert = expert;
    // kv cache
    QString prjKVPath =  m_expert->prjPath() + QDir::separator() + "KV";
#ifdef _WIN32
    std::wstring std_rootDir = prjKVPath.toStdWString();
    MMKV::initializeMMKV(std_rootDir);
#elif defined(__linux__)
    std::string std_rootDir = prjKVPath.toStdString();
    MMKV::initializeMMKV(std_rootDir, MMKVLogInfo);
#endif

    std::string std_id = "__var__";
    m_mmkv = MMKV::mmkvWithID(std_id, MMKVMode::MMKV_MULTI_PROCESS,nullptr,&std_rootDir);

    m_kvLoaded = m_mmkv != nullptr;

    return m_kvLoaded;
}

void CMDDeviceCloudClient::unload()
{
    if(m_mmkv)
        m_mmkv->onExit();

    m_deviceCommandEngine.stop();
    m_deviceGetter.stop();

    if(m_variableRedis != NULL)
    {
        delete m_variableRedis;
        m_variableRedis = NULL;
    }
    if(m_alarmRedis != NULL)
    {
        delete m_alarmRedis;
        m_alarmRedis = NULL;
    }
    if(m_subscribeRedis != NULL)
    {
        delete m_subscribeRedis;
        m_subscribeRedis = NULL;
    }
    if(m_publishRedis != NULL)
    {
        delete m_publishRedis;
        m_publishRedis = NULL;
    }

    createSwitchs("", false);

    m_testing = false;
}

bool CMDDeviceCloudClient::isIpAFile(const QString& strIP)
{
    QFile testFile(strIP);
    return testFile.exists();
}

void CMDDeviceCloudClient::test_load_from_file(const QString& strTestFile)
{
    QFile loadFile(strTestFile);
    loadFile.open(QIODevice::ReadOnly);
    QByteArray saveData = loadFile.readAll();
    QString msg(saveData);
    handleCloudMsg(msg);
}

/*!
\section 管理与服务器的连接
*/
bool CMDDeviceCloudClient::isPublishRedisConnected()
{
    return (m_publishRedis && m_publishRedis->isconnected());
}

void CMDDeviceCloudClient::onRedisConnected()
{
    CMDRedisClient* clientConn = (CMDRedisClient*)sender();
    if(clientConn == m_subscribeRedis)
    {
        m_lsDeviceFlow->LOG_INFO("连接已经建立：信息订阅");

        //! 首先发送订阅命令
        m_lsDeviceFlow->LOG_INFO("发送订阅命令");
        QList<QByteArray> rawCmd;
        rawCmd.append("Subscribe");
        rawCmd.append("GetDeviceDriverInfoPush");
        m_subscribeRedis->asyncSendCommand(rawCmd);

        //! 然后再建立其他连接
        //! -----------------------------------------------------------
        m_lsDeviceFlow->LOG_INFO("尝试建立连接：信息发布");
        m_publishRedis->openConnection();

        m_lsDeviceFlow->LOG_INFO("尝试建立连接：变量发送");
        m_variableRedis->openConnection();

        m_lsDeviceFlow->LOG_INFO("尝试建立连接：报警发送");
        m_alarmRedis->openConnection();
    }
    else if(clientConn == m_publishRedis)
    {
        m_lsDeviceFlow->LOG_INFO("连接已经建立：信息发布");

        //! 发送命令，获取设备列表
        //! ------------------------------------------------
//        sendCommand_GetDeviceIDList();
        sendCommand_GetDeviceIDList_ByEngine();
    }
    else if(clientConn == m_variableRedis)
    {
        m_lsDeviceFlow->LOG_INFO("连接已经建立：变量发送");
    }
    else if(clientConn == m_alarmRedis)
    {
        m_lsDeviceFlow->LOG_INFO("连接已经建立：报警发送");
    }
    else{}
}

void CMDDeviceCloudClient::onRedisDisconnected()
{
    CMDRedisClient* clientConn = (CMDRedisClient*)sender();
    if(clientConn == m_subscribeRedis)
    {
        m_lsDeviceFlow->LOG_INFO("连接已经断开：信息订阅");
    }
    else if(clientConn == m_publishRedis)
    {
        m_lsDeviceFlow->LOG_INFO("连接已经断开：信息发布");
    }
    else if(clientConn == m_variableRedis)
    {
        m_lsDeviceFlow->LOG_INFO("连接已经断开：变量发送");
    }
    else if(clientConn == m_alarmRedis)
    {
        m_lsDeviceFlow->LOG_INFO("连接已经断开：报警发送");
    }
    else{}
}

/*!
\section 发送命令到服务器
*/
void CMDDeviceCloudClient::sendCommand_GetDeviceIDList()
{
    if(!m_publishRedis || !m_publishRedis->isconnected())
        return;

    m_lsDeviceFlow->LOG_INFO("请求属于 " + m_cloudClientID + " 的设备列表");

    QStringList nameList, valueList;
    nameList << "Type" << "Value";
    valueList << "RTDBID" << m_cloudClientID;
    QString cmdContent = helperBuildJsonString(nameList, valueList);

    QList<QByteArray> rawCmd;
    rawCmd.append("publish");
    rawCmd.append("GetDeviceDriverInfoReq");
    rawCmd.append(cmdContent.toUtf8());
    m_publishRedis->asyncSendCommand(rawCmd);
}

void CMDDeviceCloudClient::sendCommand_GetDeviceIDList_ByEngine()
{
    m_deviceCommandEngine.submitDeviceIDList();
}

void CMDDeviceCloudClient::sendCommand_GetDeviceInfo(const QString& deviceID)
{
    if(!m_publishRedis || !m_publishRedis->isconnected())
        return;

    m_lsDeviceFlow->LOG_INFO("请求设备 " + deviceID + " 的信息");

    QStringList nameList, valueList;
    nameList << "Type" << "Value";
    valueList << "DeviceID" << deviceID;
    QString cmdContent = helperBuildJsonString(nameList, valueList);

    QList<QByteArray> rawCmd;
    rawCmd.append("publish");
    rawCmd.append("GetDeviceDriverInfoReq");
    rawCmd.append(cmdContent.toUtf8());
    m_publishRedis->asyncSendCommand(rawCmd);
}

bool CMDDeviceCloudClient::sendCommand_GetDeviceInfo_ByEngine(const QString& deviceID)
{
    return m_deviceCommandEngine.submitDeviceID(deviceID);
}

void CMDDeviceCloudClient::sendCommand_ReloadDevice(const QString& deviceID)
{
    if(!m_alarmRedis || !m_alarmRedis->isconnected())
        return;

    m_lsSendAlarm->LOG_INFO("发布信息：即将重新加载设备 " + deviceID);

    QStringList nameList, valueList;
    nameList << "DeviceID" << "Type";
    valueList << deviceID << "0";
    QString cmdContent = helperBuildJsonString(nameList, valueList);

    QList<QByteArray> rawCmd;
    rawCmd.append("publish");
    rawCmd.append("DeviceConfReloadPush");
    rawCmd.append(cmdContent.toUtf8());
    m_alarmRedis->asyncSendCommand(rawCmd);
}

//Ø	设备报警历史(SCADA实时数据插入)
//key:Alarm.{HisoryAlarmMsg}.DeviceID区分大小写
//Value: ZSet类型，报警信息
//功能：管理设备历史报警信息
//此ZSet表内容示例：
//		value: {"AlarmType":"AlarmInverterFailure","AlarmInfo”:"温度报警","StartTime":"2015-10-21 01:03:45","EndTime":"2015-10-21 03:03:45","Tag":"0"}
//        score: yyyyMMddHHmmssff
//        示例说明：AlarmType 报警类型（ID）
//                  AlarmInfo  报警信息
//          StartTime报警产生时间
//EndTime解除报警时间
//          Tag 报警标识 0 代表报警产生 1 代表报警解除
void CMDDeviceCloudClient::buildAlarmCmd(short type, const QString& name, const QString& msg, const QDateTime& startTime, const QDateTime& endTime,
                                                                          QList<QByteArray>& rtCmd,
                                                                          QList<QByteArray>& rtDeviceCmd,
                                                                          QList<QByteArray>& archiveAddCmd,
                                                                          QList<QByteArray>& archiveDelCmd)
{
    bool start = (type != 0);
    int index = name.indexOf('.');
    QString deviceID = name.left(index);
    QString alarmID = name.right(name.length() - index - 1);
    QString startTimeString = startTime.toString("yyyy-M-d hh:mm:ss.zzz");
    QString endTimeString = (start ? "" : endTime.toString("yyyy-M-d hh:mm:ss.zzz"));
   QString typeString = (start ? "0" : "1");

    QStringList nameList, valueList;
    nameList << "AlarmInfo" << "AlarmTime" << "Tag";
    valueList << msg << startTimeString << typeString;
    QString cmdContent = helperBuildJsonString(nameList, valueList);
//    rtCmd = "hset Alarm.DeviceAlarmMsg." + deviceID + " " + alarmID + " " + cmdContent;
    rtCmd.append("hset");
    rtCmd.append(QString("Alarm.DeviceAlarmMsg." + deviceID).toUtf8());
    rtCmd.append(alarmID.toUtf8());
    rtCmd.append(cmdContent.toUtf8());

    QStringList nameList1, valueList1;
    nameList1 << "DeviceID" << "AlarmID";
    valueList1 << deviceID << alarmID;
    QString deviceContent = helperBuildJsonString(nameList1, valueList1);
//    rtDeviceCmd = "publish SubscribeDeviceAlarmInfoChangePush " + deviceContent;
    rtDeviceCmd.append("publish");
    rtDeviceCmd.append("SubscribeDeviceAlarmInfoChangePush");
    rtDeviceCmd.append(deviceContent.toUtf8());

        //archiveAddCmd = "zadd " + zKey + " " + zScore + " " +  zAddValue;
    QString zKey = "Alarm.{HisoryAlarmMsg}." + deviceID;
    QString zScore = startTime.toString("yyyyMMddhhmmsszzz");
    QString zAddValue =  buildArchiveAlarmValue(typeString, alarmID, msg, startTimeString, endTimeString);
    archiveAddCmd.append("zadd");
    archiveAddCmd.append(zKey.toUtf8());
    archiveAddCmd.append(zScore.toUtf8());
    archiveAddCmd.append(zAddValue.toUtf8());
    if(!start)
    {
        QString zDelValue =  buildArchiveAlarmValue("0", alarmID, msg, startTimeString, "");
        //archiveDelCmd = "zrem " + zKey + " " + zDelValue;
        archiveDelCmd.append("zrem");
        archiveDelCmd.append(zKey.toUtf8());
        archiveDelCmd.append(zDelValue.toUtf8());
    }
}

QString CMDDeviceCloudClient::buildArchiveAlarmValue(const QString& typeString, const QString& alarmID, const QString& msg, const QString& startTimeString, const QString& endTimeString)
{
    QStringList nameList, valueList;
    nameList << "AlarmType" << "AlarmInfo" << "StartTime" << "EndTime" << "Tag";
    valueList << alarmID << msg << startTimeString << endTimeString << typeString;
    QString alarmValue = helperBuildJsonString(nameList, valueList);
    return alarmValue;
}

QString CMDDeviceCloudClient::helperBuildJsonString(const QStringList& nameList, const QStringList& valueList)
{
    QString jsonString = "{";
    int objSize = nameList.size();
    int index = 0;
    while(index < objSize)
    {
        QString name = nameList.at(index);
        QString value = valueList.at(index);

         jsonString += "\"" + name + "\":\"" + value + "\"";
        index ++;
        if(index < objSize)
            jsonString += ",";
    }
    jsonString += "}";
    return jsonString;
}

bool CMDDeviceCloudClient::sendAlarm(short type, const QString& name, const QString& msg, const QDateTime& startTime, const QDateTime& endTime)
{
    if(!m_sSendAlarm->isOn())
        return false;

    if(!m_alarmRedis || !m_alarmRedis->isconnected())
        return false;

    QList<QByteArray> rtCmd;
    QList<QByteArray> rtDeviceCmd;
    QList<QByteArray> archiveAddCmd;
    QList<QByteArray> archiveDelCmd;
    buildAlarmCmd(type, name, msg, startTime, endTime, rtCmd, rtDeviceCmd, archiveAddCmd, archiveDelCmd);

    m_lsSendAlarm->LOG_INFO("发送报警：" + name);
    m_alarmRedis->asyncSendCommand(rtCmd);
    m_lsSendAlarm->LOG_INFO("发布，设备报警有变化：" + name);
    m_alarmRedis->asyncSendCommand(rtDeviceCmd);
    if(type != 0)
    {
        m_lsSendAlarm->LOG_INFO("报警产生或更新，生成归档信息：" + name);
        m_alarmRedis->asyncSendCommand(archiveAddCmd);
    }
    else
    {
        m_lsSendAlarm->LOG_INFO("报警消除，更新归档信息：" + name);
        m_alarmRedis->asyncSendCommand(archiveDelCmd);
        m_alarmRedis->asyncSendCommand(archiveAddCmd);
    }

    return true;
}

bool CMDDeviceCloudClient::sendVariable(IMDChangeVariable* changedVariable)
{
    if(!m_sSendVar->isOn())
        return false;

    if(!m_variableRedis || !m_variableRedis->isconnected())
        return false;

    // 注意：此处过滤了 MindSCADA 的系统变量，但如果
    //           用户定义的变量也包含该串，则该变量也不会被发送！
    QString name = changedVariable->name();
    if(name.contains("SystemVariableGroup", Qt::CaseInsensitive))
        return false;

    QList<QByteArray> rtCmd;
    QList<QByteArray> archiveCmd;
    bool shouldArchive = helperShouldArchive(changedVariable);
    buildVariableCmd(changedVariable, shouldArchive, rtCmd, archiveCmd);

    if(m_lsSendVar->isOn())
        m_lsSendVar->LOG_INFO_IS("发送变量：" + name + ":" + changedVariable->currentValue().toString());

//    m_variableRedis->asyncSendCommand(rtCmd);

    if(shouldArchive)
    {
        if(m_lsSendVar->isOn())
            m_lsSendVar->LOG_INFO_IS("发送变量归档：" + name);
        m_variableRedis->asyncSendCommand(archiveCmd);
    }
    return true;
}

void CMDDeviceCloudClient::buildVariableCmd(IMDChangeVariable* changedVariable,
                                                                             bool shouldArchive,
                                                                             QList<QByteArray>& rtCmd,
                                                                             QList<QByteArray>& archiveCmd)
{
    QString name = changedVariable->name();
    int index = name.indexOf('.');
    QString deviceID = name.left(index);
    QString varName = name.right(name.length() - index - 1);

    // 实时变量
    QString valueString;
    if(changedVariable->dataType() == MDDataType::Bool)
        valueString = (changedVariable->currentValue().toBool() ? "1" : "0");
    else
        valueString = changedVariable->currentValue().toString();
//    rtCmd = "hset RTData.Devices." + deviceID + " " + varName + " " + valueString;
//    rtCmd.append("hset");
//    rtCmd.append(QString("RTData.Devices." + deviceID).toUtf8());
//    rtCmd.append(varName.toUtf8());
//    rtCmd.append(valueString.toUtf8());

    // 归档变量
    if(shouldArchive)
    {
        QString zKey = "VDataQuery.DVData.{" + deviceID + "}." + varName;
        QString zScore = changedVariable->timeStamp().toString("yyyyMMddhhmmsszzz");

        QStringList nameList, valueList;
        nameList << changedVariable->timeStamp().toString("yyyy-MM-dd hh:mm:ss.zzz");
        valueList << valueString;
        QString zValue = helperBuildJsonString(nameList, valueList);

//        archiveCmd = "zadd " + zKey + " " + zScore + " " +  zValue;
        archiveCmd.append("zadd");
        archiveCmd.append(zKey.toUtf8());
        archiveCmd.append(zScore.toUtf8());
        archiveCmd.append(zValue.toUtf8());
    }
}

bool CMDDeviceCloudClient::helperShouldArchive(IMDChangeVariable* changedVariable)
{
    bool goodQuality = (MDVariableDataQuality::Good == changedVariable->currentQuality());
    bool valueChanged = (VariableChangeType::ValueChanged == changedVariable->variableChangeType() ||
                                            VariableChangeType::BothChanged == changedVariable->variableChangeType());
    bool archive = (changedVariable->dataSaveRuler() != MDRTDataSaveRuler::None);

    return (goodQuality && valueChanged && archive);
}

bool CMDDeviceCloudClient::sendDeviceVariableChanged(const QList<QByteArray>& jsonMsgList)
{
    if(!m_sSendVar->isOn())
        return false;

    if(!m_variableRedis || !m_variableRedis->isconnected())
        return false;

    if(m_lsSendVar->isOn())
        m_lsSendVar->LOG_INFO_IS("发布，设备变量有变化：");

    QList<QByteArray> rawCmd;
    rawCmd.append("publish");
    rawCmd.append("SubscribeDeviceRTDBVarValueChangePush");
    rawCmd.append(jsonMsgList);
    m_variableRedis->asyncSendCommand(rawCmd);

    return true;
}

bool CMDDeviceCloudClient::sendVariableBatch(const QList<QByteArray>& redisCmdList)
{
    if(!m_sSendVar->isOn())
        return false;

    if(!m_variableRedis || !m_variableRedis->isconnected())
        return false;

    if(m_lsSendVar->isOn())
        m_lsSendVar->LOG_INFO_IS("批量发送变量：" + QString("%1").arg(redisCmdList.count()));

    QList<QByteArray> rawCmd;
    rawCmd.append(redisCmdList);
    m_variableRedis->asyncSendCommand(rawCmd);
}

bool CMDDeviceCloudClient::sendVariableBatchToKVCache(const QStringList& varNameList, const QList<QVariant>& varValueList)
{
    if(!m_sSendVar2KVCache)
        return false;
    if(!m_sSendVar2KVCache->isOn())
        return false;

    if(!m_mmkv)
        return false;
    if(!m_kvLoaded)
        return false;

    if(varNameList.count() != varValueList.count())
        return false;

    for(int i = 0;i< varNameList.count(); i++){
        QString vnamePath = varNameList[i];
        QVariant value = varValueList[i];
        vnamePath = vnamePath.replace(".", "/");
        setMMKVValue(vnamePath, value);
    }
    //m_mmkv->sync();
    return true;
}
bool CMDDeviceCloudClient::setMMKVValue(const QString& name, const QVariant& value)
{
    if(!m_mmkv)
        return false;
    std::string std_name = name.toStdString();
//    if(name == "SystemVariableGroup/DateTime") {
//        std::string ss(value.typeName());
//        m_mmkv->set(ss, "SystemVariableGroup/DateTime/tt");
//    }
    bool ret = false;
    QVariant::Type tp = value.type();
    switch(tp){
    case QVariant::Type::Bool:
        ret = m_mmkv->set(value.toBool(), std_name);
        break;
    case QVariant::Type::Int:
        ret = m_mmkv->set((int32_t)(value.toInt()), std_name);
        break;
    case QVariant::Type::UInt:
        ret = m_mmkv->set((uint32_t)value.toUInt(), std_name);
        break;
    case QVariant::Type::LongLong:
        ret = m_mmkv->set((int64_t)value.toLongLong(), std_name);
        break;
    case QVariant::Type::ULongLong:
        ret = m_mmkv->set((uint64_t)value.toULongLong(), std_name);
        break;
    case QVariant::Type::Double:
        ret = m_mmkv->set(value.toDouble(), std_name);
        break;
    case QVariant::Type::DateTime:
        ret = m_mmkv->set((int64_t)(value.toDateTime().toSecsSinceEpoch()), std_name);
        break;
    case QVariant::Type::String:
        std::string s = value.toString().toStdString();
        ret = m_mmkv->set(s, std_name);
        break;
    }
    if(m_lsSendVar2KVCache->isOn())
    {
        QString strinfo = QString("setMMKVValue type=[%1] name=%2, value=[%3]").arg(tp).arg(name).arg(value.toString());
        m_lsSendVar2KVCache->LOG_INFO_IS(strinfo);
    }
    return ret;
}

/*!
\section 处理来自服务器的设备信息
*/
void CMDDeviceCloudClient::messageRet(QList<QByteArray>& msg)
{
    int msgCount = msg.count();
    if(msgCount == 1)//general
    {
        if(m_lsDeviceFlow->isOn())
            m_lsDeviceFlow->LOG_INFO_IS("收到【普通消息】");
    }
    else if(msgCount >= 2)//subscribe: channel+contents
    {
        QString content = QString(msg.at(1));
        if(m_lsDeviceFlow->isOn())
            m_lsDeviceFlow->LOG_INFO_IS("收到【订阅消息】");
        if(m_lsDeviceDetail->isOn())
            m_lsDeviceDetail->LOG_INFO_IS("收到【订阅消息】：" + content);

        //! --------------------------------
        if(m_sHandleCloudMsg->isOn())
        {
            handleCloudMsg(content);
        }
    }
    else
    {
        if(m_lsDeviceFlow->isOn())
            m_lsDeviceFlow->LOG_ERROR_IS("收到消息通知，但消息队列为空！");
    }
}

bool CMDDeviceCloudClient::handleCloudMsg(const QString& msg)
{
    //! 1
    //! ------------------------------------------------------------------------
    QByteArray deviceData = msg.toUtf8();
    QJsonParseError jError;
    QJsonDocument jsonDocument = (QJsonDocument::fromJson(deviceData, &jError));
    if(jError.error != QJsonParseError::NoError || !jsonDocument.isObject())
    {
        m_lsDeviceFlow->LOG_ERROR("原始数据转换为 json 时出错！ - 错误码：" + QVariant(jError.error).toString());
        m_lsDeviceFlow->LOG_ERROR("原始数据转换为 json 时出错！ - 错误数据：" + msg);

        return false;
    }

    //! 2
    //! ------------------------------------------------------------------------
    QVariantMap rootMap = jsonDocument.toVariant().toMap();
    int iResult =  rootMap["Result"].toInt();
    if(iResult != 0)
    {
        m_lsDeviceFlow->LOG_ERROR("原始数据成功转换为 json，但结果字段 Result != 0，数据不可用！");
        return false;
    }

    //! 3 根据消息类型分别处理
    //! ------------------------------------------------------------------------
    QString msgType = rootMap["Type"].toString();
    if(0 == msgType.compare("DeviceIDArrayFeedback", Qt::CaseInsensitive))
    {
        m_lsDeviceFlow->LOG_INFO("消息类型 = 设备ID列表，开始解析 ... [" + msgSize(msg) + "] - " + msg);

        handleDeviceIDList(rootMap);
    }
    else if(0 == msgType.compare("DeviceDriverInfoFeedback", Qt::CaseInsensitive))
    {
        m_lsDeviceFlow->LOG_INFO("消息类型 = 单个设备信息，开始解析 ... [" + msgSize(msg) + "]");

        handleDeviceInfo(rootMap);
    }
    else
    {
        m_lsDeviceFlow->LOG_ERROR("消息类型 = <不支持的类型，无法解析> [type = " + msgType + "]");
    }

    return true;
}

bool CMDDeviceCloudClient::handleDeviceIDList(const QVariantMap& rootMap)
{
    //! 1
    //! ------------------------------------------------------------------------
    QString cloudClientID = rootMap["RTDBID"].toString();
    if(m_cloudClientID.compare(cloudClientID, Qt::CaseInsensitive) != 0)
    {
        m_lsDeviceFlow->LOG_ERROR("停止解析设备列表：RTDBID 不匹配！期望 = " + m_cloudClientID + "，实际 = " + cloudClientID);
        return false;
    }

    //! 2 开始获取 deviceIDList 包含的设备信息
    //! ------------------------------------------------------------------------
    QVariantList deviceIDList =  rootMap["DeviceIDList"].toList();
    m_lsDeviceFlow->LOG_INFO(QString("开始获取设备信息，设备总数 %1 ...").arg(deviceIDList.count()));
    m_deviceGetter.start(deviceIDList);

    return true;
}

bool CMDDeviceCloudClient::handleDeviceInfo(const QVariantMap& rootMap)
{
    //! 1
    //! ------------------------------------------------------------------------
    QString cloudClientID = rootMap["RTDBID"].toString();
    if(m_cloudClientID.compare(cloudClientID, Qt::CaseInsensitive) != 0)
    {
        m_lsDeviceFlow->LOG_ERROR("停止解析单个设备：RTDBID 不匹配！期望 = " + m_cloudClientID + "，实际 = " + cloudClientID);
        return false;
    }

    //! 2 解析设备信息
    //! ------------------------------------------------------------------------
    QVariantMap deviceMap =  rootMap["MDDeviceDriverInfo"].toMap();
    QString deviceID = deviceMap["@DeviceID"].toString();
    if(m_deviceGetter.isDeviceProcessed(deviceID) &&
            !m_testing)
    {
        m_lsDeviceFlow->LOG_INFO("[" + deviceID + "]已经被解析过，不再重复解析！");
        return false;
    }
    m_lsDeviceFlow->LOG_INFO("开始 [" + deviceID + "]...");

    // 实时数据库
    QVariantList driverList =  deviceMap["MDDevicesDriver"].toList();
    QVariantList varList =  deviceMap["MDDevicesVarInfo"].toList();

    // 报警条件；数据归档
    QVariantList alarmList =  deviceMap["MDDevicesAlarmInfo"].toList();
    QVariantList dataArchiveList =  deviceMap["MDDevicesDataArchive"].toList();

    //! 3 即将解析设备具体信息，通知到云端
    //! ------------------------------------------------------------------------
    m_lsDeviceFlow->LOG_INFO("即将解析设备具体信息，通知到云端");
    sendCommand_ReloadDevice(deviceID);

    //! 4 处理设备信息
    //! ------------------------------------------------------------------------
    m_lsDeviceFlow->LOG_INFO("通知相关模块，解析设备具体信息 ...");
    if(m_expert)
       m_expert->handleDeviceData(deviceID, driverList, varList, alarmList, dataArchiveList);

    m_lsDeviceFlow->LOG_INFO("结束！[" + deviceID + "]");

    //! 5 完成对 deviceID 的处理，请求下一个设备
    //! ------------------------------------------------------------------------
    m_deviceGetter.completeDevice(deviceID);

    return true;
}

QString CMDDeviceCloudClient::msgSize(const QString& msg)
{
    QString str;
    QByteArray byteArray = msg.toLatin1();
    if(byteArray.size() < 1024)
    {
        str = QString("%1 字节").arg(byteArray.size());
    }
    else
    {
        str = QString("%1 K").arg(byteArray.size()/1024);
    }
    return str;
}

/*!
\section 功能开关和日志开关
*/
void CMDDeviceCloudClient::createSwitchs(const QString& pid, bool create)
{
    if(create)
    {
        m_sHandleCloudMsg = CMDFuncSwitchFactory::createSwitch(pid, "DeviceCloud.HandleCloudMsg", "处理来自设备云的信息");
        m_sSendAlarm = CMDFuncSwitchFactory::createSwitch(pid, "DeviceCloud.SendAlarm", "发送报警数据到设备云");
        m_sSendVar = CMDFuncSwitchFactory::createSwitch(pid, "DeviceCloud.SendVar", "发送变量数据到设备云");
        m_sSendVar2KVCache = CMDFuncSwitchFactory::createSwitch(pid, "KVCache.SendVar", "发送变量数据到键值缓存");

        QString subFileDir = "DeviceCloud";
        m_lsDeviceFlow = CMDLogSwitchFactory::createSwitchWithSubFileDir(pid, "DeviceCloud.DeviceFlow", "获取设备流程", subFileDir);
        m_lsDeviceDetail = CMDLogSwitchFactory::createSwitchWithSubFileDir(pid, "DeviceCloud.DeviceDetail", "设备详细信息", subFileDir);
        m_lsSendAlarm = CMDLogSwitchFactory::createSwitchWithSubFileDir(pid, "DeviceCloud.SendAlarm", "发送报警数据到设备云", subFileDir);
        m_lsSendVar = CMDLogSwitchFactory::createSwitchWithSubFileDir(pid, "DeviceCloud.SendVar", "发送变量数据到设备云", subFileDir);
        m_lsSendVar2KVCache = CMDLogSwitchFactory::createSwitchWithSubFileDir(pid, "KVCache.SendVar", "发送变量数据到键值缓存", "KVCache");
    }
    else
    {
        m_sHandleCloudMsg = NULL;
        m_sSendAlarm = NULL;
        m_sSendVar = NULL;
        m_sSendVar2KVCache = NULL;

        m_lsDeviceFlow = NULL;
        m_lsDeviceDetail = NULL;
        m_lsSendAlarm = NULL;
        m_lsSendVar = NULL;
        m_lsSendVar2KVCache = NULL;
    }
}
