#include "MDSerialPortCommunicationObject.h"
#include <QStringList>
#include <assert.h>
#include <QCoreApplication>
#include <qdebug.h>
#include <QSettings>
/*
CMDSerialPortCommunicationObject::CMDSerialPortCommunicationObject()
{
    m_pCurrentEventCallBack=NULL;
    m_pSerialPort=NULL;
    m_portName = "";
    m_baudRate = QSerialPort::Baud9600;
    m_stopBits = QSerialPort::OneStop;
    m_dataBits = QSerialPort::Data8;
    m_parity = QSerialPort::NoParity;
    m_recvLength =0;
    m_responesTimeout = 3000;
    m_commStatus = CommunicationStatus::DisConnected;
}

CMDSerialPortCommunicationObject::~CMDSerialPortCommunicationObject()
{
    if(NULL!=m_pSerialPort)
    {
         delete m_pSerialPort;
         m_pSerialPort=NULL;
    }
}
qint16 CMDSerialPortCommunicationObject::getResponesTimeout()
{
    return m_responesTimeout;
}

void CMDSerialPortCommunicationObject::setResponesTimeout(qint16 timeOut)
{
    m_responesTimeout = timeOut;
}
CommunicationStatus CMDSerialPortCommunicationObject::getCommStatus()
{
    return m_commStatus;
}
void CMDSerialPortCommunicationObject::setCommStatus(CommunicationStatus commStatus)
{
    m_commStatus = commStatus;
}
qint16 CMDSerialPortCommunicationObject::setEventCallback( IEventCallback *onEvent)
{
    m_pCurrentEventCallBack = onEvent;
    return 1;
}


void CMDSerialPortCommunicationObject::setCommParameter(QString commParameter)
{
    m_commParaeter.clear();
    m_commParaeter = commParameter;
    QStringList  paramList = m_commParaeter.split(',');
    if(paramList.size()<5)
    {
        return;
    }
    m_portName = paramList.at(0).toUpper();
    uint baud =paramList.at(1).toUInt();
    switch(baud)
    {
        case 1200:
           m_baudRate=QSerialPort::Baud1200;
          break;

        case 2400:
           m_baudRate=QSerialPort::Baud2400;
          break;
        case 4800:
           m_baudRate=QSerialPort::Baud4800;
          break;
        case 9600:
           m_baudRate=QSerialPort::Baud9600;
          break;

        case 19200:
           m_baudRate=QSerialPort::Baud19200;
          break;
        case 38400:
           m_baudRate=QSerialPort::Baud38400;
          break;

        case 57600:
           m_baudRate=QSerialPort::Baud57600;
          break;

        case 115200:
           m_baudRate=QSerialPort::Baud115200;
          break;
        default:
           m_baudRate=QSerialPort::UnknownBaud;
           break;
    }
    uint databits=paramList.at(2).toUInt();
    switch(databits)
    {
        case 5:
            m_dataBits=QSerialPort::Data5;
            break;
        case 6:
            m_dataBits=QSerialPort::Data6;
            break;
        case 7:
            m_dataBits=QSerialPort::Data7;
            break;
        case 8:
            m_dataBits=QSerialPort::Data8;
            break;
        default:
            m_dataBits=QSerialPort::UnknownDataBits;
            break;
    }
     uint stopBits=paramList.at(3).toUInt();
     switch(stopBits)
     {
        case 1:
            m_stopBits= QSerialPort::OneStop;
            break;
        case 2:
             m_stopBits= QSerialPort::OneAndHalfStop;
            break;
        case 3:
            m_stopBits= QSerialPort::TwoStop;
            break;
        default:
            m_stopBits= QSerialPort::UnknownStopBits;
            break;
     }
     uint parity=paramList.at(4).toUInt();
     switch(parity)
     {
        case 0:
            m_parity=QSerialPort::NoParity;
            break;
        case 1:
            m_parity=QSerialPort::OddParity;
            break;
        case 2:
            m_parity=QSerialPort::EvenParity;
            break;
        case 3:
            m_parity=QSerialPort::MarkParity;
            break;
        case 4:
            m_parity=QSerialPort::SpaceParity;
            break;
        default:
            m_parity=QSerialPort::UnknownParity;
            break;
    }
}

QString CMDSerialPortCommunicationObject::getCommParameter()
{
    return m_commParaeter;
}

qint16 CMDSerialPortCommunicationObject::startConnect()
{
  qint16 ret=0;

  m_recvLength = 0;

  stopConnect();

  if(NULL==m_pSerialPort)
  {
      m_pSerialPort = new QSerialPort();
  }
  if(NULL==m_pSerialPort)
  {
      setCommStatus(CommunicationStatus::DisConnected);
      onEvent(CommunicationEventType::DisConnected);
      return ret;
  }


  setCommStatus(CommunicationStatus::Connecting);

  onEvent(CommunicationEventType::Connecting);

  m_pSerialPort->setPortName(m_portName);
  if(m_pSerialPort->open(QIODevice::ReadWrite))
  {
      m_pSerialPort->setBaudRate(9600);
      m_pSerialPort->setDataBits((QSerialPort::Data8));
      m_pSerialPort->setParity(QSerialPort::NoParity);
      m_pSerialPort->setStopBits(QSerialPort::OneStop);
      m_pSerialPort->setFlowControl(QSerialPort::NoFlowControl);

      connect(m_pSerialPort, SIGNAL(readyRead()), this, SLOT(dataReceived()));
      setCommStatus(CommunicationStatus::Connected);
      onEvent(CommunicationEventType::Connected);
      ret=1;
       return ret;
      if (m_pSerialPort->setBaudRate(m_baudRate)
              && m_pSerialPort->setDataBits(m_dataBits)
              && m_pSerialPort->setParity(m_parity)
              && m_pSerialPort->setStopBits(m_stopBits)
              && m_pSerialPort->setFlowControl(QSerialPort::NoFlowControl)
              )
      {
          connect(m_pSerialPort, SIGNAL(readyRead()), this, SLOT(dataReceived()));
          setCommStatus(CommunicationStatus::Connected);
          onEvent(CommunicationEventType::Connected);
          ret=1;
      }
      else
      {
          m_pSerialPort->close();
          setCommStatus(CommunicationStatus::DisConnected);
          onEvent(CommunicationEventType::DisConnected);

      }
  }
  else
  {
      m_pSerialPort->close();
      setCommStatus(CommunicationStatus::DisConnected);
      onEvent(CommunicationEventType::DisConnected);
  }

  return ret;
}

qint16 CMDSerialPortCommunicationObject::stopConnect()
{
  qint16 ret=0;
  m_recvLength = 0;
  if(NULL != m_pSerialPort)
  {
      ret=m_pSerialPort->disconnect();
      m_pSerialPort->close();

      QThread::msleep(10);

      if(NULL!=m_pSerialPort)
      {
          delete m_pSerialPort;
          m_pSerialPort=NULL;
      }

      setCommStatus(CommunicationStatus::DisConnected);
      onEvent(CommunicationEventType::DisConnected);
  }
  return ret;
}

qint16 CMDSerialPortCommunicationObject::sendData(quint8 *buffer, qint16 count)
{
    qint16 ret=0;
    if(NULL != m_pSerialPort &&
       getCommStatus()==CommunicationStatus::Connected )
    {

        QT_TRY
        {
             //m_pSerialPort->clear(QSerialPort::AllDirections);
             m_pSerialPort->write((char*)buffer, count);
             //if(m_pSerialPort->waitForBytesWritten(500))
             {
                  //m_pSerialPort->waitForReadyRead(1);
                  ret=1;
             }
            // else//time out
             {
                 ret=-1;
             }
        }
        QT_CATCH(...)
        {

            ret=-1;
        }
    }
    return ret;
}

qint16 CMDSerialPortCommunicationObject::getData(quint8 *buffer, qint16 startIndex, qint16 bufferSize)
{
    qint16 numOfBytes=0;
    if(NULL != m_pSerialPort)
    {
        m_recvBufferMutex.lock();
        QT_TRY
        {
            if (m_recvLength + startIndex > bufferSize)
            {
                numOfBytes = -1;
            }
            else
            {
                memcpy(buffer+startIndex,m_recvBuffer,m_recvLength);
                numOfBytes = m_recvLength;
            }

       }
       QT_CATCH(...)
       {
       }
       m_recvLength = 0;
       m_recvBufferMutex.unlock();
    }
    return numOfBytes;
}

void CMDSerialPortCommunicationObject::dataReceived()
{
    if(NULL != m_pSerialPort)
    {
        m_recvBufferMutex.lock();

        qint64 bytesAvailable = m_pSerialPort->bytesAvailable();
        while(bytesAvailable>0)
        {
            if(bytesAvailable>4096)
            {
                bytesAvailable = 4096;
            }
            qint64 recvLength = m_pSerialPort->read((char*)m_recvBufferTemp,bytesAvailable);

            if(m_recvLength+recvLength>4096)
            {
                m_recvLength = 0;
            }

            memcpy(m_recvBuffer+m_recvLength,m_recvBufferTemp,recvLength);
            m_recvLength = m_recvLength+recvLength;

            bytesAvailable = m_pSerialPort->bytesAvailable();
        }
        m_recvBufferMutex.unlock();
        onEvent(CommunicationEventType::ReceivedData);
    }
}
void CMDSerialPortCommunicationObject::onEvent(const CommunicationEventType eventType)
{
    if(NULL != m_pCurrentEventCallBack)
    {
       m_pCurrentEventCallBack->onEvent(eventType);
    }
}

*/

CMDSerialPortCommunicationObject::CMDSerialPortCommunicationObject()
{
    m_pSerialPort=NULL;
    m_portName = "";
    /*
    m_baudRate = QSerialPort::Baud9600;
    m_stopBits = QSerialPort::OneStop;
    m_dataBits = QSerialPort::Data8;
    m_parity = QSerialPort::NoParity;
    */
    m_qtexFlow = FlowType::FLOW_OFF;
    m_qtexBaud = BaudRateType::BAUD9600;
    m_qtexParity = ParityType::PAR_EVEN;
    m_qtexStopBit = StopBitsType::STOP_1;
    m_qtexDataBit = DataBitsType::DATA_8;

    m_recvLength =0;
    m_commStatus = CommunicationStatus::DisConnected;

    m_pSubCallback = NULL;
}

CMDSerialPortCommunicationObject::~CMDSerialPortCommunicationObject()
{
    if(NULL!=m_pSerialPort)
    {
        m_pSerialPort->close();
        delete m_pSerialPort;
        m_pSerialPort=NULL;
    }
}
/*!
\section 获取、设置通信状态
*/
CommunicationStatus CMDSerialPortCommunicationObject::getCommStatus()
{
    return m_commStatus;
}
void CMDSerialPortCommunicationObject::setCommStatus(CommunicationStatus commStatus)
{
    m_commStatus = commStatus;
}

/*!
\section 获取、设置通信参数
*/
void CMDSerialPortCommunicationObject::setCommParameter(QString commParameter)
{
    m_commParaeter.clear();
    m_commParaeter = commParameter;
    QStringList  paramList = m_commParaeter.split(',');
    if(paramList.size()<5)
    {
        return;
    }
    m_portName = paramList.at(0);//.toUpper();
    uint baud =paramList.at(1).toUInt();
    switch(baud)
    {
        case 1200:
           //m_baudRate=QSerialPort::Baud1200;
           m_qtexBaud = BaudRateType::BAUD1200;
          break;

        case 2400:
           //m_baudRate=QSerialPort::Baud2400;
           m_qtexBaud = BaudRateType::BAUD2400;
          break;
        case 4800:
           //m_baudRate=QSerialPort::Baud4800;
           m_qtexBaud = BaudRateType::BAUD4800;
          break;
        case 9600:
           //m_baudRate=QSerialPort::Baud9600;
           m_qtexBaud = BaudRateType::BAUD9600;
          break;

        case 19200:
           //m_baudRate=QSerialPort::Baud19200;
           m_qtexBaud = BaudRateType::BAUD19200;
          break;
        case 38400:
           //m_baudRate=QSerialPort::Baud38400;
           m_qtexBaud = BaudRateType::BAUD38400;
          break;

        case 57600:
           //m_baudRate=QSerialPort::Baud57600;
           m_qtexBaud = BaudRateType::BAUD57600;
          break;

        case 115200:
           //m_baudRate=QSerialPort::Baud115200;
           m_qtexBaud = BaudRateType::BAUD115200;
          break;
        default:
           //m_baudRate=QSerialPort::UnknownBaud;
           m_qtexBaud = BaudRateType::BAUD9600;
           break;
    }
    uint databits=paramList.at(2).toUInt();
    switch(databits)
    {
        case 5:
            //m_dataBits=QSerialPort::Data5;
            m_qtexDataBit = DataBitsType::DATA_5;
            break;
        case 6:
            //m_dataBits=QSerialPort::Data6;
            m_qtexDataBit = DataBitsType::DATA_6;
            break;
        case 7:
            //m_dataBits=QSerialPort::Data7;
            m_qtexDataBit = DataBitsType::DATA_7;
            break;
        case 8:
            //m_dataBits=QSerialPort::Data8;
            m_qtexDataBit = DataBitsType::DATA_8;
            break;
        default:
            //m_dataBits=QSerialPort::UnknownDataBits;
            m_qtexDataBit = DataBitsType::DATA_8;
            break;
    }
     uint stopBits=paramList.at(3).toUInt();
     switch(stopBits)
     {
        case 0:
            //m_stopBits= QSerialPort::OneStop;
            m_qtexStopBit = StopBitsType::STOP_1;
            break;
        case 1:
             //m_stopBits= QSerialPort::OneAndHalfStop;
             m_qtexStopBit = StopBitsType::STOP_1_5;
            break;
        case 2:
            //m_stopBits= QSerialPort::TwoStop;
            m_qtexStopBit = StopBitsType::STOP_2;
            break;
        default:
            //m_stopBits= QSerialPort::UnknownStopBits;
            m_qtexStopBit = StopBitsType::STOP_1;
            break;
     }
     uint parity=paramList.at(4).toUInt();
     switch(parity)
     {
        case 0:
            //m_parity=QSerialPort::NoParity;
            m_qtexParity = ParityType::PAR_NONE;
            break;
        case 1:
            //m_parity=QSerialPort::OddParity;
            m_qtexParity = ParityType::PAR_ODD;
            break;
        case 2:
            //m_parity=QSerialPort::EvenParity;
            m_qtexParity = ParityType::PAR_EVEN;
            break;
        case 3:
            //m_parity=QSerialPort::MarkParity;
            m_qtexParity = ParityType::PAR_MARK;
            break;
        case 4:
            //m_parity=QSerialPort::SpaceParity;
            m_qtexParity = ParityType::PAR_SPACE;
            break;
        default:
            //m_parity=QSerialPort::UnknownParity;
            m_qtexParity = ParityType::PAR_NONE;
            break;
    }
}

QString CMDSerialPortCommunicationObject::getCommParameter()
{
    return m_commParaeter;
}

bool CMDSerialPortCommunicationObject::connect(qint32 msecs)
{
  bool ret=false;

  m_recvLength = 0;

  disConnect(100);

  if(NULL==m_pSerialPort)
  {
      //m_pSerialPort = new QSerialPort();
      m_pSerialPort = new CMDExtSerialPort(m_portName,QextSerialBase::Polling);
  }
  if(NULL==m_pSerialPort)
  {
      setCommStatus(CommunicationStatus::DisConnected);
      return ret;
  }
  setCommStatus(CommunicationStatus::Connecting);
  //m_pSerialPort->setPortName(m_portName);
  if(m_pSerialPort->open(QIODevice::ReadWrite))
  {
	/*
      if (m_pSerialPort->setBaudRate(m_baudRate)
              && m_pSerialPort->setDataBits(m_dataBits)
              && m_pSerialPort->setParity(m_parity)
              && m_pSerialPort->setStopBits(m_stopBits)
              && m_pSerialPort->setFlowControl(QSerialPort::NoFlowControl)
              )
      {
          setCommStatus(CommunicationStatus::Connected);
          ret=true;
      }
      else
      {
          m_pSerialPort->close();
          setCommStatus(CommunicationStatus::DisConnected);

      }
	  */
      m_pSerialPort->setBaudRate(m_qtexBaud);
      m_pSerialPort->setParity(m_qtexParity);
      m_pSerialPort->setStopBits(m_qtexStopBit);
      m_pSerialPort->setDataBits(m_qtexDataBit);
      m_pSerialPort->setTimeout(1);
      m_pSerialPort->setFlowControl(m_qtexFlow);
      setCommStatus(CommunicationStatus::Connected);
      ret=true;
  }
  else
  {
      m_pSerialPort->close();
      setCommStatus(CommunicationStatus::DisConnected);
  }
  return ret;
}
bool CMDSerialPortCommunicationObject::disConnect(qint32 msecs)
{
  bool ret=false;
  m_recvLength = 0;
  if(NULL != m_pSerialPort)
  {
      if(getCommStatus() == CommunicationStatus::Connected)
        m_pSerialPort->close();

      if(NULL!=m_pSerialPort)
      {
          //m_pSerialPort->deleteLater();
          delete m_pSerialPort;
          m_pSerialPort=NULL;
      }
      setCommStatus(CommunicationStatus::DisConnected);
      ret=true;
  }
  return ret;
}
bool CMDSerialPortCommunicationObject::writeData(quint8 *buffer, qint16 count,qint32 msecs)
{
    bool ret=false;
    if(NULL != m_pSerialPort &&
       getCommStatus()==CommunicationStatus::Connected )
    {

        QT_TRY
        {
             /*
			 m_pSerialPort->clearError();
             m_pSerialPort->clear();
				*/
             /*
             static qint64 index = 1;
             index++;
             QSettings *configIniWrite = new QSettings("C:\\hahaya.ini", QSettings::IniFormat);
             QString strKey = QString("/writeData/DataLength%1_%2").arg(index).arg(count);

             QString strValue="";
             for (int i = 0;  i<count;  i++)
             {
                 strValue = strValue+QString("%1 ").arg(buffer[i], 2, 16, QChar('0')).toUpper();
             }
             configIniWrite->setValue(strKey, strValue);
             delete configIniWrite;

             */
             if(count == m_pSerialPort->write((char*)buffer, count))
             {
                 m_pSerialPort->flush();
                 //if(m_pSerialPort->waitForBytesWritten(msecs))
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

qint64 CMDSerialPortCommunicationObject::readData(quint8 *buffer, qint16 startIndex, qint16 bufferSize,qint32 msecs)
{
    qint64 ret=0;
    if(NULL != m_pSerialPort &&
       getCommStatus()==CommunicationStatus::Connected )
    {
        QT_TRY
        {
            if(m_pSerialPort->bytesAvailable() > 0)
            {
                //qint64 nsize = m_pSerialPort->bytesAvailable();
                qint64 recvLength = m_pSerialPort->read((char*)m_recvBuffer,4096);

                if(recvLength > 0)
                {
                    //USB转串口拔掉后，能进入到这里，但recvLength很大
                    if(recvLength > 4096)
                        return -2;

                    if (recvLength + startIndex > bufferSize)
                    {
                        ret = -1;
                    }
                    else
                    {
                        memcpy(buffer+startIndex,m_recvBuffer,recvLength);
                        ret = recvLength;
                    }
                }
            }
            /*
            int nIndex = 0,nCount = msecs/10;
            while(nIndex <= nCount)
            {
                //if (m_pSerialPort->waitForReadyRead(msecs))
                if(m_pSerialPort->bytesAvailable() > 0)
                {
                    //qint64 nsize = m_pSerialPort->bytesAvailable();
                    qint64 recvLength = m_pSerialPort->read((char*)m_recvBuffer,4096);

                    if(recvLength > 0)
                    {
                        if (recvLength + startIndex > bufferSize)
                        {
                            ret = -1;
                        }
                        else
                        {
                            memcpy(buffer+startIndex,m_recvBuffer,recvLength);
                            ret = recvLength;
                        }
                        break;
                    }
                }
                nIndex++;
                QThread::msleep(10);
            }//end while
            */
        }
        QT_CATCH(...)
        {
        }
    }
    return ret;
}

bool CMDSerialPortCommunicationObject::subscribes(const std::vector<std::string> &entityBrowseNames)
{
    return true;
}

bool CMDSerialPortCommunicationObject::registSubCallback(subCallbackFunc callback, void* contex)
{
    m_pSubCallback = callback;
    return true;
}

void CMDSerialPortCommunicationObject::asyncWriteVariable(QString address, MDDataType type, QVariant value)
{

}
