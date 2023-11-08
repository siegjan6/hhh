#include "MDChannel.h"

CMDChannel::CMDChannel()
{
    m_ransactionID = 3;
    m_bLoadSuccessFlag = 0;
    QString strPath = QCoreApplication::applicationDirPath();
    strPath = strPath + "/Prodave6.dll";
    m_pLibraryLoad= NULL;
    m_pLibraryLoad = new QLibrary(strPath);
    if(m_pLibraryLoad)
    {
        m_bLoadSuccessFlag = m_pLibraryLoad->load();
    }
    else
    {
        m_bLoadSuccessFlag = 0;
    }
}
CMDChannel::~CMDChannel()
{
    if(m_pLibraryLoad)
    {
        pfn_UnloadConnection_ex6 unConnect = (pfn_UnloadConnection_ex6)m_pLibraryLoad->resolve("UnloadConnection_ex6");
        unConnect((getDeviceCount()-1));
        if(m_bLoadSuccessFlag)
        {
            m_pLibraryLoad->unload();
        }
        delete m_pLibraryLoad;
        m_pLibraryLoad = NULL;
    }
    m_bLoadSuccessFlag = 0;
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
QLibrary* CMDChannel::libraryLoad()
{
    return m_pLibraryLoad;
}
bool CMDChannel::loadSuccessFlag()
{
    return m_bLoadSuccessFlag;
}
CON_TABLE_TYPE* CMDChannel::currentConnect(int index)
{
    return &m_con_Table_Array[index];
}
