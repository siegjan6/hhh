
#ifndef MDCHANNEL_H
#define MDCHANNEL_H

#include "BaseChannel.h"

class CMDChannel:public CBaseChannel
{
public:
    CMDChannel();
    ~CMDChannel();
public:
    /*!
     * \brief getTransactionID:
        * 生成连续ID,用于Tcp通信,ID最大值0xffff,最小值3.
     * \return:
        * quint16型ID.
     */
    quint16 getTransactionID();
private:
    quint16 m_ransactionID;//！当前ID
};
#endif // MDCHANNEL_H
