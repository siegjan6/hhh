#include "MDRefreshDataBlock.h"
#include "MDDevice.h"
#include "MDDataBlock.h"
#include"MDDataBlockDialog.h"
MDRefreshDataBlock::MDRefreshDataBlock(CMDDataBlockDialog *pDataBlockDialog,QObject *parent) :
    QObject(parent)
{
    m_pDataBlockDialog = pDataBlockDialog;
    pTimer = new QTimer(this);
    connect(pTimer, SIGNAL(timeout()), this, SLOT(getTagTableStatu()));
}
MDRefreshDataBlock::~MDRefreshDataBlock()
{
    if(NULL!=pTimer)
    {
        if(pTimer->isActive())
        {
            pTimer->stop();
            delete pTimer;
            pTimer = NULL;
        }
        else
        {
            delete pTimer;
            pTimer = NULL;
        }
    }
}
void MDRefreshDataBlock::getTagTableStatu()
{

}
