#include "MDObjBatchOperationExpert.h"
#include "MDBatchAddWidget.h"

CMDObjBatchOperationExpert::CMDObjBatchOperationExpert()
{
    m_batchAddWidget = new CMDBatchAddWidget();
    connect(m_batchAddWidget,SIGNAL(sendNewJson(QJsonObject)),this,SIGNAL(newJsonCreate(QJsonObject)));
}

CMDObjBatchOperationExpert::~CMDObjBatchOperationExpert()
{
    if(m_batchAddWidget)
    {
        delete m_batchAddWidget;
        m_batchAddWidget = NULL;
    }
}

void CMDObjBatchOperationExpert::setRefJson(const QJsonObject &json)
{
    m_batchAddWidget->setAddJson(json);
    m_batchAddWidget->show();
}

void CMDObjBatchOperationExpert::setErrorInfor(const QString &str)
{
    m_batchAddWidget->setErrorInfor(str);
}

