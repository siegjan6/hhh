/****************************************************************************
*
*  Copyright (C) 2002-2013 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 2013.11.13. (nocisoft)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#include "ncrditemsourceselector.h"
#include "ui_ncrditemsourceselector.h"

NCRDItemSourceSelector::NCRDItemSourceSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NCRDItemSourceSelector)
{
    ui->setupUi(this);
}

NCRDItemSourceSelector::~NCRDItemSourceSelector()
{
    delete ui;
}
