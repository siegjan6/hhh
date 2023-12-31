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
#ifndef NCRDXMLDEFWRITER_H
#define NCRDXMLDEFWRITER_H

#include "ncreportxmldefwriter.h"

/*!
Subclass of NCReportXMLDefWriter for internal purpuses
 */

/*!

section 功能

【NCReportXMLDefWriter的派生类，用于写XML文件】

*/
class NCRDXMLDefWriter : public NCReportXMLDefWriter
{
public:
	NCRDXMLDefWriter();
	NCRDXMLDefWriter(QString *);

	~NCRDXMLDefWriter();

protected:
	void writeReport();
	
};


#endif
