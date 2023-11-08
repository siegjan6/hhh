/****************************************************************************
*
*  Copyright (C) 2002-2012 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport Report Generator System
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  norbert@nocisoft.com if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#include "ncrdsetzonedialog.h"
#include "ui_ncrdsetzonedialog.h"

//! 多语言
#include "MDMultiLanguage.h"

NCRDSetZoneDialog::NCRDSetZoneDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NCRDSetZoneDialog)
{
	ui->setupUi(this);

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDSetZoneDialog::~NCRDSetZoneDialog()
{
	delete ui;
}

int NCRDSetZoneDialog::zoneID() const
{
	return ui->sbZone->value();
}
