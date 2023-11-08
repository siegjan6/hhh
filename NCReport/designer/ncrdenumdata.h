/****************************************************************************
*
*  Copyright (C) 2002-2011 Helta Kft. / NociSoft Software Solutions
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

#ifndef NCRDENUMDATA_H
#define NCRDENUMDATA_H

#include "ncreportdata.h"
#include <Qt>

/*!

\section 功能

报表相关控件属性枚举类
【提供了多个静态函数，用于设置相关属性】。

*/
class NCRDEnumData
{
public:
    NCRDEnumData();

	static Qt::Alignment indexToAlignH( int index );
	static Qt::Alignment indexToAlignV( int index );
	static int alignToIndex( const Qt::Alignment align );
	static QString alignToText( const Qt::Alignment align );
	static Qt::BrushStyle indexToBrushStyle( int index );
	static Qt::PenStyle indexToPenStyle( int index );
	static Qt::BGMode indexToBGMode( int index );
	static NCReportData::DataType indexToDataType( int index );
};

#endif // NCRDENUMDATA_H
