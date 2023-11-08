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
#include "ncrdopenfromdbdialog.h"
#include "ncrdmainwindow.h"
#include "ncrdconnectiondialog.h"
#include "ncrdformgeometry.h"

#include <QMessageBox>

//! 多语言
#include "MDMultiLanguage.h"

NCRDOpenFromDBDialog::NCRDOpenFromDBDialog(NCRDMainWindow* parent)
: QDialog( parent ), Ui::NCRDOpenFromDBDialogUI()
{
    mwindow = parent;
    setupUi(this);

    QHash<QString,NCRDConnection>::const_iterator i = mwindow->dbConnections().constBegin();
    int index=0, defaultIndex=0;
    while (i != mwindow->dbConnections().constEnd()) {
        comboConn->addItem( i.value().name );
        if ( i.value().name == mwindow->defaultConnectionID() )
            defaultIndex = index;
        ++i;
        ++index;
    }
    comboConn->setCurrentIndex( defaultIndex );

    tableName->setText( mwindow->defaultDBTable() );
    reportColumn->setText( mwindow->defaultDBColumn() );
    keyColumn->setText( mwindow->defaultDBKeyColumn() );
    keyValue->setText( mwindow->defaultDBKeyValue() );

    NCRDFormGeometry *fg = new NCRDFormGeometry(this);
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    fg->restore();

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDOpenFromDBDialog::~NCRDOpenFromDBDialog()
{
}

/*$SPECIALIZATION$*/
void NCRDOpenFromDBDialog::reject()
{
  QDialog::reject();
}

void NCRDOpenFromDBDialog::accept()
{
  QDialog::accept();
}

NCReportSource NCRDOpenFromDBDialog::reportSource() const
{
    NCReportSource rs;
    rs.setSourceType(NCReportSource::Database);
    rs.setConnectionID(comboConn->currentText());
    rs.setTableName(tableName->text());
    rs.setColumnName(reportColumn->text());
    rs.setKeyColumnName(keyColumn->text());
    rs.setKeyValue(keyValue->text());
    rs.setFileName("DB:"+rs.keyValue());
    return rs;
}

