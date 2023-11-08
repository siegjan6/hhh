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
     * \brief getTransactionID
     * 获取id
     */
    quint16 getTransactionID();
private:
    quint16 m_transactionID;//!ID值
};
#endif // MDCHANNEL_H
