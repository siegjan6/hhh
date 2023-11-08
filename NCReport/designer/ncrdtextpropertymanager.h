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
#ifndef NCRDTEXTPROPERTYMANAGER_H
#define NCRDTEXTPROPERTYMANAGER_H

#include "ncrdpropertymanager.h"
#include <QFont>
#include <QColor>

QT_BEGIN_NAMESPACE
class QAction;
class QFontComboBox;
class QComboBox;
class QActionGroup;
QT_END_NAMESPACE

/*!
This is a manager class provides managing the connection between text/label report items and the specified toolbar.\n
 */

/*!

section 功能

【提供文本/标签和指定的工具栏之间的连接的管理类】

*/
class NCRDTextPropertyManager : public NCRDPropertyManager
{
	Q_OBJECT
public:
    NCRDTextPropertyManager( NCRDMainWindow* parent );
    ~NCRDTextPropertyManager();
	
	void setEnabled( bool );
    void applyChangesToItem( NCReportItem* item, int property );
    void applyChangesToManager( NCReportItem* item );
	void buildMenu(QMenu*);
	void buildToolBar(QToolBar*);
	void reset();
    void setAlignment(Qt::Alignment a);
			
public slots:
    void textAlignHorizontal( QAction* );
    void textAlignVertical( QAction* action);
    void textBold();
	void textItalic();
	void textUnderline();
	void textColor();
    void textFamily(const QString &f);
    void textSize(const QString &p);
	
private:
    void fontChanged(const QFont &f);
    void colorChanged(const QColor &c);
    void alignmentChanged(Qt::Alignment a);
	
    QAction *actAlignLeft;
    QAction *actAlignRight;
    QAction *actAlignHCenter;
    QAction *actAlignJustify;
    QAction *actAlignTop;
    QAction *actAlignVCenter;
    QAction *actAlignBottom;
    QAction *actBold;
    QAction *actItalic;
    QAction *actUnderline;
    QAction *actColor;
    QActionGroup *actGroupHAlign;
    QActionGroup *actGroupVAlign;

    QFontComboBox *comboFont;
    QComboBox *comboSize;
	QFont m_font;
	Qt::Alignment m_alignment;
	QColor m_color;
};


#endif
