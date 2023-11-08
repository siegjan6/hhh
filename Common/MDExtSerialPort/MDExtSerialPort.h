#ifndef _QEXTSERIALPORT_H_
#define _QEXTSERIALPORT_H_

#include <QtGlobal>

//POSIX CODE
#ifdef Q_OS_WIN32
#include "win_qextserialport.h"
#define QextBaseType  Win_QextSerialPort
#else
#include "posix_qextserialport.h"
#define QextBaseType Posix_QextSerialPort
#endif

#if defined(EXTSERIALPORT_LIBRARY)
#  define EXTSERIALPORTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EXTSERIALPORTSHARED_EXPORT Q_DECL_IMPORT
#endif

class EXTSERIALPORTSHARED_EXPORT CMDExtSerialPort: public QextBaseType
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
