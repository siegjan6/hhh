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
#ifndef NCRDFORMGEOMETRY_H
#define NCRDFORMGEOMETRY_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QWidget;
class QSplitter;
QT_END_NAMESPACE

/*!

\section 功能

报表显示界面几何形状管理类
【管理报表文件几何形状，提供存储几何位置大小等接口函数】。

*/
class NCRDFormGeometry : public QObject
{
	Q_OBJECT
public:
	NCRDFormGeometry( QWidget* parent, QSplitter *splitter =0, const QString&  = QString() );
	~NCRDFormGeometry();

public slots:
	void save();
	void restore();

private:
	QWidget *m_form;
	QSplitter *m_splitter;
	QString m_name;

	QString key( const QString &prefix=QString() ) const;
	bool isMainWindow() const;
	void fixLayouts();
};

#endif // NCRDFORMGEOMETRY_H
