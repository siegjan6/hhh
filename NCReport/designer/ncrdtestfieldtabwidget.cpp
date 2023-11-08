/****************************************************************************
*
*  Copyright (C) 2002-2015 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 2015. 02. 23. (nszabo)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#include "ncrdtestfieldtabwidget.h"
#include "ui_ncrdtestfieldtabwidget.h"

NCRDTestFieldTabWidget::NCRDTestFieldTabWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NCRDTestFieldTabWidget)
{
    ui->setupUi(this);
}

NCRDTestFieldTabWidget::~NCRDTestFieldTabWidget()
{
    delete ui;
}
