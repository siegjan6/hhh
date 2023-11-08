#include "MDChannel.h"

CMDChannel::CMDChannel()
{
    m_transactionID = 3;
}
CMDChannel::~CMDChannel()
{
}

ushort CMDChannel::getTransactionID()
{
    if (m_transactionID >= 0xFFFF)
    {
        m_transactionID = 3;
    }
    else
    {
        m_transactionID++;
    }

    return m_transactionID;
}
