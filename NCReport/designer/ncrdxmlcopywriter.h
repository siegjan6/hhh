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
#ifndef NCRDXMLCOPYWRITER_H
#define NCRDXMLCOPYWRITER_H

#include "ncreportxmlwriter.h"

class NCReportSection;

/*!
This class generates the XML snippet when using Copy/Paste function
 */

/*!

section 功能

【当代码中使用复制／黏贴功能时，用于写入XML文件片段】

*/
class NCRDXMLCopyWriter : public NCReportXMLWriter
{
public:
	NCRDXMLCopyWriter( NCReportSection* sect, QString* );
	~NCRDXMLCopyWriter();

protected:
	void writeReport();
	void writeItems( NCReportSection* );
	
private:
	NCReportSection *section;
};

#endif
