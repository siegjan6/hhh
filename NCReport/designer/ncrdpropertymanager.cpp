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
#include "ncrdpropertymanager.h"
#include "ncrdmainwindow.h"

#include <QToolBar>
#include <QMenu>

NCRDPropertyManager::NCRDPropertyManager( NCRDMainWindow * parent )
	: QObject( parent ), m_mainwin(parent)
{
	m_menu =0;
	m_toolbar =0;
	m_doc =0;
	m_scene =0;
}

NCRDPropertyManager::~ NCRDPropertyManager()
{
}

void NCRDPropertyManager::setScene(NCRDSectionScene *scene )
{
	m_scene = scene;
}

void NCRDPropertyManager::setDocument(NCRDDocument *doc)
{
	m_doc = doc;
}

QToolBar * NCRDPropertyManager::toolbar()
{
	return m_toolbar;
}

QMenu * NCRDPropertyManager::menu()
{
	return m_menu;
}

void NCRDPropertyManager::buildMenu( QMenu *)
{
}

void NCRDPropertyManager::buildToolBar(QToolBar *)
{
}

// void NCRDPropertyManager::applyPropsToItem(NCReportLabelItem *item)
// {
// }
// 
// void NCRDPropertyManager::applyPropsToManager(NCReportLabelItem *item)
// {
// }

void NCRDPropertyManager::setEnabled(bool set )
{
	if ( m_toolbar )
		m_toolbar->setEnabled(set);
	if ( m_menu )
		m_menu->setEnabled(set);	
}


void NCRDPropertyManager::reset()
{
}

void NCRDPropertyManager::applyChanges( int property )
{
	//if ( m_scene )
	//	m_scene->actionManagerChanged();		
	if ( m_doc )
        m_doc->actionManagerChanged(property);
}
