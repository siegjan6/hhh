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
#ifndef NCRDTESTDATAWRITER_H
#define NCRDTESTDATAWRITER_H

#include <QXmlStreamWriter>

QT_BEGIN_NAMESPACE
class QStandardItemModel;
QT_END_NAMESPACE

/*!
Test data XML writer
*/

/*!

\section 功能

【写测试数据到XML文件管理类】
＃＃＃暂时屏蔽测试数据窗体相关功能＃＃＃

*/
class NCRDTestDataWriter : public QXmlStreamWriter
{
	
public:
    NCRDTestDataWriter();
    ~NCRDTestDataWriter();

	QStandardItemModel* model();
	void setModel( QStandardItemModel* );
	
	bool write( const QString& fname );
	bool write(QIODevice *device );

private:
	QString basetag;
	QStandardItemModel *m_model;
	
	void writeContent();
};

#endif
