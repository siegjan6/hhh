/*!
\author luyj 2021.09.14
\section OPCUaServer 全局静态指针
*/
#ifndef IUASERVER_H
#define IUASERVER_H

#include <QUaServer>
//class UaServerGlobal
//{
//public:
//	static void* m_pUaServer;
//};

//void* UaServerGlobal::m_pUaServer = 0;
#pragma comment(linker, "/SECTION:Shared,RWS")

#pragma data_seg("Shared")

void* m_pUaServer = 0;

#pragma data_seg()

#endif // IUASERVER_H
