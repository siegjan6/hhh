#include "MDChannel.h"

CMDChannel::CMDChannel()
{
    m_ransactionID = 3;
}
CMDChannel::~CMDChannel()
{
}

ushort CMDChannel::getTransactionID()
{
    if (m_ransactionID >= 0xFFFF)
    {
        m_ransactionID = 3;
    }
    else
    {
        m_ransactionID++;
    }

    return m_ransactionID;
}
