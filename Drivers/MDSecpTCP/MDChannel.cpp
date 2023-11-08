#include "MDChannel.h"

CMDChannel::CMDChannel()
{
    m_ransactionID = 3;//!设定ID初始值
}
CMDChannel::~CMDChannel()
{
}
/*!
 * \brief CMDChannel::getTransactionID:
    *返回连续quint16类型的ID
 */
quint16 CMDChannel::getTransactionID()
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
