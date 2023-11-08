/****************************************************************************
* 
*  Copyright (C) 2007-2010 Helta Kft. / NociSoft Software Solutions
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
#include "ncrdformgeometry.h"
#include "info.h"
#include "ncrd_global.h"

#include <QSettings>
#include <QWidget>
#include <QSplitter>
#include <QMainWindow>
#include <QMetaObject>
#include <QGridLayout>
#include <QApplication>
#include <QDesktopWidget>

#define NCRD_DIALOG_GEOMETRY_SAVE_ON

#ifdef Q_WS_X11
	#define NCRD_DIALOG_POSX_TRIM		1
	#define NCRD_DIALOG_POSY_TRIM		27
#else
	#define NCRD_DIALOG_POSX_TRIM		0
	#define NCRD_DIALOG_POSY_TRIM		0
#endif

#if defined(Q_WS_MAC) || defined(Q_OS_MAC)
#define NCRD_DIALOG_MAC_LAYOUT_PREFERRED	6
#endif

NCRDFormGeometry::NCRDFormGeometry(QWidget* parent, QSplitter *splitter, const QString& name )
	: QObject( parent ), m_form(parent), m_splitter(splitter), m_name(name)
{
	if (name.isNull())
		m_name = parent->objectName();
	//restore();
}

NCRDFormGeometry::~NCRDFormGeometry()
{
}

void NCRDFormGeometry::save()
{

#ifdef NCRD_DIALOG_GEOMETRY_SAVE_ON
	QSettings settings;
	settings.setValue( key(), m_form->saveGeometry());
	if (m_splitter) {
		QByteArray state = m_splitter->saveState();
		settings.setValue( key("splitter/"), state );
	}
	if ( isMainWindow() ) {
		QMainWindow *mw = qobject_cast<QMainWindow*>(m_form);
		if (mw)
			settings.setValue( key("state/"), mw->saveState());
	} else {
//#ifdef Q_WS_X11
//		settings.setValue(key("pos/"),m_form->pos());
//#endif
	}
#endif
}

void NCRDFormGeometry::restore()
{
#ifdef NCRD_DIALOG_GEOMETRY_SAVE_ON

	QSettings settings;
	QByteArray state;

	state = settings.value( key() ).toByteArray();
	if (!state.isNull())
		m_form->restoreGeometry(state);

	if (m_splitter) {
		QByteArray state = settings.value( key("splitter/") ).toByteArray();
		if (!state.isNull())
			m_splitter->restoreState( state );
		else
			m_splitter->setSizes( QList<int>() << 150 << 400 );
	}
	if (isMainWindow()) {
		QMainWindow *mw = qobject_cast<QMainWindow*>(m_form);
		if (mw) {
			QByteArray state = settings.value(key("state/")).toByteArray();
			if (!state.isNull())
				mw->restoreState( state );
		}
	}
#ifdef Q_WS_X11
	// fix Linux form "go down" bug
//	if (!state.isNull())
//		m_form->move( m_form->x() - NCRD_DIALOG_POSX_TRIM, m_form->y() - NCRD_DIALOG_POSY_TRIM );
#endif
	fixLayouts();
#endif
}

QString NCRDFormGeometry::key(const QString& prefix) const
{
	return QString("geometry/%1%2").arg(prefix).arg(m_name);
}

bool NCRDFormGeometry::isMainWindow() const
{
	return (QString(m_form->metaObject()->className()) == "QMainWindow");
}

//void NCRDFormGeometry::fixLayouts()
//{
//#if defined(Q_WS_MAC) || defined(Q_OS_MAC)
//	QList<QGridLayout*> list = m_form->findChildren<QGridLayout*>();
//	for (int i = 0; i < list.size(); ++i) {
//		QGridLayout *lay = list.at(i);
//		if (lay) {
//			lay->setVerticalSpacing(NCRD_DIALOG_MAC_LAYOUT_PREFERRED);
//			//lay->setSpacing(NCRD_DIALOG_MAC_LAYOUT_PREFERRED);
//		}

//	}
//#endif
//}

void NCRDFormGeometry::fixLayouts()
{
#if defined(Q_WS_MAC) || defined(Q_OS_MAC)
	QList<QLayout*> list = m_form->findChildren<QLayout*>();
	for (int i = 0; i < list.size(); ++i) {
		QLayout *lay = list.at(i);
		if (lay)
			lay->setSpacing(NCRD_DIALOG_MAC_LAYOUT_PREFERRED);
	}
#endif
}
