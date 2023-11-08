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

#include "ncrdtextoutputtemplateselect.h"
#include "ui_ncrdtextoutputtemplateselect.h"
#include "ncrdfileselector.h"

NCRDTextOutputTemplateSelect::NCRDTextOutputTemplateSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NCRDTextOutputTemplateSelect)
{
	ui->setupUi(this);
	selector = new NCRDFileSelector(this,ui->wFileName, ui->btnSelect);
	selector->setFileTypes("*.html *.xhtml *.xml *.txt *.csv *.xls *.tpl *.tmpl");
	selector->setDescription( tr("Template files") );
	selector->setEditMode(NCRDFileSelector::TypeAndSelect);
	selector->setSelectMode(NCRDFileSelector::Open);
}

NCRDTextOutputTemplateSelect::~NCRDTextOutputTemplateSelect()
{
	delete ui;
}

QString NCRDTextOutputTemplateSelect::fileName() const
{
	return selector->fileName();
}
