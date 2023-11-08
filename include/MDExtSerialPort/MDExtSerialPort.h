
#ifndef _QEXTSERIALPORT_H_
#define _QEXTSERIALPORT_H_

#ifdef Q_OS_LINUX
#include "posix_qextserialport.h"
#define QextBaseType Posix_QextSerialPort
#else
#include "win_qextserialport.h"
#define QextBaseType Win_QextSerialPort
#endif


class CMDExtSerialPort: public QextBaseType
{
	Q_OBJECT
	
	public:
		typedef QextSerialBase::QueryMode QueryMode;

        CMDExtSerialPort();
        CMDExtSerialPort(const QString & name, QueryMode mode = QextSerialBase::Polling);
        CMDExtSerialPort(PortSettings const& s, QueryMode mode = QextSerialBase::Polling);
        CMDExtSerialPort(const QString & name, PortSettings const& s, QueryMode mode = QextSerialBase::Polling);
        CMDExtSerialPort(const CMDExtSerialPort& s);
        CMDExtSerialPort& operator=(const CMDExtSerialPort&);
        virtual ~CMDExtSerialPort();

};

#endif
