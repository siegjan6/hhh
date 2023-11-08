/****************************************************************************
* 
*  Copyright (C) 2002-2008 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: nszabo@helta.hu, info@nocisoft.com
*  Web: www.nocisoft.com
* 
*  This file is part of the NCReport reporting software
* 
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  nszabo@helta.hu if any conditions of this licensing are not clear
*  to you.
* 
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* 
****************************************************************************/

#ifndef NCRDCUSTOMDATASOURCE_H
#define NCRDCUSTOMDATASOURCE_H

#include "ncreportdatasource.h"

/*!
Quasi datasource class for creating it in the designer.\n
Internally used only.
*/
class NCRDCustomDataSource : public NCReportDataSource
{
	Q_OBJECT
public:
	NCRDCustomDataSource( QObject *parent=0 );
	~NCRDCustomDataSource();
	
    virtual bool open(OPEN_MODEL model);
	virtual bool close();
	virtual bool first();
	virtual bool last();
    virtual bool next(bool isSamplingPageBreak = false);
    virtual bool previous();
	virtual bool seek(int index);
	virtual int size() const;
	virtual QVariant value( const QString&, bool* ok=0, int nQTDataRole = -1) const;
	virtual QVariant value( int, bool* ok=0, int nQTDataRole = -1 ) const;
	virtual bool read( NCReportXMLReader* );
	virtual bool write( NCReportXMLWriter* );
	virtual bool hasNext();
	virtual bool isValid() const;
	virtual int columnIndexByName(const QString& columnname) const;

};

#endif
