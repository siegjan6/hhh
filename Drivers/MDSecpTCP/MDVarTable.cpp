#include "MDVarTable.h"
#include "MDDevice.h"
#include "MDProtocolCommand.h"
#include <QDebug>

MDVarTable::MDVarTable(CMDDevice *pDev,QObject *parent) :
    QObject(parent)
{
    slaveConnectFlag = 0;
    m_serverIP = "";
    m_serverPort = 20000;
    m_serverSlaveIP = "";
    m_serverSlavePort = 20000;
    m_selectGetTabType = 5;

    m_nCounter = 0;

    m_tabNO = 1;
    m_dataTabNO = 1;
    pDevice = pDev;

    m_packSize         = 0;

    m_tagTabNum     = 0;
    m_dataTabNum   = 0;
    pTimer = new QTimer(this);
    connect(pTimer, SIGNAL(timeout()), this, SLOT(disConnectFromMain()));

    m_pSocket = new QTcpSocket(this);
    connect(m_pSocket, SIGNAL(connected()), this, SLOT(getVar()));
    connect(m_pSocket, SIGNAL(sigGetVar()), this, SLOT(getVar()));
    connect(m_pSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(slaveConnect(QAbstractSocket::SocketState)));
}
MDVarTable::~MDVarTable()
{
    if(pTimer == NULL)
    {
       delete pTimer;
        pTimer = NULL;
    }
    if(NULL != m_pSocket)
    {
        DisConnect();
        m_pSocket->deleteLater();
        m_pSocket = NULL;
    }
}
void MDVarTable::setParameter(QString serverIP,qint16 serverPort,QString serverSlaveIP,qint16 serverSlavePort)
{
    m_serverIP = serverIP;
    m_serverPort = serverPort;
    m_serverSlaveIP = serverSlaveIP;
    m_serverSlavePort = serverSlavePort;
}
void MDVarTable::Connect()
{
    try
    {
        //pTimer->start(1000);
        if(NULL != m_pSocket)
        {
            m_pSocket->connectToHost(m_serverIP,m_serverPort);
        }
     }
     catch(...)
     {
     }
}
void MDVarTable::ConnectSlave()
{
    try
    {
        //pTimer->start(1000);
        if(NULL != m_pSocket)
        {
            m_pSocket->connectToHost(m_serverSlaveIP,m_serverSlavePort);
        }
     }
     catch(...)
     {
     }
}
void MDVarTable::DisConnect()
{
    try
    {
        if(NULL != m_pSocket)
        {
           m_pSocket->disconnectFromHost();
        }
    }
    catch (...)
    {
    }

}
bool MDVarTable::writeData(quint8 *writeBuffer, qint16 count)
{
    bool ret=false;
    if(NULL != m_pSocket )
    {
        QT_TRY
        {
             if(count == m_pSocket->write((char*)writeBuffer, count))
             {
                 if(m_pSocket->waitForBytesWritten(50))
                 {
                     ret=true;
                 }
             }
        }
        QT_CATCH(...)
        {
        }
    }
    return ret;
}
qint64 MDVarTable::readData(quint8 *readBuffer,qint16 startIndex)
{
    qint64 ret=0;
    qint64 recvLength = 0;
    quint8 readBufferTemp[4096];
    if(NULL != m_pSocket )
    {
        QT_TRY
        {
            if (m_pSocket->waitForReadyRead(500))
            {
                recvLength = m_pSocket->read((char*)readBufferTemp,4096);
                memcpy(readBuffer+startIndex,readBufferTemp,recvLength);
                ret = recvLength;
            }
        }
        QT_CATCH(...)
        {
        }
    }
    return ret;

}

qint16 MDVarTable::queryTagList(qint16 tabNum)
{
    qint16 ver;
    qint32 cmdType;

    ver = SECP_PROTOCOL_VER;
    cmdType = SECP_CMD_RD_TAG;
    memcpy(m_sendBuffer, &ver, 2);
    memset(m_sendBuffer+2, 0, 32);
    memcpy(m_sendBuffer+34, &cmdType, 4);
    memcpy(m_sendBuffer+38, &tabNum, 2);

    qint16 pdBufLen = 40;
    return pdBufLen;

}
qint16 MDVarTable::queryDataTagList(qint16 dataTabNum)
{
    qint16 ver;
    qint32 cmdType;

    ver = SECP_PROTOCOL_VER;
    cmdType = SECP_CMD_RD_DATA;
    memcpy(m_sendBuffer, &ver, 2);
    memset(m_sendBuffer+2, 0, 32);
    memcpy(m_sendBuffer+34, &cmdType, 4);
    memcpy(m_sendBuffer+38, &dataTabNum, 2);

    qint16 pdBufLen = 40;
    return pdBufLen;
}
void MDVarTable::getVar()
{
    qint64 currentRevLen = 0;
    qint16 startIndex        = 0;
    qint16 getVarStatus       = 0;
    bool    bFirstPackFlag = false;
    while(true)
    {
            bFirstPackFlag = false;
            qint16 sendLength ;
            sendLength = queryTagList(m_tabNO);
            bool writeSucess = writeData(m_sendBuffer,sendLength);
            if(writeSucess)
            {
                while(true)
                {
                    currentRevLen = readData(m_recvBuffer,startIndex);
                    if(currentRevLen >16 || bFirstPackFlag)
                    {
                        memcpy(&m_packSize, m_recvBuffer+12, 4);
                        if(bFirstPackFlag == false)
                        {
                            bFirstPackFlag  = true;
                        }
                         startIndex  = startIndex+ currentRevLen ;
                         if(startIndex<m_packSize)
                         {
                             if(currentRevLen == 0)
                             {
                                 getVarStatus = 1;
                                 break;
                             }
                             else
                             {
                                 continue;
                             }
                         }
                         else if(startIndex>m_packSize)
                         {
                                getVarStatus = 1;
                                break;
                         }
                         else
                         {
                             break;
                         }
                    }
                    else
                    {
                        getVarStatus = 1;
                        break;
                    } 
                }
                if(getVarStatus != 0)
                {
                    break;
                }
                else
                {
                     startIndex = 0;
                     if(m_recvBuffer[3] == 0x80)
                     {
                         getVarStatus = 1;
                         break;
                     }
                     else
                     {
                         qint16 ver = (m_recvBuffer[1]<<8) + m_recvBuffer[0];
                         if(ver != SECP_PROTOCOL_VER)
                         {
                             getVarStatus = 1;
                             break;
                         }
                         qint16  packTyp = (m_recvBuffer[3]<<8) + m_recvBuffer[2];
                         if(packTyp!=SECP_CMD_RD_TAG)
                         {
                            getVarStatus = 1;
                             break;
                         }
                         m_tagTabNum = m_recvBuffer[16];
                         m_dataTabNum = m_recvBuffer[18];

                         char lable[8];

                         memcpy(lable, m_recvBuffer+4, 8);
                         QString strLable(lable);
                         quint8  currentTagTabNo = m_recvBuffer[17];

                         pDevice->tabNumTagMap.insert(currentTagTabNo,strLable);
                         qint16 pos = 24;
                         while(pos< m_packSize)
                         {
                             qint16 tagID = (m_recvBuffer[pos+1]<<8) + m_recvBuffer[pos];
                             qint16 tagTyp = (m_recvBuffer[pos+3]<<8) + m_recvBuffer[pos+2];
                             qint16 dataLen = (m_recvBuffer[pos+5]<<8) + m_recvBuffer[pos+4];
                             qint16 nameLen = (m_recvBuffer[pos+7]<<8) + m_recvBuffer[pos+6];
                             if(nameLen > 32 || tagID >= MAX_TAG_NUM)
                             {
                                 getVarStatus = 1;
                                 break;
                             }
                             char  tagName[33];
                             memset(tagName,0,33);
                             memcpy(tagName,m_recvBuffer+8+pos,nameLen);
                             QString strTagName(tagName);
                             MDVarPrope *pVarPrope = new MDVarPrope();
                             pVarPrope->m_tagID = tagID;
                             pVarPrope->m_tagName = strTagName;
                             pVarPrope->m_tagTyp = tagTyp;
                             pVarPrope->m_tagLength = dataLen;
                             pDevice->idTabMap.insert(pVarPrope->m_tagID,pVarPrope);
                             pDevice->nameTabMap.insert(pVarPrope->m_tagName,pVarPrope);
                             pos = pos+nameLen + 8;
                         }
                     }
                }
                m_tabNO++;
                if(m_tabNO-1 <m_tagTabNum )
                {
                    continue;
                }
                else
                {
                    m_tabNO = 1;
                    break;
                }
            }
            else
            {
                getVarStatus = 2;
                break;
            }
    }
    if(getVarStatus!=0 || m_dataTabNum<1)
    {
        DisConnect();
        return;
    }

    while(true)
    {
        bFirstPackFlag = false;
        qint16 sendLength ;
        sendLength = queryDataTagList(m_dataTabNO);
        bool writeSucess = writeData(m_sendBuffer,sendLength);
        if(writeSucess)
        {
            while(true)
            {
                currentRevLen = readData(m_recvBuffer,startIndex);
                if(currentRevLen >16 || bFirstPackFlag)
                {
                    memcpy(&m_packSize, m_recvBuffer+12, 4);
                    if(bFirstPackFlag == false)
                    {
                        bFirstPackFlag  = true;
                    }
                     startIndex  = startIndex+ currentRevLen ;
                     if(startIndex<m_packSize)
                     {
                         if(currentRevLen == 0)
                         {
                             getVarStatus = 2;
                             break;
                         }
                         else
                         {
                             continue;
                         }
                     }
                     else if(startIndex>m_packSize)
                     {
                            getVarStatus = 2;
                            break;
                     }
                     else
                     {
                         break;
                     }
                }
                else
                {
                    getVarStatus = 10;
                    break;
                }
            }
            if(getVarStatus!=0)
            {
                break;
             }
            else
            {
                 startIndex = 0;
                 if(m_recvBuffer[3] == 0x80)
                 {
                     getVarStatus = 2;
                     break;
                 }
                 else
                 {
                     qint16 ver = (m_recvBuffer[1]<<8) + m_recvBuffer[0];
                     qint16  packTyp = (m_recvBuffer[3]<<8) + m_recvBuffer[2];
                     if(ver != SECP_PROTOCOL_VER)
                     {
                         getVarStatus = 2;
                          break;
                      }

                      if(packTyp!=SECP_CMD_RD_DATA)
                      {
                              getVarStatus = 2;
                              break;
                      }
                      qint16 pos = 20;
                      IDAndPro tempIDAndPrope;
                      tempIDAndPrope.clear();
                      while(pos < m_packSize)
                      {
                          qint16 tagID = (m_recvBuffer[pos+1]<<8) + m_recvBuffer[pos];
                          if(tagID >= MAX_TAG_NUM)
                          {
                                 getVarStatus = 2;
                                  break;
                           }
                          MDVarPrope *pVarPrope = NULL;
                          if(pDevice->idTabMap.contains(tagID))
                          {
                              IDAndPro::iterator findID = pDevice->idTabMap.find(tagID);
                              pVarPrope = findID.value();
                              tempIDAndPrope.insert(tagID,pVarPrope);
                              pos = pos + 3 +pVarPrope->m_tagLength;
                          }
                          else
                          {
                              getVarStatus = 2;
                               break;
                          }
                      }
                      if(getVarStatus == 0)
                      {
                             pDevice->dataTabNOAndPrope.insert(m_dataTabNO,tempIDAndPrope);
                      }
                 }
            }
            m_dataTabNO++;
            if(m_dataTabNO-1 <m_dataTabNum )
            {
                continue;
            }
            else
            {
                m_dataTabNO = 1;
                pDevice->m_configFlag = true;
                break;
            }
        }
        else
        {
            break;
        }
    }
    DisConnect();
}

void MDVarTable::slaveConnect(QAbstractSocket::SocketState state)
{
    if(state == QAbstractSocket::SocketState::UnconnectedState)
    {
        if(slaveConnectFlag == 0)
        {
            ConnectSlave();
            slaveConnectFlag = 1;
            pDevice->m_bFromSlave = 1;
        }
    }
    else if(state == QAbstractSocket::SocketState::ClosingState)
    {
        slaveConnectFlag = 1;
    }
}
void MDVarTable::disConnectFromMain()
{
    if(pDevice->m_configFlag == 1)
    {
        pTimer->stop();
        slaveConnectFlag = 1;
    }
    else
    {
        m_nCounter++;
        if(m_nCounter >5)
        {
            m_nCounter = 0;
            pTimer->stop();
            DisConnect();
        }
    }
}
