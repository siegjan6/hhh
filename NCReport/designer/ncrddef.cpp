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
#include "ncrddef.h"
#include "ncrdsectionscene.h"
#include "ncrdmainwindow.h"
#include "ncrddocument.h"

NCRDDef::NCRDDef(NCRDMainWindow * mw, NCRDDocument * doc, QObject * parent) 
	: NCReportDef( parent )
{
	m_mainwin = mw;
	m_document = doc;
}

NCRDDef::~ NCRDDef()
{
}

NCReportSection * NCRDDef::createReportSection()
{
	return new NCRDSectionScene( m_mainwin, m_document, this );
}


NCRDMainWindow* NCRDDef::mainWindow()
{
	return m_mainwin;
}
