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
#ifndef NCRDSPECTEXTOPTWIDGET_H
#define NCRDSPECTEXTOPTWIDGET_H

#include "ui_spectextoptwidget.h"

/*!

\section 功能

【文本属性对话框相配置子界面管理类，嵌入在字段设置对话框中】

*/
class NCRDSpecTextOptWidget : public QWidget, public Ui::NCRDSpecTextOptWidget
{
    Q_OBJECT

public:
    explicit NCRDSpecTextOptWidget(QWidget *parent = 0);
	void setLabelMode();
	void setAlignment( const Qt::Alignment );
	Qt::Alignment getAlignment( const Qt::Alignment ) const;
};

#endif // NCRDSPECTEXTOPTWIDGET_H
