#include "MDOPCUaCommunicationObject.h"
#include "QStringList"
#include <string.h>


#include "OPCUaClient.h"
#include <QDebug>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/cfg/env.h"

static void load_spdlog_levels()
{
    // Set the log level to "info" and mylogger to "trace":
    // SPDLOG_LEVEL=info,uacpp_logger=info && ./example
    spdlog::cfg::load_env_levels();
    // or from command line:
    // ./example SPDLOG_LEVEL=info,uacpp_logger=info
    // #include "spdlog/cfg/argv.h" // for loading levels from argv
    // spdlog::cfg::load_argv_levels(args, argv);
}
static bool spdloginitOPCClient = false;


CMDOPCUaCommunicationObject::CMDOPCUaCommunicationObject()
{
    m_pClient=NULL;
    m_commParaeter="";
    m_localIP="";
    m_serverIP="";
    m_localPort=20000;
    m_serverPort=20000;
    m_mode="Client";
    m_recvLength =0;
    m_commStatus = CommunicationStatus::DisConnected;
    m_pSubCallback = NULL;
    m_pContex = NULL;

}

CMDOPCUaCommunicationObject::~CMDOPCUaCommunicationObject()
{
    if(NULL != m_pClient)
    {
        m_pClient->thread_stop();
        m_pClient->disconnect();
        delete m_pClient;
        m_pClient = NULL;
    }
}
CommunicationStatus CMDOPCUaCommunicationObject::getCommStatus()
{
    return m_commStatus;
}
void CMDOPCUaCommunicationObject::setCommStatus(CommunicationStatus commStatus)
{
    m_commStatus = commStatus;
}
void CMDOPCUaCommunicationObject::setCommParameter(QString commParameter)
{
    m_commParaeter.clear();
    m_commParaeter = commParameter;
    QStringList  paramList = m_commParaeter.split(',');//mode,目标ip,目标port,本地ip,本地port
    if(paramList.size()<5)
    {
        return;
    }
    m_mode = paramList[0];
    m_serverIP = paramList[1];
    if (paramList[2].length()>0)
    {
        m_serverPort = paramList[2].toInt();
    }
    else
    {
        m_serverPort = -1;
    }
    m_localIP = paramList[3];
    if (m_localIP.length()==0)
    {
        m_localIP = "localhost";
    }
    if (paramList[4].length() > 0)
    {
        m_localPort =paramList[4].toInt();
    }
    else
    {
        m_localPort = -1;
    }
}

QString CMDOPCUaCommunicationObject::getCommParameter()
{
    return m_commParaeter;
}

bool CMDOPCUaCommunicationObject::connect(qint32 msecs)
{
   bool ret=false;
   try
   {
        if(!spdloginitOPCClient)
        {
           // Log levels can be loaded from argv/env using "SPDLOG_LEVEL"
           load_spdlog_levels();

           // Customize msg format for all loggers
           spdlog::set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");

           // periodically flush all *registered* loggers every 3 seconds:
           // warning: only use if all your loggers are thread safe ("_mt" loggers)
           spdlog::flush_every(std::chrono::seconds(3));

           spdlog::info("Welcome to ua cpp mindscadasvc wrapper!");

           spdlog::init_thread_pool(8192, 1);

           auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
           //console_sink->set_level(spdlog::level::warn);
           console_sink->set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");


           // Create a file rotating logger with 5mb size max and 3 rotated files
           auto max_size = 1048576 * 5;
           auto max_files = 3;
//           auto file_sink = spdlog::rotating_logger_mt<spdlog::async_factory>("uacpp_logger", "logs/uacpp_mindscadasvc.log", max_size, max_files);
           auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/uacpp_mindscadasvc.log", max_size, max_files);
           //file_sink->set_level(spdlog::level::warn);
           file_sink->set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");
           spdlog::info("file_sink");

           std::vector<spdlog::sink_ptr> sinks{ console_sink, file_sink };
           auto logger = std::make_shared<spdlog::async_logger>("uacpp_logger", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
           //auto logger = std::make_shared<spdlog::logger>("uacpp_logger", sinks.begin(), sinks.end());
           spdlog::register_logger(logger);

           logger->info("start");

           spdlog::get("uacpp_logger")->info("=================uacpp start=================");
           spdloginitOPCClient = true;
        }

        std::string address = m_commParaeter.toStdString(); //"opc.tcp://192.167.100.188:4840";
        if(NULL == m_pClient)
        {
            m_pClient = new OPCUaClient();
            m_pClient->initCacheRuntime();
        }
        setCommStatus(CommunicationStatus::Connecting);
        OPCUA::StatusCode retval = m_pClient->connect(address);
        if(retval != OPCUA::StatusCode::ALL_OK){
            disConnect(msecs);
            setCommStatus(CommunicationStatus::DisConnected);
            return false;
        }
        else{
            setCommStatus(CommunicationStatus::Connected);
            return true;
        }
    }
    catch(...)
    {
       spdlog::error("spdlog init error!");
    }
    return ret;
}
bool CMDOPCUaCommunicationObject::disConnect(qint32 msecs)
{
    bool ret=false;
    try
    {
        m_recvLength = 0;
        if(NULL != m_pClient)
        {
           if(getCommStatus() == CommunicationStatus::Connected)
           {
               setCommStatus(CommunicationStatus::DisConnecting);
               OPCUA::StatusCode retval = m_pClient->disconnect();
               if(retval == OPCUA::StatusCode::ALL_OK)
               {
                   qDebug() << "ua client disConnect:" << (int)retval;
                   ret = true;
               }
           }
           setCommStatus(CommunicationStatus::DisConnected);
        }
    }
    catch (...)
    {
    }
    return ret;
}

bool CMDOPCUaCommunicationObject::writeData(quint8 *buffer, qint16 count,qint32 msecs)
{
    bool ret=false;
    if(NULL != m_pClient &&
       getCommStatus()==CommunicationStatus::Connected )
    {
        QT_TRY
        {
//             if(count == m_pTcpSocket->write((char*)buffer, count))
//             {
//                 if(m_pTcpSocket->waitForBytesWritten(msecs))
//                 {
//                     ret=true;
//                 }
//             }

            if(count >=3){
                if(buffer[2] == 0xFF){
                    // 订阅
                    if(NULL != m_pClient &&
                       getCommStatus()==CommunicationStatus::Connected )
                    {
                        QT_TRY
                        {
                            qDebug() << "====================start subscribes default";
                            OPCUA::StatusCode retval = m_pClient->subscribes(m_entityBrowseNames,"default");
                            if(retval == OPCUA::StatusCode::ALL_OK)
                            {
                                qDebug() << "====================subscribes default succeed!!!";
                                memcpy(m_recvBuffer, buffer, count);
                                m_recvBuffer[2] = 0xFE;
                                // execute client's upcall interface (i.e. its method "run_once()") in a separate thread (just for testing, can also be executed in this main)
//                                m_pClient->thread_run((quint8*)m_recvBuffer);
                                m_pClient->thread_run(m_pSubCallback, m_pContex);
                                return true;
                            }
                        }
                        QT_CATCH(...)
                        {
                        }
                    }
                }
//                else if(buffer[2] == 0x00){
//                    // read
//                    if(NULL != m_pClient &&
//                       getCommStatus()==CommunicationStatus::Connected )
//                    {
//                        std::vector<OPCUA::Variant> values;
//                        OPCUA::StatusCode retval = m_pClient->readValues(m_entityBrowseNames,values);
//                        QString strbuf;
//                        if(retval == OPCUA::StatusCode::ALL_OK)
//                        {
//                            for(int i=0;i<m_entityBrowseNames.size();i++)
//                            {
//                                if(values[i].isScalar())
//                                {
//                                    //qDebug() << m_entityBrowseNames[i].data() << values[i].toString().data();
//                                    strbuf.append(values[i].toString().data());
//                                    strbuf.append("|");
//                                }
//                            }
//                        }
//                        qDebug() << strbuf;
//                        QByteArray ba = strbuf.toLatin1();
//                        char* mm = ba.data();
//                        if(ba.length() <=65535){
//                            memcpy(m_recvBuffer+9, mm, ba.length());
//                        }
//                        else
//                            memset(m_recvBuffer, 0, 65535);
//                    }
//                }
//                else if(m_recvBuffer[2] == 0xFD){
//                    // sub callback
//                    if(NULL != m_pClient &&
//                       getCommStatus()==CommunicationStatus::Connected )
//                    {
//                        quint16 datalength = (quint16)m_recvBuffer[4];
//                    }
//                }
                else {
                    memset(m_recvBuffer, 0, 65535);
                    ret = true;
                }
            }

        }
        QT_CATCH(...)
        {
        }
    }
    return ret;
}
qint64 CMDOPCUaCommunicationObject::readData(quint8 *buffer, qint16 startIndex, qint16 bufferSize,qint32 msecs)
{
    qint64 ret=0;
    if(NULL != m_pClient &&
       getCommStatus()==CommunicationStatus::Connected )
    {
        QT_TRY
        {
//            if (m_pTcpSocket->waitForReadyRead(msecs))
//            {
//                qint64 recvLength = m_pTcpSocket->read((char*)m_recvBuffer,4096);
//                //QByteArray requestData = m_clientSocket->readAll();
//                if(recvLength > 0)
//                {
//                    if (recvLength + startIndex > bufferSize)
//                    {
//                        ret = -1;
//                    }
//                    else
//                    {
//                        memcpy(buffer+startIndex,m_recvBuffer,recvLength);
//                        ret = recvLength;
//                    }
//                }
//            }
            qint64 recvLength = bufferSize;
            memcpy(buffer+startIndex,m_recvBuffer,recvLength);
            return recvLength;
        }
        QT_CATCH(...)
        {
        }
    }
    return ret;
}

bool CMDOPCUaCommunicationObject::subscribes(const std::vector<std::string> &entityBrowseNames)
{
    m_entityBrowseNames = entityBrowseNames;

    return false;
}

bool CMDOPCUaCommunicationObject::registSubCallback(subCallbackFunc callback, void* contex)
{
    m_pSubCallback = callback;
    m_pContex = contex;

    return true;
}

void CMDOPCUaCommunicationObject::asyncWriteVariable(QString address, MDDataType type, QVariant value)
{
    if(m_pClient){
        QString uaAddress = address.replace('.','/');
        int index = uaAddress.indexOf("Objects/");
        if(index==0)
            uaAddress = uaAddress.mid(QString("Objects.").length());
        m_pClient->asyncWriteVariable(uaAddress, type, value);
    }
}

void CMDOPCUaCommunicationObject::asyncWriteVariables(QList<QString> addresses, QList<MDDataType> types, QList<QVariant> values)
{
    if(m_pClient){
        QList<QString> uaAddresses;
        foreach(QString address, addresses){
            QString uaAddress = address.replace('.','/');
            int index = uaAddress.indexOf("Objects/");
            if(index==0)
                uaAddress = uaAddress.mid(QString("Objects.").length());
            uaAddresses.append(uaAddress);
        }
        m_pClient->asyncWriteVariables(uaAddresses, types, values);
    }
}
