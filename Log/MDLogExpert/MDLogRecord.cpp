#include "MDLogRecord.h"
#include "MDDatabaseRecordset.h"
#include "MDDataDefine.h"
#include "MDDataGroupFactory.h"
#include "MDActionSPSession.h"
#include "MDLogContacter.h"

CMDLogRecord::CMDLogRecord()
{
    m_contacter = NULL;
}

CMDLogRecord::~CMDLogRecord()
{
    m_fields.removeAllItem();
}

CMDLogRecord::CMDLogRecord(const CMDLogRecord& obj)
{
    copy(&obj);
}

CMDLogRecord& CMDLogRecord::operator = (const CMDLogRecord& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDLogRecord::copy(const CMDLogRecord* source)
{
    m_fields = source->m_fields;

    return true;
}

/*!
\section 初始化
*/
bool CMDLogRecord::init(const CMDLogMessage& msg, IMDDataGroup* logGroup, CMDLogContacter* contacter)
{
    m_contacter = contacter;

    if(!logGroup)
        return false;

    m_fields.setDataGroupName(logGroup->name());
    m_fields.setRecordDateTime(msg.dt());
    for(int i = 0; i < logGroup->itemCount(); i++)
    {
        IMDDataItem* groupItem = logGroup->getItem(i);
        QVariant value = initItemValue(groupItem->name(), msg);

        IMDDataItem* varItem = CMDDataGroupFactory::createDataItem();
        varItem->setName(groupItem->name());
        varItem->setData(value);
        varItem->setDataType(groupItem->dataType());
        varItem->setDescription(groupItem->name());
        m_fields.addItem(varItem);
    }

    return true;
}

void CMDLogRecord::setData(const CMDLogMessage& msg)
{
    m_fields.setRecordDateTime(msg.dt());
    for(int i = 0; i < m_fields.itemCount(); i++)
    {
        IMDDataItem* item = m_fields.getItem(i);
        QVariant value = initItemValue(item->name(), msg);
        item->setData(value);
    }
}

QVariant CMDLogRecord::initItemValue(const QString& itemName, const CMDLogMessage& msg)
{
    QVariant value;//= groupItem->data();
    if(0 == itemName.compare(LOG_RECORDTIME, Qt::CaseInsensitive))
    {
        value = QVariant(msg.dt());
    }
    else if(0 == itemName.compare(LOG_RECORDTIME_MS, Qt::CaseInsensitive))
    {
        value = QVariant(msg.dt().time().msec());
    }
    else if(0 == itemName.compare(LOG_MESSAGE, Qt::CaseInsensitive))
    {
        value = QVariant(msg.msg());
    }
    else if(0 == itemName.compare(LOG_TYPE, Qt::CaseInsensitive))
    {
        value = QVariant((int)msg.object()->type());
    }
    else if(0 == itemName.compare(LOG_NAME, Qt::CaseInsensitive))
    {
        value = QVariant(msg.spSession()->actionSP().actionIDObjName());
    }
    else if(0 == itemName.compare(LOG_USER, Qt::CaseInsensitive))
    {
        value = QVariant(m_contacter->currentAccount());
    }
    else if(0 == itemName.compare(LOG_PROTECTMODE, Qt::CaseInsensitive))
    {
        value = QVariant((int)msg.spSession()->actionSP().protectMode());
    }
    else if(0 == itemName.compare(LOG_OPERATER, Qt::CaseInsensitive))
    {
        value = QVariant(msg.spSession()->operatorInput().sigID());
    }
    else if(0 == itemName.compare(LOG_OPERATETIME, Qt::CaseInsensitive))
    {
        value = QVariant(msg.spSession()->operatorOutput().sigTime());
    }
    else if(0 == itemName.compare(LOG_VERIFIER, Qt::CaseInsensitive))
    {
        value = QVariant(msg.spSession()->verifierInput().sigID());
    }
    else if(0 == itemName.compare(LOG_VERIFYTIME, Qt::CaseInsensitive))
    {
        value = QVariant(msg.spSession()->verifierOutput().sigTime());
    }
    else if(0 == itemName.compare(LOG_OPERATECOMMENT, Qt::CaseInsensitive))
    {
        value = QVariant(msg.spSession()->operatorInput().sigComment());
    }
    else if(0 == itemName.compare(LOG_VERIFYCOMMENT, Qt::CaseInsensitive))
    {
        value = QVariant(msg.spSession()->verifierInput().sigComment());
    }
    else
    {
        QVariant varValue = m_contacter->variableValue(itemName);
        if(varValue.isValid())
            value = varValue;
    };

    return value;
}

/*!
\section 在 recordSet 中增加一行，并填充该行的内容
*/
void CMDLogRecord::toDatabaseRecord(CMDDatabaseRecordset* recordSet)
{
    int row = recordSet->addRow();

    int colCount = m_fields.itemCount();
    for(int i = 0; i < colCount; i++)
    {
        QVariant data = m_fields.getItem(i)->data();
        recordSet->setValue(row, i, data);
    }
}
