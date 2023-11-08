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
#ifndef NCRDPROPERTYMANAGER_H
#define NCRDPROPERTYMANAGER_H

#include <QObject>
#include <QPointer>

#include "ncrdsectionscene.h"
#include "ncrddocument.h"

QT_BEGIN_NAMESPACE
class QToolBar;
class QMenu;
QT_END_NAMESPACE

class NCRDMainWindow;
class NCReportItem;
class NCRDSectionScene;

/*!
This is an abstract manager class provides managing the connection between report items and the specified toolbars.\n
*/

/*!

\section 功能

属性管理类
【这是一个抽象类提供管理报表条目和指定工具栏之间的连接。】

*/
class NCRDPropertyManager : public QObject
{
	Q_OBJECT
public:
    NCRDPropertyManager( NCRDMainWindow* parent );
    ~NCRDPropertyManager();

    enum Property { All=0x0, FontFamily, FontSize, FontBold, FontItalic, FontUnderline, Alignment, Color, LineWidth, LineColor, FillColor, FillPattern, LineStyle };
	
	void setDocument( NCRDDocument* );
	void setScene( NCRDSectionScene* );
	QToolBar* toolbar();
	QMenu* menu();
	
	virtual void setEnabled( bool );
	virtual void buildMenu(QMenu*);
	virtual void buildToolBar(QToolBar*);
    virtual void applyChangesToItem( NCReportItem* item, int property ) =0;
    virtual void applyChangesToManager( NCReportItem* item ) =0;
	virtual void reset();
    virtual void applyChanges(int property);
	
protected:
	QToolBar* m_toolbar;
	QMenu* m_menu;
	NCRDMainWindow *m_mainwin;
	QPointer<NCRDSectionScene> m_scene;
	QPointer<NCRDDocument> m_doc;
};

#endif
