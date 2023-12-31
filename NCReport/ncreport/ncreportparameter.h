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
#ifndef NCREPORTPARAMETER_H
#define NCREPORTPARAMETER_H

//#include <QString>
//#include <QVariant>
//#include <QHash>

#include "ncreportdata.h"

/*!
Report parameter class. The class is obsolete, not used anymore.
*/

/*!

section 功能

报表参数管理类
【此类已经过期，不再使用】

*/
class NCREPORTSHARED_EXPORT NCReportParameter : public NCReportData
{
public:
	NCReportParameter();
	NCReportParameter( const QString& id, const QVariant& value, NCReportData::DataType = NCReportData::Text );
	~NCReportParameter();

	/*!
	Evaluates the parameter's value into a string expression
	*/
	//static void evaluate( QString&, const NCReportParameter& par );
	//static void evaluate( QString&, const QHash<QString,NCReportParameter>& parameters );
	
};


#endif
