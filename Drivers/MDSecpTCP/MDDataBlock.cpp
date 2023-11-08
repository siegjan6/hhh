#include "MDDataBlock.h"
#include <QStringList>
#include "MDProtocolCommand.h"
#include "MDDriverFunctions.h"
#include "PropertyBrowser.h"
#include "MDVarInformation.h"
CMDDataBlock::CMDDataBlock()
{
    m_tabNumber=1;
    m_dataTabNumber=1;
    m_dataBufferSize=0;
    m_dataBuffer=NULL;
    setEnabled(true);
    //*********************属性框变量*********************//
    this->initTranslateNameAndPropertyType();
    m_tabVar_Pro = "";
    //**************************************************//
}
CMDDataBlock::~CMDDataBlock()
{
    if(NULL !=m_dataBuffer)
    {
        delete []m_dataBuffer;
    }
}

bool CMDDataBlock::start()
{
    return CBaseDataBlock::start();
}

bool CMDDataBlock::stop()
{
    return CBaseDataBlock::stop();
}
void CMDDataBlock::copy(IDataBlock *otherDataBlock)
{
    if(NULL== otherDataBlock)
    {
        return;
    }
    CBaseDataBlock::copy(otherDataBlock);
    m_tabNumber= ((CMDDataBlock*)otherDataBlock)->tabNumber();
    m_dataTabNumber = ((CMDDataBlock*)otherDataBlock)->dataTabNumber();
 }


void CMDDataBlock::setTabNumber(qint16 number)
{
    m_tabNumber = number;
}

qint16 CMDDataBlock::tabNumber()
{
    return m_tabNumber;
}


void CMDDataBlock::setDataTabNumber(qint16 number)
{
    m_dataTabNumber = number;
}

qint16 CMDDataBlock::dataTabNumber()
{
    return m_dataTabNumber;
}


bool CMDDataBlock::getDataBufferUShortValue(qint16 internalAddress,  quint16 &data)
{
    bool bRet = false;
    QMutexLocker locker(&m_dataBufferMutex);

    if (NULL == m_dataBuffer || m_dataBufferSize<=0)
    {
       return bRet;
    }

    if (internalAddress < m_dataBufferSize-1 && internalAddress>=0)
    {
        quint8 tempbuffer[4];
        tempbuffer[0] = m_dataBuffer[internalAddress + 1];
        tempbuffer[1] =  m_dataBuffer[internalAddress];
        memcpy(&data,tempbuffer,2);
        bRet = true;
    }
    return bRet;
}

//获取数据缓冲区
bool CMDDataBlock::getDataBuffer(quint8* dataBuf, qint16 offset, qint16 length)
{
    bool bRet = false;
    QMutexLocker locker(&m_dataBufferMutex);
    if (NULL == m_dataBuffer || m_dataBufferSize<=0)
    {
       return bRet;
    }
    memcpy(dataBuf,m_dataBuffer+offset,length);

    bRet = true;
    return bRet;
}

qint16 CMDDataBlock::getDataBufferSize()
{
    return m_dataBufferSize;
}
void CMDDataBlock::calculateDataBufferSize()
{

}

bool CMDDataBlock::checkVarAddress(IMDRTDBVariable* nsVar,QString &error)
{
    bool ret = false;

    if(NULL!=nsVar)
    {
        return true;
    }
    if (NULL == nsVar)
    {
        return ret;
    }
    if (nsVar->address().isNull() || nsVar->address().isEmpty())
    {
        return ret;
    }

    QT_TRY
    {

   }
   QT_CATCH (...)
   {

   }
   return ret;
}

void CMDDataBlock::initialVariables()
{
    QString error;
    for (int varIndex = 0; varIndex < getVariableCount(); varIndex++)
    {
        checkVarAddress(getVariableAt(varIndex),error) ;
    }
}

void CMDDataBlock::refreshVariables()
{
    bool bRightValue = false;
    bool bAddress  = false;
    bool bDataType  = false;
    QVariant varDataValue;
    CMDDevice * pDev = (CMDDevice *)device();
    if(NULL == pDev)
    {
        return;
    }
    int changeVarCount =0;
    IMDRTDBVariable *lastChangeVar = NULL;
    //!变化变量列表
    QList<IMDRTDBVariable*> changevarList;
    for (int varIndex = 0; varIndex < getVariableCount(); varIndex++)
    {
        IMDRTDBVariable *nsVar = getVariableAt(varIndex);
        if (NULL == nsVar)
        {
            continue;
        }
        if (nsVar->address().isNull() || nsVar->address().isEmpty())
        {
            bAddress = true;
        }
        else
        {
            QStringList addressSplit = nsVar->address().split(".");
            if(addressSplit.count()<4)
            {
                bAddress = true;
            }
            else
            {
                QString tempName = addressSplit[2];
                tempName.remove(" ");
                if(pDev->nameTabMapRun.contains(tempName))
                {
                    NameAndPro::iterator varPrope = pDev->nameTabMapRun.find(tempName);
                    MDVarPrope *pVar = varPrope.value();
                    if(m_dataBufferVariantMap.contains(pVar->m_tagID))
                    {
                        QMap<qint16,QVariant>::iterator varVariant = m_dataBufferVariantMap.find(pVar->m_tagID);
                        MDDataType tempType = nsVar->originalDataType();
                        switch (tempType)
                        {
                        case MDDataType::Bool:
                        {
                            if(pVar->m_tagTyp == 1)
                            {
                                varDataValue = varVariant.value();
                                bRightValue = true;                               
                            }
                            else
                            {
                                bDataType = true;
                            }

                        }
                            break;
                        case MDDataType::SByte:
                        {
                            if(pVar->m_tagTyp == 8)
                            {
                                varDataValue = varVariant.value();
                                bRightValue = true;
                            }
                            else
                            {
                                bDataType = true;
                            }
                        }
                            break;
                        case MDDataType::Byte:
                        {
                            if(pVar->m_tagTyp == 2 || pVar->m_tagTyp == 5)
                            {
                                varDataValue = varVariant.value();
                                bRightValue = true;          
                            }
                            else
                            {
                                bDataType = true;
                            }
                        }
                            break;
                        case MDDataType::Int16:
                        {
                            if(pVar->m_tagTyp == 9)
                            {
                                varDataValue = varVariant.value();
                                bRightValue = true;               
                            }
                            else
                            {
                                bDataType = true;
                            }

                        }
                            break;
                        case MDDataType::UInt16:
                        {
                            if(pVar->m_tagTyp == 3||pVar->m_tagTyp == 6)
                            {
                                varDataValue = varVariant.value();
                                bRightValue = true;   
                            }
                            else
                            {
                                bDataType = true;
                            }

                        }
                            break;
                        case MDDataType::Int32:
                        {
                            if(pVar->m_tagTyp == 10)
                            {
                                varDataValue = varVariant.value();
                                bRightValue = true;
                            }
                            else
                            {
                                bDataType = true;
                            }

                        }
                            break;
                        case MDDataType::UInt32:
                        {
                            if(pVar->m_tagTyp == 4||pVar->m_tagTyp == 7)
                            {
                                varDataValue = varVariant.value();
                                bRightValue = true;
                            }
                            else
                            {
                                bDataType = true;
                            }

                        }
                            break;
                        case MDDataType::Float:
                        {
                            if(pVar->m_tagTyp == 11)
                            {
                                varDataValue = varVariant.value();
                                bRightValue = true;
                            }
                            else
                            {
                                bDataType = true;
                            }

                        }
                            break;
                        case MDDataType::String:
                        {
                            if(pVar->m_tagTyp == 20)
                            {
                                varDataValue = varVariant.value();
                                bRightValue = true;
                            }
                            else
                            {
                                bDataType = true;
                            }

                        }
                            break;
                        default:
                            break;
                        }
                    }
                    else
                    {
                        bAddress = true;
                    }
                }
                else
                {
                    bAddress = true;
                }
            }
        }
        if(true ==bAddress)
        {
            bAddress = false;
            nsVar->refreshData(varDataValue, timeStamp(), MDVariableDataQuality::AddressError, false);
        }
        else
        {
            if(true ==bDataType)
            {
                bDataType = false;
                nsVar->refreshData(varDataValue, timeStamp(), MDVariableDataQuality::DataTypeError, false);
            }
            else if(true == bRightValue)
            {
                bRightValue = false;
                if(nsVar->refreshData(varDataValue, timeStamp(), quality(), false))
                {
                    lastChangeVar = nsVar;
                    changeVarCount++;
                    changevarList.append(nsVar);
                }
//                bRightValue = false;
//                nsVar->refreshData(varDataValue, timeStamp(), quality(), false);
            }

        }
    }
    if(changeVarCount>0 && lastChangeVar!=NULL)
    {
        lastChangeVar->notifyDeviceVariablesChange(changevarList);
        changevarList.clear();
    }
}

void CMDDataBlock::refreshVariablesQuality()
{
    IMDRTDBVariable *lastChangeVar = NULL;
    for (int varIndex = 0; varIndex < getVariableCount(); varIndex++)
    {
        IMDRTDBVariable *nsVar = getVariableAt(varIndex);
        if (NULL != nsVar)
        {
            lastChangeVar =nsVar;
            nsVar->refreshData(QVariant(), timeStamp(), quality(), true);
        }
    }
    if(lastChangeVar!=NULL)
    {
        QList<IMDRTDBVariable*> changevarList;
        lastChangeVar->notifyDeviceVariablesChange(changevarList,true);
        changevarList.clear();
    }
//    for (int varIndex = 0; varIndex < getVariableCount(); varIndex++)
//    {
//        IMDRTDBVariable *nsVar = getVariableAt(varIndex);
//        if (NULL != nsVar)
//        {
//            nsVar->refreshData(NULL, timeStamp(), quality(), true);
//        }
//    }
}


CBaseProtocolCommand *CMDDataBlock::createCommand(ProtocolCommandType cmdType)
{
    CMDProtocolCommand *cmd = new CMDProtocolCommand();
    cmd->setChannel (getChannel());
    cmd->setDataBlock (this);
    cmd->setDevice(device());
    cmd->setCommandType(cmdType);
    cmd->setCommandResult(ProtocolCommandResult::None);
    return cmd;
}
//分析命令，将数据拷贝到数据区，并更新变量
void CMDDataBlock::parseCommand(CBaseProtocolCommand *cmd)
{
    if(NULL == cmd)
    {
        return;
    }
    CMDProtocolCommand  *pProtocoCommand = (CMDProtocolCommand*)cmd;

    if(cmd->getCommandType() == ProtocolCommandType::Read)
    {
        if (cmd->getCommandResult() == ProtocolCommandResult::Right)
        {
           QMutexLocker locker(&m_dataBufferMutex);
           m_dataBufferVariantMap.clear();
           for(QMap<qint16,QVariant>::iterator i =pProtocoCommand->tempIDAndVariantMap.begin();i !=pProtocoCommand->tempIDAndVariantMap.end();++i)
           {
                m_dataBufferVariantMap.insert(i.key(),i.value());
           }
           this->onReceiveCommand(true, cmd->getCommandResult());
           refreshVariables();
        }
        else
        {
           MDVariableDataQuality oldQuality = quality();
           this->onReceiveCommand(true, cmd->getCommandResult());
           if (oldQuality != quality())
           {
               refreshVariablesQuality();
           }
        }
    }
    else
    {
         this->onReceiveCommand(false, cmd->getCommandResult());
    }

}
void CMDDataBlock::getVariableAddressList(QStringList &varAddressList)
{
    CMDDevice * pDev = (CMDDevice *) device();
    if(NULL == pDev)
    {
        return;
    }
    if(pDev->m_configFlag == 1)
    {
        if(pDev->dataTabNOAndPrope.contains(m_dataTabNumber))
        {
            DataTabNoAndPrope::iterator currentDataNOAndPrope = pDev->dataTabNOAndPrope.find(m_dataTabNumber);
            IDAndPro tempPropMap = currentDataNOAndPrope.value();
            for(IDAndPro::iterator i = tempPropMap.begin();i!=tempPropMap.end();++i)
            {
                 MDVarPrope * pVarPrope = i.value();
                 QString strTempType = "";
                 switch(pVarPrope->m_tagTyp)
                 {
                 case 0:
                     strTempType= "Undef";
                     break;
                 case 1:
                     strTempType= "Bool";
                     break;
                 case 2:
                     strTempType= "Byte";
                     break;
                 case 3:
                     strTempType= "Word";
                     break;
                 case 4:
                     strTempType= "Dword";
                     break;
                 case 5:
                     strTempType= "Usint";
                     break;
                 case 6:
                     strTempType= "Uint";
                     break;
                 case 7:
                     strTempType= "Udint";
                     break;
                 case 8:
                     strTempType= "Sint";
                     break;
                 case 9:
                     strTempType= "Int";
                     break;
                 case 10:
                     strTempType= "Dint";
                     break;
                 case 11:
                     strTempType= "Real";
                     break;
                 case 20:
                     strTempType= "String";
                     break;
                 case 24:
                     strTempType= "Bit";
                     break;
                 default:
                     break;
                 }
                 varAddressList.append
                 (QString("%1.%2.%3.%4")
                                       .arg(name())
                                       .arg(m_dataTabNumber)
                                       .arg(pVarPrope->m_tagName)
                                       .arg(strTempType)
                                       );
            }
        }
    }
}
void CMDDataBlock::saveConfiguration(QDomDocument &doc,QDomElement &parentElement)
{
    if(doc.isNull()||parentElement.isNull())
    {
        return;
    }
    QDomElement variableElement = doc.createElement("DataBlock");
    parentElement.appendChild(variableElement);

    QDomElement element = doc.createElement("Name");
    variableElement.appendChild(element);
    QDomText elementText = doc.createTextNode(this->name());
    element.appendChild(elementText);

    element = doc.createElement("Description");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(this->description());
    element.appendChild(elementText);

    element = doc.createElement("Enabled");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(this->isEnabled()?"1":"0");
    element.appendChild(elementText);

    /*element = doc.createElement("TabNumber");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(QString("%1").arg(this->tabNumber()));
    element.appendChild(elementText);*/

    element = doc.createElement("DataTabNumber");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(QString("%1").arg(this->dataTabNumber()));
    element.appendChild(elementText);

    element = doc.createElement("PollRate");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(QString("%1").arg(this->pollRate()));
    element.appendChild(elementText);

}
void CMDDataBlock::serialize(CSortJsonObject &json)
{
    json.insertSimpleValue("Name",this->name());
    json.insertSimpleValue("Description",this->description());
    json.insertSimpleValue("Enabled",this->isEnabled()?QString("1"):QString("0"));
    json.insertSimpleValue("DataTabNumber",QString("%1").arg(this->dataTabNumber()));
    json.insertSimpleValue("PollRate",QString("%1").arg(this->pollRate()));
}



void CMDDataBlock::readConfiguration(QDomElement &domElement)
{
    if(domElement.isNull())
    {
        return;
    }
    QDomElement element = domElement.firstChildElement();
    while ( !element.isNull() )
    {
        if(0==QString::compare(element.tagName(),"Name", Qt::CaseInsensitive))
        {
            this->setName(element.text());
        }
        else if(0==QString::compare(element.tagName(),"Description", Qt::CaseInsensitive))
        {
            this->setDescription(element.text());
        }
        else if(0==QString::compare(element.tagName(),"Enabled", Qt::CaseInsensitive))
        {
            if(element.text().toInt()!=0)
            {
                this->setEnabled(true);
            }
            else
            {
                this->setEnabled(false);
            }
        }
        /*else if(0==QString::compare(element.tagName(),"TabNumber", Qt::CaseInsensitive))
        {
            this->setTabNumber(element.text().toInt());
        }*/
        else if(0==QString::compare(element.tagName(),"DataTabNumber", Qt::CaseInsensitive))
        {
            this->setDataTabNumber(element.text().toInt());
        }
        else if(0==QString::compare(element.tagName(),"PollRate", Qt::CaseInsensitive))
        {
            this->setPollRate(element.text().toInt());
        }
        element = element.nextSiblingElement();
    }
}
void CMDDataBlock::deserialize(const QJsonObject &json)
{
    this->setName(json.value("Name").toString(""));
    this->setDescription(json.value("Description").toString(""));
    this->setEnabled(json.value("Enabled").toString("1").toInt());
    this->setDataTabNumber(json.value("DataTabNumber").toString("1").toInt());
    this->setPollRate(json.value("PollRate").toString("1000").toInt());
}
//**********************************属性框翻译与提示***************************//
void CMDDataBlock::initTranslateNameAndPropertyType()
{
    m_translateName.insert(QString("GUID_DataBlock"),QString("数据块"));
    m_translateName.insert(QString("TabVar"),QString("数据表变量"));
    m_propertyTip.insert(QString("TabVar"),QString("数据表变量"));
}
QMap<QString,QVariant> CMDDataBlock::translateName() const//!翻译函数
{
    return m_translateName;
}
QMap<QString, QVariant> CMDDataBlock::propertyTip() const//!提示框函数
{
    return m_propertyTip;
}
QString CMDDataBlock::tabVar_Pro()
{
    return m_tabVar_Pro;
}
void CMDDataBlock::setTabVar_Pro(QString tabVar)
{
    CMDVarInformation dlg;
    dlg.setDataBlock(this);
    dlg.exec();
}
//*****************************************************************************//
