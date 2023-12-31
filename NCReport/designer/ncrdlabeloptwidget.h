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

#ifndef NCRDLABELOPTWIDGET_H
#define NCRDLABELOPTWIDGET_H

#include <QWidget>
#include "ui_labeloptwidget.h"
/*
namespace Ui {
	class NCRDLabelOptWidgetUI;
}
*/

/*!

\section 功能

标签选项类

*/
class NCRDLabelOptWidget : public QWidget, public Ui::NCRDLabelOptWidgetUI
{
    Q_OBJECT

public:
    explicit NCRDLabelOptWidget(QWidget *parent = 0);
    ~NCRDLabelOptWidget();

	void setLabelMode();
	//Ui::NCRDLabelOptWidgetUI *ui;
};

#endif // NCRDLABELOPTWIDGET_H
