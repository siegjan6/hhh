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
#include "ncrddatasourcetypedialog.h"

//! 多语言
#include "MDMultiLanguage.h"

NCRDDataSourceTypeDialog::NCRDDataSourceTypeDialog(QWidget* parent)
: QDialog( parent ), Ui::NCRDDataSourceTypeDialogUI()
{
    setupUi(this);
	  //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDDataSourceTypeDialog::~NCRDDataSourceTypeDialog()
{
}

/*$SPECIALIZATION$*/

NCReportDataSource::DataSourceType NCRDDataSourceTypeDialog::selectedType() const
{
    NCReportDataSource::DataSourceType dt = NCReportDataSource::Custom;
    if ( rbSql->isChecked() )
        dt = NCReportDataSource::Sql;
    else if ( rbText->isChecked() )
        dt = NCReportDataSource::Text;
    else if ( rbXml->isChecked() )
        dt = NCReportDataSource::Xml;
    else if ( rbStringList->isChecked() )
        dt = NCReportDataSource::StringList;
    else if ( rbItem->isChecked() )
        dt = NCReportDataSource::ItemModel;
    else if ( rbCustom->isChecked() )
        dt = NCReportDataSource::Custom;
    else if(rbDataView->isChecked())//me:扩展
        dt = NCReportDataSource::DataView;

    return dt;
}


