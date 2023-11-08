#ifndef MDCHANNEL_H
#define MDCHANNEL_H

#include "BaseChannel.h"
#include <QLibrary>
#include <QCoreApplication>
#include "Prodave6.h"



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
    /*!
     * \brief libraryLoad
     * 获取加载dll库指针
     */
    QLibrary* libraryLoad();
    /*!
     * \brief loadSuccessFlag
     * 获取加载成功标志位
     */
    bool loadSuccessFlag();

    CON_TABLE_TYPE* currentConnect(int index);
private:
    quint16 m_ransactionID;//!ID值
    QLibrary *m_pLibraryLoad;
    bool m_bLoadSuccessFlag;
    CON_TABLE_TYPE m_con_Table_Array[64];
};
#endif // MDCHANNEL_H
