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
#ifndef NCREPORTXMLCOPYREADER_H
#define NCREPORTXMLCOPYREADER_H

#include "ncreportxmlreader.h"

class NCRDSectionScene;

/*!
This class parses the XML snippet when using Copy/Paste function
*/

/*!

section 功能

【当代码中使用复制／黏贴功能时，用于读取XML文件片段】

*/
class NCRDXMLCopyReader : public NCReportXMLReader
{
public:
	NCRDXMLCopyReader( NCRDSectionScene * );
	~NCRDXMLCopyReader();
	
protected:
    void init();
    void readReport();
    void afterReadItem( NCReportItem* );
	void afterAddItem( NCReportItem* );
	void addItem( NCReportItem*, NCReportSection* );
private:
	NCRDSectionScene *m_section;
	
};

#endif
