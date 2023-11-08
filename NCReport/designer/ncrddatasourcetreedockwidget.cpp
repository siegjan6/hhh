/****************************************************************************
*
*  Copyright (C) 2002-2013 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 2013.09.26. (nocisoft)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#include "ncrddatasourcetreedockwidget.h"
#include "ui_ncrddatasourcetreedockwidget.h"

NCRDDataSourceTreeDockWidget::NCRDDataSourceTreeDockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::NCRDDataSourceTreeDockWidget)
{
    ui->setupUi(this);
}

NCRDDataSourceTreeDockWidget::~NCRDDataSourceTreeDockWidget()
{
    delete ui;
}

NCRDDataSourceTree *NCRDDataSourceTreeDockWidget::tree()
{
    return ui->treeWidget;
}


